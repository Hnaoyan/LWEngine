#include "ShootingManager.h"
#include "Application/GameObject/GameObjectLists.h"

void PlayerContext::ShootingManager::Initialize(Player* player)
{
	ISystem::Initialize(player);

}

void PlayerContext::ShootingManager::Update()
{

}

void PlayerContext::ShootingManager::OnFire(const Vector3& direct)
{
	EulerTransform transform{};
	// サイズ設定
	transform.scale = { 1.0f,1.0f,1.0f };
	// 位置
	transform.translate = player_->GetWeaponManager()->weapon_->worldTransform_.GetWorldPosition();
	float speed = globalVariables_->GetValue<float>("PlayerAttack", "DefaultSpeed");
	std::unique_ptr<IBullet> bullet = std::make_unique<IBullet>();
	bullet->Initialize();
	bullet->transform_ = transform;
	bullet->GetCollider()->SetAttribute(kCollisionAttributeBullet);
	// 対象がいるか
	if (!player_->GetWeaponManager()->GetLockOn()->ExistTarget()) {
		speed = globalVariables_->GetValue<float>("PlayerAttack", "NotLockSpeed");
	}
	// ダメージの違い
	if (isChangeAttack_) {
		isChangeAttack_ = false;
		bullet->SetDamageRatio(5.0f);
		// 弾のスケール
		bullet->transform_.scale *= 1.5f;
		// コンボのリセット
		player_->GetSystemFacede()->GetDudgeManager()->ComboReset();
	}
	else {
		// 弾のスケール
		bullet->transform_.scale *= 0.75f;
	}

	bullet->SetVelocity(direct * speed);

	bulletManager_->FindCluster("Player:NormalBullet")->AddBullet(std::move(bullet));
}

void PlayerContext::ShootingManager::TrackingFire()
{

	GenerateTracking({ 1.0f,0.5f,0.0f }, TrackingAttribute::kSuperior);
	GenerateTracking({ -1.0f,0.5f,0.0f }, TrackingAttribute::kSuperior);

	GenerateTracking(Vector3::Right(), TrackingAttribute::kSuperior);
	GenerateTracking(Vector3::Left(), TrackingAttribute::kSuperior);
}

void PlayerContext::ShootingManager::GenerateTracking(const Vector3& direct, TrackingAttribute type)
{
	Matrix4x4 rotateMatrix = Matrix4x4::MakeRotateXYZMatrix(player_->worldTransform_.transform_.rotate);
	EulerTransform transform = player_->worldTransform_.transform_;
	transform.scale *= 0.5f;

	// 回転行列と目標（初期方向
	Vector3 newDirect = Matrix4x4::TransformVector3(direct, rotateMatrix);
	float initSpeed = 30.0f;

	// 弾のインスタンス生成
	std::unique_ptr<IBullet> bullet = std::make_unique<TrackingBullet>();
	static_cast<TrackingBullet*>(bullet.get())->SetGameObject(player_->GetBoss());
	static_cast<TrackingBullet*>(bullet.get())->SetTrackType(type);
	bullet->Initialize();
	bullet->SetVelocity(newDirect * initSpeed);
	bullet->GetCollider()->SetAttribute(kCollisionAttributeBullet);
	bullet->transform_ = transform;
	bulletManager_->FindCluster("Player:TrackingBullet")->AddBullet(std::move(bullet));
}

void PlayerContext::ShootingManager::FireContainer()
{

	Vector3 direct = Vector3::Up();
	EulerTransform transform = player_->worldTransform_.transform_;
	transform.scale = { 0.2f,0.2f,0.2f };
	// 生成処理
	BulletBuilder builder;
	float containerSpeed = 10.0f;
	builder.SetTargetObject(player_).SetDirect(direct).SetSpeed(containerSpeed).SetTransform(transform).SetAttribute(TrackingAttribute::kSuperior).SetIsRandStraight(true);
	builder.SetParentAttribute(1);
	builder.SetCluster(bulletManager_->FindCluster("Player:TrackingBullet"));
	bulletManager_->FindCluster("Player:ContainerBullet")->AddBullet(builder, BulletType::kContainer);
}
