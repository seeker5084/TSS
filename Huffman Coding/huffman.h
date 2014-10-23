/**************************************

  [huffman.h] - a header file.

   This program encode ASCII text
  with Huffman-coding.
  Note that N (defined to 256)
  is a number of ASCII characters.

  > Date: April 7 2014
  > Contact: seeker5084@gmail.com
  > Written by: Hideyuki Tabata

***************************************/

#define bool int
#define true 1
#define false 0
#define N 256

typedef struct node node;
typedef struct leaf leaf;

void error(char*);
void initNodes(node**, int);
void initLeaves(leaf**, int);
void releaseNodes(node**, int);
void releaseLeaves(leaf**, int);
void insertNode(node**, int, int);
void createCodes(node*, leaf**, char [], int);
void printBinary(int, int);
void printCodeList(leaf**, int);

node** allocNodes(node**, int);
node** setupNodes(node**, unsigned int [], int);
node* getNodesHead(node**, int);
node* extractMin(node**, int);
node** makeHuffmanTree(node**, int);

leaf** allocLeaves(leaf**, int);

int findMinFreq(node*);
int getFrequency(const char [], unsigned int []);
