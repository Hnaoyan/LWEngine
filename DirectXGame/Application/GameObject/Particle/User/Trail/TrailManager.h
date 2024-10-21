#pragma once
#include "Application/GameObject/Bullet/Trail/BulletTrail.h"
#include <vector>
#include <memory>

class ICamera;

/// <summary>
/// 軌跡のインスタンス管理クラス
/// </summary>
class TrailManager
{
public:
	TrailManager();
	~TrailManager() = default;
public:
	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="camera"></param>
	void Draw(ICamera* camera);
	/// <summary>
	/// インスタンスの追加
	/// </summary>
	/// <param name="trail"></param>
	void AddTrail(std::unique_ptr<BulletTrail> trail);

private:
	// 軌跡インスタンスのコンテナ
	std::vector<std::unique_ptr<BulletTrail>> trails_;

};
