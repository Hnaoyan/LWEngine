//#pragma once
//#include <stdint.h>
//#include <vector>
//#include <map>
//#include <string>
//#include <variant>
//
//class Boss;
//class Player;
//namespace BossState {
//	class AttackState;
//	class MoveState;
//	class OrbitMoveState;
//	class UpDownState;
//	class WaitState;
//	class TeleportState;
//	class MissileAttackState;
//	class SystemDownState;
//	class MissileBarrageState;
//}
//
//namespace BossState
//{
//	// クラスのリスト
//	using StateVariant = std::variant<AttackState*, MoveState*, UpDownState*, WaitState*, TeleportState*, MissileAttackState*, OrbitMoveState*, SystemDownState*, MissileBarrageState*>;
//
//	/// <summary>
//	/// 判断クラス
//	/// </summary>
//	class StateDecider {
//	public:
//		enum class StatePattern : uint32_t {
//			kAttack,
//			kMove,
//			kUpdown,
//			kWait,
//			kTeleport,
//			kMissile,
//			kOrbitMove,
//			kMissileBarrage,
//			kMax,
//		};
//		struct StateObject {
//			std::vector<StateDecider::StatePattern> patterns;
//			uint32_t number;
//			uint32_t maxStep;
//		};
//	private:
//		Boss* boss_ = nullptr;
//		Player* player_ = nullptr;
//
//		std::string tableTag_;
//
//		void RandomTable(uint32_t random) {
//			if (random == 0) {
//				tableTag_ = "Default";
//			}
//			else if (random == 1) {
//				tableTag_ = "MoveType";
//			}
//			else {
//				tableTag_ = "AttackType";
//			}
//		}
//
//	public:
//		void Initialize(Boss* boss, Player* player);
//		void StateDecide(StateVariant nowState);
//
//	private:
//		// ステートの変更処理
//		void StateSelect(StatePattern number);
//		// テーブルの決定
//		void TableSelect(std::string tableTag);
//
//	private:
//		// テーブル中か
//		bool IsInActionSequence_ = false;
//		bool isCooltime_ = false;
//		uint32_t randomValue_ = 0;
//		// テーブル内の位置
//		uint32_t currentStep_ = 0;
//		std::map<std::string, StateObject> tables_;
//
//		std::string currentTabletag_;
//
//		std::vector<std::string> section_;
//		uint32_t sectionIndex_ = 0;
//	};
//}
