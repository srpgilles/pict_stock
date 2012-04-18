
#include <iostream>
#include <fstream>
#include "yuni/yuni.h"
#include "yuni/core/string.h"
#include "yuni/io/file.h"


//! Return false if it is not a rule prototype. Otherwise, return true and set the name of the rule
static bool identifyRulePrototype(const Yuni::String& line, Yuni::String& ruleName)
{
	bool isRule = line.startsWith("Nany::Ast::Node* Rule_");
	if (isRule)
	{
		unsigned int startIndex = 18; // Index of the 'R' from Rule_*
		unsigned int endIndex = line.find('(');
		ruleName.assign(line, endIndex - startIndex, startIndex);
	}
	return isRule;
}


static void readFullFile(const char* fileName, Yuni::String& fileContents)
{
	Yuni::IO::File::LoadFromFile(fileContents, fileName);
}


static void mergeFile(const Yuni::String& oldContents, std::ifstream& newFile, std::ofstream& merged)
{
	char line[256];
	Yuni::String s;
	Yuni::String ruleName;
	while (newFile.getline(line, 256))
	{
		merged << line << std::endl;
		bool isRule = identifyRulePrototype(s = line, ruleName);
		// Add the parenthese to make sure no rule with a longer name contains this one
		ruleName << '(';
		unsigned int index = oldContents.find(ruleName);

		// If we found a rule and it already existed in the old file
		if (isRule && index != Yuni::String::npos)
		{
			// Jump over 4 lines in the new file
			for (unsigned int i = 0; i < 4; ++i)
				newFile.getline(line, 256);
			// Copy the content from the same rule in the old file
			index = oldContents.find('{', index);
			unsigned int endIndex = oldContents.find("\n}", index) + 3;
			s.assign(oldContents, endIndex - index, index);
			merged << s;
		}
	}
}


int main(int argc, char* argv[])
{
	if (argc < 3)
	{
		std::cerr << "Insufficient number of parameters !" << std::endl;
		std::cerr << "Usage: " << argv[0] << " <old parser.cpp> <new parser.cpp>" << std::endl;
		return 1;
	}

	std::ifstream oldFile(argv[1]);
	std::ifstream newFile(argv[2]);
	std::ofstream merged("merged.parser.cpp");

	if (!oldFile.is_open())
	{
		std::cerr << "\"" << argv[1] << "\" could not be opened for writing." << std::endl;
		return 1;
	}
	oldFile.close();

	if (!newFile.is_open())
	{
		std::cerr << "\"" << argv[2] << "\" could not be opened for writing." << std::endl;
		return 1;
	}

	if (!merged.is_open())
	{
		std::cerr << "\"merged.parser.cpp\" could not be opened for writing." << std::endl;
		return 1;
	}

	Yuni::String oldContents;
	readFullFile(argv[1], oldContents);
	oldContents.replace("\r\n", "\n");
	mergeFile(oldContents, newFile, merged);

	merged.close();
	newFile.close();
	return 0;
}
