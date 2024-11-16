#pragma once
#include "Engine/Utility/Singleton.h"
#include <string>
#include <sstream>
#include <stdint.h>
#include <vector>

class CSVLoader : public Singleton<CSVLoader>
{
public:
	// ディレクトリパス
	const std::string kDefaultDirectoryPath = std::string("Resources") + ("/") + ("CSVData") + ("/");
	// 拡張子
	const std::string kExtension = ".csv";

	// データ取得
	static void LoadCSVData(std::string filePath);
	// ファイルの中身を読みだす処理
	static std::stringstream LoadCSVFile(std::string fullPath);

	static std::string CreateFullPath(const std::string& directory, const std::string& fileName);

};
