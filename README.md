# AES Encryption and Decryption Project

This project provides an implementation of AES (Advanced Encryption Standard) encryption and decryption in C. It supports both ECB (Electronic Codebook) and CBC (Cipher Block Chaining) modes and can process both regular files and BMP image files.

## Features
- AES encryption and decryption
- Support for ECB and CBC modes
- Key sizes of 128, 192, and 256 bits
- Encryption and decryption of BMP images with the option to keep the header in plaintext
- Command-line interface for easy use
- Test functions to validate the implementation

## Usage
### Compiling the Program

To compile the program run:
```sh
make
```

### Running the Program

The program can be run in different modes: encryption, decryption, and testing. The general usage pattern is:

```sh
./aes <crypt|decrypt|test> <ecb|cbc|iecb|icbc> <show|hide|x|file_key> [input_files]
```

**Arguments** :
- crypt: Encrypt the input files.
- decrypt: Decrypt the input files.
- test: Run the test functions.
- ecb: Use ECB mode.
- cbc: Use CBC mode.
- iecb: Encrypt BMP files with the header in plaintext using ECB mode.
- icbc: Encrypt BMP files with the header in plaintext using CBC mode.
- show: Display the key.
- hide: Hide the key.
- x: Provide the key in hexadecimal format.
- file_key: Path to the file containing the key.
- [input_files]: List of input files to be processed.

**Shortcuts** :
- c: crypt
- d: decrypt
- t: test
- s: show
- x: hexadecimal
- h: hide

## Examples
Encrypt a file using ECB mode with a provided key:
```sh
./aes crypt ecb file_key input.txt
```

Decrypt a BMP image using CBC mode and a key in hexadecimal format:

```sh
./aes decrypt icbc x input.bmp
```

Run tests:

```sh
./aes test
```

## Makefile

The provided Makefile helps in compiling and cleaning the project efficiently.
Targets

- aes: Compiles all AES variants (128, 192, 256).
- aes128, aes192, aes256: Compile specific AES variants.
- entropie: Compiles the entropy calculation utility.
- clear: Removes compiled binaries and object files.
- clean: Removes only object files and temporary files.

## Contributing

Contributions are welcome. Please submit a pull request or open an issue to discuss potential changes.
