#pragma once
#include <string>
static class FileHandler
{
public:
	static void readFile(std::string& path, std::string* returned);
	static void writeEncoding(std::string& path, std:: string* encodedMessage);
};

