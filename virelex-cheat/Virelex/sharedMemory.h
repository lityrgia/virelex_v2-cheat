#pragma once
#include <vector>

bool createMappingData();
void closeMappingHandle();
void writeEntitiesToShared(const std::vector<std::pair<float, float>>& entities, const int count);

const int MAX_ENTITIES = 20;

struct CoordData {
    int count = 0;

    std::pair<float, float> entities[MAX_ENTITIES];
};

namespace shared {
    inline CoordData* sharedData = nullptr;
}