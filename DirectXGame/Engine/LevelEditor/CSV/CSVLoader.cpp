#include "CSVLoader.h"
#include <cassert>
#include <algorithm>
#include <fstream>
#include <filesystem>
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

std::string CSVLoader::CreateFullPath(const std::string& directory, const std::string& fileName)
{
	// ディレクトリがなければ作成
	std::filesystem::path dir(directory);
	if (!std::filesystem::exists(directory))
	{
		std::filesystem::create_directories(directory);
	}
	// 拡張子がなければ追加
	std::string name = fileName;
	size_t isExt = name.find(".csv");
	if (isExt == std::string::npos) {
		name = name + ".csv";
	}

	return std::string(directory + name);
}
