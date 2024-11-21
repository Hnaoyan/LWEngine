#pragma once
#include "Engine/3D/WorldTransform.h"
#include "Engine/Input/Input.h"
#include "Engine/2D/Drawer/Sprite.h"

class Player;
class ICamera;

namespace PlayerContext {
	class AimManager
	{
	public:
		/// <summary>
		/// 初期化
		/// </summary>
		/// <param name="player"></param>
		void Initialize(Player* player);
		/// <summary>
		/// 更新
		/// </summary>
		void Update(ICamera* camera);
		/// <summary>
		/// ImGui
		/// </summary>
		void ImGuiDraw();

		Vector2 GetReticlePosition() { return screenPosition_; }

	private: // ユーザー

	public: // アクセッサ
		Vector3 GetWorldPosition() { return targetPosition_; }

	private: // 
		// プレイヤー
		Player* player_ = nullptr;
		// 入力
		Input* input_ = nullptr;

	private: // User
		// プレイヤーからのオフセット
		WorldTransform offSetTransform_;
		// 親の行列
		Matrix4x4 parentMatrix_{};
		// エイムの位置
		Vector3 targetPosition_{};
		// エイムのスクリーン上座標
		Vector2 screenPosition_ = {};
	};
}
