#include "lzw.h"

IntToStringMap *initMap() {
    IntToStringMap *map = newIntToStringMap(DICT_SIZE, 0.75);

    for (size_t i = 0; i < 256; i++) {
        string *value = charToString((char) i);
        IntToStringMapInsert(map, i, value);
    }

    return map;
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

    IntToStringMap *map = initMap();
    int nextCode = 256;
    int old, new;
    
    fread(&old, sizeof(int), 1, inputFile);
    string *s = IntToStringMapGet(map, old);
    fwrite(s->str, sizeof(char), s->length, outputFile);

    string *c = charToString(s->str[0]);

    while (fread(&new, sizeof(int), 1, inputFile)) {
        string *value = IntToStringMapGet(map, new);
        
        if (value != NULL) {
            s = value;
        }
        else {
            string *oldValue = IntToStringMapGet(map, old);
            s = concat(oldValue, c);
        }

        fwrite(s->str, sizeof(char), s->length, outputFile);
        
        freeString(c);
        c = charToString(s->str[0]);

        string *oldValue = IntToStringMapGet(map, old);
        string *newValue = concat(oldValue, c);
        IntToStringMapInsert(map, nextCode, newValue);
        nextCode++;
        old = new;
    }

    freeString(s);
    freeString(c);
    freeIntToStringMap(map);
    
    fclose(outputFile);
    fclose(inputFile);

    return 0;
}