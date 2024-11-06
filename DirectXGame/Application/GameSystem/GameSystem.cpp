#include "GameSystem.h"
#include "Engine/Input/Input.h"
#include "Engine/LwLib/LwEngineLib.h"
#include "Engine/LwLib/Ease/Ease.h"
#include "Engine/GlobalVariables/GlobalVariables.h"

float GameSystem::sSpeedFactor = 1.0f;
GameSystem::DashBlur GameSystem::sBlurEffect;
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

    //keyConfig_ = std::make_unique<KeyConfigManager>();
    keyConfig_.Update();

    // バインドの設定
    KeyBindUpdate();
    bloomData_ = { 0.75f,1.5f };
    // ブラー
    sBlurEffect.data.centerPoint = { 0.5f,0.5f };
    sBlurEffect.data.samplesNum = 4;
    // ビネット
    vignetteData_.scale = 16.0f;
    vignetteData_.powValue = 0.8f;
    vignetteData_.color = { 1.0f,0.0f,0.0f };
    PostEffectRender::sPostEffect = Pipeline::PostEffectType::kBloom;

}

void GameSystem::KeyBindUpdate()
{

}

void GameSystem::KeyConfigUpdate()
{

}

void GameSystem::Update()
{
    // バインドの更新
    KeyBindUpdate();
    // 入力の更新
    KeyConfigUpdate();
    
    keyConfig_.Update();

    sKeyConfigManager = keyConfig_;

    if (sBlurEffect.isActive) {
        sBlurEffect.timer.Update();
        sBlurEffect.data.blurWidth = Ease::Easing(sBlurEffect.maxWidth, 0.0f, sBlurEffect.timer.GetElapsedFrame());
    }

    PostEffectRender::PostEffectDesc desc{};
    desc.blur = sBlurEffect.data;
    desc.bloom = bloomData_;
    PostEffectRender::GetInstance()->Update(desc);
    //PostEffectRender::sPostEffect = Pipeline::PostEffectType::kBloom;

}
