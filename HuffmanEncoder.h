#pragma once
#include <string>
#include <unordered_map> // changed that to unordered_map
#include "tree.h"
#include "node.h"
#include <queue>

using namespace std;
#define debug 0

class HuffmanEncoder
{
	int operationMode = 0;
	/*
	0 = single symbol huffman encoding
	1 = modified single symbol huffman encoding
	2 = 2 block encoding
	*/
	int encodeTable = 0;
	/*
	Should the tree encoded? 
	*/

	unordered_map<string, string> encodingMap;
	unordered_map<string, int> bigramCount;

	int bigramTotal = 0;
	double bigrams = 0;
	string* fileText = new string();
	string* encText = new string();

	tree t;

	int singleSymbolFrequencies[256];	  //[a][b][c][d]
	int blockSymbolFrequencies[256][256]; //[ab][cd]
	int twoSymbolFrequencies[256][256];   //[a{b](c}d)
	int numberOfSymbols;
	

	void initStructures(string& path);
	
	
	//Frequencies
	void initializeFrequencies();
	void countSingleFrequencies();
	void countTwoSymbolFrequencies();
	void countBlockSymbolFrequencies();

	//build Tree
	void buildTree();
	void buildTreeStructure(priority_queue<node, vector<node>, greater<node>> &q);
	void buildTreeSS();
	void buildTreeMSS();
	void buildTreeBlock();
	void encodeTree(string & encodedTable); //Encodes 
	void decodeTree(string & file); //Reconstructs tree from string
	void insertCodingIntoTree(string coding, string symbol);
	
	//find encoding from tree
	void generateEncoding();


	//encode file text
	void encoder(node* n,string code);




	//statistics
	void printStatistics(double x);


	//debugging
	void printNonZeroFrequencies();

public:

	//Experiment start points
	void runSingleSymbol(string path = "source.txt");
	void runModifiedSS(string path = "source.txt");
	void runTwoBlock(string path = "source.txt");

	void runAll(string path = "source.txt");
	void encode(string path = "enc.bin");
	void decode(string writePath = "dec.txt", string readPath = "enc.bin");


	HuffmanEncoder();
};

