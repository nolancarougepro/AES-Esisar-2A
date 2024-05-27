#include "all.h"

keyt key = {}; // Clé de chiffrement
matrix mem_cbc = {}; // historique CBC et initialisation
matrix round_keys[Nr+1];

int main(int argc, char* argv[]) {
    if (argc >= 2 && argv[1][0] == 't') { //tests 
        test_sub_bytes(); 
        test_shift_rows();
        test_add_key();
        test_key_expension();
        test_mix_column();
        test_aes();
    } else if (argc >= 5) { // script 
        int key_result = get_key(argv[3], key);
        
        if(key_result == 0) { // taille de la clé insuffisante
            printf("Erreur : la longueur de la clé doit être de minimum %d bits (%d octets).\n", 4*Nk*8, 4*Nk);
            return 0;
        } else if (key_result == -1) { // Pas de fichier : on demande la clé
            if (argv[3][0] == 'x')
                ask_hexa_key();
            else 
                ask_key(argv[3][0] == 's');
        }

        int succes;

        for (int i = 4; i < argc; i++) {
            int decrypt = argv[1][0] == 'd';
            int bmp = argv[2][0] == 'i';
            int ecb = bmp?(argv[2][1] == 'e'):argv[2][0] == 'e';

            #if ask_init_vect
            if (!ecb) // Vecteur d'initialisation
                ask_init_vector();
            #endif

            #ifdef AES256
            char* output = combine_strings(argv[i], ecb?"_ecb":"_cbc", decrypt?"_decrypted":"_crypted_256", bmp?".bmp":"");
            #elif AES192
            char* output = combine_strings(argv[i], ecb?"_ecb":"_cbc", decrypt?"_decrypted":"_crypted_192", bmp?".bmp":"");
            #else //128
            char* output = combine_strings(argv[i], ecb?"_ecb":"_cbc", decrypt?"_decrypted":"_crypted_128", bmp?".bmp":"");
            #endif

            printf("Début du %s %sde %s (%s).\n", decrypt?"déchiffrement":"chiffrement", bmp?"(bitmap en clair) ":"", argv[i], (argv[2][0] == 'e')?"ECB":"CBC");
            
            double time = (double) clock() / CLOCKS_PER_SEC;

            key_expension(key); // permet de calculer les clés du roundkey
            
            if (bmp) { // on s'occupe d'une image bmp
                if (decrypt) // decrypte
                    succes = bitmap_process (argv[i], output, ecb?(&cipher_inverse):(&aes_cbc_inverse));
                else //crypte
                    succes = bitmap_process (argv[i], output, ecb?(&cipher):(&aes_cbc));
            } else {
                if (decrypt) // decrypte
                    succes = file_operation (argv[i], output, ecb?(&cipher_inverse):(&aes_cbc_inverse), 0);
                else //crypte
                    succes = file_operation (argv[i], output, ecb?(&cipher):(&aes_cbc), 1);
            }

            time = (((double) clock()) / (double) CLOCKS_PER_SEC) - time;

            if (succes) {
                printf("\nFichier %s %s (%s - %d bits) dans %s avec succès (%.5fs).\nEntropie : %lf bits par octet.\n\n", 
                    argv[i], decrypt?"déchiffré":"chiffré", ecb?"ECB":"CBC", 4*Nk*8, output, time, calculate_entropy(output));
            } else
                printf("Erreur durant le %s de %s\n", decrypt?"déchiffrage":"chiffrage", argv[i]);

            free(output);
        }
    } else
        printf("Usage : ./aes%d <crypt|decrypt|test> <ecb|cbc|iecb|icbc> <show|hide|x|file_key> [input_files]\n Raccourcis :\n\t- c = crypt\n\t- d = decrypt\n\t- t = test\n\t- s = show\n\t- x = hexadecimal\n\t- h = hide\n\t- iecb et icbc permettent de chiffrer des fichiers bmp avec l'entête en clair.\n", 4*Nk*8);
    return 0;
}