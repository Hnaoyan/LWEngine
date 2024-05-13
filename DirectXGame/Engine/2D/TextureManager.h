#pragma once
#include "../Utility/Singleton.h"
#include "../3D/Descriptor/SRVHandler.h"

#include <string>
#include <array>
#include <vector>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXTex.h>

using namespace DirectX;

class DirectXCommon;

class TextureManager : public Singleton<TextureManager>
{
public: // 静的メンバ変数

	static const uint32_t kNumTextureHandle = 256;

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
		// SRVインデックス
		uint32_t descriptorNumIndex;
	};

	static uint32_t Load(const std::string& fileName);

	void Initialize(DirectXCommon* dxCommon, std::string directoryPath = "Resources/");

private:
	// 全般
	DirectXCommon* dxCommon_ = nullptr;
	// デバイス
	ID3D12Device* device_ = nullptr;

	// ディレクトリのパス
	std::string directoryPath_;

private: // テクスチャハンドル関係
	// テクスチャハンドルのコンテナ
	std::array<uint32_t, kNumTextureHandle> textureHandles_;
	// 次の番号
	static uint32_t sNextHandleIndex;
	/// <summary>
	/// ハンドルの設定
	/// </summary>
	/// <param name="textureHandle"></param>
	void SetTextureHandle(uint32_t textureHandle);

private:
	// テクスチャのコンテナ
	std::array<Texture, SRVHandler::kDescpritorSize> textures_;

	uint32_t descriptorIndex_ = 0u;

private:
	/// <summary>
	/// 
	/// </summary>
	uint32_t LoadInternal(const std::string fileName);

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

	/// <summary>
	/// テクスチャデータの作成
	/// </summary>
	Microsoft::WRL::ComPtr<ID3D12Resource> UploadTextureData(Microsoft::WRL::ComPtr<ID3D12Resource> texture, const ScratchImage& mipImages,
		ID3D12GraphicsCommandList* commandList);
};
