#include "all.h"

#define show_occurences 0
#define max 256

void calculate_occurence(FILE* file, long occurences[max]) {
    int c;
    fseek(file, 0, SEEK_SET);
    while((c = fgetc(file)) != EOF) {
        occurences[c] += 1;
    }
}

double calculate_entropy(char* file_name) {
    FILE* file = fopen(file_name, "r");
    
    if (file == NULL) return -1;

    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);

    double entrop = 0;
    long occurences[256] = {};

    calculate_occurence(file, occurences);

    #if show_occurences
    printf(" octet | repartition | occurences\n");
    printf("-------+-------------+-------------\n");
    #endif

    for (int o = 0; o < 256; o++) {
        if (occurences[o] != 0) {
            double P = occurences[o] / ((double) size);
            entrop -= P * log2(P);
            #if show_occurences
            printf("  %3d  |   %f  | %ld\n", o, P, occurences[o]);
            #endif
        }
    }
    
    #if show_occurences
    printf("-------+-------------+-------------\n");
    printf("Nombre total d'octets : %ld\n", size);
    #endif

    fclose(file);

    return entrop;
}

/* int main(int argc, char* argv[]) {
    if (argc >= 2) {
        for (int i = 1; i < argc; i++) {
            double entropy = calculate_entropy(argv[i]);
            printf("Entropie du fichier %s : %lf bits par octet.\n", argv[i] , entropy);
        }
    } else {
        printf("Usage : ./entropie <fichiers>\n");
    }
} */
