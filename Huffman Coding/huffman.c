/**************************************

  [huffman.c] - a definition file.

   This program encode ASCII text
  with Huffman-coding.

  > Date: April 7 2014
  > Contact: seeker5084@gmail.com
  > Written by: Hideyuki Tabata

***************************************/

#include <stdio.h>
#include <stdlib.h>
#include "huffman.h"

struct node {
  struct node *next;
  struct node *left;
  struct node *right;
  unsigned int frequency;
  char character;
  bool catched;
};

struct leaf {
  int bit;
  int code;
  char character;
};

void error(char *msg)
{
  printf("Error Message: %s.\nbye...\n", msg);
  exit(1);
}

void initNodes(node **nodes, int num)
{
  int j;

  for (j = 0; j < num; j++) {
    nodes[j]->next = NULL;
    nodes[j]->left = NULL;
    nodes[j]->right = NULL;
    nodes[j]->frequency = 0;
    nodes[j]->character = '\0';
    nodes[j]->catched = false;
  }
}

void initLeaves(leaf **leaves, int num)
{
  int j;

  for (j = 0; j < num; j++) {
    leaves[j]->bit = 0;
    leaves[j]->code = 0;
    leaves[j]->character = '\0';
  }
}

node** allocNodes(node **nodes, int num)
{
  int i;

  nodes = (node**)malloc(sizeof(node*) * num);
  for (i = 0; i < num; i++) nodes[i] = (node*)malloc(sizeof(node));
  if (!nodes) error("Can not allocate struct node");
  initNodes(nodes, num);

  return nodes;
}

leaf** allocLeaves(leaf **leaves, int num)
{
  int i;

  leaves = (leaf**)malloc(sizeof(leaf*) * num);
  for (i = 0; i < num; i++) leaves[i] = (leaf*)malloc(sizeof(leaf));
  if (!leaves) error("Can not allocate struct leaf");
  initLeaves(leaves, num);

  return leaves;
}

void releaseNodes(node **nodes, int num)
{
	int i;

	for (i = 0; i < num; i++) free(nodes[i]);
	free(nodes);
	nodes = NULL;
}

void releaseLeaves(leaf **leaves, int num)
{
  int i;

  for (i = 0; i < num; i++) free(leaves[i]);
  free(leaves);
  leaves = NULL;
}

node** setupNodes(node **nodes, unsigned int frequency[], int num)
{
  int i, j;

  for (i = j = 0; j < num; i++) {
    if (frequency[i] != 0) {
      nodes[j]->next = (j != num-1) ? nodes[j+1] : NULL;
      nodes[j]->frequency = frequency[i];
      nodes[j++]->character = i;
    }
  }

  return nodes;
}

void insertNode(node **nodes, int index, int num)
{
  int j;

  for (j = 0; j < 2*num-1; j++) {
    if (nodes[j]->catched == false && !(nodes[j]->next)) {
      nodes[j]->next = nodes[index+num];
      return;
    }
  }
}

node* getNodesHead(node **nodes, int num)
{
  int j;

  for (j = 0; j < 2*num-1; j++) {
    if (nodes[j]->catched == false) return nodes[j];
  }

  return NULL;
}

int findMinFreq(node *current)
{
  int minFreq;

  for (minFreq = current->frequency; current; current = current->next) {
    if (minFreq > (current->frequency)) minFreq = current->frequency;
  }

  return minFreq;
}

node* extractMin(node **nodes, int num)
{
	node *head = getNodesHead(nodes, num);
  node *prev = NULL;
  node *current = head;
  node *next = current->next;
  int minFreq = findMinFreq(current);

  for (current = head; current; current = current->next) {
    next = current->next;
    if (minFreq == current->frequency) {
      if (!next && prev) prev->next = NULL;
      else if (prev) prev->next = next;
      break;
    }
    prev = current;
  }

  current->next = NULL;
  current->catched = true;

  return current;
}

void createCodes(node *nodes, leaf **leaves, char binary[], int level)
{
  int j;

  if (nodes->left) {
    binary[level] = 0;
    createCodes(nodes->left, leaves, binary, level+1);
  }
  if (nodes->right) {
    binary[level] = 1;
    createCodes(nodes->right, leaves, binary, level+1);
  }
  if (!(nodes->left) && !(nodes->right)) {
    for (; (*leaves)->bit != 0; leaves++);
    (*leaves)->bit = level;
    (*leaves)->character = nodes->character;
    for (j = 0; j < level; j++) {
      (*leaves)->code <<= 1;
      (*leaves)->code |= binary[j];
    }
  }
}

node** makeHuffmanTree(node **nodes, int num)
{
  int i;

  for (i = 0; i < num-1; i++) {
    nodes[i+num]->left = extractMin(nodes, num);
    nodes[i+num]->right = extractMin(nodes, num);
    nodes[i+num]->frequency = nodes[i+num]->left->frequency + nodes[i+num]->right->frequency;
    insertNode(nodes, i, num);
  }

  return nodes;
}

int getFrequency(const char text[], unsigned int frequency[])
{
  int i, num = 0;
  const char *pointer = text;

  while (*pointer != '\0') frequency[*pointer++]++;
  for (i = 0; i < N; i++) if (frequency[i] != 0) num++;

  return num;
}

void printBinary(int code, int bit)
{
  int i, binary[N] = {0};

  if (bit == 0) puts(" ");
  for (i = 0; code != 0; i++) {
    binary[bit-i-1] = code % 2;
    code = (code%2 == 1) ? (code-1)/2 : code/2;
  }
  for (i = 0; i < bit; i++) printf("%i", binary[i]);
  puts("");
}

void printCodeList(leaf **leaves, int num)
{
  int i;

  for (i = 0; i < num; i++) {
    printf("'%c' : ", leaves[i]->character);
    printBinary(leaves[i]->code, leaves[i]->bit);
  }
}
