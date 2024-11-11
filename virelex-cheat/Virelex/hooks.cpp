#include <Windows.h>
#include <string>
#include <iostream>
#include "GL/glew.h"
#include "options.h"
#pragma comment(lib, "glew32s.lib")
#include "hooks.h"
#include "sharedMemory.h"
#include <GL/gl.h>
#pragma comment(lib, "OpenGL32.lib")
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <algorithm>

glm::vec4 getPositionFromMatrix(const glm::mat4& matrix) {
    return glm::vec4(matrix[3][0], matrix[3][1], matrix[3][2], matrix[3][3]);
}

std::vector<std::pair<float, float>> coordsArray;

void WINAPI Hooks::hk_glDrawElements(GLenum mode, GLsizei count, GLenum type, const void* indices) {

    static bool glewInitialized = false;

    if (!glewInitialized) {
        if (glewInit() != GLEW_OK) {
            MessageBoxA(NULL, "Failed to activate GLEW", "Error", MB_ICONERROR);
        }
        glewInitialized = true;
    }

    bool isReady = count > 3000 && count < 300000 && mode == 4; // prob better to delete it

    GLint currentProgram;
    glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram);

    GLint textureWidth;
    glGetIntegerv(GL_TEXTURE_WIDTH, &textureWidth);

    if (textureWidth <= 50) goto THE_END;

    if (isReady) {
        GLint location = glGetUniformLocation(currentProgram, "isAnimated");
        GLint isAnimated;

        if (location == -1) {
            goto THE_END;
        }
        else {
            glGetUniformiv(currentProgram, location, &isAnimated);
        }

        if (isAnimated != 1) goto THE_END;

        GLint uniformCount = 0;
        glGetProgramiv(currentProgram, GL_ACTIVE_UNIFORMS, &uniformCount);

        if (uniformCount == 41 || uniformCount == 40 || uniformCount == 30 || uniformCount == 36) {
            goto THE_END;
        }

        glm::mat4 modelMatrix{ 0 };

        GLint modelMatrixLocation = glGetUniformLocation(currentProgram, "modelMatrix");

        if (modelMatrixLocation != -1) {
            glGetUniformfv(currentProgram, modelMatrixLocation, glm::value_ptr(modelMatrix));
        }
        else {
            goto THE_END;
        }

        GLuint blockIndexPersist = glGetUniformBlockIndex(currentProgram, "persistentBlock");
        if (blockIndexPersist == GL_INVALID_INDEX) goto THE_END;

        GLint currentUBO;
        glGetIntegerv(GL_UNIFORM_BUFFER_BINDING, &currentUBO);

        GLuint ubo{ 0 };
        glGenBuffers(1, &ubo);

        if (ubo == 0) goto THE_END;

        glBindBuffer(GL_UNIFORM_BUFFER, ubo);

        glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4) * 2, NULL, GL_STATIC_DRAW);
        glUniformBlockBinding(currentProgram, blockIndexPersist, 0);

        glm::mat4 viewMatrix{ 0 };
        glm::mat4 projectionMatrix{ 0 };

        glGetBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(viewMatrix));
        glGetBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(projectionMatrix));

        glBindBuffer(GL_UNIFORM_BUFFER, currentUBO);

        if (modelMatrix == glm::mat4(0.0f) || viewMatrix == glm::mat4(0.0f) || projectionMatrix == glm::mat4(0.0f) || projectionMatrix[3][3] != 0.0f) {
            goto THE_END;
        }

        glm::vec4 worldPos = getPositionFromMatrix(modelMatrix);

        float headHeight = 1.65f; //points at the legs, so I decided to lift it for aimbot
        worldPos.y += headHeight;

        glm::vec4 viewPosition = viewMatrix * worldPos;

        glm::vec4 clipPosition = projectionMatrix * viewPosition;

        if (clipPosition.w == 0.0f) goto THE_END;

        glm::vec4 ndcPosition = glm::vec4(clipPosition) / clipPosition.w;

        float screenX = (ndcPosition.x + 1.0f) / 2.0f * options::monitorX;
        float screenY = (1.0f - ndcPosition.y) / 2.0f * options::monitorY;

        if (screenX < options::monitorX && screenX > 0.0f && screenY > 0.0f && screenY < options::monitorY) {
            coordsArray.push_back({ screenX, screenY });
        }
    }

    THE_END:

    fn_glDrawElements(mode, count, type, indices);
}

void WINAPI Hooks::hk_glFlush() {
    
    if (!coordsArray.empty()) {
        writeEntitiesToShared(coordsArray, coordsArray.size());
        coordsArray.clear();
    }

    fn_glFlush();
}