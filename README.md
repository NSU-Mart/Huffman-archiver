# Huffman-archiver


## Description
This project implements the Huffman algorithm to encode and decode files. The Huffman algorithm is a lossless data compression method that uses variable code lengths to represent characters based on their frequency of use.

## Compilation
Use the following command to compile the program:
gcc -o huffman main.c huffman.c -lm


## Usage
The program can encode and decode files. To use the program, perform the following steps:

### Encoding a file
To encode a file, type encode at the operation selection prompt (it should look like this: Select operation (encodeFile/decodeFile):encodeFile) followed by the name of the file to be encoded (if it is in the project folder) or the name and full path to it (if the file is separate). This will create an encoded file with `.huff` extension.

### Decoding the file
To encode a file, type encode in the operation selection line (it should look like this: Select operation (encodeFile/decodeFile):decodeFile) after specifying the name of the file to be decoded (if it is in the project folder) or the name and full path to it (if the file is separate). This will restore the original file from the encoded `.huff` file. (if there is already a file with the same name in the project folder, the program will overwrite it)

## Project structure
- `main.c`: Main source code file containing the user interface logic.
- `huffman.h`: Header file containing structure and function definitions for the Huffman algorithm.
- `huffman.c`: Source code file containing the implementation of the Huffman algorithm.

## Authors
- Artem Martynov, 1st year student of IIR NSU
