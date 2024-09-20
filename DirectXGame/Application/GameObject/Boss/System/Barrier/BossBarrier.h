#pragma once
#include <stdint.h>

namespace BossSystemContext
{
	class BarrierManager
	{
	public:
		//void Initialize();
		//void Create(uint32_t generateValue);
		//void DamageProcess();
		//void BreakProcess();

	private:
		uint32_t currentHP_ = 0;
		uint32_t maxHP_ = 0;
		bool isActive_ = false;
	};
}