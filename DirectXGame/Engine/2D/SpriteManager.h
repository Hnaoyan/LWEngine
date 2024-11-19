#pragma once
#include "Engine/Utility/Singleton.h"
#include "Engine/2D/Drawer/Sprite.h"

#include <unordered_map>
#include <string>

/// <summary>
/// スプライト管理クラス
/// </summary>
class SpriteManager : public Singleton<SpriteManager>
{
private:
	// コンテナ
	static std::unordered_map<std::string, std::unique_ptr<Sprite>> sSprites;

public:
	/// <summary>
	/// スプライトの読み込み
	/// </summary>
	/// <param name="tag"></param>
	/// <param name="filePath"></param>
	static void LoadSprite(const std::string& tag, uint32_t texture);
	/// <summary>
	/// スプライトの取得
	/// </summary>
	/// <param name="tag"></param>
	/// <returns></returns>
	static Sprite* GetSprite(const std::string& tag);
};