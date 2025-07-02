#include "NetworkScene.h"
#include "Engine/Scene/SceneManager.h"
#include "Engine/2D/TextureManager.h"
#include "Engine/3D/ModelUtility/ModelRenderer.h"

#include <imgui.h>

void NetworkScene::Initialize()
{
	// 基底クラス初期化
	IScene::Initialize();

}

void NetworkScene::GPUUpdate()
{
}

void NetworkScene::Update()
{
}

void NetworkScene::Draw()
{
	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();
#pragma region 背景

	Sprite::PreDraw(commandList);



	Sprite::PostDraw();

#pragma endregion
	// 深度クリア
	dxCommon_->ClearDepthBuffer();

	ModelRenderer::PreDraw(commandList);




	ModelRenderer::PostDraw();

#pragma region UI

	Sprite::PreDraw(commandList);


	Sprite::PostDraw();

#pragma endregion
}

void NetworkScene::UIDraw()
{
	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

	Sprite::PreDraw(commandList);



	Sprite::PostDraw();
}

void NetworkScene::ImGuiDraw()
{
	ImGui::Begin("NetWork");

	ImGui::End();
}
