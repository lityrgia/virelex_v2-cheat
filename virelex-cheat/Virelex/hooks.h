#pragma once
#include <Windows.h>
#include <functional>
#include <GL/gl.h>

inline decltype(&glFlush) fn_glFlush = &glFlush;
inline decltype(&glDrawElements) fn_glDrawElements = &glDrawElements;

class Hooks {
public:
    static void WINAPI hk_glDrawElements(GLenum mode, GLsizei count, GLenum type, const void* indices);
    static void WINAPI hk_glFlush();

    static void* get_glDrawElements() {
        return reinterpret_cast<void*>(&hk_glDrawElements);
    }

    static void* get_glFlush() {
        return reinterpret_cast<void*>(&hk_glFlush);
    }
};