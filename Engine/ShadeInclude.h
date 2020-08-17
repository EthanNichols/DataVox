#pragma once
#pragma once

#include <string>
#include <fstream>
#include <iostream>

class Shadeinclude
{
public:

	static std::string Load(std::string path, std::string includeIndentifier = "#include")
	{
		includeIndentifier += " ";
		static bool isRecursiveCall = false;

		std::string fullSourceCode = "";
		std::ifstream file(path);

		if (!file.is_open())
		{
			std::cerr << "ERROR: could not open the shader at: " << path << "\n" << std::endl;
			return fullSourceCode;
		}

		std::string lineBuffer;
		bool inIfCPPMacro = false;

		while (std::getline(file, lineBuffer))
		{
			if (lineBuffer == "#pragma once")
			{
				continue;
			}

			if (lineBuffer == "#ifdef __cplusplus")
			{
				inIfCPPMacro = true;
				continue;
			}
			else if (lineBuffer == "#endif // __cplusplus")
			{
				inIfCPPMacro = false;
				continue;
			}
			else if (inIfCPPMacro)
			{
				continue;
			}

			if (lineBuffer.find(includeIndentifier) != lineBuffer.npos)
			{
				lineBuffer.erase(0, includeIndentifier.size());

				std::string pathOfThisFile;
				GetFilePath(path, pathOfThisFile);
				lineBuffer.insert(0, pathOfThisFile);

				isRecursiveCall = true;
				fullSourceCode += Load(lineBuffer);

				continue;
			}

			fullSourceCode += lineBuffer + '\n';
		}

		if (!isRecursiveCall)
			fullSourceCode += '\0';

		file.close();

		return fullSourceCode;
	}


private:

	static void GetFilePath(const std::string& fullPath, std::string& pathWithoutFileName)
	{
		size_t found = fullPath.find_last_of("/\\");
		pathWithoutFileName = fullPath.substr(0, found + 1);
	}
};