#include <iostream>
#include "HuffmanEncoder.h"
#include <string>
using namespace std;

int main()
{
	HuffmanEncoder hf;


	//hf.runSingleSymbol("lotr2.txt");
	//hf.runModifiedSS("test2.txt");
	hf.runTwoBlock("test2.txt");
}

