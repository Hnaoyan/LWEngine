#include "SkinCluster.h"
#include "../3D/Descriptor/SRVHandler.h"
#include "../Base/Utility/DxCreateLib.h"
#include "../3D/ModelData.h"

#include <algorithm>

SkinCluster SkinCluster::Create(ID3D12Device* device, const Skeleton& skeleton, const ModelData& modelData)
{
    SkinCluster skinCluster;
    // palette用のResourceを確保
    skinCluster.paletteResource = DxCreateLib::ResourceLib::CreateBufferResource(device, sizeof(WellForGPU) * skeleton.joints.size());

    WellForGPU* mappedPalette = nullptr;

    skinCluster.paletteResource->Map(0, nullptr, reinterpret_cast<void**>(&mappedPalette));
    skinCluster.mappedPalette = { mappedPalette,skeleton.joints.size() };
    skinCluster.paletteSrvHandle.first = SRVHandler::GetSrvHandleCPU();
    skinCluster.paletteSrvHandle.second = SRVHandler::GetSrvHandleGPU();
    // SRVを次に進める
    skinCluster.srvHandleIndex = SRVHandler::AllocateDescriptor();

    // palette用のsrvを作成
    D3D12_SHADER_RESOURCE_VIEW_DESC paletteSrvDesc{};
    paletteSrvDesc.Format = DXGI_FORMAT_UNKNOWN;
    paletteSrvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
    paletteSrvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
    paletteSrvDesc.Buffer.FirstElement = 0;
    paletteSrvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
    paletteSrvDesc.Buffer.NumElements = UINT(skeleton.joints.size());
    paletteSrvDesc.Buffer.StructureByteStride = sizeof(WellForGPU);

    device->CreateShaderResourceView(skinCluster.paletteResource.Get(), &paletteSrvDesc, skinCluster.paletteSrvHandle.first);
       
    // influence用のResourceを確保
    skinCluster.influenceResource = DxCreateLib::ResourceLib::CreateBufferResource(device, sizeof(VertexInfluence) * modelData.vertices.size());
    VertexInfluence* mappedInfluence = nullptr;
    skinCluster.influenceResource->Map(0, nullptr, reinterpret_cast<void**>(&mappedInfluence));
    std::memset(mappedInfluence, 0, sizeof(VertexInfluence) * modelData.vertices.size());
    skinCluster.mappedInfluence = { mappedInfluence,modelData.vertices.size() };

    // influence用のVBVを作成
    skinCluster.influenceBufferView.BufferLocation = skinCluster.influenceResource->GetGPUVirtualAddress();
    skinCluster.influenceBufferView.SizeInBytes = UINT(sizeof(VertexInfluence) * modelData.vertices.size());
    skinCluster.influenceBufferView.StrideInBytes = sizeof(VertexInfluence);

    // inverseBindPoseMatrixの保存領域を作成
    skinCluster.inverseBindPoseMatrices.resize(skeleton.joints.size());
    //Matrix4x4 identity = Matrix4x4::MakeIdentity4x4();
    //std::generate(skinCluster.inverseBindPoseMatrices.begin(), skinCluster.inverseBindPoseMatrices.end(), identity);
    for (int i = 0; i < skinCluster.inverseBindPoseMatrices.size(); ++i) {
        skinCluster.inverseBindPoseMatrices[i] = Matrix4x4::MakeIdentity4x4();
    }

    // modelDataのSkinCluster情報を解析してInfluenceの中身を埋める
    for (const auto& jointWeight : modelData.skinClusterData) { // ModelのSkinClusterの解析
        auto it = skeleton.jointMap.find(jointWeight.first);// 名前から判断
        if (it == skeleton.jointMap.end()) {    // そんな名前のJointはないので次
            continue;
        }
        // (*it).secondにはindexが入ってるのでdInverseBindPoseMatrixを代入
        skinCluster.inverseBindPoseMatrices[(*it).second] = jointWeight.second.inverseBindPoseMatrix;
        for (const auto& vertexWeight : jointWeight.second.vertexWeights) {
            auto& currentInfluence = skinCluster.mappedInfluence[vertexWeight.vertexIndex]; // 該当のInfluenceの情報を参照
            for (uint32_t index = 0; index < kNumMaxInfluence; ++index) {
                if (currentInfluence.weights[index] == 0.0f) {
                    currentInfluence.weights[index] = vertexWeight.weight;
                    currentInfluence.jointIndices[index] = (*it).second;
                    break;
                }
            }
        }
    }

    return SkinCluster(skinCluster);
}
