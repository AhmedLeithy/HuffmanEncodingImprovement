#include <iostream>
#include "HuffmanEncoder.h"
#include <string>
using namespace std;

int main()
{
	
	int mode = 0;
	string fileName;
	while(1){
		cout << "Please input the name of the text file you would like to encode, for example \"lotr2.txt\"" << endl;
		cin >> fileName;
		
		cout << "Please input the mode of operation. \n 0- Single Symbol \n 1-RBM \n 2- Size 2 Block Encoding" << endl;
		cin >> mode;

		try{
			HuffmanEncoder hf;
			if (mode == 0)
				hf.runSingleSymbol(fileName);
			else if (mode == 1)
				hf.runModifiedSS(fileName);
			else if (mode == 2)
				hf.runTwoBlock(fileName);
			else
				cout << "This is not an acceptable mode please try again" << endl;
		}
		catch (exception e)
		{
			cout << "there was an error opening the file and running the process." << endl;
		}	
	}
}

