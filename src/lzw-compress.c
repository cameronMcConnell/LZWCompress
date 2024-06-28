#include "lzw.h"

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("input and output file paths should be the only supplied arguements\n");
        return 1;
    }

    FILE *outputFile = fopen(argv[2], 'w');
    if (outputFile == NULL) {
        printf("Error opening output file: %s", argv[2]);
        return 1;
    }

    FILE *inputFile = fopen(argv[1], 'r');
    if (inputFile == NULL) {
        printf("Error opening input file: %s", argv[1]);
        return 1;
    }
    
    StringMap *map = initMap();
    string *w = newString('\0');
    int nextCode = 96;
    char c;

    while ((c = fgetc(inputFile)) != EOF) {
        string *cString = newString(c);
        string *wc = concat(w, cString);
        int value = stringMapGet(map, wc);
        
        if (value != -1) {
            freeString(w);
            freeString(cString);
            w = wc;
        }
        else {
            int wCode = stringMapGet(map, w);
            fwrite(&wCode, sizeof(int), 1, outputFile);
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

    fclose(inputFile);
    fclose(outputFile);

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