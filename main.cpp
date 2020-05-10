#include <iostream>
#include "HuffmanEncoder.h"
#include <string>
using namespace std;

int main()
{
	string path = "gibberish.txt";


	HuffmanEncoder hf;

	hf.runSingleSymbol(path);


	//HuffmanEncoder hf2;
	//hf.runModifiedSS(path);


	//HuffmanEncoder hf3;
	//hf.runTwoBlock(path);
}

