#include "sharedMemory.h"
#include <Windows.h>
#include <iostream>
#include <vector>
#include <Windows.h>

LPVOID pBuf = nullptr;
HANDLE hMapFile = nullptr;

bool connectSharedMemory() {
    hMapFile = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, L"Global\\Virelex");
    if (hMapFile == NULL) {
        return false;
    }

    pBuf = MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(CoordData));
    if (pBuf == NULL) {
        CloseHandle(hMapFile);
        return false;
    }

    shared::sharedData = (CoordData*)pBuf;

    return true;
}

void closeMappingHandle() {
    UnmapViewOfFile(pBuf);
    CloseHandle(hMapFile);
}

void getEntitiesArray(std::vector<std::pair<float, float>>& outArray) {
    int count = shared::sharedData->count;

    if (count == 0) {
        return; 
    }

    outArray.clear();
    outArray.reserve(count); 

    for (int i = 0; i < count; ++i) {
        outArray.push_back(shared::sharedData->entities[i]);
    }
}