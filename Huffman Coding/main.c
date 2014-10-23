/**************************************

  [main.c] - a main file.

   This program encode ASCII text
  with Huffman-coding.

  > Date: April 7 2014
  > Contact: seeker5084@gmail.com
  > Written by: Hideyuki Tabata

***************************************/

#include <stdio.h>
#include "huffman.h"

bool main()
{
  char binary[N] = {0};
  unsigned int frequency[N] = {0};
  const char text[] = "eeny meeny miny moe";
  int i, num = getFrequency(text, frequency);
  node **nodes = allocNodes(nodes, 2*num-1);
  leaf **leaves = allocLeaves(leaves, num);
	fprintf(stdout, "Original text: ");
	puts(text);

  nodes = setupNodes(nodes, frequency, num);
  nodes = makeHuffmanTree(nodes, num);
  createCodes(nodes[2*num-2], leaves, binary, 0);
  printCodeList(leaves, num);

  releaseNodes(nodes, 2*num-1);
  releaseLeaves(leaves, num);

  return true;
}
