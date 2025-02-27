#include "BulletManager.h"
#include "Application/GameObject/Particle/User/Bullet/CPUEffect/BulletBombCluster.h"
#include "Engine/3D/ModelUtility/ModelManager.h"
#include <cassert>
#include <imgui.h>

float BulletManager::sTrackingRefreshFrame = 15.0f;
float BulletManager::sTrackingMaxCentripetal = 2.0f;
float BulletManager::sSmoothFactor = 0.1f;
float BulletManager::sMaxCentripetalForce = 500.0f;
float BulletManager::sForceDamping = 0.1f;
float BulletManager::sSpeedLimitOffset = 150.0f;
float BulletManager::sSpeed = 1.0f;
int32_t BulletManager::sTrackingProcessType = 0;

void BulletManager::Initialize(Model* model)
{
	//models_ = models;
	model_ = model;
	clusters_.clear();

	isDrawCheck_.reset();

	trailManager_ = std::make_unique<TrailManager>();
	trailManager_->SetGPUParticle(gpuParticle_);
}

void BulletManager::Update()
{
	for (auto it = clusters_.begin(); it != clusters_.end(); ++it) {
		(*it).second->Update();
	}
}

void BulletManager::Draw(ModelDrawDesc desc)
{
	// 弾の描画
	for (auto it = clusters_.begin(); it != clusters_.end(); ++it) {
		(*it).second->Draw(desc);
	}

	// 軌跡の処理
	trailManager_->SetFlags(this->isDrawCheck_);
	trailManager_->Draw(desc.camera);
}

void BulletManager::CollisionUpdate(CollisionManager* manager)
{
	for (auto it = clusters_.begin(); it != clusters_.end(); ++it) {
		IBulletCluster* obj = static_cast<IBulletCluster*>((*it).second.get());
		if (dynamic_cast<IBulletCluster*>((*it).second.get())) {
			obj->CollisionUpdate(manager);
		}
	}
}

void BulletManager::ImGuiDraw()
{
	ImGui::SeparatorText("TrackingTestData");
	ImGui::DragFloat("sRefreshrate", &sTrackingRefreshFrame, 0.01f);
	ImGui::DragFloat("sMaxCentripetal", &sTrackingMaxCentripetal, 0.01f);
	ImGui::DragFloat("sSmoothFactor", &sSmoothFactor, 0.01f);
	ImGui::DragFloat("sMaxCentripetalForce", &sMaxCentripetalForce, 0.1f);
	ImGui::DragFloat("sForceDamping", &sForceDamping, 0.01f);
	ImGui::DragFloat("sSpeedLimitOffset", &sSpeedLimitOffset, 0.01f);
	ImGui::InputInt("sTrackingProcessType", &sTrackingProcessType, 1);
	ImGui::DragFloat("sSpeed", &sSpeed, 0.1f);
	ImGui::Text("");
	// 描画チェック
	if (ImGui::TreeNode("DrawFlags")) {
		bool isCheck = isDrawCheck_[uint32_t(TrackingAttribute::kGenius)];
		ImGui::Checkbox("Genius", &isCheck);
		isDrawCheck_[uint32_t(TrackingAttribute::kGenius)] = isCheck;

		isCheck = isDrawCheck_[uint32_t(TrackingAttribute::kSuperior)];
		ImGui::Checkbox("Superior", &isCheck);
		isDrawCheck_[uint32_t(TrackingAttribute::kSuperior)] = isCheck;

		isCheck = isDrawCheck_[uint32_t(TrackingAttribute::kInferior)];
		ImGui::Checkbox("Inferior", &isCheck);
		isDrawCheck_[uint32_t(TrackingAttribute::kInferior)] = isCheck;

		FindCluster("Boss:Superior")->SetInvisible(isDrawCheck_[uint32_t(TrackingAttribute::kSuperior)]);
		FindCluster("Boss:Inferior")->SetInvisible(isDrawCheck_[uint32_t(TrackingAttribute::kInferior)]);
		FindCluster("Boss:Genius")->SetInvisible(isDrawCheck_[uint32_t(TrackingAttribute::kGenius)]);
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("Trail")) {
		ImGui::Text("Size:%d", trailManager_->GetContainairSize());
		ImGui::TreePop();
	}
}

void BulletManager::AddCluster(const std::string& tag)
{
	std::unique_ptr<InstancedGroup> instance = std::make_unique<IBulletCluster>();
	static_cast<IBulletCluster*>(instance.get())->Initialize(model_);
	static_cast<IBulletCluster*>(instance.get())->SetTrailManager(trailManager_.get());
	static_cast<IBulletCluster*>(instance.get())->SetGPUParticle(gpuParticle_);
	static_cast<IBulletCluster*>(instance.get())->SetTag(tag);

	// 敵かプレイヤーかを判断して変化を加える
	size_t position = tag.find(":");
	if (position != std::string::npos) {
		std::string zokusei = tag.substr(0, position);
		// グローバル変数用のポインタ
		GlobalVariables* globalValue = GlobalVariables::GetInstance();
		IBulletCluster* cluster = dynamic_cast<IBulletCluster*>(instance.get());
		if (!cluster) {
			return;
		}
		if ("Player" == zokusei) {
			cluster->SetColor({ 1.0f,1.0f,1.0f,1.0f });
		}
		else if ("Boss" == zokusei) {
			std::string colorName = tag.substr(position + 1);
			// 劣等
			if (colorName == "Inferior") {
				cluster->SetColor(globalValue->GetValue<Vector4>("Bullet","InferiorColor"));
				cluster->SetTrailColor(globalValue->GetValue<Vector3>("Bullet", "InferiorTrailColor"));
			}
			// 優等
			else if (colorName == "Superior") {
				cluster->SetColor(globalValue->GetValue<Vector4>("Bullet", "SuperirorColor"));
				cluster->SetTrailColor(globalValue->GetValue<Vector3>("Bullet", "SuperirorTrailColor"));
			}
			// 秀才
			else if (colorName == "Genius") {
				cluster->SetColor(globalValue->GetValue<Vector4>("Bullet", "GeniusColor"));
				cluster->SetTrailColor(globalValue->GetValue<Vector3>("Bullet", "GeniusTrailColor"));
			}
			// コンテナ
			else if (colorName == "ContainerBullet"){
				cluster->SetColor(globalValue->GetValue<Vector4>("Bullet", "ContainerColor"));
				cluster->SetTrailColor(globalValue->GetValue<Vector3>("Bullet", "ContainerTrailColor"));
			}
			// 通常弾
			else {
				cluster->SetColor({ 1.0f,0.3f,0.0f,1.0f });
				cluster->SetTrailColor({ 0.8f,0.2f,0.0f });
			}
		}
	}

	// ここに爆発板ポリクラスター作成

	std::unique_ptr<BulletBombCluster> bombCluster = std::make_unique<BulletBombCluster>();
	bombCluster->Initialize(ModelManager::GetModel("BombPlane"));
	IBulletCluster* castPtr = static_cast<IBulletCluster*>(instance.get());

	// 
	castPtr->SetBombCluster(bombCluster.get());

	clusters_.emplace(tag, std::move(instance));
	std::string bombTag = "Bomb" + tag;
	clusters_.emplace(bombTag, std::move(bombCluster));
}

void BulletManager::AddCluster(const std::string& tag, Model* model)
{
	std::unique_ptr<InstancedGroup> instance = std::make_unique<IBulletCluster>();
	static_cast<IBulletCluster*>(instance.get())->Initialize(model);
	static_cast<IBulletCluster*>(instance.get())->SetTrailManager(trailManager_.get());
	static_cast<IBulletCluster*>(instance.get())->SetGPUParticle(gpuParticle_);
	static_cast<IBulletCluster*>(instance.get())->SetTag(tag);
	clusters_.emplace(tag, std::move(instance));
}

void BulletManager::AddCluster(const std::string& tag, Model* model, uint32_t texture)
{
	std::unique_ptr<InstancedGroup> instance = std::make_unique<IBulletCluster>();
	static_cast<IBulletCluster*>(instance.get())->Initialize(model);
	static_cast<IBulletCluster*>(instance.get())->SetTrailManager(trailManager_.get());
	static_cast<IBulletCluster*>(instance.get())->SetGPUParticle(gpuParticle_);
	static_cast<IBulletCluster*>(instance.get())->SetTexture(texture);
	static_cast<IBulletCluster*>(instance.get())->SetTag(tag);
	clusters_.emplace(tag, std::move(instance));
}

void BulletManager::DeleteCluster(const std::string& tag)
{
	// イテレータ取得
	std::unordered_map<std::string, std::unique_ptr<InstancedGroup>>::iterator it = clusters_.find(tag);
	// なければエラー
	assert(it != clusters_.end());
	// あればイテレータで削除処理
	clusters_.erase(it);
}

IBulletCluster* BulletManager::FindCluster(const std::string& tag)
{
	// イテレータ取得
	std::unordered_map<std::string, std::unique_ptr<InstancedGroup>>::iterator it = clusters_.find(tag);
	// なければエラー
	assert(it != clusters_.end());
	// 見つけたイテレータからポインタを取得
	return static_cast<IBulletCluster*>((*it).second.get());
}
