#include "all.h"

extern matrix round_keys[Nr+1];

/* Sub Bytes  */

static const octet sbox[256] = {
    0x63,0x7c,0x77,0x7b,0xf2,0x6b,0x6f,0xc5,0x30,0x01,0x67,0x2b,0xfe,0xd7,0xab,0x76,
    0xca,0x82,0xc9,0x7d,0xfa,0x59,0x47,0xf0,0xad,0xd4,0xa2,0xaf,0x9c,0xa4,0x72,0xc0,
    0xb7,0xfd,0x93,0x26,0x36,0x3f,0xf7,0xcc,0x34,0xa5,0xe5,0xf1,0x71,0xd8,0x31,0x15,
    0x04,0xc7,0x23,0xc3,0x18,0x96,0x05,0x9a,0x07,0x12,0x80,0xe2,0xeb,0x27,0xb2,0x75,
    0x09,0x83,0x2c,0x1a,0x1b,0x6e,0x5a,0xa0,0x52,0x3b,0xd6,0xb3,0x29,0xe3,0x2f,0x84,
    0x53,0xd1,0x00,0xed,0x20,0xfc,0xb1,0x5b,0x6a,0xcb,0xbe,0x39,0x4a,0x4c,0x58,0xcf,
    0xd0,0xef,0xaa,0xfb,0x43,0x4d,0x33,0x85,0x45,0xf9,0x02,0x7f,0x50,0x3c,0x9f,0xa8,
    0x51,0xa3,0x40,0x8f,0x92,0x9d,0x38,0xf5,0xbc,0xb6,0xda,0x21,0x10,0xff,0xf3,0xd2,
    0xcd,0x0c,0x13,0xec,0x5f,0x97,0x44,0x17,0xc4,0xa7,0x7e,0x3d,0x64,0x5d,0x19,0x73,
    0x60,0x81,0x4f,0xdc,0x22,0x2a,0x90,0x88,0x46,0xee,0xb8,0x14,0xde,0x5e,0x0b,0xdb,
    0xe0,0x32,0x3a,0x0a,0x49,0x06,0x24,0x5c,0xc2,0xd3,0xac,0x62,0x91,0x95,0xe4,0x79,
    0xe7,0xc8,0x37,0x6d,0x8d,0xd5,0x4e,0xa9,0x6c,0x56,0xf4,0xea,0x65,0x7a,0xae,0x08,
    0xba,0x78,0x25,0x2e,0x1c,0xa6,0xb4,0xc6,0xe8,0xdd,0x74,0x1f,0x4b,0xbd,0x8b,0x8a,
    0x70,0x3e,0xb5,0x66,0x48,0x03,0xf6,0x0e,0x61,0x35,0x57,0xb9,0x86,0xc1,0x1d,0x9e,
    0xe1,0xf8,0x98,0x11,0x69,0xd9,0x8e,0x94,0x9b,0x1e,0x87,0xe9,0xce,0x55,0x28,0xdf,
    0x8c,0xa1,0x89,0x0d,0xbf,0xe6,0x42,0x68,0x41,0x99,0x2d,0x0f,0xb0,0x54,0xbb,0x16
};

static const octet rsbox[256] = {
    0x52,0x09,0x6a,0xd5,0x30,0x36,0xa5,0x38,0xbf,0x40,0xa3,0x9e,0x81,0xf3,0xd7,0xfb,
    0x7c,0xe3,0x39,0x82,0x9b,0x2f,0xff,0x87,0x34,0x8e,0x43,0x44,0xc4,0xde,0xe9,0xcb,
    0x54,0x7b,0x94,0x32,0xa6,0xc2,0x23,0x3d,0xee,0x4c,0x95,0x0b,0x42,0xfa,0xc3,0x4e,
    0x08,0x2e,0xa1,0x66,0x28,0xd9,0x24,0xb2,0x76,0x5b,0xa2,0x49,0x6d,0x8b,0xd1,0x25,
    0x72,0xf8,0xf6,0x64,0x86,0x68,0x98,0x16,0xd4,0xa4,0x5c,0xcc,0x5d,0x65,0xb6,0x92,
    0x6c,0x70,0x48,0x50,0xfd,0xed,0xb9,0xda,0x5e,0x15,0x46,0x57,0xa7,0x8d,0x9d,0x84,
    0x90,0xd8,0xab,0x00,0x8c,0xbc,0xd3,0x0a,0xf7,0xe4,0x58,0x05,0xb8,0xb3,0x45,0x06,
    0xd0,0x2c,0x1e,0x8f,0xca,0x3f,0x0f,0x02,0xc1,0xaf,0xbd,0x03,0x01,0x13,0x8a,0x6b,
    0x3a,0x91,0x11,0x41,0x4f,0x67,0xdc,0xea,0x97,0xf2,0xcf,0xce,0xf0,0xb4,0xe6,0x73,
    0x96,0xac,0x74,0x22,0xe7,0xad,0x35,0x85,0xe2,0xf9,0x37,0xe8,0x1c,0x75,0xdf,0x6e,
    0x47,0xf1,0x1a,0x71,0x1d,0x29,0xc5,0x89,0x6f,0xb7,0x62,0x0e,0xaa,0x18,0xbe,0x1b,
    0xfc,0x56,0x3e,0x4b,0xc6,0xd2,0x79,0x20,0x9a,0xdb,0xc0,0xfe,0x78,0xcd,0x5a,0xf4,
    0x1f,0xdd,0xa8,0x33,0x88,0x07,0xc7,0x31,0xb1,0x12,0x10,0x59,0x27,0x80,0xec,0x5f,
    0x60,0x51,0x7f,0xa9,0x19,0xb5,0x4a,0x0d,0x2d,0xe5,0x7a,0x9f,0x93,0xc9,0x9c,0xef,
    0xa0,0xe0,0x3b,0x4d,0xae,0x2a,0xf5,0xb0,0xc8,0xeb,0xbb,0x3c,0x83,0x53,0x99,0x61,
    0x17,0x2b,0x04,0x7e,0xba,0x77,0xd6,0x26,0xe1,0x69,0x14,0x63,0x55,0x21,0x0c,0x7d
};

static const octet mult_2[256] = {
    0x0,0x2,0x4,0x6,0x8,0xA,0xC,0xE,0x10,0x12,0x14,0x16,0x18,0x1A,0x1C,0x1E,
    0x20,0x22,0x24,0x26,0x28,0x2A,0x2C,0x2E,0x30,0x32,0x34,0x36,0x38,0x3A,0x3C,
    0x3E,0x40,0x42,0x44,0x46,0x48,0x4A,0x4C,0x4E,0x50,0x52,0x54,0x56,0x58,0x5A,
    0x5C,0x5E,0x60,0x62,0x64,0x66,0x68,0x6A,0x6C,0x6E,0x70,0x72,0x74,0x76,0x78,
    0x7A,0x7C,0x7E,0x80,0x82,0x84,0x86,0x88,0x8A,0x8C,0x8E,0x90,0x92,0x94,0x96,
    0x98,0x9A,0x9C,0x9E,0xA0,0xA2,0xA4,0xA6,0xA8,0xAA,0xAC,0xAE,0xB0,0xB2,0xB4,
    0xB6,0xB8,0xBA,0xBC,0xBE,0xC0,0xC2,0xC4,0xC6,0xC8,0xCA,0xCC,0xCE,0xD0,0xD2,
    0xD4,0xD6,0xD8,0xDA,0xDC,0xDE,0xE0,0xE2,0xE4,0xE6,0xE8,0xEA,0xEC,0xEE,0xF0,
    0xF2,0xF4,0xF6,0xF8,0xFA,0xFC,0xFE,0x1B,0x19,0x1F,0x1D,0x13,0x11,0x17,0x15,
    0xB,0x9,0xF,0xD,0x3,0x1,0x7,0x5,0x3B,0x39,0x3F,0x3D,0x33,0x31,0x37,
    0x35,0x2B,0x29,0x2F,0x2D,0x23,0x21,0x27,0x25,0x5B,0x59,0x5F,0x5D,0x53,0x51,
    0x57,0x55,0x4B,0x49,0x4F,0x4D,0x43,0x41,0x47,0x45,0x7B,0x79,0x7F,0x7D,0x73,
    0x71,0x77,0x75,0x6B,0x69,0x6F,0x6D,0x63,0x61,0x67,0x65,0x9B,0x99,0x9F,0x9D,
    0x93,0x91,0x97,0x95,0x8B,0x89,0x8F,0x8D,0x83,0x81,0x87,0x85,0xBB,0xB9,0xBF,
    0xBD,0xB3,0xB1,0xB7,0xB5,0xAB,0xA9,0xAF,0xAD,0xA3,0xA1,0xA7,0xA5,0xDB,0xD9,
    0xDF,0xDD,0xD3,0xD1,0xD7,0xD5,0xCB,0xC9,0xCF,0xCD,0xC3,0xC1,0xC7,0xC5,0xFB,
    0xF9,0xFF,0xFD,0xF3,0xF1,0xF7,0xF5,0xEB,0xE9,0xEF,0xED,0xE3,0xE1,0xE7,0xE5
};

static const octet mult_3[256] = {
    0x0,0x3,0x6,0x5,0xC,0xF,0xA,0x9,0x18,0x1B,0x1E,0x1D,0x14,0x17,0x12,0x11,
    0x30,0x33,0x36,0x35,0x3C,0x3F,0x3A,0x39,0x28,0x2B,0x2E,0x2D,0x24,0x27,0x22,
    0x21,0x60,0x63,0x66,0x65,0x6C,0x6F,0x6A,0x69,0x78,0x7B,0x7E,0x7D,0x74,0x77,
    0x72,0x71,0x50,0x53,0x56,0x55,0x5C,0x5F,0x5A,0x59,0x48,0x4B,0x4E,0x4D,0x44,
    0x47,0x42,0x41,0xC0,0xC3,0xC6,0xC5,0xCC,0xCF,0xCA,0xC9,0xD8,0xDB,0xDE,0xDD,
    0xD4,0xD7,0xD2,0xD1,0xF0,0xF3,0xF6,0xF5,0xFC,0xFF,0xFA,0xF9,0xE8,0xEB,0xEE,
    0xED,0xE4,0xE7,0xE2,0xE1,0xA0,0xA3,0xA6,0xA5,0xAC,0xAF,0xAA,0xA9,0xB8,0xBB,
    0xBE,0xBD,0xB4,0xB7,0xB2,0xB1,0x90,0x93,0x96,0x95,0x9C,0x9F,0x9A,0x99,0x88,
    0x8B,0x8E,0x8D,0x84,0x87,0x82,0x81,0x9B,0x98,0x9D,0x9E,0x97,0x94,0x91,0x92,
    0x83,0x80,0x85,0x86,0x8F,0x8C,0x89,0x8A,0xAB,0xA8,0xAD,0xAE,0xA7,0xA4,0xA1,
    0xA2,0xB3,0xB0,0xB5,0xB6,0xBF,0xBC,0xB9,0xBA,0xFB,0xF8,0xFD,0xFE,0xF7,0xF4,
    0xF1,0xF2,0xE3,0xE0,0xE5,0xE6,0xEF,0xEC,0xE9,0xEA,0xCB,0xC8,0xCD,0xCE,0xC7,
    0xC4,0xC1,0xC2,0xD3,0xD0,0xD5,0xD6,0xDF,0xDC,0xD9,0xDA,0x5B,0x58,0x5D,0x5E,
    0x57,0x54,0x51,0x52,0x43,0x40,0x45,0x46,0x4F,0x4C,0x49,0x4A,0x6B,0x68,0x6D,
    0x6E,0x67,0x64,0x61,0x62,0x73,0x70,0x75,0x76,0x7F,0x7C,0x79,0x7A,0x3B,0x38,
    0x3D,0x3E,0x37,0x34,0x31,0x32,0x23,0x20,0x25,0x26,0x2F,0x2C,0x29,0x2A,0xB,
    0x8,0xD,0xE,0x7,0x4,0x1,0x2,0x13,0x10,0x15,0x16,0x1F,0x1C,0x19,0x1A
};

static const octet mult_9[256] = {
    0x0,0x9,0x12,0x1B,0x24,0x2D,0x36,0x3F,0x48,0x41,0x5A,0x53,0x6C,0x65,0x7E,0x77,
    0x90,0x99,0x82,0x8B,0xB4,0xBD,0xA6,0xAF,0xD8,0xD1,0xCA,0xC3,0xFC,0xF5,0xEE,
    0xE7,0x3B,0x32,0x29,0x20,0x1F,0x16,0xD,0x4,0x73,0x7A,0x61,0x68,0x57,0x5E,
    0x45,0x4C,0xAB,0xA2,0xB9,0xB0,0x8F,0x86,0x9D,0x94,0xE3,0xEA,0xF1,0xF8,0xC7,
    0xCE,0xD5,0xDC,0x76,0x7F,0x64,0x6D,0x52,0x5B,0x40,0x49,0x3E,0x37,0x2C,0x25,
    0x1A,0x13,0x8,0x1,0xE6,0xEF,0xF4,0xFD,0xC2,0xCB,0xD0,0xD9,0xAE,0xA7,0xBC,
    0xB5,0x8A,0x83,0x98,0x91,0x4D,0x44,0x5F,0x56,0x69,0x60,0x7B,0x72,0x5,0xC,
    0x17,0x1E,0x21,0x28,0x33,0x3A,0xDD,0xD4,0xCF,0xC6,0xF9,0xF0,0xEB,0xE2,0x95,
    0x9C,0x87,0x8E,0xB1,0xB8,0xA3,0xAA,0xEC,0xE5,0xFE,0xF7,0xC8,0xC1,0xDA,0xD3,
    0xA4,0xAD,0xB6,0xBF,0x80,0x89,0x92,0x9B,0x7C,0x75,0x6E,0x67,0x58,0x51,0x4A,
    0x43,0x34,0x3D,0x26,0x2F,0x10,0x19,0x2,0xB,0xD7,0xDE,0xC5,0xCC,0xF3,0xFA,
    0xE1,0xE8,0x9F,0x96,0x8D,0x84,0xBB,0xB2,0xA9,0xA0,0x47,0x4E,0x55,0x5C,0x63,
    0x6A,0x71,0x78,0xF,0x6,0x1D,0x14,0x2B,0x22,0x39,0x30,0x9A,0x93,0x88,0x81,
    0xBE,0xB7,0xAC,0xA5,0xD2,0xDB,0xC0,0xC9,0xF6,0xFF,0xE4,0xED,0xA,0x3,0x18,
    0x11,0x2E,0x27,0x3C,0x35,0x42,0x4B,0x50,0x59,0x66,0x6F,0x74,0x7D,0xA1,0xA8,
    0xB3,0xBA,0x85,0x8C,0x97,0x9E,0xE9,0xE0,0xFB,0xF2,0xCD,0xC4,0xDF,0xD6,0x31,
    0x38,0x23,0x2A,0x15,0x1C,0x7,0xE,0x79,0x70,0x6B,0x62,0x5D,0x54,0x4F,0x46
};
static const octet mult_B[256] = {
    0x0,0xB,0x16,0x1D,0x2C,0x27,0x3A,0x31,0x58,0x53,0x4E,0x45,0x74,0x7F,0x62,0x69,
    0xB0,0xBB,0xA6,0xAD,0x9C,0x97,0x8A,0x81,0xE8,0xE3,0xFE,0xF5,0xC4,0xCF,0xD2,
    0xD9,0x7B,0x70,0x6D,0x66,0x57,0x5C,0x41,0x4A,0x23,0x28,0x35,0x3E,0xF,0x4,
    0x19,0x12,0xCB,0xC0,0xDD,0xD6,0xE7,0xEC,0xF1,0xFA,0x93,0x98,0x85,0x8E,0xBF,
    0xB4,0xA9,0xA2,0xF6,0xFD,0xE0,0xEB,0xDA,0xD1,0xCC,0xC7,0xAE,0xA5,0xB8,0xB3,
    0x82,0x89,0x94,0x9F,0x46,0x4D,0x50,0x5B,0x6A,0x61,0x7C,0x77,0x1E,0x15,0x8,
    0x3,0x32,0x39,0x24,0x2F,0x8D,0x86,0x9B,0x90,0xA1,0xAA,0xB7,0xBC,0xD5,0xDE,
    0xC3,0xC8,0xF9,0xF2,0xEF,0xE4,0x3D,0x36,0x2B,0x20,0x11,0x1A,0x7,0xC,0x65,
    0x6E,0x73,0x78,0x49,0x42,0x5F,0x54,0xF7,0xFC,0xE1,0xEA,0xDB,0xD0,0xCD,0xC6,
    0xAF,0xA4,0xB9,0xB2,0x83,0x88,0x95,0x9E,0x47,0x4C,0x51,0x5A,0x6B,0x60,0x7D,
    0x76,0x1F,0x14,0x9,0x2,0x33,0x38,0x25,0x2E,0x8C,0x87,0x9A,0x91,0xA0,0xAB,
    0xB6,0xBD,0xD4,0xDF,0xC2,0xC9,0xF8,0xF3,0xEE,0xE5,0x3C,0x37,0x2A,0x21,0x10,
    0x1B,0x6,0xD,0x64,0x6F,0x72,0x79,0x48,0x43,0x5E,0x55,0x1,0xA,0x17,0x1C,
    0x2D,0x26,0x3B,0x30,0x59,0x52,0x4F,0x44,0x75,0x7E,0x63,0x68,0xB1,0xBA,0xA7,
    0xAC,0x9D,0x96,0x8B,0x80,0xE9,0xE2,0xFF,0xF4,0xC5,0xCE,0xD3,0xD8,0x7A,0x71,
    0x6C,0x67,0x56,0x5D,0x40,0x4B,0x22,0x29,0x34,0x3F,0xE,0x5,0x18,0x13,0xCA,
    0xC1,0xDC,0xD7,0xE6,0xED,0xF0,0xFB,0x92,0x99,0x84,0x8F,0xBE,0xB5,0xA8,0xA3
};

static const octet mult_D[256] = {
    0x0,0xD,0x1A,0x17,0x34,0x39,0x2E,0x23,0x68,0x65,0x72,0x7F,0x5C,0x51,0x46,0x4B,
    0xD0,0xDD,0xCA,0xC7,0xE4,0xE9,0xFE,0xF3,0xB8,0xB5,0xA2,0xAF,0x8C,0x81,0x96,
    0x9B,0xBB,0xB6,0xA1,0xAC,0x8F,0x82,0x95,0x98,0xD3,0xDE,0xC9,0xC4,0xE7,0xEA,
    0xFD,0xF0,0x6B,0x66,0x71,0x7C,0x5F,0x52,0x45,0x48,0x3,0xE,0x19,0x14,0x37,
    0x3A,0x2D,0x20,0x6D,0x60,0x77,0x7A,0x59,0x54,0x43,0x4E,0x5,0x8,0x1F,0x12,
    0x31,0x3C,0x2B,0x26,0xBD,0xB0,0xA7,0xAA,0x89,0x84,0x93,0x9E,0xD5,0xD8,0xCF,
    0xC2,0xE1,0xEC,0xFB,0xF6,0xD6,0xDB,0xCC,0xC1,0xE2,0xEF,0xF8,0xF5,0xBE,0xB3,
    0xA4,0xA9,0x8A,0x87,0x90,0x9D,0x6,0xB,0x1C,0x11,0x32,0x3F,0x28,0x25,0x6E,
    0x63,0x74,0x79,0x5A,0x57,0x40,0x4D,0xDA,0xD7,0xC0,0xCD,0xEE,0xE3,0xF4,0xF9,
    0xB2,0xBF,0xA8,0xA5,0x86,0x8B,0x9C,0x91,0xA,0x7,0x10,0x1D,0x3E,0x33,0x24,
    0x29,0x62,0x6F,0x78,0x75,0x56,0x5B,0x4C,0x41,0x61,0x6C,0x7B,0x76,0x55,0x58,
    0x4F,0x42,0x9,0x4,0x13,0x1E,0x3D,0x30,0x27,0x2A,0xB1,0xBC,0xAB,0xA6,0x85,
    0x88,0x9F,0x92,0xD9,0xD4,0xC3,0xCE,0xED,0xE0,0xF7,0xFA,0xB7,0xBA,0xAD,0xA0,
    0x83,0x8E,0x99,0x94,0xDF,0xD2,0xC5,0xC8,0xEB,0xE6,0xF1,0xFC,0x67,0x6A,0x7D,
    0x70,0x53,0x5E,0x49,0x44,0xF,0x2,0x15,0x18,0x3B,0x36,0x21,0x2C,0xC,0x1,
    0x16,0x1B,0x38,0x35,0x22,0x2F,0x64,0x69,0x7E,0x73,0x50,0x5D,0x4A,0x47,0xDC,
    0xD1,0xC6,0xCB,0xE8,0xE5,0xF2,0xFF,0xB4,0xB9,0xAE,0xA3,0x80,0x8D,0x9A,0x97
};

static const octet mult_E[256] = {
    0x0,0xE,0x1C,0x12,0x38,0x36,0x24,0x2A,0x70,0x7E,0x6C,0x62,0x48,0x46,0x54,0x5A,
    0xE0,0xEE,0xFC,0xF2,0xD8,0xD6,0xC4,0xCA,0x90,0x9E,0x8C,0x82,0xA8,0xA6,0xB4,
    0xBA,0xDB,0xD5,0xC7,0xC9,0xE3,0xED,0xFF,0xF1,0xAB,0xA5,0xB7,0xB9,0x93,0x9D,
    0x8F,0x81,0x3B,0x35,0x27,0x29,0x3,0xD,0x1F,0x11,0x4B,0x45,0x57,0x59,0x73,
    0x7D,0x6F,0x61,0xAD,0xA3,0xB1,0xBF,0x95,0x9B,0x89,0x87,0xDD,0xD3,0xC1,0xCF,
    0xE5,0xEB,0xF9,0xF7,0x4D,0x43,0x51,0x5F,0x75,0x7B,0x69,0x67,0x3D,0x33,0x21,
    0x2F,0x5,0xB,0x19,0x17,0x76,0x78,0x6A,0x64,0x4E,0x40,0x52,0x5C,0x6,0x8,
    0x1A,0x14,0x3E,0x30,0x22,0x2C,0x96,0x98,0x8A,0x84,0xAE,0xA0,0xB2,0xBC,0xE6,
    0xE8,0xFA,0xF4,0xDE,0xD0,0xC2,0xCC,0x41,0x4F,0x5D,0x53,0x79,0x77,0x65,0x6B,
    0x31,0x3F,0x2D,0x23,0x9,0x7,0x15,0x1B,0xA1,0xAF,0xBD,0xB3,0x99,0x97,0x85,
    0x8B,0xD1,0xDF,0xCD,0xC3,0xE9,0xE7,0xF5,0xFB,0x9A,0x94,0x86,0x88,0xA2,0xAC,
    0xBE,0xB0,0xEA,0xE4,0xF6,0xF8,0xD2,0xDC,0xCE,0xC0,0x7A,0x74,0x66,0x68,0x42,
    0x4C,0x5E,0x50,0xA,0x4,0x16,0x18,0x32,0x3C,0x2E,0x20,0xEC,0xE2,0xF0,0xFE,
    0xD4,0xDA,0xC8,0xC6,0x9C,0x92,0x80,0x8E,0xA4,0xAA,0xB8,0xB6,0xC,0x2,0x10,
    0x1E,0x34,0x3A,0x28,0x26,0x7C,0x72,0x60,0x6E,0x44,0x4A,0x58,0x56,0x37,0x39,
    0x2B,0x25,0xF,0x1,0x13,0x1D,0x47,0x49,0x5B,0x55,0x7F,0x71,0x63,0x6D,0xD7,
    0xD9,0xCB,0xC5,0xEF,0xE1,0xF3,0xFD,0xA7,0xA9,0xBB,0xB5,0x9F,0x91,0x83,0x8D
};

// Fonction sub_bytes
// Modifie directement sur la matrice state d'entree
void sub_bytes (matrix state) {
    static int i;
    for(i = 0; i < Matrix_size; i++)
        state[i] = sbox[state[i]];
}

// Fonction inv_sub_bytes
// Modifie directement sur la matrice state d'entree
void inv_sub_bytes (matrix state) {
    static int i;
    for(i = 0; i < Matrix_size; i++)
        state[i] = rsbox[state[i]];
}

/* Shift Rows */

// Fonction shift_rows
// Modifie directement sur la matrice state d'entree

void shift_rows (matrix state) {
    /*
    matrix tmp; copy_matrix(state, tmp);

    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            state[i][j] = tmp[i][(j+i)%4];
    */

    static octet tmp1,tmp2; 
    tmp1 = state[1];
    state[1] = state[5];
    state[5] = state[9];
    state[9] = state[13];
    state[13] = tmp1;

    tmp1 = state[2];
    tmp2 = state[6];
    state[2] = state[10];
    state[10] = tmp1;
    state[6] = state[14];
    state[14] = tmp2; 

    tmp1 = state[3];
    state[3] = state[15];
    state[15] = state[11];
    state[11] = state[7];
    state[7] = tmp1;

}

// Fonction inv_shift_rows
// Modifie directement sur la matrice state d'entree
void inv_shift_rows (matrix state) {
    /*
    static matrix tmp; copy_matrix(state, tmp);

    static int i, j;
    for (i = 0; i < 4; i++)
        for (j = 0; j < 4; j++)
            state[i][j] = tmp[i][(j+(3*i))%4];
    */
    static octet tmp1,tmp2;
    tmp1 = state[13]; 
    state[13] = state[9];
    state[9] = state[5];
    state[5] = state[1];
    state[1] = tmp1;

    tmp1 = state[10];
    tmp2 = state[6];
    state[10] = state[2];
    state[2] = tmp1;
    state[6] = state[14];
    state[14] = tmp2;

    tmp1 = state[3];
    state[3] = state[7];
    state[7] = state[11];
    state[11] = state[15];
    state[15] = tmp1;
}

/* Key Expension */

// Réalise le xor entre les deux matrices key et state et le renvoie dans le state
void add_key (matrix state, matrix key) {
    static int i;
    for(i = 0; i < Matrix_size; i++)
        state[i] ^= key[i]; // xor entre chaque cases
}

void g_function (octet last_word[4]) {
    static octet tmp[4]; copy_word(last_word, tmp);
    static int i;

    for(i = 0; i < 4; i++)
        last_word[i] = tmp[(i+1)%4];

    for(i = 0; i < 4; i++)
        last_word[i] = sbox[last_word[i]];
}

extern int aes_type;

void sub_word(word w){
    static int i;
    for(i = 0; i < 4; i++)
        w[i] = sbox[w[i]];
}

void all_key_expension(keyt key, octet w[Nb*(Nr+1)][4]){
    int i = 0;
    word temp;
    while (i < Nk){
        for(int j = 0; j<4; j++){
            w[i][j] = key[4*i+j];
        }
        i += 1;
    }
    word r_con = {0x0,0x0,0x0,0x0};
    i = Nk;

    while (i < Nb * (Nr+1)){
        for(int j = 0; j<4; j++){   
            temp[j] = w[i-1][j];
        }
        if ((i % Nk) == 0){
            r_con[0] = mod_irr_gf((0x01)<<((i/Nk)-1));
            g_function(temp);
            xor_word(temp,temp, r_con);
        }
        else if ((Nk > 6) && (i % Nk == 4)){
            sub_word(temp);
        }
        xor_word(w[i],w[i-Nk], temp);
        i += 1;
    }
}

void key_expension(keyt key) {
    octet w[Nb*(Nr+1)][4] = {{}};

    all_key_expension(key, w);

    int select = 0, counter = 0;

    for(int i = 0; i < (Nb*(Nr+1)); i++){
        for(int j = 0; j<4; j++){
            round_keys[select][counter] = w[i][j];
            counter++;
            if (counter == Matrix_size) {
                counter = 0;
                select++;
            }
        }
    }
} 

void mix_column(matrix state) {
    static matrix tmp;

    copy_matrix(state, tmp);

    state[0] = mult_2[tmp[0]] ^ mult_3[tmp[1]] ^ tmp[2] ^ tmp[3];
    state[4] = mult_2[tmp[4]] ^ mult_3[tmp[5]] ^ tmp[6] ^ tmp[7];
    state[8] = mult_2[tmp[8]] ^ mult_3[tmp[9]] ^ tmp[10] ^ tmp[11];
    state[12] = mult_2[tmp[12]] ^ mult_3[tmp[13]] ^ tmp[14] ^ tmp[15];
    state[1] = tmp[0] ^ mult_2[tmp[1]] ^ mult_3[tmp[2]] ^ tmp[3];
    state[5] = tmp[4] ^ mult_2[tmp[5]] ^ mult_3[tmp[6]] ^ tmp[7];
    state[9] = tmp[8] ^ mult_2[tmp[9]] ^ mult_3[tmp[10]] ^ tmp[11];
    state[13] = tmp[12] ^ mult_2[tmp[13]] ^ mult_3[tmp[14]] ^ tmp[15];
    state[2] = tmp[0] ^ tmp[1] ^ mult_2[tmp[2]] ^ mult_3[tmp[3]];
    state[6] = tmp[4] ^ tmp[5] ^ mult_2[tmp[6]] ^ mult_3[tmp[7]];
    state[10] = tmp[8] ^ tmp[9] ^ mult_2[tmp[10]] ^ mult_3[tmp[11]];
    state[14] = tmp[12] ^ tmp[13] ^ mult_2[tmp[14]] ^ mult_3[tmp[15]];
    state[3] = mult_3[tmp[0]] ^ tmp[1] ^ tmp[2] ^ mult_2[tmp[3]];
    state[7] = mult_3[tmp[4]] ^ tmp[5] ^ tmp[6] ^ mult_2[tmp[7]];
    state[11] = mult_3[tmp[8]] ^ tmp[9] ^ tmp[10] ^ mult_2[tmp[11]];
    state[15] = mult_3[tmp[12]] ^ tmp[13] ^ tmp[14] ^ mult_2[tmp[15]];

    /*
    const static matrix M = {{0x02, 0x03, 0x01, 0x01},
                             {0x01, 0x02, 0x03, 0x01},
                             {0x01, 0x01, 0x02, 0x03},
                             {0x03, 0x01, 0x01, 0x02}};

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            state[i][j] = mult_calc_gf(M[i][0], tmp[0][j]) ^ mult_calc_gf(M[i][1], tmp[1][j])
                        ^ mult_calc_gf(M[i][2], tmp[2][j]) ^ mult_calc_gf(M[i][3], tmp[3][j]);
        }
    }
    */
   
}

void inv_mix_column(matrix state) {
    static matrix tmp;

    copy_matrix(state, tmp);

    state[0] = mult_E[tmp[0]] ^ mult_B[tmp[1]] ^ mult_D[tmp[2]] ^ mult_9[tmp[3]];
    state[4] = mult_E[tmp[4]] ^ mult_B[tmp[5]] ^ mult_D[tmp[6]] ^ mult_9[tmp[7]];
    state[8] = mult_E[tmp[8]] ^ mult_B[tmp[9]] ^ mult_D[tmp[10]] ^ mult_9[tmp[11]];
    state[12] = mult_E[tmp[12]] ^ mult_B[tmp[13]] ^ mult_D[tmp[14]] ^ mult_9[tmp[15]];
    state[1] = mult_9[tmp[0]] ^ mult_E[tmp[1]] ^ mult_B[tmp[2]] ^ mult_D[tmp[3]];
    state[5] = mult_9[tmp[4]] ^ mult_E[tmp[5]] ^ mult_B[tmp[6]] ^ mult_D[tmp[7]];
    state[9] = mult_9[tmp[8]] ^ mult_E[tmp[9]] ^ mult_B[tmp[10]] ^ mult_D[tmp[11]];
    state[13] = mult_9[tmp[12]] ^ mult_E[tmp[13]] ^ mult_B[tmp[14]] ^ mult_D[tmp[15]];
    state[2] = mult_D[tmp[0]] ^ mult_9[tmp[1]] ^ mult_E[tmp[2]] ^ mult_B[tmp[3]];
    state[6] = mult_D[tmp[4]] ^ mult_9[tmp[5]] ^ mult_E[tmp[6]] ^ mult_B[tmp[7]];
    state[10] = mult_D[tmp[8]] ^ mult_9[tmp[9]] ^ mult_E[tmp[10]] ^ mult_B[tmp[11]];
    state[14] = mult_D[tmp[12]] ^ mult_9[tmp[13]] ^ mult_E[tmp[14]] ^ mult_B[tmp[15]];
    state[3] = mult_B[tmp[0]] ^ mult_D[tmp[1]] ^ mult_9[tmp[2]] ^ mult_E[tmp[3]];
    state[7] = mult_B[tmp[4]] ^ mult_D[tmp[5]] ^ mult_9[tmp[6]] ^ mult_E[tmp[7]];
    state[11] = mult_B[tmp[8]] ^ mult_D[tmp[9]] ^ mult_9[tmp[10]] ^ mult_E[tmp[11]];
    state[15] = mult_B[tmp[12]] ^ mult_D[tmp[13]] ^ mult_9[tmp[14]] ^ mult_E[tmp[15]];

    /* 
    const static matrix M = {{0x0e, 0x0b, 0x0d, 0x09},
                             {0x09, 0x0e, 0x0b, 0x0d},
                             {0x0d, 0x09, 0x0e, 0x0b},
                             {0x0b, 0x0d, 0x09, 0x0e}};
    
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            state[i][j] = mult_calc_gf(M[i][0], tmp[0][j]) ^ mult_calc_gf(M[i][1], tmp[1][j])
                        ^ mult_calc_gf(M[i][2], tmp[2][j]) ^ mult_calc_gf(M[i][3], tmp[3][j]);
        }
    } */
}

void cipher (matrix state) {

    add_key(state, round_keys[0]);
    
    for (int i = 1; i < Nr; i++) {
        sub_bytes(state);
        shift_rows(state);
        mix_column(state);
        add_key(state, round_keys[i]);
    }

    sub_bytes(state);
    shift_rows(state);
    add_key (state,(round_keys[Nr]));
}

void cipher_inverse(matrix state) {
    add_key(state, round_keys[Nr]);

    for (int i = Nr; i > 1; i--) {
        inv_shift_rows(state);
        inv_sub_bytes(state);
        add_key(state, round_keys[i-1]);
        inv_mix_column(state);
    }

    inv_shift_rows(state);
    inv_sub_bytes(state);
    add_key(state, round_keys[0]);
}

extern matrix key;
extern matrix mem_cbc;

void aes_cbc (matrix state) {
    xor_state(state, mem_cbc); // xor
    cipher(state); // crypt
    copy_matrix(state, mem_cbc); // prochain vecteur
}

void aes_cbc_inverse (matrix state) {
    static matrix tmp_cbc_inv;
    copy_matrix(state, tmp_cbc_inv); // prochain vecteur
    cipher_inverse(state); // crypt inverse 
    xor_state(state, mem_cbc); // xor avevc vecteur
    copy_matrix(tmp_cbc_inv, mem_cbc); // prochain vecteur
}