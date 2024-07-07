#pragma once
#include "../Utility/Singleton.h"

#include <string>

class LevelLoader : public Singleton<LevelLoader>
{
public:
	// ディレクトリパス
	const std::string kDefaultDirectoryPath = std::string("Resources") + ("/") + ("LevelData") + ("/");
	// 拡張子
	const std::string kExtension = ".json";
public:
	/// <summary>
	/// jsonの読み込み
	/// </summary>
	/// <param name="filename"></param>
	void LoadSceneData(const std::string& filename);

};
