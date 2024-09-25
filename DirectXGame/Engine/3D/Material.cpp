#include "Material.h"
#include "../Base/DirectXCommon.h"
#include "../Base/Utility/DxCreateLib.h"
#include "Engine/2D/TextureManager.h"
#include "imgui.h"

void Material::CreateMaterial()
{
	// 色
	color_ = { 1.0f,1.0f,1.0f,1.0f };
	// 無し
	enableLighting_ = kNone;

	shininess_ = 100.0f;

	uvTransform_ = {
		{1.0f,1.0f,1.0f},
		{0.0f,0.0f,0.0f},
		{0.0f,0.0f,0.0f},
	};

	threshold_ = 0.0f;
	dissolveColor_ = {};
	dissolveTexture_ = TextureManager::Load("Resources/Dissolve/noise0.png");

	// リソース作成
	// デバイス
	ID3D12Device* device = DirectXCommon::GetInstance()->GetDevice();

	// マテリアルバッファの生成
	buffer_.CreateConstantBuffer(device);

	// データの転送
	buffer_.cMap_->color = color_;
	buffer_.cMap_->enableLighting = enableLighting_;
	buffer_.cMap_->uvTransform = Matrix4x4::MakeIdentity4x4();
	buffer_.cMap_->shininess = shininess_;
	buffer_.cMap_->dissolveColor = { 0.0f,0.0f,0.0f };
	buffer_.cMap_->dissolveThreshold = threshold_;
}

void Material::Update()
{
	// データの転送
	buffer_.cMap_->color = color_;
	buffer_.cMap_->enableLighting = enableLighting_;
	buffer_.cMap_->uvTransform = Matrix4x4::MakeUvTransformMatirx(uvTransform_.scale, uvTransform_.rotate, uvTransform_.translate);
	buffer_.cMap_->shininess = shininess_;
	buffer_.cMap_->coefficient = 0;
	buffer_.cMap_->dissolveThreshold = threshold_;
}

void Material::ImGuiDraw(const std::string& objName)
{
	std::string name = objName;
	ImGui::SeparatorText(name.c_str());
	name = objName + ":MaterialColor";
	ImGui::ColorEdit4(name.c_str(), &color_.x);
	name = objName + ":Shininess";
	ImGui::DragFloat(name.c_str(), &shininess_, 0.01f);
	name = objName + ":Coefficient";
	ImGui::DragFloat(name.c_str(), &coefficient_, 0.01f);
}
