#include <iostream>
#include <queue>
#include <vector>
#include <map> 
#include "HuffmanEncoder.h"
#include "FileHandler.h"
#include "tree.h"
#include <bitset>
#include <cmath>

using namespace std;

//dreams
//n block encoding


//INITIALIZATION
void HuffmanEncoder::initializeFrequencies()
{
	for (int i = 0; i < 256; i++) 
	{
		singleSymbolFrequencies[i] = 0;
		for (int j = 0; j < 256; j++)
		{
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

void HuffmanEncoder::buildTreeStructure(priority_queue<node, vector<node>, greater<node>>& queue)
{
	while (queue.size() > 1) 
	{
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

void HuffmanEncoder::buildTreeSS()
{
	priority_queue<node,vector<node>,greater<node>> queue;

	for (int i = 0; i < 256; i++)
		if (singleSymbolFrequencies[i])
		{
			string s(1, char(i)); 
			node temp = node(s, singleSymbolFrequencies[i]); 
			queue.push(temp);
		}
	//cout << "minimum  symbol \t" << queue.top().symbol << " frequency \t" << queue.top().frequency << endl;

	//tree t;
	buildTreeStructure(queue);
}
struct bigram{
	int i, j, frequency;
	
	bigram(int ii, int ji, int freq) {
		i = ii;
		j = ji;
		frequency = freq;
	}
	bigram(const bigram& b) {
		i = b.i;
		j = b.j;
		frequency = b.frequency;
	}

	bigram() {}

	bool operator<(const bigram& right) const {
		return frequency < right.frequency;
	}

	bool operator>(const bigram& right) const {
		return frequency > right.frequency;
	}
};

void HuffmanEncoder::buildTreeMSS()
{
	priority_queue<node, vector<node>, greater<node>> queue;

	priority_queue<bigram> bigramQueue;

	for (int i = 0; i < 256; i++)
		for (int j = 0; j < 256; j++) 
		{
			if (twoSymbolFrequencies[i][j]) 
			{
				bigram bi = bigram(i, j, twoSymbolFrequencies[i][j]);
				bigramQueue.push(bi);
				/*
				- criteria AVERAGE
				- manual thresholds


				-log(p) => codeLength

				ciel(-log(p)) => codeLength

				codelengh(p1p2) < codelength(p1) + codelength(p2)

				ciel(-log(p1p2)) < ciel(-log(p1)) + codelength(p2)

				*/
				/*
				float n = (*fileText).length();
				if (
					ceil(-log2(twoSymbolFrequencies[i][j]/(n-1))) < ceil(-log2(singleSymbolFrequencies[i] / n)) +ceil(-log2(singleSymbolFrequencies[j] /n))
					//&& singleSymbolFrequencies[i] >= twoSymbolFrequencies[i][j]
					//&& singleSymbolFrequencies[j] >= twoSymbolFrequencies[i][j]
					)
				{
					string c1 = string(1, char(i));
					string c2 = string(1, char(j));
					string symbolName = c1 + c2;

					node temp = node(symbolName, twoSymbolFrequencies[i][j]);
					queue.push(temp);

					int initialSingle = singleSymbolFrequencies[i];
					int initialSingle2 = singleSymbolFrequencies[j];
					int twosymbol = twoSymbolFrequencies[i][j];
					
					singleSymbolFrequencies[i] -= twoSymbolFrequencies[i][j];
					singleSymbolFrequencies[j] -= twoSymbolFrequencies[i][j];
					
					if (singleSymbolFrequencies[i] < 0 || singleSymbolFrequencies[j] < 0) {
						int x = 2;
					}
					
					//if this two symbol is included, then the frequency each 
					//letter must be reduced by the frequency of the combination
					
				}
			*/

			}
		}
	float n = (*fileText).length();

	while (!bigramQueue.empty()) {
		bigram maxFreq = bigramQueue.top();
		bigramQueue.pop();

		if (
			ceil(-log2(maxFreq.frequency / (n - 1))) < (ceil(-log2(singleSymbolFrequencies[maxFreq.i] / n)) + ceil(-log2(singleSymbolFrequencies[maxFreq.j] / n)))
			&& singleSymbolFrequencies[maxFreq.i] >= maxFreq.frequency
			&& singleSymbolFrequencies[maxFreq.j] >= maxFreq.frequency
			)
		{
			string c1 = string(1, char(maxFreq.i));
			string c2 = string(1, char(maxFreq.j));
			string symbolName = c1 + c2;

			node temp = node(symbolName, maxFreq.frequency);
			queue.push(temp);

			int initialSingle = singleSymbolFrequencies[maxFreq.i];
			int initialSingle2 = singleSymbolFrequencies[maxFreq.j];

			singleSymbolFrequencies[maxFreq.i] =  maxFreq.frequency > initialSingle ? 0 : initialSingle - (maxFreq.frequency/2.0);
			singleSymbolFrequencies[maxFreq.j] =  maxFreq.frequency > initialSingle2 ? 0 : initialSingle2 - (maxFreq.frequency/2.0);

			//if this two symbol is included, then the frequency each 
			//letter must be reduced by the frequency of the combination
		}
	}




	operationMode = 0;
	printNonZeroFrequencies();
	operationMode = 1;

	for (int i = 0; i < 256; i++)
		if (singleSymbolFrequencies[i]) 
		{
			string s(1, char(i));
			node temp = node(s, singleSymbolFrequencies[i]);
			queue.push(temp);
		}
	//cout << "minimum  symbol \t" << queue.top().symbol << " frequency \t" << queue.top().frequency << endl;

	//tree t;
	buildTreeStructure(queue);
}

void HuffmanEncoder::buildTreeBlock()
{
	priority_queue<node, vector<node>, greater<node>> queue;

	for (int i = 0; i < 256; i++)
		for (int j = 0; j < 256; j++)
		{
			if (blockSymbolFrequencies[i][j]) 
			{
				string c1 = string(1,char(i));
				string c2 = string(1,char(j));
				string symbolName = c1+c2;

				node temp = node(symbolName, blockSymbolFrequencies[i][j]);
				queue.push(temp);
			}
		}
	//cout << "minimum  symbol \t" << queue.top().symbol << " frequency \t" << queue.top().frequency << endl;

	//tree t;
	buildTreeStructure(queue);
}

void HuffmanEncoder::encodeTree(string & encodedTable)
{	
	/*
	char* decText = (char*)malloc(sizeof(char) * (20000 + 1));
	int size = 20000;

				if (position > size - 1)
				{
					size = size + 50000;
					decText = (char*)realloc(decText, sizeof(char) * (size + 1));
				}
	*/

	/*
		NumberOfRows in two bytes

		each row:
		A) 1 bit saying whether bigram
		B) 2 bytes for length of encoding because (log2(256*256)) in the worst case tree. (right right right etc.)
		C) 1 byte for char 1
		D) 1 byte for char2 (if A == 1)
		E) Encoding
	*/

	char* encodedTableArr = (char*)malloc(sizeof(char) * (5000 + 1));
	int size = 5000;
	int cursor = 16;

	string rows = bitset<16>(encodingMap.size()).to_string();
	
	for (int j = 0; j < 16; j++) {
		encodedTableArr[j] = rows[j];
	}

	for (auto i : encodingMap) {
		unsigned char bigram = i.first.length() == 1 ? '0' : '1';
		string codingLength = bitset<16>(i.second.length()).to_string();
		string char1  =  bitset<8>(int(i.first[0])).to_string();
		string char2;
		if(bigram == '1')
			char2 = bitset<8>(int(i.first[1])).to_string();
		


		if (cursor > size - 1) {
			size = size + 2000;
			encodedTableArr = (char*)realloc(encodedTableArr, sizeof(char) * (size + 1));
		}

		encodedTableArr[cursor] = bigram;
		cursor++;


		for (int j = 0; j < 16; j++) {
			if (cursor > size - 1) {
				size = size + 2000;
				encodedTableArr = (char*)realloc(encodedTableArr, sizeof(char) * (size + 1));
			}
			encodedTableArr[cursor] = codingLength[j];
			cursor++;
		}

		for (int j = 0; j < 8; j++) {
			if (cursor > size - 1) {
				size = size + 2000;
				encodedTableArr = (char*)realloc(encodedTableArr, sizeof(char) * (size + 1));
			}
			encodedTableArr[cursor] = char1[j];
			cursor++;
		}

		if (bigram == '1') {
			for (int j = 0; j < 8; j++) {
				if (cursor > size - 1) {
					size = size + 2000;
					encodedTableArr = (char*)realloc(encodedTableArr, sizeof(char) * (size + 1));
				}
				encodedTableArr[cursor] = char2[j];
				cursor++;
			}
		}

		for (int j = 0; j < i.second.length(); j++) {
			if (cursor > size - 1) {
				size = size + 2000;
				encodedTableArr = (char*)realloc(encodedTableArr, sizeof(char) * (size + 1));
			}
			encodedTableArr[cursor] = i.second[j];
			cursor++;
		}
	}

	encodedTable = string(encodedTableArr).substr(0,cursor);
}

void HuffmanEncoder::decodeTree(string& file)
{
	unordered_map<string, string>  encodingMap2 = encodingMap;
	encodingMap.clear();
	t.~tree();

	int rows = int(bitset<16>(file.substr(0, 16)).to_ulong());

	int cursor = 16;
	/*
		each row:
		A) 1 bit saying whether bigram
		B) 2 bytes for length of encoding because (log2(256*256)) in the worst case tree. (right right right etc.)
		C) 1 byte for char 1
		D) 1 byte for char2 (if A == 1)
		E) Encoding
	*/

	for (int i = 0; i < rows; i++) {
		bool bigram = file[cursor] == '1';
		cursor++;
		int length = int(bitset<16>(file.substr(cursor, 16)).to_ulong());
		cursor += 16;

		string symbol;
		char char1 = char(bitset<8>(file.substr(cursor, 8)).to_ulong());
		cursor += 8;
		char char2;
		
		symbol += char1;

		if (bigram) {
			char2 = char(bitset<8>(file.substr(cursor, 8)).to_ulong());
			cursor += 8;
			symbol += char2;
		}

		string encoding = file.substr(cursor, length);
		cursor += length;

		encodingMap[symbol] = encoding;
	}

	file = file.substr(cursor, file.size() - cursor);


	node* newRoot = new node;
	t.root = newRoot;
	for (auto i : encodingMap) {
		insertCodingIntoTree(i.second, i.first);
	}

}

void HuffmanEncoder::insertCodingIntoTree(string coding, string symbol) {

	node* cursor = t.root;
	for (int i = 0; i < coding.length(); i++) {
		if (coding[i] == '0') {
			if (cursor->right == NULL) {
				node* newNode = new node;
				cursor->right = newNode;
				cursor = newNode;
			}
			else {
				cursor = cursor->right;
			}
		}
		else {
			if (cursor->left == NULL) {
				node* newNode = new node;
				cursor->left = newNode;
				cursor = newNode;
			}
			else {
				cursor = cursor->left;
			}
		}

		if (i == coding.length() - 1) {
			cursor->symbol = symbol;
		}
	}
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
	else
	{
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
	if (operationMode == 0)
	{
		for (auto i : encodingMap) 
		{
			cout << i.first << "\t" << i.second << endl;
				sum += i.second.length() * singleSymbolFrequencies[i.first[0]];
		}
	}
	else 
		if (operationMode == 1) 
		{
			for (auto i : encodingMap) 
			{
				cout << i.first << "\t" << i.second << endl;
				if(i.first.length()==2)
					sum += i.second.length() * twoSymbolFrequencies[i.first[0]][i.first[1]];
				else
					sum += i.second.length() * singleSymbolFrequencies[i.first[0]];
			}
		}
		else 
		{
			for (auto i : encodingMap) 
			{
				cout << i.first << "\t" << i.second << endl;
				sum += i.second.length() * blockSymbolFrequencies[i.first[0]][i.first[1]];
			}
		}



	int addedZeroes = (8 - sum % 8) % 8;


	char* encText = new char[sum+addedZeroes+8];
	int cursor = 0;

	if (operationMode==0)
	{
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
	} 
	else if (operationMode==1) //modified SS
	{
		for(int i=0; i<l;i++)
		{
			char stringArray[2];

			string st1;
			string st2;
			if (i!=l-1) //ya basha enta last wala la2? 2a3ooz men segmentation fault
			{
				st2 += (*fileText)[i];
				st2 += (*fileText)[i + 1];
				if (encodingMap.find(st2) != encodingMap.end()) 
				{
					st1 = st2;
					i++;
				}
				else
				{
					st1 += (*fileText)[i];
				}
			}
			else 
			{
				st1 += (*fileText)[i];
			}
			string code = encodingMap[st1];
			for (int j = 0; j < code.size(); j++)
			{
				encText[cursor] = code[j];
				cursor++;
			}

			if (i % 50000 == 0)
				cout << i << endl;
		}
	}
	else
	{
		for(int i=0; i<l;i+=2)
		{	
			char stringArray[2];

			string st;

			st += (*fileText)[i];
			st += (*fileText)[i + 1]; 
			
			string code = encodingMap[st];
			for (int j = 0; j < code.size(); j++)
			{
				encText[cursor] = code[j];
				cursor++;
			}

			if (i % 50000 == 0)
				cout << i << endl;
		}
	}
	
	//cout << "starting adding zeroes\n";

	for (int i = 0; i < addedZeroes; i++) 
	{
		//cout << encText << endl;

		encText[cursor] = '0';
		cursor++;
	}
	 
	string bs = bitset<8> (addedZeroes).to_string();

	//cout << "starting adding bs";

	for (int i = 0; i < 8; i++) 
	{

		//cout << encText << endl;
		encText[cursor] = bs[i];
		cursor++;
	}

	//cout << encText << endl;


	string encFile = string(encText).substr(0,cursor);



	if (encodeTable) {
		string encodedTable = "";
		encodeTree(encodedTable);
		encFile = encodedTable+ encFile ;

	}

	FileHandler::writeEncoding(path, &encFile);

	double encl = encFile.length();
	printStatistics(encl);
}

void HuffmanEncoder::printStatistics(double x) //entropy, compression ratio.
{
	int total = (*fileText).length();
	double fileEntropy = 0;
	double encEntropy = 0;
	double temp = 0;
	if (operationMode != 0)
		countSingleFrequencies();

	for (int i = 0; i < 256; i++) //for file entropy based on its freq
	{
		if (singleSymbolFrequencies[i] > 0)
		{
			temp = static_cast<double>(singleSymbolFrequencies[i]) / static_cast<double>(total);
			fileEntropy = fileEntropy - temp * (log2(temp));
			temp = 0;
		}
	}

	double avgLength = 0;
	if (operationMode == 0)
	{
		for (auto i : encodingMap)
		{
			avgLength = avgLength + static_cast<double>(singleSymbolFrequencies[i.first[0]]) / static_cast<double>(total) * i.second.length();
		}
	}
	else
		if (operationMode == 1)
		{
			for (auto i : encodingMap)
			{
				if (i.first.length() == 1)
					temp = static_cast<double>(singleSymbolFrequencies[i.first[0]]) / static_cast<double>(total);
				else
					temp = static_cast<double>(twoSymbolFrequencies[i.first[0]][i.first[1]]) / (static_cast<double>(total)*2);

				avgLength = avgLength + temp * i.second.length();
				encEntropy = fileEntropy - temp * (log2(temp));
				temp = 0;
			}

		}
		else
		{
			for (auto i : encodingMap)
			{
				temp = static_cast<double>(blockSymbolFrequencies[i.first[0]][i.first[1]]) / static_cast<double>(total);
				avgLength = avgLength + temp * i.second.length();
				encEntropy = fileEntropy - temp * (log2(temp));
				temp = 0;
			}
			//avgLength = avgLength / 2; //isn't that what we did in assignment? cause it's a block of two? 
		}

	double comp = x / (8 * static_cast<double>(total));
	cout << "some stats..." << endl;
	cout << "avg length of encoding: " << avgLength << endl;
	if (operationMode != 0)
		cout << "entropy of encoding: " << encEntropy << endl;
	cout << "entropy of file: " << fileEntropy << endl;
	cout << "Compression ratio: " << comp << endl;
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

	if (encodeTable) {
		decodeTree(encText);
	}

	node* cursor = t.root;
	
	int position = 0;
	char* decText = (char*)malloc(sizeof(char) * (20000 + 1));
	int size = 20000;
	

	if(operationMode==0)
	{
		for (int index = 0; index < encText.size(); index++) 
		{
			if (encText[index] == '0') 
				cursor = cursor->right;
			else 
				cursor = cursor->left;

			if (!cursor->right && !cursor->left)
			{
				if (position > size - 1)
				{
					size = size + 50000;
					decText = (char*)realloc(decText, sizeof(char) * (size + 1));
				}

				decText[position] = (cursor->symbol)[0]; //2 characters
				position++;
				cursor = t.root;
			}
		}
	}
	else 
		if (operationMode == 1) 
		{
			for (int index = 0; index < encText.size(); index++)
			 {
				if (encText[index] == '0')
					cursor = cursor->right;
				else
					cursor = cursor->left;

				if (!cursor->right && !cursor->left) 
				{
					if (position > size - 1) 
					{
						size = size + 50000;
						decText = (char*)realloc(decText, sizeof(char) * (size + 1));
					}
					if(cursor->symbol.length()>1)
					{
						decText[position] = (cursor->symbol)[0]; //2 characters
						position++;
						decText[position] = (cursor->symbol)[1]; //2 characters
						position++;
					}
					else 
					{
						decText[position] = (cursor->symbol)[0]; //2 characters
						position++;
					}
					cursor = t.root;
				}
			}
		}
		else 
		{
			for (int index = 0; index < encText.size(); index++) 
			{
				if (encText[index] == '0')
					cursor = cursor->right;
				else
					cursor = cursor->left;

				if (!cursor->right && !cursor->left) 
				{
					if (position > size - 1) 
					{
						size = size + 50000;
						decText = (char*)realloc(decText, sizeof(char) * (size + 1));
					}
				
					decText[position] = (cursor->symbol)[0]; //2 characters
					position++;
					decText[position] = (cursor->symbol)[1]; //2 characters
					position++;
					cursor = t.root;
				}
			}
		}
	string s = string(decText).substr(0,position);
	FileHandler::writeFile(writePath, &s);
}

//DEBUGGING
void HuffmanEncoder::printNonZeroFrequencies()
{
	if (operationMode == 0) 
	{
		for (int i = 0; i < 256; i++)
			if (singleSymbolFrequencies[i] != 0)
				cout << char(i) << '\t' << singleSymbolFrequencies[i] << '\n';
	}
	else if (operationMode == 1)
	{
		for (int i = 0; i < 256; i++)
			for (int j = 0; j < 256; j++)
				if (twoSymbolFrequencies[i][j] != 0)
					cout << char(i) << char(j) << '\t' << twoSymbolFrequencies[i][j] << '\n';
	}
	else if (operationMode == 2) 
	{
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
	encode();
	decode();
}


void HuffmanEncoder::runModifiedSS(string path)
{
	operationMode = 1;
	initStructures(path);
	
	buildTree();
	encode();
	decode();
}


void HuffmanEncoder::runTwoBlock(string path)
{
	operationMode = 2;
	initStructures(path);
	
	buildTree();
	encode();
	decode();
}


HuffmanEncoder::HuffmanEncoder()
{
}
