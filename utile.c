#include "all.h"

int irrGF[9] = {1,1,0,1,1,0,0,0,1};
extern matrix key;
extern matrix mem_cbc; 
octet mult_calculated [256][256] = {};

// Permet de calculer les 256*256 multiplications possibles
void calculate_mult () {
    for (int i = 0; i < 256; i++)
        for (int j = 0; j < 256; j++)
            mult_calculated[i][j] = mult_gf(i, j);
}

octet mult_calc_gf (octet a, octet b) {
    return mult_calculated[a][b];
}

void get_table (octet a) {
    printf("static const octet mult_%X[256] = {\n\t", a);
    for (int i = 0; i < 256; i++) {
        printf("0x%X", mult_calc_gf(a, i));
        if (i != 255)
            printf(",");
        if (i%15 == 0 && i != 0)
            printf("\n\t");
    }
    printf("};\n");
}

// affiche la matrice 4x4 state
void print_matrix (matrix state){
    for (int i = 0; i < Matrix_size; i++) {
        if (i % (Matrix_size / 4) == 0)
            printf("\n");
        printf("%02x ", state[i/4+(i%4)*4]);
    }
    printf("\n");
}

// verifie si deux matrices sont egales membre par membre
int egals_matrix (matrix a, matrix b) {
    int eq = 1;
    for (int i = 0; i < Matrix_size; i++) {
        if (a[i] != b[i]) {
            printf ("%s%d est fausse car %X != %X\n%s", red, i, a[i], b[i], reset);
            eq = 0;
        }
    }
    return eq;
}

// copie les elements de la matrice source vers la matrice dest
void copy_matrix (matrix source, matrix dest) {
    static int i;
    for (i = 0; i < Matrix_size; i++)
        dest[i] = source[i];
}

void copy_word (octet source[4], octet dest[4]) {
    static int i;
    for (i = 0; i < 4; i++)
        dest[i] = source[i];
}

// nettoie une matrice en remplacant toutes ses valeurs par des 0
void clean_matrix (matrix state) {
    static matrix mat_zero = { };
    copy_matrix (mat_zero, state);
}

void transpose_matrix(matrix state) {
    static matrix tmp; copy_matrix(state, tmp);

    static int i;
    for (i = 0; i < Matrix_size; i++)
        state[i] = tmp[i/4+(i%4)*4];
}

// Concatene 4 chaines de caractère en une seule, en ecrasant l'extension si elle est présente
char* combine_strings(char* file, char* protocole, char* mode, char* extension) {
    int l1 = strlen(file), l2 = strlen(protocole), l3 = strlen(mode), lext = strlen(extension);
    char* output = malloc((l1 + l2 + l3) * sizeof(char) + 1);

    if (output == NULL) 
        perror("malloc");
    else {
        for (int i = 0; i < l1 - lext; i++)
            output[i] = file[i];
        for (int i = 0; i < l2; i++)
            output[i + l1 - lext] = protocole[i];
        for (int i = 0; i < l3; i++)
            output[i + l1 + l2 - lext] = mode[i];
        for (int i = 0; i < lext; i++)
            output[i + l1 + l2 + l3 - lext] = extension[i];
        output[l1 + l2 + l3 + lext] = '\0';
    }
    return output;
}

int dozens (unsigned int value) {
    int counter = 0;
    while (value) {
        counter++;
        value >>= 1;
    }
    return counter;
}

// Modulo polynome irreductible dans gf
octet mod_irr_gf (unsigned int value) {
    unsigned int reduct = value;
    while (reduct > 255) {
        reduct ^= 283 << (dozens(reduct) - 9);
    }
    return reduct;
}

// Multiplication entre deux octets modulo l'irreductible de gf
octet mult_gf (octet a, octet b) {
    if (a > b) return mult_gf (b, a);

    int shift = 0;
    unsigned int result = 0;
    
    while (a > 0) {
        result ^= (a&1) * b << shift;
        a >>= 1;
        shift++;
    }

    return mod_irr_gf (result);
}

// Demande d'entrer la clé a l'utilisateur 
// show_key permet d'afficher ou non en clair la clé entrée
void ask_key(int show_key) {
    printf("Entrez la clé de chiffrement (sans accents) : ");
    octet c, i = 0;
    while (i < 4*Nk) {
        c = getch();
        if (c == 127 && i > 0) {
            printf("\nEntrez la clé de chiffrement (sans accents) : ");
            i--;
            for (int j = 0; j < i; j++)
                if (show_key) // On affiche les caractères
                    printf("%c", key[j]);
                else
                    printf("*");
        } else if (c >= ' ' && c != 127) {
            if (show_key) // On affiche les caractères
                printf("%c",c);
            else
                printf("*");
            key[i] = c;
            i++;
        }
    }
    printf("\n");
}

void ask_hexa_key() {
    octet c, i = 0, oct_vect, oct_write = 0;

    int number;

    printf("Entrez la clé de chiffrement (en hexadecimal) : ");
    while (i < 2*4*Nk) {
        c = getch();
        number = c >= '0' && c <= '9';
        if (number || (c >= 'a' && c <= 'f')) {
            printf("%c",c);
            i++;
            if ((i+1)%2 && i != 0) {
                oct_vect += c - (number?'0':87);
                key[oct_write] = oct_vect;
                oct_write++;
                printf(" ");
            } else
                oct_vect = (c - (number?'0':87)) << 4;
        }
    }
    printf("\n");
}

// getchar qui n'affiche pas sur le terminal
// source : https://stackoverflow.com/questions/34185216/getting-an-character-as-a-password-with-sign-and-match-with-your-password
int getch() {
    int ch;
    struct termios oldt, newt;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag = newt.c_lflag & ~(ICANON|ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

    return ch;
}

void xor_word(octet wres[4], octet w1[4], octet w2[4]) {
    static int i;
    for(i = 0; i<4; i++)
        wres[i] = w1[i] ^ w2[i];
}

void xor_state(matrix wres, matrix w) {
    static int i;
    for(i = 0; i < Matrix_size; i++)
        wres[i] ^= w[i];
}

void ask_init_vector() {
    printf("Entrez le vecteur d'initialisation (en hexadécimal) : ");

    octet c, i = 0, oct_vect, oct_write = 0;

    int number;

    while (i < 32) {
        c = getch();
        number = c >= '0' && c <= '9';
        if (number || (c >= 'a' && c <= 'f')) {
            printf("%c",c);
            i++;
            if ((i+1)%2 && i != 0) {
                oct_vect += c - (number?'0':87);
                mem_cbc[oct_write] = oct_vect;
                oct_write++;
                printf(" ");
            } else
                oct_vect = (c - (number?'0':87)) << 4;
        }
    }
    printf("\n");
}

/* void print_mix_col() {
    const matrix M = {{0x0e, 0x0b, 0x0d, 0x09},
                    {0x09, 0x0e, 0x0b, 0x0d},
                    {0x0d, 0x09, 0x0e, 0x0b},
                    {0x0b, 0x0d, 0x09, 0x0e}};

    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            printf("state[%d][%d] = mult_%X[tmp[0][%d]] ^ mult_%X[tmp[1][%d]] ^ mult_%X[tmp[2][%d]] ^ mult_%X[tmp[3][%d]];\n", i, j, M[i][0], j, M[i][1], j, M[i][2], j, M[i][3], j);
} */