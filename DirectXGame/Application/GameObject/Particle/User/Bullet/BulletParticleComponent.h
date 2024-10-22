#pragma once

class IBullet;

namespace BulletParticle
{
	/// <summary>
	/// コンポーネント
	/// </summary>
	class Component {
	public:
		void SetBullet(IBullet* bullet) { bullet_ = bullet; }

	protected:
		IBullet* bullet_ = nullptr;

	};

}
