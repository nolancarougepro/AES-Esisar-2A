#include "all.h"

// Permet d'écrire la matrice state dans le fichier output
void write_matrix (FILE* output, matrix state) {
    static int i;
    for (i = 0; i < Matrix_size; i++)
        fputc(state[i],output);
}

long file_size (FILE* file) {
    long last_seek = ftell(file);
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, last_seek);
    return size;
}

// Lit les octets du fichier input_name et les implemente dans le processus "p_process" puis l'écrit dans le fichier "output_name"
int file_operation (char* input_name, char* output_name, void (*p_process) (matrix state), int crypt) {
    FILE* input = fopen(input_name, "r");
    FILE* output = fopen(output_name, "w");

    matrix data_matrix = {}; // Initialise la matrice à 0

    if(input == NULL) {
        printf("\nErreur : le fichier %s est introuvable.\n", input_name);
        return 0;
    }
    if (output == NULL) return 0;

    #if write_size_octets == 8
        long long size = 0;
    #else
        long size = 0;
    #endif
    
    int counter = 0, c;

    if(crypt == 1) { // On ajoute la taille du fichier quand on crypte
        #if write_size
            size = file_size(input);
            
            #if write_size_octets == 8
                data_matrix[0] = size >> (8*7);
                data_matrix[1] = size >> (8*6);
                data_matrix[2] = size >> (8*5);
                data_matrix[3] = size >> (8*4);
                data_matrix[4] = size >> (8*3);
                data_matrix[5] = size >> (8*2);
                data_matrix[6] = size >> (8*1);
                data_matrix[7] = size;
            #else 
                data_matrix[0] = size >> (8*3);
                data_matrix[1] = size >> (8*2);
                data_matrix[2] = size >> (8*1);
                data_matrix[3] = size;
            #endif
            counter = write_size_octets;
        #endif

        while((c = fgetc(input)) != EOF) {
            data_matrix[counter] = c;

            if(counter == Matrix_size - 1) {
                p_process(data_matrix);
                write_matrix(output, data_matrix);
                counter = 0;
            } else
                counter++;
        }

        if (counter != 0) { // il reste des octets en fin de fichier
            for (int i = counter; i < Matrix_size; i++) // on comble avec des 0 (pas obligatoire...)
                data_matrix[i] = 0;
            p_process(data_matrix);
            write_matrix(output, data_matrix);
        }
    } else {
        #if write_size
            while(counter != Matrix_size && (c = fgetc(input)) != EOF) {
                data_matrix[counter] = c;
                counter++;
            }
            counter = 0;
            
            p_process(data_matrix);
            
            // On récupère la taille du fichier
            #if write_size_octets == 8
                size += data_matrix[0] << (8*7);
                size += data_matrix[1] << (8*6);
                size += data_matrix[2] << (8*5);
                size += data_matrix[3] << (8*4);
                size += data_matrix[4] << (8*3);
                size += data_matrix[5] << (8*2);
                size += data_matrix[6] << (8*1);
                size += data_matrix[7];
            #else
                size += data_matrix[0] << (8*3);
                size += data_matrix[1] << (8*2);
                size += data_matrix[2] << (8*1);
                size += data_matrix[3];
            #endif

            for (int i = write_size_octets; i < Matrix_size; i++)
                fputc(data_matrix[i], output);
            size -= 16 - write_size_octets;
        #endif

        while((c = fgetc(input)) != EOF) {
            data_matrix[counter] = c;

            if (counter == 15) {
                p_process(data_matrix);
                if (size >= 16 || !write_size)
                    write_matrix(output, data_matrix);
                else {
                    for (int i = 0; i < size; i++)
                        fputc(data_matrix[i], output);
                    break;
                }
                counter = 0;
                size -= 16;
            } else
                counter++;
        }
    }
    
    // Si on est pas allé jusqu'au bout du fichier par un bloc de 128 bits (mais moins)
    /* if (crypt) {
        if (counter != 0) {
            p_process(data_matrix);
            print_matrix(data_matrix);
            write_matrix(output, data_matrix);
        }
    } */

    fclose(output);
    fclose(input);
    return 1;
}

// Récupère la clé dans le fichier file_key et l'écrit dans la matrice key
// Renvoie 1 si la clé a été copiée correctement
// Renvoie 0 en cas de clé de taille non suffisante
// Renvoie -1 si le fichier est introuvable ou erreur
int get_key (char* file_keys, matrix key) {
    FILE* file = fopen(file_keys, "r");

    if(file == NULL) {
        return -1;
    }

    clean_matrix(key);
    int counter = 0, c;

    while((c = fgetc(file)) != EOF && counter < 4*Nk) {
        key[counter] = c;
        counter++;
    }

    fclose(file);
    return counter == 4*Nk;
}