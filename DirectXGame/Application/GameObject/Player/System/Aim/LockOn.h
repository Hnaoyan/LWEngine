#pragma once
#include <vector>
#include <memory>
#include <list>
#include <variant>

// 敵
class IGameObject;
class SampleEnemy;
class Player;
class ICamera;
class Boss;

namespace PlayerContext {
	/// <summary>
	/// ロックオン処理を管理する
	/// </summary>
	class LockOn
	{
	public:
		// ロックオンデータ
		struct LockOnData {
			float threshold = 0.0f;
			float minDistanceZ = 5.0f;
			float maxDistanceZ = 50.0f;
		};
		// ロックオン対象のオブジェクトクラス
		using LockTarget = std::variant<SampleEnemy*, Boss*>;

	public:
		/// <summary>
		/// 初期化
		/// </summary>
		/// <param name="player"></param>
		void Initialize(Player* player);
		/// <summary>
		/// 更新（対象の処理
		/// </summary>
		void Update();
		/// <summary>
		/// 入力による対象設定・対象解除
		/// </summary>
		void ToggleLockOn(ICamera* camera);
		/// <summary>
		/// 対象を変更
		/// </summary>
		/// <param name="camera"></param>
		void ChangeLockOnTarget(ICamera* camera);
		/// <summary>
		/// ImGui
		/// </summary>
		void ImGuiDraw();
	private:
		/// <summary>
		/// ターゲット解除
		/// </summary>
		void TargetRelease();
		/// <summary>
		/// 敵を探す処理
		/// </summary>
		/// <param name="camera"></param>
		void SearchTarget(ICamera* camera);
		/// <summary>
		/// ロックオン対象を変更
		/// </summary>
		/// <param name="camera"></param>
		void ChangeTarget(ICamera* camera);

	public:
		// リストの設定
		void SetEnemyList(std::vector<std::unique_ptr<SampleEnemy>>* lists) { enemys_ = lists; }
		void SetBoss(Boss* boss) { boss_ = boss; }
		// ターゲットのポインタ
		IGameObject* GetTarget() const { return target_; }
		// 対象があるかどうか
		bool ExistTarget() const { return target_ ? true : false; }
	private:
		// プレイヤー
		Player* player_ = nullptr;
		// リスト
		std::vector<std::unique_ptr<SampleEnemy>>* enemys_ = nullptr;
		Boss* boss_ = nullptr;
		// ターゲット
		IGameObject* target_ = nullptr;
		// 内積の閾値
		LockOnData data{};

	};
}
