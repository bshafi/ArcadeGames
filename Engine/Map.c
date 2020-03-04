#include <assert.h>
#include <stdlib.h>

#include "Map.h"

Map CreateMap(uint32_t data_type_size, void (*destructor)(void*)) {
    assert(data_type_size != 0);
    assert(destructor);

    return (Map) {
        NULL,
        NULL,
        data_type_size,
        0,
        0,
        destructor
    };
}
void DestroyMap(Map *map) {
    assert(map);

    for (uint32_t i = 0; i < map->size; ++i) {
        free(map->names[i]);
        map->destructor((void*)&map->data[i * map->data_type_size]);
    }
    free(map->names);
    free(map->data);

    map->names = NULL;
    map->data = NULL;
    map->data_type_size = 0;
    map->size = 0;
    map->destructor = NULL;
}

void InsertElemMap(Map *map, const char *name, void *elem) {
    assert(map);
    assert(name);
    assert(elem);

    if (map->capacity == 0) {
        const uint32_t INITIAL_CAPACITY = 16;
        map->names = malloc(sizeof(char*) * INITIAL_CAPACITY);
        assert(map->names != NULL);
        map->data = malloc(map->data_type_size * INITIAL_CAPACITY);
        assert(map->data != NULL);
        map->size = 0;
        map->capacity = INITIAL_CAPACITY;
    }

    ++map->size;
    if (map->size > map->capacity) {
        assert(map->capacity != 0);

        map->capacity *= 2;
        map->names = realloc(map->names, sizeof(char*) * map->capacity);
        assert(map->names);
        map->data = realloc(map->data, map->data_type_size * map->capacity);
        assert(map->data);
    }

    assert(map->size != 0);
    const uint32_t last_index = map->size - 1;
    map->names[last_index] = strdup(name);
    assert(map->names[last_index]);
    memcpy((void*)&map->data[last_index * map->data_type_size], elem, map->data_type_size);
}
int64_t GetIndexMap(Map *map, const char *name) {
    assert(map);
    assert(name);

    for (uint32_t i = 0; i < map->size; ++i) {
        if (strcmp(name, map->names[i]) == 0) {
            //return (void*)&map->data[i * map->data_type_size];
            return (int32_t)i;
        }
    }
    return -1;
}
void* GetElemMap(Map *map, int64_t i) {
    assert(map);
    if (i == -1) {
        return NULL;
    }
    assert(0 <= i && i < map->size);
    return (void*)&map->data[i * map->data_type_size];
}