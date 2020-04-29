#include <iostream>
#include "HuffmanEncoder.h"
#include "FileHandler.h"

using namespace std;

//INITIALIZATION
void HuffmanEncoder::initializeFrequencies()
{
	for (int i = 0; i < 256; i++) {
		singleSymbolFrequencies[i] = 0;
		for (int j = 0; j < 256; j++){
			blockSymbolFrequencies[i][j] = 0;
			twoSymbolFrequencies[i][j] = 0;
		}
	}
}
void HuffmanEncoder::countSingleFrequencies()
{
	#ifdef debug
		cout << "Counting Single..";
	#endif

	for (int i = 0; i < (*fileText).length(); i++) 
		singleSymbolFrequencies[(*fileText)[i]]++;
}
void HuffmanEncoder::countTwoSymbolFrequencies()
{
	#ifdef debug
		cout << "Counting twoSymbols..";
	#endif
	int	fileTextLength = (*fileText).length();
	int loopLength = fileTextLength - 1;
	for (int i = 0; i < loopLength; i++)
		twoSymbolFrequencies[(*fileText)[i]][(*fileText)[i + 1]]++;

}
void HuffmanEncoder::countBlockSymbolFrequencies()
{
	#ifdef debug
		cout << "Counting Blocks..";
	#endif
	int	fileTextLength = (*fileText).length();
	int loopLength = fileTextLength % 2 ? fileTextLength -1 : fileTextLength -2;
	for (int i = 0; i < loopLength; i += 2)
		blockSymbolFrequencies[(*fileText)[i]][(*fileText)[i+1]]++;

}

void HuffmanEncoder::initStructures(string& path)
{
	FileHandler::readFile(path, fileText);
	initializeFrequencies();

	if (operationMode == 0)
		countSingleFrequencies();
	else if (operationMode == 1){
		countTwoSymbolFrequencies();
		countSingleFrequencies();
	}
	else if (operationMode == 2)
		countBlockSymbolFrequencies();

	#ifdef debug
		printNonZeroFrequencies();
	#endif 

}



// ENCODING

void HuffmanEncoder::encode()
{
	//build tree
	//translate into map
}


//DECODING

void HuffmanEncoder::decode(string path)
{
	string* s = new string("poop");
	FileHandler::writeEncoding(path, s);
}

//DEBUGGING
void HuffmanEncoder::printNonZeroFrequencies()
{
	if (operationMode == 0) {
		for (int i = 0; i < 256; i++)
			if (singleSymbolFrequencies[i] != 0)
				cout << char(i) << '\t' << singleSymbolFrequencies[i] << '\n';
	}
	else if (operationMode == 1) {
		for (int i = 0; i < 256; i++)
			for (int j = 0; j < 256; j++)
				if (twoSymbolFrequencies[i][j] != 0)
					cout << char(i) << char(j) << '\t' << twoSymbolFrequencies[i][j] << '\n';
	}
	else if (operationMode == 2) {
		for (int i = 0; i < 256; i++)
			for (int j = 0; j < 256; j++)
				if (blockSymbolFrequencies[i][j] != 0)
					cout << char(i) << char(i) << '\t' << blockSymbolFrequencies[i][j] << '\n';
	}
}



//RUNNING
void HuffmanEncoder::runSingleSymbol(string path)
{
	operationMode = 1;
	initStructures(path);
	

	encode();


}



HuffmanEncoder::HuffmanEncoder()
{
}
