#pragma once
#include "Engine/3D/Drawer/Model.h"
#include "Engine/LwLib/Utillity/FrameTimer.h"
#include <utility>
#include <unordered_map>

class Boss;

namespace BossSystemContext 
{
	/// <summary>
	/// アニメーションマネージャ
	/// </summary>
	class AnimationManager
	{
	public:
		// 階層パーツの情報
		struct Hierarchy
		{
			// モデル
			Model* model = nullptr;
			// テクスチャ
			uint32_t texture = 0u;
			// 座標などの情報
			WorldTransform worldTransform;
			// イージングの始点終点
			std::pair<Vector3, Vector3> easePoint;
		};
		// アニメーションの種類
		enum class AnimType : uint32_t
		{
			kOpen,
			kClose,
			kNone,
		};
		// アニメーションの状態
		enum class AnimState : uint32_t
		{
			kIdle,
			kRun,
			kEnd,
		};

	public:
		// 初期化
		void Initialize(Boss* boss);
		// 更新
		void Update();
		// 描画
		void Draw(ICamera* camera, DrawDesc::LightDesc lightDesc);
	public:
		// 受付
		void AnimationExecute(AnimType type);
		void AnimationExecute(AnimType type, float easeFrame);
	private:
		// ヒエラルキー作成
		void CreateHierarchy(std::string hierarchyName, std::string modelTag, const EulerTransform& transform);
		void CreateHierarchy(std::string hierarchyName, std::string modelTag, const EulerTransform& transform, uint32_t texture);
	private:
		// ボス本体
		Boss* boss_ = nullptr;
		// パーツごと
		std::unordered_map<std::string, Hierarchy> hierarchys_;
		// タイマー
		FrameTimer animTimer_;
		// アニメーション中か
		AnimState animState_;
		// どのアニメーション状態か
		AnimType animType_;
	public:
		// オープン状態
		bool IsOpen() { return animType_ == AnimType::kOpen && !animTimer_.IsActive(); }
	};
}
