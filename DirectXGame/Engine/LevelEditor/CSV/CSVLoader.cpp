#include "CSVLoader.h"
#include <cassert>
#include <algorithm>
#include <fstream>
#include <numbers>

void CSVLoader::LoadCSVData(std::string filePath)
{
	std::stringstream popCommands;
	// ファイルを開く
	std::ifstream file;
	std::string fullPath = CSVLoader::GetInstance()->kDefaultDirectoryPath + filePath + CSVLoader::GetInstance()->kExtension;
	file.open(fullPath);
	assert(file.is_open());
	// ファイル内容を文字列に
	popCommands << file.rdbuf();
	// 閉じる
	file.close();
}

std::stringstream CSVLoader::LoadCSVFile(std::string fullPath)
{
	std::stringstream popCommands;
	// ファイルを開く
	std::ifstream file;
	//std::string fullPath = kDefaultDirectoryPath + filePath + kExtension;
	file.open(fullPath);
	assert(file.is_open());
	// ファイル内容を文字列に
	popCommands << file.rdbuf();
	// 閉じる
	file.close();

	return popCommands;
}
