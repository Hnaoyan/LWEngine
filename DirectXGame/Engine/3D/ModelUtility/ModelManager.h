#pragma once
#include "Engine/Utility/Singleton.h"
#include "../Drawer/Model.h"
#include <unordered_map>

/// <summary>
/// モデル管理クラス
/// </summary>
class ModelManager :public Singleton<ModelManager>
{
public:
	enum LoadType
	{
		Obj,
		Gltf,

	};

private:
	// モデルのコンテナ（名前と紐づけ
	static std::unordered_map<std::string, std::unique_ptr<Model>> sModels_;

public:
	/// <summary>
	/// アニメーションモデルの読み込み
	/// </summary>
	/// <param name="modelName"></param>
	/// <param name="filePath"></param>
	static void LoadAnimModel(const std::string& tag, const std::string& filePath);

	static void LoadModel(const std::string& tag, const std::string& filePath, LoadType loadType);

	/// <summary>
	/// モデルの取得
	/// </summary>
	/// <param name="tag"></param>
	/// <returns></returns>
	static Model* GetModel(const std::string& tag);
};