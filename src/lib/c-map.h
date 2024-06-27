#ifndef C_MAP
#define C_MAP

#include <stdlib.h>
#include "c-string.h"

typedef struct {
    string *key;
    int value;
    int isOccupied;
} StringMapEntry;

typedef struct {
    int key;
    int value;
    int isOccupied;
} IntMapEntry;

typedef struct {
    size_t size;
    size_t count;
    float loadFactor;
    StringMapEntry *entries;
} StringMap;

typedef struct {
    size_t size;
    size_t count;
    float loadFactor;
    IntMapEntry *entries;
} IntMap;

StringMap *newStringMap(size_t size, float loadFactor);

IntMap *newIntMap(size_t size, float loadFactor);

size_t quadraticProbe(size_t hash, size_t attempt, size_t size);

size_t hashFunction(int key, size_t size);

int stringToInt(string *str);

void resizeStringMap(StringMap *map);

void stringMapInsert(StringMap *map, string *key, int value);

int stringMapGet(StringMap *map, string *key);

void stringMapDelete(StringMap *map, string *key);

void resizeIntMap(IntMap *map);

void intMapInsert(IntMap *map, int key, int value);

int intMapGet(IntMap *map, int key);

void intMapDelete(IntMap *map, int key);

#endif