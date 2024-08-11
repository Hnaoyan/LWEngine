#include "Skybox.h"
#include "../../2D/TextureManager.h"
#include "../ModelLoader.h"
#include "imgui.h"

GeneralPipeline Skybox::sPipeline_;
uint32_t Skybox::sSerialNumber_ = 0u;

Skybox* Skybox::CreateSkybox(std::string fileName)
{
    // メモリ確保
    Skybox* instance = new Skybox();
    // 初期化
    instance->Initialize(fileName);
    return instance;
}

void Skybox::Initialize(const std::string& fileName)
{
    // 頂点生成
    {
        modelData_.vertices.resize(24);
        for (int i = 0; i < 24; ++i) {
            modelData_.vertices[i].normal = {};
            modelData_.vertices[i].texcoord = {};
        }
        //// 右面
        //modelData_.vertices[0].position = { 1.0f,1.0f,1.0f,1.0f };
        //modelData_.vertices[1].position = { 1.0f,1.0f,-1.0f,1.0f };
        //modelData_.vertices[2].position = { 1.0f,-1.0f,1.0f,1.0f };
        //modelData_.vertices[3].position = { 1.0f,-1.0f,-1.0f,1.0f };
        //// 左面
        //modelData_.vertices[4].position = { -1.0f,1.0f,-1.0f,1.0f };
        //modelData_.vertices[5].position = { -1.0f,1.0f,1.0f,1.0f };
        //modelData_.vertices[6].position = { -1.0f,-1.0f,-1.0f,1.0f };
        //modelData_.vertices[7].position = { -1.0f,-1.0f,1.0f,1.0f };
        //// 前面
        //modelData_.vertices[8].position = { -1.0f,1.0f,1.0f,1.0f };
        //modelData_.vertices[9].position = { 1.0f,1.0f,1.0f,1.0f };
        //modelData_.vertices[10].position = { -1.0f,-1.0f,1.0f,1.0f };
        //modelData_.vertices[11].position = { 1.0f,-1.0f,1.0f,1.0f };
        //// 後面
        //modelData_.vertices[12].position = { 1.0f,1.0f,-1.0f,1.0f };
        //modelData_.vertices[13].position = { -1.0f,1.0f,-1.0f,1.0f };
        //modelData_.vertices[14].position = { 1.0f,-1.0f,-1.0f,1.0f };
        //modelData_.vertices[15].position = { -1.0f,-1.0f,-1.0f,1.0f };
        //// 上面
        //modelData_.vertices[12].position = { 1.0f,1.0f,1.0f,1.0f };
        //modelData_.vertices[13].position = { -1.0f,1.0f,1.0f,1.0f };
        //modelData_.vertices[14].position = { 1.0f,-1.0f,1.0f,1.0f };
        //modelData_.vertices[15].position = { -1.0f,-1.0f,1.0f,1.0f };
        // 右面
        modelData_.vertices[0].position = { 1.0f,1.0f,1.0f,1.0f };
        modelData_.vertices[1].position = { 1.0f,1.0f,-1.0f,1.0f };
        modelData_.vertices[2].position = { 1.0f,-1.0f,1.0f,1.0f };
        modelData_.vertices[3].position = { 1.0f,-1.0f,-1.0f,1.0f };

        modelData_.indices.push_back(0);
        modelData_.indices.push_back(1);
        modelData_.indices.push_back(2);
        modelData_.indices.push_back(2);
        modelData_.indices.push_back(1);
        modelData_.indices.push_back(3);

        // 左面
        modelData_.vertices[4].position = { -1.0f,1.0f,-1.0f,1.0f };
        modelData_.vertices[5].position = { -1.0f,1.0f,1.0f,1.0f };
        modelData_.vertices[6].position = { -1.0f,-1.0f,-1.0f,1.0f };
        modelData_.vertices[7].position = { -1.0f,-1.0f,1.0f,1.0f };

        modelData_.indices.push_back(4);
        modelData_.indices.push_back(5);
        modelData_.indices.push_back(6);
        modelData_.indices.push_back(6);
        modelData_.indices.push_back(5);
        modelData_.indices.push_back(7);

        // 前面
        modelData_.vertices[8].position = { -1.0f,1.0f,1.0f,1.0f };
        modelData_.vertices[9].position = { 1.0f,1.0f,1.0f,1.0f };
        modelData_.vertices[10].position = { -1.0f,-1.0f,1.0f,1.0f };
        modelData_.vertices[11].position = { 1.0f,-1.0f,1.0f,1.0f };

        modelData_.indices.push_back(8);
        modelData_.indices.push_back(9);
        modelData_.indices.push_back(10);
        modelData_.indices.push_back(10);
        modelData_.indices.push_back(9);
        modelData_.indices.push_back(11);

        // 背面
        modelData_.vertices[12].position = { 1.0f,1.0f,-1.0f,1.0f };
        modelData_.vertices[13].position = { -1.0f,1.0f,-1.0f,1.0f };
        modelData_.vertices[14].position = { 1.0f,-1.0f,-1.0f,1.0f };
        modelData_.vertices[15].position = { -1.0f,-1.0f,-1.0f,1.0f };

        modelData_.indices.push_back(12);
        modelData_.indices.push_back(13);
        modelData_.indices.push_back(14);
        modelData_.indices.push_back(14);
        modelData_.indices.push_back(13);
        modelData_.indices.push_back(15);

        // 上面
        modelData_.vertices[16].position = { 1.0f,1.0f,1.0f,1.0f };
        modelData_.vertices[17].position = { -1.0f,1.0f,1.0f,1.0f };
        modelData_.vertices[18].position = { 1.0f,1.0f,-1.0f,1.0f };
        modelData_.vertices[19].position = { -1.0f,1.0f,-1.0f,1.0f };

        modelData_.indices.push_back(16);
        modelData_.indices.push_back(17);
        modelData_.indices.push_back(18);
        modelData_.indices.push_back(18);
        modelData_.indices.push_back(17);
        modelData_.indices.push_back(19);

        // 下面
        modelData_.vertices[20].position = { -1.0f,-1.0f,1.0f,1.0f };
        modelData_.vertices[21].position = { 1.0f,-1.0f,1.0f,1.0f };
        modelData_.vertices[22].position = { -1.0f,-1.0f,-1.0f,1.0f };
        modelData_.vertices[23].position = { 1.0f,-1.0f,-1.0f,1.0f };

        modelData_.indices.push_back(20);
        modelData_.indices.push_back(21);
        modelData_.indices.push_back(22);
        modelData_.indices.push_back(22);
        modelData_.indices.push_back(21);
        modelData_.indices.push_back(23);
    
    }

    // メッシュ生成
    mesh_ = std::make_unique<Mesh>();
    mesh_->CreateMesh(&modelData_);

    // マテリアル生成
    material_ = std::make_unique<Material>();
    material_->CreateMaterial();

    // マテリアルの情報
    MaterialData data{};
    data.textureFilename = "Resources/Skybox/" + fileName;
    data.textureHandle = TextureManager::Load(data.textureFilename);
    modelData_.material = data;

    // トランスフォーム
    worldTransform_.Initialize();
    worldTransform_.transform_.scale = { 20.0f,20.0f,20.0f };

    // シリアル番号設定
    serialNum_ = sSerialNumber_;
    // 全体のシリアル
    sSerialNumber_++;

}

void Skybox::Draw(const ModelDrawDesc& desc)
{
    // パイプラインの設定
    sPipeline_ = std::get<GeneralPipeline>(GraphicsPSO::sPipelines_[size_t(Pipeline::Order::kSkybox)]);

    // マテリアル更新
    material_->Update();
    worldTransform_.UpdateMatrix();

    // ルートシグネチャの設定
    Model::sCommandList_->SetGraphicsRootSignature(sPipeline_.rootSignature.Get());
    // パイプラインステートの設定
    Model::sCommandList_->SetPipelineState(sPipeline_.pipelineState.Get());

    // ワールド行列
    Model::sCommandList_->SetGraphicsRootConstantBufferView(
        static_cast<UINT>(Pipeline::SkyBoxRegister::kWorldTransform),
        worldTransform_.GetCBuffer()->GetGPUVirtualAddress());
    // ビュープロジェクション行列
    Model::sCommandList_->SetGraphicsRootConstantBufferView(
        static_cast<UINT>(Pipeline::SkyBoxRegister::kViewProjection),
        desc.camera->GetCBuffer()->GetGPUVirtualAddress());

    //---メッシュの設定---//
    // 頂点バッファの設定
    Model::sCommandList_->IASetVertexBuffers(0, 1, &mesh_->vbView_);
    // インデックスバッファの設定
    Model::sCommandList_->IASetIndexBuffer(&mesh_->ibView_);

    //---マテリアルの設定---//
    // SRVのセット
    TextureManager::GetInstance()->SetGraphicsRootDescriptorTable(
        Model::sCommandList_, static_cast<UINT>(Pipeline::SkyBoxRegister::kTexture), modelData_.material.textureHandle);
    // マテリアル
    Model::sCommandList_->SetGraphicsRootConstantBufferView(
        static_cast<UINT>(Pipeline::SkyBoxRegister::kMaterial), material_->buffer_.cBuffer->GetGPUVirtualAddress());

    // ドローコール
    Model::sCommandList_->DrawIndexedInstanced(UINT(modelData_.indices.size()), 1, 0, 0, 0);

}

void Skybox::ImGuiDraw()
{
    std::string name = "Skybox" + std::to_string(serialNum_);
    ImGui::Begin(name.c_str());
    name = "Position" + std::to_string(serialNum_);
    float speed = 0.01f;
    ImGui::DragFloat3(name.c_str(), &worldTransform_.transform_.translate.x, speed);
    name = "Scale" + std::to_string(serialNum_);
    ImGui::DragFloat3(name.c_str(), &worldTransform_.transform_.scale.x, speed);
    ImGui::End();
}
