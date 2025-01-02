#include "IGameObject.h"
#include <cassert>

int32_t IGameObject::sSerialNumber = 0u;

void IGameObject::Initialize(Model* model)
{
#ifdef IMGUI_ENABLED
	InitializeGlobalValue();
#endif // IMGUI_ENABLED

	// モデルの設定
	assert(model);
	model_ = model;
	// シリアル番号の設定
	commonSerialNumber_ = sSerialNumber;
	sSerialNumber++;
	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();

	// 丸影の初期化
	roundShadow_ = std::make_unique<RoundShadow>();
	roundShadow_->Initialize(this);
}

void IGameObject::Update()
{
	// 行列の更新
	worldTransform_.UpdateMatrix();
	// 丸影の更新
	roundShadow_->Update();
}
