#include "lzw.h"

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("input and output file paths should be the only supplied arguements\n");
        return 1;
    }

    FILE *outputFile = fopen(argv[2], "wb");
    if (outputFile == NULL) {
        printf("Error opening output file: %s", argv[2]);
        return 1;
    }

    FILE *inputFile = fopen(argv[1], "rb");
    if (inputFile == NULL) {
        printf("Error opening input file: %s", argv[1]);
        fclose(outputFile);
        return 1;
    }

    StringMap *map = initMap();
    int nextCode = 96;
    
    int code;
    fread(&code, sizeof(int), 1, inputFile);
    string *key = getKeyFromValue(map, code);
    fwrite(key->str, sizeof(char), key->length, outputFile);
    string *w = copyString(key);

    while (fread(&code, sizeof(int), 1, inputFile)) {
        string *key = getKeyFromValue(map, code);
        string *entry;
        
        if (key != NULL) {
            entry = copyString(key);
        }
        else {
            string *wFirstChar = newString(w->str[0]);
            entry = concat(w, wFirstChar);
            freeString(wFirstChar);
        }

        fwrite(entry->str, sizeof(char), entry->length, outputFile);
        
        string *entryFirstChar = newString(entry->str[0]);
        string *newKey = concat(w, entryFirstChar);
        freeString(entryFirstChar);

        stringMapInsert(map, newKey, nextCode);
        nextCode++;

        freeString(w);
        w = entry;
    }

    freeString(w);
    freeStringMap(map);
    
    fclose(outputFile);
    fclose(inputFile);

    return 0;
}

StringMap *initMap() {
    StringMap *map = newStringMap(DICT_SIZE, 0.75);

    for (size_t i = 1; i < 96; i++) {
        string *key = newString((char) i);
        stringMapInsert(map, key, i);
    }

    return map;
}

string *getKeyFromValue(StringMap *map, int value) {
    for (size_t i = 0; i < map->size; i++) {
        if (map->entries[i].value == value) {
            return map->entries[i].key;
        }
    }

    return NULL;
}