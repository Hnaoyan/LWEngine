#include "IWeapon.h"
#include "Application/GameObject/IGameObject.h"

void IWeapon::Initialize(IGameObject* object)
{
	// 初期化
	worldTransform_.Initialize();
	// 親子関係
	worldTransform_.parent_ = &object->worldTransform_;
}

void IWeapon::Update()
{

	// 更新
	worldTransform_.UpdateMatrix();
}
