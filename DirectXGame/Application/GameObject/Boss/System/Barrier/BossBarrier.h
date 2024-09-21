#pragma once
#include <stdint.h>
#include <memory>
#include "Engine/3D/Drawer/Model.h"

class Boss;

namespace BossSystemContext
{
	class BarrierManager
	{
	public:
		void Initialize(Boss* boss);
		void Update();
		void Draw(ICamera* camera, const DrawDesc::LightDesc& lightDesc);
		void Create(uint32_t generateValue);
		//void DamageProcess();
		//void BreakProcess();
		bool IsActive() { return param.isActive; }
	private:
		struct Parameter {
			uint32_t currentHP = 0;
			uint32_t maxHP = 0;
			bool isActive = false;
		};

		Parameter param{};

	private: // ポインタなどのオブジェクトとしての情報
		Boss* boss_ = nullptr;
		WorldTransform worldTransform_;
		uint32_t textureHandle_ = 0u;
		Model* model_ = nullptr;
		std::unique_ptr<Material> material_;

	};
}