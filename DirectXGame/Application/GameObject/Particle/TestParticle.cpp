#include "TestParticle.h"

void TestParticle::Initialie(Model* model)
{
	model_ = model;
	material_ = std::make_unique<Material>();
	material_->CreateMaterial();

	worldTransform_.Initialize();

	eulerTransform_.scale = Vector3(1.0f, 1.0f, 1.0f);
}

void TestParticle::Update()
{

	actionTimer_.Update();

	if (actionTimer_.IsActive()) {
		isInvisible_ = false;
		Vector3 maxScale = Vector3(10.0f, 10.0f, 1.0f);
		eulerTransform_.scale = Ease::Easing(Vector3(1.0f, 1.0f, 1.0f), maxScale, actionTimer_.GetElapsedFrame());

	}
	else {
		//isInvisible_ = true;
	}

	worldTransform_.UpdateMatrix();
	//worldTransform_.worldMatrix_ = Matrix4x4::MakeBillBoardMatrix()
}

void TestParticle::Draw(ModelDrawDesc desc)
{
	// 隠す処理
	if (isInvisible_) {
		return;
	}

	worldTransform_.worldMatrix_ = Matrix4x4::MakeBillBoardMatrix(eulerTransform_.translate, desc.camera->transform_.translate, eulerTransform_.scale, Vector3::Up());
	worldTransform_.TransferMatrix();

	DrawDesc::LightDesc lightDesc{};
	lightDesc.directionalLight = desc.directionalLight;
	lightDesc.pointLight = desc.pointLight;
	lightDesc.spotLight = desc.spotLight;

	DrawDesc::ModelDesc modelDesc{};
	modelDesc.SetDesc(model_);
	modelDesc.worldTransform = &worldTransform_;
	modelDesc.material = material_.get();

	ModelRenderer::NormalDraw(desc.camera, modelDesc, lightDesc);

}

void TestParticle::ActiveAception(const float& frame)
{
	actionTimer_.Start(frame);
	isInvisible_ = false;

}
