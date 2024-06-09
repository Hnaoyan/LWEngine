#include "TextureManager.h"
#include "../Utility/StringConverter.h"
#include "../Base/DirectXCommon.h"
#include "../Base/Utility/DxCreateLib.h"

#include <cassert>
#include <d3dx12.h>

uint32_t TextureManager::sNextHandleIndex = 0u;

uint32_t TextureManager::Load(const std::string& fileName)
{
    // テクスチャ自体の読み込み
    uint32_t textureNum = TextureManager::GetInstance()->LoadInternal(fileName);
    // ハンドルに設定
    TextureManager::GetInstance()->SetTextureHandle(textureNum);

    return textureNum;
}

void TextureManager::SetGraphicsRootDescriptorTable(ID3D12GraphicsCommandList* commandList, UINT rootParamIndex, uint32_t textureHandle)
{
    // デスクリプタヒープの配列
    assert(textureHandle < textures_.size());
    ID3D12DescriptorHeap* ppHeaps[] = { dxCommon_->GetSrvHandler()->GetSrvHeap()};
    commandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

    // シェーダリソースビューをセット
    commandList->SetGraphicsRootDescriptorTable(
        rootParamIndex, textures_[textureHandle].gpuDescriptorHandle);

}

void TextureManager::Initialize(DirectXCommon* dxCommon, std::string directoryPath)
{
    assert(dxCommon);

    dxCommon_ = dxCommon;
    directoryPath_ = directoryPath;

    device_ = dxCommon_->GetDevice();
    //descriptorIndex_ = 2;
}

const D3D12_RESOURCE_DESC TextureManager::GetResourceDesc(uint32_t textureHandle)
{
    assert(textureHandle < textures_.size());
    Texture& texture = textures_.at(textureHandle);
    return texture.resource->GetDesc();
}

void TextureManager::SetTextureHandle(uint32_t textureHandle)
{
    textureHandles_[textureHandle] = textureHandle;
    sNextHandleIndex++;
}

uint32_t TextureManager::LoadInternal(const std::string fileName)
{
    for (uint32_t i = 0; i < textures_.size(); ++i) {
        if (textures_[i].name == fileName) {
            return i;
        }
        else if(textures_[i].name == "") {
            break;  
        }
    }

    assert(descriptorIndex_ < SRVHandler::kDescpritorSize);
    
    uint32_t handleNum = descriptorIndex_;

    Texture& texture = textures_.at(handleNum);
    texture.name = fileName;

    ScratchImage mipImages = LoadTexture(fileName);
    DirectX::TexMetadata metadata = mipImages.GetMetadata();
    texture.resource = CreateTextureResource(metadata);

    Microsoft::WRL::ComPtr<ID3D12Resource> intermediateResource = UploadTextureData(texture.resource, mipImages, dxCommon_->GetCommandList());
    // metaDataを基にSRVの設定
    D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
    D3D12_RESOURCE_DESC resDesc = texture.resource->GetDesc();

    srvDesc.Format = resDesc.Format;
    srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
    srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels = UINT(metadata.mipLevels);

    texture.cpuDescriptorHandle = SRVHandler::GetSrvHandleCPU();
    texture.gpuDescriptorHandle = SRVHandler::GetSrvHandleGPU();
    texture.descriptorNumIndex = SRVHandler::GetNextDescriptorNum();
    // SRVのインデックスのインクリメント
    //SRVHandler::sNextDescriptorNum_++;
    SRVHandler::AllocateNextDescriptorNum();
    // テクスチャ管理インデックスのインクリメント
    descriptorIndex_++;

    // SRVの生成
    device_->CreateShaderResourceView(texture.resource.Get(), &srvDesc, texture.cpuDescriptorHandle);

    return handleNum;
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
    // metadataを基にReasourceの設定
    D3D12_RESOURCE_DESC resourceDesc{};
    resourceDesc.Width = UINT(metadata.width);	// Textureの幅
    resourceDesc.Height = UINT(metadata.height);	// Textureの高さ
    resourceDesc.MipLevels = UINT16(metadata.mipLevels);	// mipmapの数
    resourceDesc.DepthOrArraySize = UINT16(metadata.arraySize);	// 奥行き or 配列Textureの配列数
    resourceDesc.Format = metadata.format;	// TexutureのFormat
    resourceDesc.SampleDesc.Count = 1;	// サンプリングカウント。1固定
    resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION(metadata.dimension);	// Textureの次元数。普段使っているのは2次元

    // 利用するHeapの設定。非常に特殊な運用。02_04exで一般的なケース版がある
    D3D12_HEAP_PROPERTIES heapProperties{};
    heapProperties.Type = D3D12_HEAP_TYPE_DEFAULT;	// 細かい設定を行う
    //heapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;	// WriteBackポリシーでVPUアクセス可能
    //heapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;	// プロセッサ

    // Resourceの生成
    Microsoft::WRL::ComPtr<ID3D12Resource> resource = nullptr;
    HRESULT hr = device_->CreateCommittedResource(
        &heapProperties,	// Heapの設定
        D3D12_HEAP_FLAG_NONE,	// Heapの特殊な設定。特になし
        &resourceDesc,	// Resourceの設定
        D3D12_RESOURCE_STATE_COPY_DEST,	// 初回のResourceState。Textureは基本読むだけ
        nullptr,	// Clear最適値。使わないのでnullptr
        IID_PPV_ARGS(&resource));
    assert(SUCCEEDED(hr));
    return resource;
}

[[nodiscard]]
Microsoft::WRL::ComPtr<ID3D12Resource> TextureManager::UploadTextureData(Microsoft::WRL::ComPtr<ID3D12Resource> texture, const ScratchImage& mipImages, ID3D12GraphicsCommandList* commandList)
{
    std::vector<D3D12_SUBRESOURCE_DATA> subresources;
    PrepareUpload(device_, mipImages.GetImages(), mipImages.GetImageCount(), mipImages.GetMetadata(), subresources);
    uint64_t intermediateSize = GetRequiredIntermediateSize(texture.Get(), 0, UINT(subresources.size()));
    //ID3D12Resource* intermediateResource = DxCreateLib::ResourceLib::CreateBufferResource(device_, intermediateSize);
    
    Microsoft::WRL::ComPtr<ID3D12Resource> intermediateResource = DxCreateLib::ResourceLib::CreateBufferResource(device_, intermediateSize);
    UpdateSubresources(commandList, texture.Get(), intermediateResource.Get(), 0, 0, UINT(subresources.size()), subresources.data());
    // Textureへの転送後は利用できるよう、D3D12_RESOURCE_STATE_COPY_DESTからD3D12_RESOURCE_STATE_GENERIC_READへResourceStateを変更する
    D3D12_RESOURCE_BARRIER barrier{};
    barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
    barrier.Transition.pResource = texture.Get();
    barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
    barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
    barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_GENERIC_READ;
    commandList->ResourceBarrier(1, &barrier);

    return intermediateResource;
}
