#include "JustDudgeManager.h"
#include "../../Player.h"
#include "Application/GameSystem/GameSystem.h"
#include "Engine/2D/TextureManager.h"
#include "Engine/3D/ModelUtility/ModelManager.h"
#include "Engine/3D/ModelUtility/ModelRenderer.h"
#include <imgui.h>

PlayerContext::JustDodgeManager::JustDodgeManager(Player* player)
{
	// 設定
	ISystem::Initialize(player);
	// 残像用コライダー
	collider_.Initialize(player_->worldTransform_.transform_.scale.x, player_);
	collider_.SetAttribute(kCollisionAttributePlayer);

	dodgeColliderObject.material = std::make_unique<Material>();
	dodgeColliderObject.material->CreateMaterial();
	dodgeColliderObject.material->color_.w = 0.05f;
	dodgeColliderObject.transform.Initialize();
	dodgeColliderObject.transform.parent_ = &player_->worldTransform_;
	dodgeColliderObject.texture = TextureManager::Load("Resources/default/white2x2.png");
}

void PlayerContext::JustDodgeManager::Initialize(Player* player)
{
	// 設定
	player_ = player;
	// 残像用コライダー
	collider_.Initialize(player_->worldTransform_.transform_.scale.x, player_);
	collider_.SetAttribute(kCollisionAttributePlayer);

	dodgeColliderObject.material = std::make_unique<Material>();
	dodgeColliderObject.material->CreateMaterial();
	dodgeColliderObject.material->color_.w = 0.05f;
	dodgeColliderObject.transform.Initialize();
	dodgeColliderObject.transform.parent_ = &player_->worldTransform_;
	dodgeColliderObject.texture = TextureManager::Load("Resources/default/white2x2.png");
}

void PlayerContext::JustDodgeManager::DodgeExcept()
{
	// 回避受付開始
	dodgeTimer_.Start(justDodgeFrame_);
	// コライダーの座標
	colliderPosition_ = player_->worldTransform_.GetWorldPosition();
}

void PlayerContext::JustDodgeManager::InvisibleExcept(const float& frame)
{
	// スローモーション（無敵時間）開始処理
	invisible_.activeTimer.Start(frame);
	float keepFrame = 60.0f * 5.0f;
	comboData_.keepTimer.Start(keepFrame);
	comboData_.nowCombo++;

	invisible_.delayFrame = 0.0f;
	invisible_.receptionFlag = true;

	// 最大値に行ったら
	if (comboData_.nowCombo >= comboData_.maxCombo) {
		comboData_.nowCombo = comboData_.maxCombo;
		// 攻撃の変化
		player_->GetSystemFacede()->GetShootingManager()->SetAttackPattern(true);
	}
}

void PlayerContext::JustDodgeManager::Update()
{
	// 
	dodgeColliderObject.Update();
	// 無敵時間の処理（ジャスト回避
	if (invisible_.activeTimer.IsActive()) {
		GameSystem::sSpeedFactor = slowFactor_;
		player_->SetColor(Vector4(1.0f, 1.0f, 1.0f, 0.3f));
	}
	// 戻り始める処理
	if (invisible_.activeTimer.IsEnd()) {
		invisible_.returnTimer.Start(45.0f);
		//GameSystem::sSpeedFactor = 1.0f;
		player_->SetColor(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
	}
	// 戻ってくる時間
	if (invisible_.returnTimer.IsActive()) {
		GameSystem::sSpeedFactor = Ease::Easing(slowFactor_, 1.0f, invisible_.returnTimer.GetElapsedFrame());
	}
	// スローモーション後の猶予
	if (invisible_.returnTimer.IsEnd()) {
		dodgeCount_ = 0;
		invisible_.afterTimer.Start(30.0f);
	}
	// コンボ中の色
	if (comboData_.keepTimer.IsActive()) {
		// コンボ
		switch (comboData_.nowCombo)
		{
		case 0:
			player_->SetColor(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
			break;
		case 1:
			player_->SetColor(Vector4(1.0f, 1.0f, 0.0f, 0.7f));
			break;
		case 2:
			player_->SetColor(Vector4(1.0f, 0.0f, 0.0f, 0.7f));
			break;
		case 3:
			player_->SetColor(Vector4(0.0f, 1.0f, 0.0f, 0.7f));
			break;
		case 4:
			player_->SetColor(Vector4(0.0f, 0.0f, 1.0f, 0.7f));
			break;
		case 5:
			player_->SetColor(Vector4(0.0f, 1.0f, 1.0f, 1.0f));
			break;
		default:
			break;
		}
	}
	if (comboData_.keepTimer.IsEnd()) {
		player_->SetColor(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
	}

	// コンボ維持タイマー
	comboData_.keepTimer.Update();
	// コンボリセット
	if (comboData_.keepTimer.IsEnd()) {
		comboData_.nowCombo = 0;
	}
	// 回避タイマー
	dodgeTimer_.Update();
	// 無敵関係のタイマー
	invisible_.Update();
	// コライダーの更新
	if (IsActive()) {
		collider_.radius_ = player_->worldTransform_.transform_.scale.x * globalVariables_->GetValue<float>("PlayerAction", "DodgeSize");
	}
	else {
		collider_.radius_ = player_->worldTransform_.transform_.scale.x;
	}
	collider_.Update(colliderPosition_);
}

void PlayerContext::JustDodgeManager::Draw(ModelDrawDesc desc)
{
	DrawDesc::LightDesc light{};
	DrawDesc::ModelDesc model{};
	light.directionalLight = desc.directionalLight;
	light.pointLight = desc.pointLight;
	light.spotLight = desc.spotLight;
	model.SetDesc(ModelManager::GetModel("ColliderSphere"));
	model.material = dodgeColliderObject.material.get();
	model.worldTransform = &dodgeColliderObject.transform;
	model.texture = dodgeColliderObject.texture;
	ModelRenderer::NormalDraw(desc.camera, model, light);
}

void PlayerContext::JustDodgeManager::ImGuiDraw()
{
	static float bar = 0.01f;
	ImGui::DragFloat("Bar", &bar, 0.01f);
	ImGui::DragFloat("JustDudgeFrame", &justDodgeFrame_, bar);
	ImGui::DragFloat("SlowFactor", &slowFactor_, bar);
	ImGui::DragFloat3("scale", &dodgeColliderObject.transform.transform_.scale.x, bar);
	ImGui::InputInt("ComboCount", &comboData_.nowCombo);
	ImGui::InputInt("DodgeCount", &dodgeCount_);
}
