#pragma once
#include "Engine/3D/Drawer/Model.h"
#include <memory>

class Player;

class PlayerRoundShadow
{
public:
	PlayerRoundShadow() {};
	void Initialize(Player* player);
	void Update();
	void Draw(ModelDrawDesc desc);
	void ImGuiDraw();

private:
	// プレイヤー
	Player* player_ = nullptr;
	// トランスフォーム
	WorldTransform worldTransform_;
	// 影用の板ポリ
	Model* model_ = nullptr;
	// マテリアル
	std::unique_ptr<Material> material_;

	bool isInvisible_ = false;
	uint32_t texture_ = 0u;

};
