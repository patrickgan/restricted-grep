#include <stdio.h>

int main(int argc, char *argv[]) {
    char c = 0;
    int newlines, words, bytes, state;
    char *filename = "";
    FILE *fp;
    newlines = words = bytes = 0;
    state = 0; 

    if (argc==2) {
        filename = argv[1];    
        fp = fopen((const char *) filename, "r");
    } else {
        fp = stdin; 
    }
    while ((c = fgetc(fp)) != EOF) {
        bytes++;
        if (c == '\n')
            newlines++;
        if (c == ' ' || c == '\t' || c == '\n' || c == '\r')
            state = 0;
        else if (state == 0) {
            if (c != '\x00' && c != '\0') {
                state = 1;
                words++;
            }
        }
    }
    fclose(fp);
    printf(" %d %d %d %s\n", newlines, words, bytes, filename);
    return 0;
}
