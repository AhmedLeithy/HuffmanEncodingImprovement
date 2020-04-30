#include "FileHandler.h"
#include <fstream>
#include <sstream>

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

void FileHandler::writeEncoding(std::string& path, std::string* encodedMessage)
{
	ofstream writer;
	writer.open(path);

	writer << *encodedMessage;
	writer.close();
}

	//need to take into account that this sohuld be written in binary, so take each 8 vals as a character, then convert that character into a byte?
	//finally, mod the length by 8 to add 000's in the very end


void FileHandler::readEncoding(std::string& path, std::string* encodedMessage)
{


}
