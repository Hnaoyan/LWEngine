#include "LevelLoader.h"
#include "LevelData.h"

#include <json.hpp>
#include <fstream>
#include <cassert>

void LevelLoader::LoadSceneData(const std::string& filename)
{
	// フルパス作成
	const std::string fullpath = kDefaultDirectoryPath + filename + kExtension;
	// ファイルストリーム
	std::ifstream file;

	// ファイル開く
	file.open(fullpath);
	// ファイルオープン失敗をチェック
	if (file.fail()) {
		assert(0);
	}

	// JSON文字列から解凍したデータ
	nlohmann::json deserialized;

	// 解凍
	file >> deserialized;

	// 正しいレベルデータファイルかチェック
	assert(deserialized.is_object());
	assert(deserialized.contains("name"));
	assert(deserialized["name"].is_string());

	// "name"を文字列として取得
	std::string name = deserialized["name"].get<std::string>();
	// 正しいレベルデータファイルかチェック
	assert(name.compare("scene") == 0);

	// レベルデータ格納用インスタンスを生成
	LevelData* levelData = new LevelData();

	// "objects"の全オブジェクトを走査
	for (nlohmann::json& object : deserialized["objects"]) {
		assert(object.contains("type"));

		// 種別を取得
		std::string type = object["type"].get<std::string>();

		// MESH
		if (type.compare("MESH") == 0) {
			// 要素追加
			levelData->objects.emplace_back(LevelData::ObjectData{});
			// 今追加した要素の参照を得る
			LevelData::ObjectData& objectData = levelData->objects.back();

			if (object.contains("name")) {
				// ファイル名
				objectData.filename = object["name"];
			}

			// トランスフォームのデータ
			nlohmann::json& transform = object["transform"];

			// 
			objectData.transform.translate.x = (float)transform["translation"][0];
			objectData.transform.translate.y = (float)transform["translation"][2];
			objectData.transform.translate.z = (float)transform["translation"][1];
			//
			objectData.transform.rotate.x = -(float)transform["rotation"][0];
			objectData.transform.rotate.y = -(float)transform["rotation"][2];
			objectData.transform.rotate.z = -(float)transform["rotation"][1];
			//
			objectData.transform.scale.x = (float)transform["scaling"][0];
			objectData.transform.scale.y = (float)transform["scaling"][2];
			objectData.transform.scale.z = (float)transform["scaling"][1];

		}


		// 再帰関数
		if (object.contains("children")) {

		}

	}
}
