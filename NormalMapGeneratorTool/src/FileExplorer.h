#pragma once
#include <vector>

enum class FileType
{
	IMAGE, TEXT, NONE
};

class FileExplorer
{
public:
	static FileExplorer instance;
	bool isDirty = true;
	bool shouldDisplay = false;
	std::string path = "C:\\";
	FileExplorer();
	void display();
	void displayDialog(std::string* pathOutput, FileType filter = FileType::NONE);
	std::string getOutputPath();
	~FileExplorer();
private:

	std::vector<std::string> paths;
	std::string* outputPath = nullptr;
	FileType fileFilter = FileType::NONE;
	bool pathTypeCheck(std::vector<std::string> endTypes, std::string& _path);
};