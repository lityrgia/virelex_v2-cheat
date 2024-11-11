#include "sharedMemory.h"
#include <Windows.h>
#include <iostream>
#include <vector>

LPVOID pBuf = nullptr;
HANDLE hMapFile = nullptr;

bool createMappingData() {
    hMapFile = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(CoordData), L"Global\\Virelex");
    if (hMapFile == NULL) {
        //std::cerr << "[-] Failed to create file mapping object!" << std::endl;
        return false;
    }

    pBuf = MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(CoordData));
    if (pBuf == NULL) {
        //std::cerr << "[-] Failed to map view of file!" << std::endl;
        return false;
    }

    shared::sharedData = (CoordData*)pBuf;

    return true;
}

void closeMappingHandle() {
    UnmapViewOfFile(pBuf);
    CloseHandle(hMapFile);
}

void writeEntitiesToShared(const std::vector<std::pair<float, float>>& entities, const int count) {
    shared::sharedData->count = count;

    for (int i = 0; i < shared::sharedData->count; i++) {
        shared::sharedData->entities[i] = entities[i];
    }
}