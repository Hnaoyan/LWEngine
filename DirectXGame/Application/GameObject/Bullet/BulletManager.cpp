#include "BulletManager.h"
#include "Application/GameObject/Particle/User/Bullet/CPUEffect/BulletBombCluster.h"
#include "Engine/3D/ModelUtility/ModelManager.h"
#include <cassert>
#include <imgui.h>

void BulletManager::Initialize(Model* model)
{
	//models_ = models;
	model_ = model;
	clusters_.clear();

	isDrawCheck_.reset();

	trailManager_ = std::make_unique<TrailManager>();
	trailManager_->SetGPUParticle(gpuParticle_);

	AddCluster("Player:NormalBullet");
	AddCluster("Player:TrackingBullet");
	AddCluster("Player:TInferior");
	AddCluster("Player:TSuperior");
	AddCluster("Player:TGenius");
	AddCluster("Player:DivisionBullet");

	AddCluster("Boss:TrackingBullet");
	AddCluster("Boss:Inferior");
	AddCluster("Boss:Superior");
	AddCluster("Boss:Genius");
	AddCluster("Boss:NormalBullet");
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

		if ("Player" == zokusei) {
			static_cast<IBulletCluster*>(instance.get())->SetColor({ 1.0f,1.0f,1.0f,1.0f });
		}
		else if ("Boss" == zokusei) {
			std::string colorName = tag.substr(position + 1);
			if (colorName == "Inferior") {
				static_cast<IBulletCluster*>(instance.get())->SetColor({ 1.0f,0.0f,0.0f,1.0f });
				static_cast<IBulletCluster*>(instance.get())->SetTrailColor({ 1.0f,0.6f,0.6f });
			}
			else if (colorName == "Superior") {
				static_cast<IBulletCluster*>(instance.get())->SetColor({ 0.0f,1.0f,0.0f,1.0f });
				static_cast<IBulletCluster*>(instance.get())->SetTrailColor({ 0.6f,1.0f,0.6f });
			}
			else if (colorName == "Genius") {
				static_cast<IBulletCluster*>(instance.get())->SetColor({ 0.0f,0.0f,1.0f,1.0f });
				static_cast<IBulletCluster*>(instance.get())->SetTrailColor({ 0.6f,0.6f,1.0f });
			}
			else {
				static_cast<IBulletCluster*>(instance.get())->SetColor({ 0.0f,0.0f,0.0f,1.0f });
				static_cast<IBulletCluster*>(instance.get())->SetTrailColor({ 0.2f,0.2f,0.2f });
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
