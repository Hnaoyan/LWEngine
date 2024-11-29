#include "GameSystem.h"
#include "Engine/Input/Input.h"
#include "Engine/LwLib/LwEnginePaths.h"
#include "Engine/GlobalVariables/GlobalVariables.h"

#include <imgui.h>

float GameSystem::sSpeedFactor = 1.0f;
KeyConfigManager GameSystem::sKeyConfigManager;

float GameSystem::GameSpeedFactor()
{
    if (sSpeedFactor <= sDefaultGameSpeed) {
        sSpeedFactor = sDefaultGameSpeed;
    }
    return (1.0f / sSpeedFactor) * kDeltaTime;
}

void GameSystem::Initialize()
{
    input_ = Input::GetInstance();
    // リプレイマネージャ生成
    replayManager_ = ReplayManager();
    // キーコンフィグ生成
    keyConfig_ = KeyConfigManager();
    // ポストエフェクト生成
    postEffectManager_ = PostEffectManager();
    postEffectManager_.Initialize(this);

    // ポストエフェクトレンダラーの設定
    PostEffectRender::sPostEffect = Pipeline::PostEffectType::kBloom;
}

void GameSystem::Update()
{
    // バインドの更新
    keyConfig_.Update(&replayManager_);
    // 静的なクラスに渡す
    sKeyConfigManager = keyConfig_;
    // ポストエフェクト
    postEffectManager_.Update();
    // リプレイ用に記録
    replayManager_.RecordFrame(&keyConfig_);
    // フレームカウント
    recordNowFrame_++;
}

void GameSystem::ImGuiDraw()
{
    ImGui::Begin("GameSystem");
    replayManager_.ImGuiDraw();

    postEffectManager_.ImGuiDraw();
    //if (ImGui::Button("Vignette")) {

    //}

    ImGui::End();
}

void GameSystem::LaunchReplay()
{
    // リプレイの準備
    replayManager_.ReplaySetUp();
}
