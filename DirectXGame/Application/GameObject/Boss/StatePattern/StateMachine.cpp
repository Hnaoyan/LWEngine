#include "StateMachine.h"
#include "Engine/LwLib/LwEngineLib.h"
#include "Application/GameObject/GameObjectLists.h"

void BossState::StateManager::Initialize(Boss* boss)
{
	boss_ = boss;
}

void BossState::StateManager::ChangeRequest(std::unique_ptr<IState> newState)
{
	if (boss_->GetState()) {
		// 終了処理
		boss_->GetState()->Exit();
	}

	newState->PreInitialize(boss_);
	newState->Initialize();

	boss_->ChangeState(std::move(newState));
}
void BossState::StateDecider::Initialize(Boss* boss, Player* player)
{
	// ポインタの設定
	boss_ = boss;
	player_ = player;

	tableTag_ = "Default";
	//tables_[tableTag_].patterns.push_back(StatePattern::kMissile);
	tables_[tableTag_].patterns.push_back(StatePattern::kMove);
	tables_[tableTag_].patterns.push_back(StatePattern::kUpdown);
	tables_[tableTag_].patterns.push_back(StatePattern::kAttack);
	tables_[tableTag_].patterns.push_back(StatePattern::kWait);
	tables_[tableTag_].patterns.push_back(StatePattern::kUpdown);
	tables_[tableTag_].maxStep = (uint32_t)tables_[tableTag_].patterns.size() - 1;

	tableTag_ = "MoveType";
	//tables_[tableTag_].patterns.push_back(StatePattern::kMissile);
	tables_[tableTag_].patterns.push_back(StatePattern::kUpdown);
	tables_[tableTag_].patterns.push_back(StatePattern::kMove);
	tables_[tableTag_].patterns.push_back(StatePattern::kUpdown);
	tables_[tableTag_].patterns.push_back(StatePattern::kMove);
	tables_[tableTag_].patterns.push_back(StatePattern::kWait);
	tables_[tableTag_].maxStep = (uint32_t)tables_[tableTag_].patterns.size() - 1;

	tableTag_ = "AttackType";
	tables_[tableTag_].patterns.push_back(StatePattern::kUpdown);
	tables_[tableTag_].patterns.push_back(StatePattern::kMissile);
	tables_[tableTag_].patterns.push_back(StatePattern::kWait);
	tables_[tableTag_].patterns.push_back(StatePattern::kAttack);
	tables_[tableTag_].patterns.push_back(StatePattern::kUpdown);
	tables_[tableTag_].maxStep = (uint32_t)tables_[tableTag_].patterns.size() - 1;

	tableTag_ = "MoveAttack";
	tables_[tableTag_].patterns.push_back(StatePattern::kMove);
	tables_[tableTag_].patterns.push_back(StatePattern::kMissile);
	//tables_[tableTag_].patterns.push_back(StatePattern::kWait);
	tables_[tableTag_].patterns.push_back(StatePattern::kOrbitMove);
	tables_[tableTag_].patterns.push_back(StatePattern::kAttack);
	tables_[tableTag_].maxStep = (uint32_t)tables_[tableTag_].patterns.size() - 1;

	tableTag_ = "UpDownMove";
	tables_[tableTag_].patterns.push_back(StatePattern::kUpdown);
	tables_[tableTag_].patterns.push_back(StatePattern::kMove);
	tables_[tableTag_].patterns.push_back(StatePattern::kMissile);
	tables_[tableTag_].patterns.push_back(StatePattern::kMove);
	tables_[tableTag_].patterns.push_back(StatePattern::kUpdown);
	tables_[tableTag_].maxStep = (uint32_t)tables_[tableTag_].patterns.size() - 1;

	section_.push_back("AttackType");
	section_.push_back("MoveAttack");
	section_.push_back("UpDownMove");
	section_.push_back("MoveAttack");

	currentStep_ = 0;
	IsInActionSequence_ = false;
	isCooltime_ = false;
}

void BossState::StateDecider::StateDecide(StateVariant nowState)
{
	nowState;
	if (!IsInActionSequence_) {
		// テーブルの選択（乱数
		//randomValue_ = LwLib::GetRandomValue(0, 6);
		//RandomTable(randomValue_);
		
		if (sectionIndex_ < section_.size()) {
			std::string tag = section_[sectionIndex_];
			currentTabletag_ = tag;
			StateSelect(tables_[tag].patterns[currentStep_]);
		}
		else {
			sectionIndex_ = 0;
			StateSelect(tables_[section_[sectionIndex_]].patterns[currentStep_]);
		}

		float x_zLength = Vector2::Length(Vector2(boss_->worldTransform_.GetWorldPosition().x, boss_->worldTransform_.GetWorldPosition().z) -
			Vector2(player_->worldTransform_.GetWorldPosition().x, player_->worldTransform_.GetWorldPosition().z));
		if (x_zLength > 75.0f)
		{

		}

		IsInActionSequence_ = true;
		currentStep_++;
		return;
	}
	else {
		// パターンテーブルの最大まで通っている場合
		if (currentStep_ > tables_[section_[sectionIndex_]].maxStep) {
			sectionIndex_++;
			isCooltime_ = true;
			IsInActionSequence_ = false;
			currentStep_ = 0;
		}
		else {
			// 次のパターン
			StateSelect(tables_[section_[sectionIndex_]].patterns[currentStep_]);
			currentStep_++;
			return;
		}
	}
	if (isCooltime_) {
		boss_->StateManager()->ChangeRequest(std::make_unique<WaitState>());
		WaitState* newState = static_cast<WaitState*>(boss_->GetState());
		// 体力少ない場合
		if (boss_->GetHealth()->GetHPRatio() <= 0.5f) {
			newState->SetTimer(30.0f);
		}
		// 通常状態
		else {
			newState->SetTimer(150.0f);
		}
		isCooltime_ = false;
		return;
	}
}

void BossState::StateDecider::TableSelect(std::string tableTag) {
	currentTabletag_ = tableTag;
	StateSelect(tables_[currentTabletag_].patterns[0]);
	IsInActionSequence_ = true;
	currentStep_ = 0;
	currentStep_++;
}

void BossState::StateDecider::StateSelect(StatePattern number)
{
	switch (number)
	{
	case BossState::StateDecider::StatePattern::kAttack:
		boss_->StateManager()->ChangeRequest(std::make_unique<AttackState>());
		break;
	case BossState::StateDecider::StatePattern::kMove:
		boss_->StateManager()->ChangeRequest(std::make_unique<MoveState>());
		break;
	case BossState::StateDecider::StatePattern::kUpdown:
		boss_->StateManager()->ChangeRequest(std::make_unique<UpDownState>());
		break;
	case BossState::StateDecider::StatePattern::kWait:
		boss_->StateManager()->ChangeRequest(std::make_unique<WaitState>());
		break;
	case BossState::StateDecider::StatePattern::kTeleport:
		boss_->StateManager()->ChangeRequest(std::make_unique<TeleportState>());
		break;
	case BossState::StateDecider::StatePattern::kMissile:
		boss_->StateManager()->ChangeRequest(std::make_unique<MissileAttackState>());
		break;
	case BossState::StateDecider::StatePattern::kOrbitMove:
		boss_->StateManager()->ChangeRequest(std::make_unique<OrbitMoveState>());
		break;
	case BossState::StateDecider::StatePattern::kMax:
		break;
	}
}

void BossState::IState::PreInitialize(Boss* boss)
{
	// ポインタ設定
	boss_ = boss;

}

void BossState::IState::RotateUpdate()
{
	Vector3 normalize = boss_->GetPlayer()->worldTransform_.GetWorldPosition() - boss_->worldTransform_.GetWorldPosition();
	normalize = Vector3::Normalize(normalize);
	normalize *= -1.0f;
	boss_->worldTransform_.transform_.rotate.y = std::atan2f(normalize.x, normalize.z);
}

void BossState::IState::TimerUpdate(StateVariant state)
{
	// 変更タイマー
	changeTimer_.Update();
	// 終了時に変更
	if (changeTimer_.IsEnd()/* || !changeTimer_.IsActive()*/) {
		boss_->GetDecider()->StateDecide(state);
		return;
	}
}

void BossState::IState::GenerateBullet(const Vector3& direct, const float& speed)
{
	EulerTransform transform = boss_->worldTransform_.transform_;
	transform.scale = GlobalVariables::GetInstance()->GetValue<Vector3>("BossNormalBullet", "Scale");
	// 生成
	std::unique_ptr<IBullet> bullet = std::make_unique<BossSystemContext::NormalBullet>();
	bullet->Initialize();
	bullet->SetVelocity(direct * speed);
	bullet->transform_ = transform;
	bullet->Update();

	boss_->GetNormalBulletCluster()->AddBullet(std::move(bullet));
}
