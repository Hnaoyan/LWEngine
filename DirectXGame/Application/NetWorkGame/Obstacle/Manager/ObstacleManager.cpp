#include "ObstacleManager.h"
#include "../Obstacle.h"
#include "Engine/3D/ModelUtility/ModelManager.h"
#include "Engine/Collision/CollisionManager.h"

#include <imgui.h>

void ObstacleManager::Initialize()
{
	AddObstacle(Vector2(5.0f, 0.0f),Vector2(1.0f,6.0f));
	AddObstacle(Vector2(0.0f, -5.0f), Vector2(6.0f, 1.0f));
}

void ObstacleManager::Update()
{
	for (std::list<std::unique_ptr<Obstacle>>::iterator it = obstacles_.begin();
		it != obstacles_.end(); ++it)
	{
		(*it)->Update();
	}
}

void ObstacleManager::ImGuiDraw()
{
	
	ImGui::Begin("障害物管理");

	if (ImGui::Button("追加")) {
		AddObstacle(Vector2(0.0f, 0.0f), Vector2(1.0f, 1.0f));
	}
	ImGui::BeginChild("オブジェクト", ImVec2(350, 250), true);
	for (std::list<std::unique_ptr<Obstacle>>::iterator it = obstacles_.begin();
		it != obstacles_.end(); ++it)
	{
		(*it)->ImGuiDraw();
	}
	ImGui::EndChild();
	ImGui::End();

}

void ObstacleManager::CollisionUpdate(CollisionManager* manager)
{
	for (std::list<std::unique_ptr<Obstacle>>::iterator it = obstacles_.begin();
		it != obstacles_.end(); ++it)
	{
		(*it)->SetCollision(manager);
	}
}

void ObstacleManager::Draw(ModelDrawDesc desc)
{
	for (std::list<std::unique_ptr<Obstacle>>::iterator it = obstacles_.begin();
		it != obstacles_.end(); ++it)
	{
		(*it)->Draw(desc);
	}
}

void ObstacleManager::AddObstacle(const Vector2& position, const Vector2& size)
{
	Model* model = ModelManager::GetModel("Cube");
	std::unique_ptr<Obstacle> instance = std::make_unique<Obstacle>();
	instance->Initialize(model);
	instance->GetWorldTransform()->transform_.translate = Vector3(position.x, position.y, 0.0f);
	instance->GetWorldTransform()->transform_.scale = Vector3(size.x, size.y, 1.0f);

	obstacles_.push_back(std::move(instance));
}
