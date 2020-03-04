#pragma once

#include "vec2i.h"

typedef struct Map Map;
struct Map {
    char **names;
    uint8_t *data;
    uint32_t data_type_size;
    uint32_t size;
    uint32_t capacity;
    void (*destructor)(void*);
};

Map CreateMap(uint32_t data_type_size, void (*)(void*));
void DestroyMap(Map *);

void InsertElemMap(Map *, const char *name, void *elem);
int64_t GetIndexMap(Map *, const char *name);
void* GetElemMap(Map *, int64_t);
