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
	public:
		// 初期化
		void Initialize(Boss* boss);
		// 更新
		void Update();
		// 描画
		void Draw(ICamera* camera, const DrawDesc::LightDesc& lightDesc);
		// 生成
		void Create(float generateValue);
		// ダメージ
		void DamageProcess(float damage);
		// 壊れる
		void BarrierBreak();
		
	public: // アクセッサ
		bool IsActive() { return param.isActive; }
		bool IsShattered() { return param.isShattered; }
		void SetIsShattered(bool isShatter) { param.isShattered = isShatter; }
	private:
		struct Parameter {
			float currentHP = 0;
			float maxHP = 0;
			bool isActive = false;
			bool isShattered = false;
		};

		Parameter param{};
		// 復活するまでの時間
		FrameTimer recoveryTime_;
	private: // ポインタなどのオブジェクトとしての情報
		Boss* boss_ = nullptr;
		WorldTransform worldTransform_;
		uint32_t textureHandle_ = 0u;
		Model* model_ = nullptr;
		std::unique_ptr<Material> material_;

	};
}