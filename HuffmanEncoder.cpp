#include <iostream>
#include <queue>
#include <vector>
#include <map> 
#include "HuffmanEncoder.h"
#include "FileHandler.h"
#include "tree.h"
#include <bitset>
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

	if (fileTextLength%2)
		throw exception("CHARACTER LENGTH IS NOT EVEN");

	for (int i = 0; i < fileTextLength-1; i = i+2)
		blockSymbolFrequencies[(*fileText)[i]][(*fileText)[i+1]]++;

}

void HuffmanEncoder::buildTree()
{
	if (operationMode == 0)
		buildTreeSS();
	else if (operationMode == 1)
		buildTreeMSS();
	else
		buildTreeBlock();
}

void HuffmanEncoder::buildTreeSS()
{
	priority_queue<node,vector<node>,greater<node>> queue;

	for (int i = 0; i < 256; i++)
		if (singleSymbolFrequencies[i]) {
			string s(1, char(i)); 
			node temp = node(s, singleSymbolFrequencies[i]); 
			queue.push(temp);
		}

	cout << "minimum  symbol \t" << queue.top().symbol << " frequency \t" << queue.top().frequency << endl;


	//tree t;
	while (queue.size() > 1) {
		node first = queue.top();
		queue.pop();

		node second = queue.top();
		queue.pop();

		node newnode = t.combineNodes(first, second);
		newnode.frequency = first.frequency + second.frequency;
		newnode.symbol = first.symbol + second.symbol;


		queue.push(newnode);
	}

	node root = queue.top();

	t.root = new node(root);

}
void HuffmanEncoder::buildTreeMSS()
{
}
void HuffmanEncoder::buildTreeBlock()
{
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



// WRITING A CODE FOR EACH SYMBOL
void HuffmanEncoder::encoder(node* n, string code)
{
	if (!n->left && !n->right) //a leaf
		encodingMap[n->symbol] = code;
	else{
		encoder(n->left, code + "1");
		encoder(n->right, code + "0");
	}
}


void HuffmanEncoder::encode(string path) //where are we calling dis?
{
	encoder(t.root,"");

	//string encfile = "";
	int l = (*fileText).length();

	int sum = 0;
	for (auto i : encodingMap) {
		cout << i.first << "\t" << i.second << endl;
		sum += i.second.length() * singleSymbolFrequencies[i.first[0]];
	}

	int addedZeroes = (8 - sum % 8) % 8;


	char* encText = new char[sum+addedZeroes+8];
	int cursor = 0;

	for(int i=0; i<l;i++)
	{
		string st = string(1,(*fileText)[i]);
		
		//encfile = encfile + encodingMap[st];

		string code = encodingMap[st];
		for (int j = 0; j < code.size(); j++)
		{
			encText[cursor] = code[j];
			cursor++;
		}

		if (i % 50000 == 0)
			cout << i << endl;
	}


	//cout << "starting adding zeroes\n";

	for (int i = 0; i < addedZeroes; i++) {
		//cout << encText << endl;

		encText[cursor] = '0';
		cursor++;
	}
	 
	string bs = bitset<8> (addedZeroes).to_string();


	//cout << "starting adding bs";
	for (int i = 0; i < 8; i++) {

		//cout << encText << endl;
		encText[cursor] = bs[i];
		cursor++;
	}
	//cout << encText << endl;

	string encFile = string(encText).substr(0,sum+addedZeroes+8);
	FileHandler::writeEncoding(path, &encFile); 
}

/*
[5]
bytes[3]
character lengthofencoding encoding
a 1 0
b 2 10
c 3 110
0101010010101010010101010101010111111000

01001010 01111001 01010101 [011][00000]
*/

//DECODING
void HuffmanEncoder::decode(string writePath, string readPath)
{
	//rebuildTree();

	//ok so we traverse the tree until we hit a leaf and that gives us the symbol

	string encText;
	FileHandler::readEncoding(readPath, &encText);

	node* cursor = t.root;
	
	int position = 0;
	char* decText = (char*)malloc(sizeof(char) * (20000 + 1));
	int size = 20000;
	
	for (int index = 0; index < encText.size(); index++) {
		if (encText[index] == '0') 
			cursor = cursor->right;
		else 
			cursor = cursor->left;

		if (!cursor->right && !cursor->left) {
			if (position > size - 1) {
				size = size + 50000;
				decText = (char*)realloc(decText, sizeof(char) * (size + 1));
			}

			decText[position] = (cursor->symbol)[0];
			position++;
			cursor = t.root;
		}
	}

	string s = string(decText).substr(0,position);
	FileHandler::writeFile(writePath, &s);
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
					cout << char(i) << char(j) << '\t' << blockSymbolFrequencies[i][j] << '\n';
	}
}



//RUNNING
void HuffmanEncoder::runSingleSymbol(string path)
{
	operationMode = 0;
	initStructures(path);
	
	buildTree();
	//encoder();
}



HuffmanEncoder::HuffmanEncoder()
{
}
