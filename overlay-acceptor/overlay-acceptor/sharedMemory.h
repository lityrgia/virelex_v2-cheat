#pragma once
#include <vector>

bool connectSharedMemory();
void closeMappingHandle();
void getEntitiesArray(std::vector<std::pair<float, float>>& outArray);

const int MAX_ENTITIES = 20;

struct CoordData {
    int count = 0;

    std::pair<float, float> entities[MAX_ENTITIES];
};

namespace shared {
    inline CoordData* sharedData = nullptr;
}