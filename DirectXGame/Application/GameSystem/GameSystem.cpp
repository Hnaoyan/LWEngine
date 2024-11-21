#include "GameSystem.h"
#include "Engine/Input/Input.h"
#include "Engine/LwLib/LwEnginePaths.h"
#include "Engine/GlobalVariables/GlobalVariables.h"

#include <imgui.h>

float GameSystem::sSpeedFactor = 1.0f;
DashBlur GameSystem::sBlurEffect;
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
    // UI
    uiManager_ = GameUIManager();
    // ブラー
    sBlurEffect.data.centerPoint = { 0.5f,0.5f };
    sBlurEffect.data.samplesNum = 4;

    PostEffectRender::sPostEffect = Pipeline::PostEffectType::kBloom;
}

void GameSystem::Update()
{
    // バインドの更新
    keyConfig_.Update(&replayManager_);
    // 静的なクラスに渡す
    sKeyConfigManager = keyConfig_;

    // ポストエフェクト関係の受付
    if (sBlurEffect.isActive) {
        sBlurEffect.timer.Update();
        sBlurEffect.data.blurWidth = Ease::Easing(sBlurEffect.maxWidth, 0.0f, sBlurEffect.timer.GetElapsedFrame());
    }

    PostEffectRender::PostEffectDesc desc{};
    desc.blur = sBlurEffect.data;
    desc.bloom = postEffectManager_.bloomData_;
    desc.vignette = postEffectManager_.vignetteData_;
    PostEffectRender::GetInstance()->Update(desc);
    
    // リプレイ用に記録
    replayManager_.RecordFrame(&keyConfig_);
    // フレームカウント
    nowFrame_++;
}

void GameSystem::ImGuiDraw()
{

    ImGui::Begin("GameSystem");
    replayManager_.ImGuiDraw();
    ImGui::End();

}

void GameSystem::LaunchReplay()
{
    // リプレイの準備
    keyConfig_.BeginReplay();
}
