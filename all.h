#define Nb 4
#define Matrix_size 16

#define write_size 1
#define ask_init_vect 0
#define write_size_octets 4

#ifdef AES192
    #define Nk 6
    #define Nr 12
#elif AES256
    #define Nk 8
    #define Nr 14
#else
    #define Nk 4
    #define Nr 10
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <termios.h>
#include <unistd.h>

#define octet uint8_t
typedef octet matrix [4*Nb];
typedef octet keyt [4*Nk];
typedef octet word [4];

#define red "\x1B[31m"
#define green "\x1B[32m"
#define reset "\x1B[0m"

/* utile.c */

void print_matrix (matrix state);
int egals_matrix (matrix a, matrix b);
void copy_matrix (matrix source, matrix dest);
void copy_word (octet source[4], octet dest[4]);
void clean_matrix (matrix state);
int get_key (char* file_keys, matrix key);
void transpose_matrix(matrix state);
octet mod_irr_gf (unsigned int val);
octet mult_gf (octet a, octet b);
char* combine_strings(char* file, char* protocole, char* mode, char* extension);
void ask_key(int show_key);
int getch(); 
void xor_word(octet wres[4], octet w1[4], octet w2[4]);
void xor_state(matrix wres, matrix w);
void calculate_mult();
octet mult_calc_gf(octet a, octet b);
void get_table(octet a);
void print_mix_col();
void ask_init_vector();
void ask_hexa_key();

/* fonctions.c */

void sub_bytes (matrix state);
void inv_sub_bytes (matrix state);
void shift_rows (matrix state);
void inv_shift_rows (matrix state);
void add_key (matrix key, matrix state);
void g_function (octet last_word[4]);
void key_expension(keyt key);
void SubWord(word w);
void all_key_expension(keyt key, octet w[Nb*(Nr+1)][4]);
void mix_column(matrix state);
void inv_mix_column(matrix state);
void cipher (matrix state);
void cipher_inverse(matrix state);
void aes_ecb (matrix state);
void aes_ecb_inverse (matrix state);
void aes_cbc (matrix state);
void aes_cbc_inverse (matrix state);

void rotate (octet word[4]);
void rotate_n (octet word[4], int n);

/* tests.c */

void test_sub_bytes ();
void test_shift_rows ();
void test_add_key();
void test_key_expension();
void test_mix_column();
void test_aes();

/* files.c */

void write_matrix (FILE* output, matrix state);
int file_operation (char* input_name, char* output_name, void (*p_process) (matrix state), int crypt);

/* p_bmp.c */

#include "bitmap.h"

int is_bmp (char* input_name);
int bitmap_process (char* input_name, char* output_name, void (*p_process) (matrix state));

/* Entropie */

double calculate_entropy(char* file_name);