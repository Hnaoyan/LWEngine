#pragma once

class Boss;

namespace BossSystemContext
{

	class UIManager {
	public:
		void Initialize(Boss* boss);
		void Draw();

	private:
		Boss* boss_ = nullptr;

	};

}
