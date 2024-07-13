#include "AnimSampleObject.h"
#include "imgui.h"
#include "../../Engine/2D/TextureManager.h"
#include "../../Engine/3D/Drawer/Skydome.h"
#include "../../Engine/Input/Input.h"
#include "../../Engine/LwLib/LwEngineLib.h"

int32_t AnimSampleObject::sSerialNumber = 0u;
std::string AnimSampleObject::sObjectName = "SampleObject";

void AnimSampleObject::Initialize(Model* model, Model* cube)
{
	// モデル
	animModel_ = model;

	debugCube_ = cube;

	texture_ = TextureManager::Load("Resources/uvChecker.png");

	debugSphere_.reset(Skydome::CreateSkydome());

	// 番号
	serialNumber_ = sSerialNumber;
	sSerialNumber++;

	// 名前
	objectName_ = "Human" + std::to_string(serialNumber_);

	worldTransform_.Initialize();
	animation_.Initialize(animModel_->GetModelDataAdress());
	//animation_.modelData_ = &animModel_->GetModelData();
	//animation_.animData_ = animModel_->GetModelData().animData;
}

void AnimSampleObject::Update()
{

	Input* input = Input::GetInstance();
	//XINPUT_STATE joyState;
	Vector2 leftStick = input->XGetLeftJoystick();
	//if (input->GetJoystickState(0, joyState)) {
		if (leftStick.x < 0) {
			worldTransform_.transform_.rotate.y = 3.14f + 1.57f;
		}
		else if (leftStick.x > 0) {
			worldTransform_.transform_.rotate.y = 1.57f;
		}
		worldTransform_.transform_.translate.x += leftStick.x * kDeltaTime;
	//}
	if (input->PressKey(DIK_D)) {
		worldTransform_.transform_.translate.x += 0.01f;
	}
	else if (input->PressKey(DIK_A)) {
		worldTransform_.transform_.translate.x -= 0.01f;
	}

	// アニメーション更新
	animation_.Update();

	// アニメーションのローカル行列を
	//worldTransform_.localMatrix_ = animation_.localMatrix_;
	worldTransform_.UpdateMatrix();

}

void AnimSampleObject::Draw(const ModelDrawDesc& desc)
{
	ModelDrawDesc drawDesc{};
	drawDesc.worldTransform = &worldTransform_;
	drawDesc.camera = desc.camera;
	drawDesc.directionalLight = desc.directionalLight;
	drawDesc.spotLight = desc.spotLight;
	drawDesc.pointLight = desc.pointLight;

	animModel_->SkinningDraw(drawDesc, &animation_, texture_);


	//for (int i = 0; i < animation_.skeleton_.joints.size(); ++i) {
	//	Matrix4x4 local = animation_.skeleton_.joints[i].localMatrix;

	//	//animation_.skeleton_.joints[i].debug_.worldMatrix_ = Matrix4x4::Multiply(worldTransform_.worldMatrix_, local);
	//	//animation_.skeleton_.joints[i].debug_.transform_.translate = animation_.skeleton_.joints[i].transform.translate;
	//	animation_.skeleton_.joints[i].debug_.worldMatrix_ = animation_.skeleton_.joints[i].skeletonSpaceMatrix;
	//	animation_.skeleton_.joints[i].debug_.transform_.scale = { 0.2f,0.2f,0.2f };
	//	animation_.skeleton_.joints[i].debug_.UpdateMatrix();
	//	//animation_.skeleton_.joints[i].debug_.worldMatrix_ = Matrix4x4::Multiply(animation_.skeleton_.joints[i].debug_.worldMatrix_, worldTransform_.worldMatrix_);
	//	//animation_.skeleton_.joints[i].debug_.UpdateMatrix();
	//	desc.worldTransform = &animation_.skeleton_.joints[i].debug_;
	//	desc.camera = camera;
	//	debugCube_->Draw(desc);
	//}

}

void AnimSampleObject::ImGuiDraw()
{

	ImGui::Begin(objectName_.c_str());
	std::string name = "Position" + std::to_string(serialNumber_);
	ImGui::DragFloat3(name.c_str(), &worldTransform_.transform_.translate.x, 0.01f);
	name = "Rotate" + std::to_string(serialNumber_);
	ImGui::DragFloat3(name.c_str(), &worldTransform_.transform_.rotate.x, 0.01f);
	name = "Scale" + std::to_string(serialNumber_);
	ImGui::DragFloat3(name.c_str(), &worldTransform_.transform_.scale.x, 0.01f);

	animation_.ImGuiDraw(objectName_.c_str());
	this->animModel_->GetMaterial()->ImGuiDraw(objectName_);

	ImGui::End();

}
