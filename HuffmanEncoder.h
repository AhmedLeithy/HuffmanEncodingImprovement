#pragma once
#include <string>
#include <map>
using namespace std;
#define debug 1

class HuffmanEncoder
{
	int operationMode = 0;
	/*
	0 = single symbol huffman encoding
	1 = modified single symbol huffman encoding
	2 = 2 block encoding
	*/

	map<string, string> encodingMap;

	string* fileText = new string();
	

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
	
	//find encoding from tree
	void generateEncoding();


	//encode file text
	void encode();


	//statistics
	void printStatistics();


	//debugging
	void printNonZeroFrequencies();

public:

	//Experiment start points
	void runSingleSymbol(string path = "source.txt");
	void runModifiedSS(string path = "source.txt");
	void runTwoBlock(string path = "source.txt");

	void runAll(string path = "source.txt");

	void decode(string path = "dec.txt");


	HuffmanEncoder();
};

