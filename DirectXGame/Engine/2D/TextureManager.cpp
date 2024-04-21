#include "TextureManager.h"
#include "../Utility/StringConverter.h"
#include "../Base/DirectXCommon.h"
#include "../Base/Utility/DxCreateLib.h"

#include <cassert>
#include <d3dx12.h>

void TextureManager::Initialize(DirectXCommon* dxCommon, std::string directoryPath)
{
    assert(dxCommon);

    dxCommon_ = dxCommon;
    directoryPath_ = directoryPath;

    device_ = dxCommon_->GetDevice();

}

ScratchImage TextureManager::LoadTexture(const std::string& filePath)
{
    // テクスチャファイルを呼んでプログラムで扱えるように
    ScratchImage image{};

    std::wstring filePathW = ConvertString(filePath);
    HRESULT result = LoadFromWICFile(filePathW.c_str(), WIC_FLAGS_FORCE_SRGB, nullptr, image);
    assert(SUCCEEDED(result));

    // ミップマップの作成
    ScratchImage mipImages{};
    result = GenerateMipMaps(image.GetImages(), image.GetImageCount(), image.GetMetadata(), TEX_FILTER_SRGB, 0, mipImages);
    assert(SUCCEEDED(result));

    return mipImages;
}

Microsoft::WRL::ComPtr<ID3D12Resource> TextureManager::CreateTextureResource(const TexMetadata& metadata)
{
    return Microsoft::WRL::ComPtr<ID3D12Resource>();
}

Microsoft::WRL::ComPtr<ID3D12Resource> TextureManager::UploadTextureData(Microsoft::WRL::ComPtr<ID3D12Resource> texture, const ScratchImage& mipImages, ID3D12GraphicsCommandList* commandList)
{
    std::vector<D3D12_SUBRESOURCE_DATA> subresources;
    PrepareUpload(device_, mipImages.GetImages(), mipImages.GetImageCount(), mipImages.GetMetadata(), subresources);
    uint64_t intermediateSize = GetRequiredIntermediateSize(texture.Get(), 0, UINT(subresources.size()));
    ID3D12Resource* intermediateResource = DxCreateLib::ResourceLib::CreateBufferResource(device_, intermediateSize);
    UpdateSubresources(commandList, texture.Get(), intermediateResource, 0, 0, UINT(subresources.size()), subresources.data());

    // Textureへの転送後は利用できるよう、D3D12_RESOURCE_STATE_COPY_DESTからD3D12_RESOURCE_STATE_GENERIC_READへResourceStateを変更する
    D3D12_RESOURCE_BARRIER barrier{};
    barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
    barrier.Transition.pResource = texture.Get();
    barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
    barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
    barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_GENERIC_READ;
    commandList->ResourceBarrier(1, &barrier);

    return Microsoft::WRL::ComPtr<ID3D12Resource>();
}
