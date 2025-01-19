#include "PlayerParticleManager.h"
#include "Application/GameObject/GameObjectLists.h"
#include "Application/GameObject/Particle/User/ParticlePaths.h"
#include "Engine/Particle/GPUParticleSystem.h"
#include "Engine/3D/ModelUtility/ModelManager.h"
#include "Engine/3D/ModelUtility/ModelRenderer.h"
#include "Engine/2D/TextureManager.h"

#include <cassert>

void PlayerContext::ParticleManager::Initialize(Player* player)
{
	assert(player);
	ISystem::Initialize(player);

	// 移動のエミッター作成
	CreateMoveEmitter();
	moveTrail_ = std::make_unique<MoveTrail>();
	moveTrail_->Initialize(player);
}

void PlayerContext::ParticleManager::Update()
{
	// 移動パーティクル
	MoveParticleUpdate();
	moveTrail_->Update();
}

void PlayerContext::ParticleManager::Draw(ICamera* camera)
{
	moveTrail_->Draw(camera);
}

void PlayerContext::ParticleManager::MoveParticleUpdate()
{
	// 生成の制御
	bool moveCheck = std::fabsf(player_->velocity_.x) >= 0.1f || std::fabsf(player_->velocity_.z) >= 0.1f;
	bool isState = std::holds_alternative<IdleVertical*>(player_->VerticalState()->GetVariant());
	if (moveCheck && isState) {
		//moveParams_.isActive = true;
	}
	else {
		moveParams_.isActive = false;
	}
}

void PlayerContext::ParticleManager::CreateMoveEmitter()
{
	// 移動のパーティクル
	std::unique_ptr<GPUParticleEmitter> instance = std::make_unique<PlayerParticle::MoveEffect>();
	instance->Initialize(ModelManager::GetModel("Plane"));
	PlayerParticle::MoveEffect* pre = static_cast<PlayerParticle::MoveEffect*>(instance.get());
	pre->SetPlayer(player_);
	gpuParticle_->CreateEmitter(std::move(instance), "PlayerMove");
}

void PlayerContext::MoveTrail::Initialize(Player* player)
{
	assert(player);
	// 
	player_ = player;

	// ワールドトランスフォームの初期化
	//worldTransform_.transform_.translate.y = GlobalVariables::GetInstance()->GetValue<float>("PlayerBasic", "TrailY");

	// ポリゴンの初期化
	triangle_ = std::make_unique<Trail3D>();
	triangle_->SetCamera(player_->GetCamera());
	triangle_->SetMaxWidth(0.5f);
	Vector3 rgb = GlobalVariables::GetInstance()->GetValue<Vector3>("PlayerBasic", "TrailColor");
	triangle_->SetColor(Vector4(rgb.x, rgb.y, rgb.z, 1.0f));

	// 保存数の最大値
	// ここの値を内部で書き換えてもスムーズになるような処理も作る
	trailPoint_.second = 20;
}

void PlayerContext::MoveTrail::Update()
{
#ifdef IMGUI_ENABLED
	Vector3 rgb = GlobalVariables::GetInstance()->GetValue<Vector3>("PlayerBasic", "TrailColor");
	triangle_->SetColor(Vector4(rgb.x, rgb.y, rgb.z, 1.0f));
	//worldTransform_.transform_.translate.y = GlobalVariables::GetInstance()->GetValue<float>("PlayerBasic", "TrailY");
#endif // IMGUI_ENABLED

	// トランスフォームの更新
	float yOffset = player_->GetRoundShadow()->GetWorldTransform()->GetWorldPosition().y + 0.5f;
	Vector3 position = Vector3(player_->worldTransform_.GetWorldPosition().x,
		yOffset, player_->worldTransform_.GetWorldPosition().z);
	// 要素追加
	trailPoint_.first.push_back(position);
	// 最大値内に収める
	if (trailPoint_.first.size() > trailPoint_.second) {
		trailPoint_.first.erase(trailPoint_.first.begin());
	}
}

void PlayerContext::MoveTrail::Draw(ICamera* camera)
{
	size_t maxNumber = trailPoint_.first.size() - 1;
	if (trailPoint_.first[0] == trailPoint_.first[maxNumber]) {
		return;
	}

	if (trailPoint_.first.size() > 8) {
		std::vector<Vector3> interpolatedPoints;
		for (int i = 1; i < trailPoint_.first.size() - 2; ++i) {
			for (float t = 0.0f; t <= 1.0f; t += (1.0f / 30.0f)) {
				t = std::clamp(t, 0.0f, 1.0f);
				interpolatedPoints.push_back(LwLib::Curve::CatmullRomSpline(trailPoint_.first[i - 1], trailPoint_.first[i], trailPoint_.first[i + 1], trailPoint_.first[i + 2], t));
			}
		}
		//triangle_->Update(interpolatedPoints);
		triangle_->LerpWidthVertex(trailPoint_.first);
		triangle_->Update();
		isInvisible_ = false;
	}
	else {
		// 頂点の更新
		isInvisible_ = true;
		triangle_->Update();
	}

	// 描画
	if (!isInvisible_) {
		ModelRenderer::TrailDraw(camera, triangle_.get());
	}
}
