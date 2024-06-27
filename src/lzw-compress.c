#include "lzw-compress.h"

/*
Things I need:
---------------------
1. Arguement checking.
2. Opening output file.
3. LZW compression algorithm.
4. Writing a file.
*/

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
    string *w = newString("");
    
}

StringMap *initMap() {
    StringMap *map = newStringMap(DICT_SIZE, 0.75);

    for (size_t i = 1; i < 96; i++) {
        string *key = newString((char) i);
        stringMapInsert(map, key, i);
    }

    return map;
}