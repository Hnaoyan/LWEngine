#pragma once
#include <stdint.h>
#include <vector>
#include <map>
#include <string>
#include <variant>
#include <optional>

class Boss;
class Player;
namespace BossState {
	// ステート
	class AttackState;	// 通常射撃
	class MoveState;	// 移動
	class OrbitMoveState;	// 半円
	class UpDownState;	// 上下
	class WaitState;	// 待機
	class TeleportState;	// 瞬間移動
	class TurnMissileState;		// 急旋回攻撃
	class MissileAttackState;	// 追従攻撃
	class SystemDownState;	// ダウン状態
	class MissileBarrageState;	// 順にミサイル
	class MissileWaveState;		// 波ミサイル
	class MissileContainerState;	// デンドロビウム
	// 
	class PreDecide;
}

namespace BossState
{
	// クラスのリスト
	using StateVariant = std::variant<AttackState*, MoveState*, UpDownState*,
		WaitState*, TeleportState*, MissileAttackState*, OrbitMoveState*,
		SystemDownState*, MissileBarrageState*, MissileWaveState*, TurnMissileState*,
		MissileContainerState*>;

	/// <summary>
	/// 判断クラス
	/// </summary>
	class StateDecider {
	public:
		enum class StatePattern : uint32_t {
			kAttack,
			kMove,
			kUpdown,
			kWait,
			kTeleport,
			kMissile,
			kOrbitMove,
			kMissileBarrage,
			kMissileWave,
			kMissileContainer,
			kMissileTurn,
			kMax,
		};
		struct StateObject {
			std::vector<StateDecider::StatePattern> patterns;
			uint32_t number;
			uint32_t maxStep;
		};
	private:
		Boss* boss_ = nullptr;
		Player* player_ = nullptr;

		std::string tableTag_;

		void RandomTable(uint32_t random) {
			if (random == 0) {
				tableTag_ = "Default";
			}
			else if (random == 1) {
				tableTag_ = "MoveType";
			}
			else {
				tableTag_ = "AttackType";
			}
		}

		bool isAlgorithm_ = true;

	public:
		void Initialize(Boss* boss, Player* player);
		void StateDecide(StateVariant nowState);

	private:
		// ステートの変更処理
		void StateSelect(StatePattern number);
		// テーブルの決定
		void TableSelect(std::string tableTag);

		//void DefaultDecider();

	private:
		// テーブル中か
		bool IsInActionSequence_ = false;
		// 待機状態フラグ
		bool isCooltime_ = false;
		uint32_t randomValue_ = 0;
		// テーブル内の位置
		uint32_t currentStep_ = 0;
		// テーブルごとの配列
		std::map<std::string, StateObject> tables_;
		// ステートのリクエスト
		std::optional<StateVariant> request_;
		// 現在のテーブル名
		std::string currentTabletag_;
		// テーブルの名前リスト
		std::vector<std::string> section_;
		// セクション中の番号
		uint32_t sectionIndex_ = 0;
	};
}
