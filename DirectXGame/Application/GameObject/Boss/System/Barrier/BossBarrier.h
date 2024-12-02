#pragma once
#include <stdint.h>
#include <memory>
#include "Engine/3D/Drawer/Model.h"
#include "Engine/LwLib/Utillity/FrameTimer.h"

class Boss;

namespace BossSystemContext
{
	class BarrierManager
	{
	private:
		// データ用
		struct Parameter {
			float currentHP = 0;
			float maxHP = 0;
			bool isActive = false;
			bool isShattered = false;
		};
		// アニメーション用
		struct AnimParameter {
			FrameTimer vanishTimer;	 // 破壊演出用タイマー
			FrameTimer reappearTimer; // 修復演出用タイマー
			//---ディゾルブ---//
			uint32_t shaveTexture;	// 削れている表現用のテクスチャ
			uint32_t recoveryTexture;	// 復活時の表現用のテクスチャ
		};

	public:
		// 初期化
		void Initialize(Boss* boss);
		// 更新
		void Update();
		// 描画
		void Draw(ICamera* camera, const DrawDesc::LightDesc& lightDesc);
		// ImGui
		void ImGuiDraw();
	public:
		// 生成
		void Create(float generateValue);
		// ダメージ
		void DamageProcess(float damage);
		// 壊れる
		void BarrierBreakExcept();
		
	private:
		//
		Parameter param{};
		//
		AnimParameter animParam{};
		// 復活するまでの時間
		FrameTimer recoveryTime_;
	private: // ポインタなどのオブジェクトとしての情報
		Boss* boss_ = nullptr;
		WorldTransform worldTransform_;
		uint32_t textureHandle_ = 0u;
		Model* model_ = nullptr;
		std::unique_ptr<Material> material_;
		bool isInvisible_ = true;
	public: // アクセッサ
		bool IsActive() { return param.isActive; }
		bool IsShattered() { return param.isShattered; }
		void SetIsShattered(bool isShatter) { param.isShattered = isShatter; }

	};
}