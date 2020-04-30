#include <iostream>
#include "HuffmanEncoder.h"
#include <string>
using namespace std;

int main()
{
	HuffmanEncoder hf;

	hf.runSingleSymbol("lotr2.txt");
	//hf.runSingleSymbol();
	hf.encode();
	hf.decode();
}

