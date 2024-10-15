#pragma once
#include "Engine/3D/Instancing/InstancedGroup.h"

class CollisionManager;
class IBullet;

class BulletCluster : public InstancedGroup 
{
public:
	static uint32_t sSerialNumber;
	uint32_t serialNumber_ = 0u;

public: // 仮想関数
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model"></param>
	void Initialize(Model* model) override;
	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;
	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="desc"></param>
	void Draw(ModelDrawDesc desc) override;
	/// <summary>
	/// ImGui
	/// </summary>
	void ImGuiDraw() override;
public: // USER
	/// <summary>
	/// 衝突処理
	/// </summary>
	/// <param name="manager"></param>
	void CollisionUpdate(CollisionManager* manager);
	/// <summary>
	/// 弾の追加
	/// </summary>
	/// <param name="position"></param>
	/// <param name="direct"></param>
	void AddBullet(std::unique_ptr<IBullet> bullet);
private:
	uint32_t texture_ = 0;

};