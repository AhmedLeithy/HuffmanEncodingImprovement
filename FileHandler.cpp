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
