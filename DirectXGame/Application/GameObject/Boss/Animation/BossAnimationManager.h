#pragma once
#include "Engine/3D/Drawer/Model.h"
#include "Engine/LwLib/LwEnginePaths.h"
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

		// ヒットエフェクト
		struct HitEffect
		{
			FrameTimer timer;	// それぞれの状態
			int32_t state = 0u;	// 0:0に向かっていく 1:1に戻ってくる
			float alpha = 0.0f;		// アルファ
			float returnFrame = 6.0f;	// 1~0~1のフレーム
			// セットアップ関数
			void StartSetup() {
				timer.Start(returnFrame);
				state = 0;
				alpha = 1.0f;
			}
			// 更新処理
			void Update() {
				// タイマーの更新
				timer.Update();
				// 状態ごとのアルファの値
				if (state == 0) {
					alpha = Ease::Easing(1.0f, 0.0f, timer.GetElapsedFrame());
				}
				else {
					alpha = Ease::Easing(0.0f, 1.0f, timer.GetElapsedFrame());
				}
				if (timer.IsEnd()) {
					if (state == 0) {
						state++;
						timer.Start(returnFrame);
					}
				}
			}
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
		// ImGui
		void ImGuiDraw();
	public:
		// 受付
		void AnimationExecute(AnimType type);
		void AnimationExecute(AnimType type, float easeFrame);

		// ダメージを食らった時の点滅
		void AnimationDamageExecute() {
			// アクティブならスキップ
			if (hitEffect_.timer.IsActive()) {
				return;
			}
			hitEffect_.StartSetup();
		}
		// オープン状態
		bool IsOpen() { return animType_ == AnimType::kOpen && !animTimer_.IsActive(); }

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
		// 死亡用のタイマー
		FrameTimer deathTimer_;

		// 食らったときのエフェクト処理
		HitEffect hitEffect_;


		// 共通のマテリアル
		std::unique_ptr<Material> commonMaterial_;

	};
}
