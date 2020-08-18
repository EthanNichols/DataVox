#pragma once
#pragma once

#include <fstream>
#include <iostream>
#include <regex>
#include <string>





class Shadeinclude
{
private:

	const std::string INCLUDE_IDENTIFIER = "#include ";

public:

	std::string Load(std::string path)
	{
		static bool isRecursiveCall = false;

		std::string fullSourceCode = "";
		std::ifstream file(path);

		if (!file.is_open())
		{
			std::cerr << "ERROR: could not open the shader at: " << path << "\n" << std::endl;
			return fullSourceCode;
		}

		std::string line;

		while (std::getline(file, line))
		{
			ScrubCPPData(line);

			if (line.empty())
			{
				continue;
			}

			if (line.find(INCLUDE_IDENTIFIER) != line.npos)
			{
				line.erase(0, INCLUDE_IDENTIFIER.size());

				std::string pathOfThisFile;
				GetFilePath(path, pathOfThisFile);
				line.insert(0, pathOfThisFile);

				isRecursiveCall = true;
				fullSourceCode += Load(line);

				continue;
			}

			fullSourceCode += line + '\n';
		}

		if (!isRecursiveCall)
			fullSourceCode += '\0';

		file.close();

		return fullSourceCode;
	}


private:

	void GetFilePath(const std::string& fullPath, std::string& pathWithoutFileName)
	{
		size_t found = fullPath.find_last_of("/\\");
		pathWithoutFileName = fullPath.substr(0, found + 1);
	}


private:

	const std::string PRAGMA_ONCE = "#pragma once";
	const std::string IF_DEF = "#ifdef";
	const std::string IF_NOT_DEF = "#ifndef";
	const std::string END_IF = "#endif";
	const std::string CPP = "__cplusplus";

	const std::string CPP_DEF = IF_DEF + " " + CPP;
	const std::string CPP_NOT_DEF = IF_NOT_DEF + " " + CPP;

	const std::string CPP_END_IF = END_IF + " // " + CPP;
	const std::string CPP_NOT_END_IF = END_IF + " // ! " + CPP;

	const std::string STRUCT_DEF = "struct";
	const std::string PUBLIC_FUNCTIONS = "public:";

	bool inIfCPPMacro = false;
	bool inStruct = false;
	bool inStructFunctions = false;

	void ScrubCPPData(std::string& line)
	{
		bool scrubLine = false;

		if (line.find(PRAGMA_ONCE) != line.npos ||
			line.find(IF_DEF) != line.npos ||
			line.find(IF_NOT_DEF) != line.npos ||
			line.find(END_IF) != line.npos)
		{
			scrubLine = true;
		}

		if (inIfCPPMacro)
		{
			scrubLine = true;

		}
		if (line.find(CPP_DEF) != line.npos)
		{
			inIfCPPMacro = true;
		}
		else if (line.find(CPP_END_IF) != line.npos)
		{
			inIfCPPMacro = false;
		}

		if (scrubLine)
		{
			line = "";
			return;
		}

		if (inStruct && line.find("};") != line.npos)
		{
			inStruct = false;
			inStructFunctions = false;
		}
		if (line.find(STRUCT_DEF) != line.npos)
		{
			inStruct = true;
		}
		else if (inStruct && line.find(PUBLIC_FUNCTIONS) != line.npos)
		{
			inStructFunctions = true;
		}
		if (inStruct)
		{
			std::regex floatsRegex = std::regex("(\\S* \\S*)=.*;");
			line = std::regex_replace(line, floatsRegex, "$1;");

			if (inStructFunctions)
			{
				line = "";
			}
		}
	}
};