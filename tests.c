#include "all.h"

extern matrix round_keys[Nr+1];

matrix matrix_source =  {0x00,0x11,0x22,0x33,
                        0x44,0x55,0x66,0x77,
                        0x88,0x99,0xaa,0xbb,
                        0xcc,0xdd,0xee,0xff};

matrix work_matrix =    {0x00,0x11,0x22,0x33,
                        0x44,0x55,0x66,0x77,
                        0x88,0x99,0xaa,0xbb,
                        0xcc,0xdd,0xee,0xff};

// Affiche si le test "test" est passe et renvoie "test_name" avec le succes ou non
void test_print (int test, char* test_name) {
    if (test)
        printf("Test %s %sOK%s\n", test_name, green, reset);
    else
        printf("Test %s %sNON OK%s\n", test_name, red, reset);
}

// Tests fonction sub_byte et son inverse
void test_sub_bytes () {
    matrix matrice_expected_SB = {0xd4,0xe0,0xb8,0x1e,
                                  0x27,0xbf,0xb4,0x41,
                                  0x11,0x98,0x5d,0x52,
                                  0xae,0xf1,0xe5,0x30};

    matrix matrice_expected_ISB = {0x19,0xa0,0x9a,0xe9,
                                   0x3d,0xf4,0xc6,0xf8,
                                   0xe3,0xe2,0x8d,0x48,
                                   0xbe,0x2b,0x2a,0x08};
    copy_matrix(matrice_expected_ISB, work_matrix);
    sub_bytes(work_matrix);
    test_print (egals_matrix(work_matrix, matrice_expected_SB), "sub_bytes");
    
    /* Test de l'inverse */

    inv_sub_bytes(work_matrix);
    test_print (egals_matrix(work_matrix, matrice_expected_ISB), "inv_sub_bytes");
}

// Tests fonction shift_rows et son inverse
void test_shift_rows () {
    matrix matrice_expected_ISR = {  1, 2, 3, 4,
                                     5, 6, 7, 8,
                                     9,10,11,12,
                                    13,14,15,16};
    transpose_matrix(matrice_expected_ISR);

    matrix matrice_expected_SR = {  1, 2, 3, 4,
                                    6, 7, 8, 5,
                                   11,12, 9,10,
                                   16,13,14,15};
    transpose_matrix(matrice_expected_SR);

    copy_matrix(matrice_expected_ISR, work_matrix);

    shift_rows(work_matrix);
    test_print (egals_matrix(work_matrix, matrice_expected_SR), "shift_rows");
    
    /* Test de l'inverse */

    inv_shift_rows(work_matrix);
    test_print (egals_matrix(work_matrix, matrice_expected_ISR), "inv_shift_rows");
}

void test_add_key () {
    matrix matrice_expected_AK = {0x00,0x00,0x00,0x00,
                                  0x00,0x00,0x00,0x00,
                                  0x00,0x00,0x00,0x00,
                                  0x00,0x00,0x00,0x00};

    add_key(work_matrix, work_matrix);
    test_print (egals_matrix(work_matrix, matrice_expected_AK), "add_key");
}

/* Mix Columns */

void test_mix_column(){
    matrix matrice_expected_MC = {0x04,0xe0,0x48,0x28,
                                  0x66,0xcb,0xf8,0x06,
                                  0x81,0x19,0xd3,0x26,
                                  0xe5,0x9a,0x7a,0x4c};
    transpose_matrix(matrice_expected_MC);

    matrix matrice_expected_IMC = {0xd4,0xe0,0xb8,0x1e,
                                  0xbf,0xb4,0x41,0x27,
                                  0x5d,0x52,0x11,0x98,
                                  0x30,0xae,0xf1,0xe5};
    transpose_matrix(matrice_expected_IMC);

    copy_matrix(matrice_expected_IMC, work_matrix);

    //print_matrix(work_matrix);
    mix_column(work_matrix);
    //print_matrix(work_matrix);

    test_print (egals_matrix(work_matrix, matrice_expected_MC), "mix_columns");

    inv_mix_column(work_matrix);
    // print_matrix(work_matrix);

    test_print (egals_matrix(work_matrix, matrice_expected_IMC), "inverse_mix_columns");
}


#ifdef AES192
    keyt keys = {0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f,0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17};
    
    /* Key Expension */

    void test_key_expension() {
        key_expension(keys);

        keyt last_key = {0xa4,0x97,0x0a,0x33,0x1a,0x78,0xdc,0x09,0xc4,0x18,0xc2,0x71,0xe3,0xa4,0x1d,0x5d};
        
        test_print(egals_matrix(round_keys[12], last_key), "key_expension_192");
    }

    void test_aes () {
        matrix source_state2 = {};
        matrix output_state = {0xdd,0xa9,0x7c,0xa4,0x86,0x4c,0xdf,0xe0,0x6e,0xaf,0x70,0xa0,0xec,0x0d,0x71,0x91};  
        
        copy_matrix(matrix_source, source_state2);

        key_expension(keys);

        cipher(source_state2);
        test_print (egals_matrix(source_state2, output_state), "cipher");

        cipher_inverse(source_state2);
        test_print (egals_matrix(source_state2, matrix_source), "cipher_inverse");
    }
#elif AES256
    keyt keys = {0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f,0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1a,0x1b,0x1c,0x1d,0x1e,0x1f};
    
    /* Key Expension */

    void test_key_expension() {
        key_expension(keys);

        keyt last_key = {0x24,0xfc,0x79,0xcc,0xbf,0x09,0x79,0xe9,0x37,0x1a,0xc2,0x3c,0x6d,0x68,0xde,0x36};
        
        test_print(egals_matrix(round_keys[14], last_key), "key_expension_256");
    }

    void test_aes () {
        matrix source_state2 = {};
        matrix output_state = {0x8e,0xa2,0xb7,0xca,0x51,0x67,0x45,0xbf,0xea,0xfc,0x49,0x90,0x4b,0x49,0x60,0x89};  
        
        copy_matrix(matrix_source, source_state2);

        key_expension(keys);

        cipher(source_state2);
        test_print (egals_matrix(source_state2, output_state), "cipher");

        cipher_inverse(source_state2);
        test_print (egals_matrix(source_state2, matrix_source), "cipher_inverse");
    }
#else
    keyt keys = {0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f};
    
    /* Key Expension */

    void test_key_expension() {
        key_expension(keys);

        keyt last_key = {0x13,0x11,0x1d,0x7f,0xe3,0x94,0x4a,0x17,0xf3,0x07,0xa7,0x8b,0x4d,0x2b,0x30,0xc5};
        
        test_print(egals_matrix(round_keys[10], last_key), "key_expension_128");
    }

    /* AES */

    void test_aes () {
        matrix source_state2 = {};
        matrix output_state = {0x69,0xc4,0xe0,0xd8,0x6a,0x7b,0x04,0x30,0xd8,0xcd,0xb7,0x80,0x70,0xb4,0xc5,0x5a};  
        
        copy_matrix(matrix_source, source_state2);

        key_expension(keys);

        cipher(source_state2);
        test_print (egals_matrix(source_state2, output_state), "cipher");

        cipher_inverse(source_state2);
        test_print (egals_matrix(source_state2, matrix_source), "cipher_inverse");
    }
#endif

