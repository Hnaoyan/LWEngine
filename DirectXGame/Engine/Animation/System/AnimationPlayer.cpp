#include "AnimationPlayer.h"
#include "imgui.h"
#include "../../Input/Input.h"

void AnimationPlayer::Initialize(AnimationData* data)
{
	animationData_ = data;
}

void AnimationPlayer::Update()
{

	Input* input = Input::GetInstance();
	if (input->TriggerKey(DIK_LSHIFT)) {
		playType_ = PlayBack::kForward;
	}
	if (input->TriggerKey(DIK_RSHIFT)) {
		playType_ = PlayBack::kReverse;
	}
	if (input->TriggerKey(DIK_RETURN)) {
		if (isStop_) {
			isStop_ = false;
		}
		else {
			isStop_ = true;
		}
	}
	// 停止なら
	if (isStop_) {
		return;
	}
	// アニメーション再生処理
	switch (playType_)
	{
	case AnimationPlayer::PlayBack::kForward:
		animationTime_ += 1.0f / 60.0f;
		animationTime_ = std::fmod(animationTime_, animationData_->duration);
		break;
	case AnimationPlayer::PlayBack::kReverse:
		animationTime_ -= 1.0f / 60.0f;

		if (animationTime_ < 0) {
			animationTime_ = animationData_->duration;
		}
		break;
	}
}

void AnimationPlayer::ImguiDraw(std::string parentName)
{
#ifdef _DEBUG
	ImGui::Checkbox(parentName.c_str(), &isForward_);

	if (isForward_) {
		playType_ = PlayBack::kForward;
	}
	else {
		playType_ = PlayBack::kReverse;
	}
	std::string name = parentName + "IsStop";
	ImGui::Checkbox(name.c_str(), &isStop_);
#endif // _DEBUG

}
