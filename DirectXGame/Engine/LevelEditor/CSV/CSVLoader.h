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

	void LoadCSVData(std::string filePath);


};
