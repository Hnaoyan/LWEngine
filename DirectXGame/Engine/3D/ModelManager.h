#pragma once
#include "../Utility/Singleton.h"
#include "Drawer/Model.h"
#include <unordered_map>

class ModelManager :public Singleton<ModelManager>
{
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
	/// <summary>
	/// 通常モデルの読み込み
	/// </summary>
	/// <param name="modelName"></param>
	/// <param name="filePath"></param>
	static void LoadNormalModel(const std::string& tag, const std::string& filePath);

	/// <summary>
	/// Objの読み込み
	/// </summary>
	/// <param name="tag"></param>
	/// <param name="filePath"></param>
	static void LoadObjModel(const std::string& tag, const std::string& filePath);

	/// <summary>
	/// モデルの取得
	/// </summary>
	/// <param name="tag"></param>
	/// <returns></returns>
	static Model* GetModel(const std::string& tag);
};