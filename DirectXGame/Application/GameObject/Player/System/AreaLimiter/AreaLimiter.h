#pragma once
#include "Engine/LwLib/LwEnginePaths.h"
#include "Engine/Collision/Collider/ColliderLists.h"
#include "Engine/3D/Drawer/Model.h"
#include <memory>
#include "../IPlayerSystem.h"

class Player;

namespace PlayerContext
{
	/// <summary>
	/// 座標制限
	/// </summary>
	class AreaLimiter : public ISystem
	{
	public:
		AreaLimiter() {};
		~AreaLimiter() = default;

		// 初期化
		void Initialize(Player* player) override;
		// 更新
		void Update() override;

	private:
		Vector3 limitPoint_ = {};

	};

}
