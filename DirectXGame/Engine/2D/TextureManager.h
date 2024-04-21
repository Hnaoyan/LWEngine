#pragma once
#include "../Utility/Singleton.h"

#include <string>
#include <vector>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXTex.h>

using namespace DirectX;

class DirectXCommon;

class TextureManager : public Singleton<TextureManager>
{
public:

	struct Texture
	{
		// テクスチャリソース
		Microsoft::WRL::ComPtr<ID3D12Resource> resource;
		// シェーダーのハンドル（CPU)
		D3D12_CPU_DESCRIPTOR_HANDLE cpuDescriptorHandle;
		// シェーダーのハンドル（GPU)
		D3D12_GPU_DESCRIPTOR_HANDLE gpuDescriptorHandle;
		// 名前
		std::string name;

	};

	void Initialize(DirectXCommon* dxCommon, std::string directoryPath = "Resources/Texture/");

private:
	DirectXCommon* dxCommon_ = nullptr;
	ID3D12Device* device_ = nullptr;


	std::string directoryPath_;

public:

	/// <summary>
	/// テクスチャの情報ロード
	/// </summary>
	/// <param name="filePath"></param>
	/// <returns></returns>
	ScratchImage LoadTexture(const std::string& filePath);

	/// <summary>
	/// テクスチャのリソース作成
	/// </summary>
	/// <param name="device"></param>
	/// <param name="metadata"></param>
	/// <returns></returns>
	Microsoft::WRL::ComPtr<ID3D12Resource> CreateTextureResource(const TexMetadata& metadata);


	Microsoft::WRL::ComPtr<ID3D12Resource> UploadTextureData(Microsoft::WRL::ComPtr<ID3D12Resource> texture, const ScratchImage& mipImages,
		ID3D12GraphicsCommandList* commandList);

};
