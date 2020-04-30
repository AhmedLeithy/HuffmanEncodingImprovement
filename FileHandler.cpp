#include "FileHandler.h"
#include <fstream>
#include <sstream>
#include <bitset>
#include <iostream>
using namespace std;

void FileHandler::readFile(std::string& path, string* returned)
{
	ifstream source;
	source.open(path);

	if (source.is_open()) {
		stringstream buffer;
		buffer << source.rdbuf();
		*returned = buffer.str();
	}
	else {
		throw exception("File Could Not Be Opened");
	}
}

void FileHandler::writeFile(std::string& path, std::string* encodedMessage)
{
	ofstream writer;
	writer.open(path);

	writer << *encodedMessage;
	writer.close();
}

void FileHandler::writeEncoding(std::string& path, std::string* encodedMessage)
{
	ofstream writer;
	writer.open(path, ofstream::binary);

	int byteStart = 0;

	while(byteStart<(*encodedMessage).size()-1){
		bitset<8> temp((*encodedMessage).substr(byteStart, 8));
		//cout << temp.to_ulong();
		writer << char(temp.to_ulong());
		byteStart += 8;
	}

	writer.close();
}

	//need to take into account that this sohuld be written in binary, so take each 8 vals as a character, then convert that character into a byte?
	//finally, mod the length by 8 to add 000's in the very end


void FileHandler::readEncoding(std::string& path, std::string* encodedMessage)
{
	ifstream source;
	source.open(path, ifstream::binary);
	string encoded;
	if (source.is_open()) {
		stringstream buffer;
		buffer << source.rdbuf();
		encoded = buffer.str();
	}
	else {
		throw exception("File Could Not Be Opened");
	}

	char* bitString = new char[encoded.size() * 8];
	int cursor = 0;
	//cout << bitString << endl;
	for (int i = 0; i < encoded.size(); i++) {
		string bs = bitset<8> (encoded[i]).to_string();
		for (int j = 0; j < 8; j++) {
			bitString[cursor] = bs[j];
			cursor++;
		}
		//cout << bitString << endl;
	}
	*encodedMessage = string(bitString).substr(0,encoded.size()*8);


	unsigned long addedZeroes = bitset<8>((*encodedMessage).substr((*encodedMessage).size()-8, 8)).to_ulong();
	(*encodedMessage).erase((*encodedMessage).size()-8-addedZeroes,8+addedZeroes);
}
