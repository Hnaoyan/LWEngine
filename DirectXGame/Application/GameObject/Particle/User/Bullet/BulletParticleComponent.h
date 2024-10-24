#pragma once

class IBullet;
class BulletTrail;

namespace BulletParticle
{
	/// <summary>
	/// コンポーネント
	/// </summary>
	class Component {
	public: // セッター
		void SetBullet(IBullet* bullet) { bullet_ = bullet; }
		void SetTrail(BulletTrail* trail) { trail_ = trail; }

	protected: // それぞれのポインタ
		// 弾本体クラス
		IBullet* bullet_ = nullptr;
		// 軌跡クラス
		BulletTrail* trail_ = nullptr;
	};

}
