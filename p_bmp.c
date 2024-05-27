#include "all.h"

int is_bmp(char* input_name) {
    FILE* input = fopen(input_name, "r");

    if(input == NULL)
        return 0;
    
    unsigned short signature = 0;

    fread(&signature, sizeof(unsigned short), 1, input);

    fclose(input);
    return signature == 0x4D42;
}

int bitmap_process(char* input_name, char* output_name, void (*p_process) (matrix state)) {
    FILE* input = fopen(input_name, "r");

    fichierEntete* fichier_entete = malloc(sizeof(fichierEntete));
    
    // Test succès allocations de mémoire
    if(fichier_entete == NULL) {
        perror("malloc");
        exit(0);
    }
    if(input == NULL) {
        printf("\nErreur : le fichier %s est introuvable.\n", input_name);
        return 0;
    }

    FILE* output = fopen(output_name, "w");
    if (output == NULL) return 0;

    // Pour se placer après les métadonnées
    fread(fichier_entete, sizeof(fichierEntete), 1, input);
    fwrite(fichier_entete, sizeof(fichierEntete), 1, output);

    unsigned int offset = fichier_entete->offset - ftell(input);
    int c;

    while(offset > 0 && (c = fgetc(input)) != EOF) {
        fputc(c, output);
        offset--;
    }

    matrix data_matrix = {}; // Initialise la matrice à 0

    int counter = 0;

    // On crypte ou décrypte...
    while((c = fgetc(input)) != EOF) {
        data_matrix[counter] = c;
        if(counter == Matrix_size - 1) {
            p_process(data_matrix);
            write_matrix(output, data_matrix);
            clean_matrix(data_matrix);
            counter = 0;
        } else
            counter++;
    }

    // Si on est pas aller jusqu'au bout du fichier par un bloc de 128 bits (mais moins)
    if (counter != 0) {
        p_process(data_matrix);
        write_matrix(output, data_matrix);
    }

    // Fermeture du fichier et libération mémoire
    fclose(output);
    fclose(input);
        
    free(fichier_entete);
    
    return 1;
}