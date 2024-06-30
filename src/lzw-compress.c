#include "lzw.h"

StringMap *initMap() {
    StringMap *map = newStringMap(DICT_SIZE, 0.75);

    for (size_t i = 0; i < 256; i++) {
        string *key = charToString((char) i);
        stringMapInsert(map, key, i);
    }

    return map;
}

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

    FILE *inputFile = fopen(argv[1], "r");
    if (inputFile == NULL) {
        printf("Error opening input file: %s", argv[1]);
        fclose(outputFile);
        return 1;
    }
    
    StringMap *map = initMap();

    char c = fgetc(inputFile);
    string *w = charToString(c);
    int nextCode = 256;

    while ((c = fgetc(inputFile)) != EOF) {
        string *cString = charToString(c);
        string *wc = concat(w, cString);
        int value = stringMapGet(map, wc);
        
        if (value != -1) {
            freeString(w);
            freeString(cString);
            w = wc;
        }
        else {
            int code = stringMapGet(map, w);
            fwrite(&code, sizeof(int), 1, outputFile);
            stringMapInsert(map, wc, nextCode);
            nextCode++;
            freeString(w);
            w = cString;
            freeString(wc);
        }
    }

    if (w->str[0] != '\0') {
        int wCode = stringMapGet(map, w);
        fwrite(&wCode, sizeof(int), 1, outputFile);
    }

    freeString(w);
    freeStringMap(map);

    fclose(inputFile);
    fclose(outputFile);

    return 0;
}