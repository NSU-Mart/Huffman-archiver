#ifndef HUFFMAN_H
#define HUFFMAN_H

// Определения структур
struct node {
    char character;
    int frequency;
    struct node *left;
    struct node *right;
    struct node *next;
};

struct table {
    char character;
    unsigned int num;
    int size;
    struct table *next;
};

// Определения функций
void message();
void process(char *argv[]);
void encodeFile(struct node **head, const char *filename);
void decodeFile(struct table **head, const char *filename);
void mergeSort(struct node **head);
struct node *sortedMerge(struct node *a, struct node *b);
void split(struct node *source, struct node **fhead, struct node **bhead);
void printTree(struct node *cur);
void createTree(struct node **head);
void updateList(struct node **head, struct node *node);
struct node *addNode(struct node *node1, struct node *node2);
struct table *getTableNode();
void createTable(struct table **header, struct node *cur, unsigned int number, int size);
void printTable(struct table *head);
int fileExists(const char *filename);
int checkExtension(const char *filename, const char *extension);
int conditions(char *argv[]);
char *fileName(const char *filename);
struct table *findNode(struct table *head, char c);
void fileEncoding(struct table *head, const char *filename);
void freeNodesList(struct node **head);
void freeHuffmanTable(struct table **head);
char findCharacter(struct table *head, unsigned int number, int size);

#endif // HUFFMAN_H
