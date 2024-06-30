#include "lzw.h"

StringMap *initMap() {
    StringMap *map = newStringMap(DICT_SIZE, 0.75);

    for (size_t i = 0; i < 256; i++) {
        string *key = charToString((char) i);
        stringMapInsert(map, key, i);
    }

    return map;
}

string *getKeyFromValue(StringMap *map, int value) {
    for (size_t i = 0; i < map->size; i++) {
        if (map->entries[i].isOccupied == 1 && map->entries[i].value == value) {
            return map->entries[i].key;
        }
    }

    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("input and output file paths should be the only supplied arguements\n");
        return 1;
    }

    FILE *outputFile = fopen(argv[2], "w");
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
    int nextCode = 256;
    int old; int new;
    
    fread(&old, sizeof(int), 1, inputFile);
    string *s = getKeyFromValue(map, old);
    fwrite(s->str, sizeof(char), s->length, outputFile);

    string *c = charToString(s->str[0]);

    while (fread(&new, sizeof(int), 1, inputFile)) {
        string *key = getKeyFromValue(map, new);
        
        if (key != NULL) {
            s = key;
        }
        else {
            s = getKeyFromValue(map, old);
            s = concat(s, c);
        }

        fwrite(s->str, sizeof(char), s->length, outputFile);
        
        freeString(c);

        c = charToString(s->str[0]);
        string *oldKey = getKeyFromValue(map, old);
        string *newKey = concat(oldKey, c);

        stringMapInsert(map, newKey, nextCode);
        nextCode++;
        old = new;
    }

    freeString(s);
    freeString(c);
    freeStringMap(map);
    
    fclose(outputFile);
    fclose(inputFile);

    return 0;
}