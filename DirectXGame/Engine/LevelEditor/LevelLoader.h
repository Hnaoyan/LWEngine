#pragma once
#include "../Utility/Singleton.h"
#include "LevelData.h"

#include <string>
#include <memory>
#include <map>

/// <summary>
/// Blenderからインポートするクラス
/// </summary>
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

	static std::unique_ptr<LevelData> data_;

	static std::map<std::string, std::unique_ptr<LevelData>> datas_;
};
