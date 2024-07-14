#include "Input.h"
#include "../WindowAPI/WindowAPI.h"
#include <cassert>

#include <XInput.h>
#include <basetsd.h>
#include <commctrl.h>
#include <dbt.h>
#include <oleauto.h>
#include <shellapi.h>
#include <wbemidl.h>
#include <wrl/client.h>

#include <Windows.h>
#include <dinputd.h>
#include <memory>

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "xinput.lib")

namespace {

	bool IsPress(const DIMOUSESTATE2& mouseState, int32_t buttonNumber) {
		assert(0 <= buttonNumber && buttonNumber < _countof(mouseState.rgbButtons));
		return (mouseState.rgbButtons[buttonNumber] & 0x80) != 0;
	}

} // namespace

Input::~Input() {
	if (dKeyBoard_) {
		dKeyBoard_->Unacquire();
	}
	if (dMouse_) {
		dMouse_->Unacquire();
	}
}

void Input::Initialize()
{
	WindowAPI* winApp = WindowAPI::GetInstance();
	hwnd_ = winApp->GetHwnd();

	HRESULT result = S_FALSE;
	// DirectInputの初期化
	result = DirectInput8Create(winApp->GetHInstance(), DIRECTINPUT_VERSION, IID_IDirectInput8,
		(void**)&directInput_, nullptr);
	assert(SUCCEEDED(result));

	// キーボードデバイスの生成
	result = directInput_->CreateDevice(GUID_SysKeyboard, &dKeyBoard_, NULL);
	assert(SUCCEEDED(result));
	// 入力データ形式のセット
	result = dKeyBoard_->SetDataFormat(&c_dfDIKeyboard);    // 標準形式
	assert(SUCCEEDED(result));
	// 排他制御レベルのセット
	result = dKeyBoard_->SetCooperativeLevel(winApp->GetHwnd(),
		DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(result));

	// マウスの生成
	result = directInput_->CreateDevice(GUID_SysMouse, &dMouse_, nullptr);
	assert(SUCCEEDED(result));
	// 入力データ形式のセット
	result = dMouse_->SetDataFormat(&c_dfDIMouse2);
	assert(SUCCEEDED(result));
	// 排他制御レベルのセット
	result = dMouse_->SetCooperativeLevel(winApp->GetHwnd(),
		DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	assert(SUCCEEDED(result));


	XJoyConnected();
}

void Input::Update()
{
	// キーボードの更新
	KeyboardUpdate();
	// マウスの更新
	MouseUpdate();
	// コントローラー
	XJoystickUpdate();
}

void Input::KeyboardUpdate()
{
	dKeyBoard_->Acquire();
	preKey_ = key_;

	// キーの入力
	key_.fill(0);
	dKeyBoard_->GetDeviceState(sizeof(key_), key_.data());
}

bool Input::TriggerKey(BYTE keyNumber) const
{
	if (key_[keyNumber] && !preKey_[keyNumber]) {
		return true;
	}
	return false;
}

bool Input::PressKey(BYTE keyNumber) const
{
	if (key_[keyNumber]) {
		return true;
	}
	return false;
}

bool Input::ReleaseKey(BYTE keyNumber) const
{
	if (!key_[keyNumber] && preKey_[keyNumber]) {
		return true;
	}
	return false;
}

Vector2 Input::XGetLeftJoystick(float deadZone)
{
	Vector2 sThumbL = { (float)xJoyState_.Gamepad.sThumbLX / SHRT_MAX, (float)xJoyState_.Gamepad.sThumbLY / SHRT_MAX };

	if (std::fabsf(sThumbL.x) < deadZone && std::fabsf(sThumbL.y) < deadZone) {
		sThumbL = { 0.0f,0.0f };
	}

	return Vector2(sThumbL);
}

Vector2 Input::XGetRightJoystick(float deadZone)
{
	Vector2 sThumbR = { (float)xJoyState_.Gamepad.sThumbRX / SHRT_MAX, (float)xJoyState_.Gamepad.sThumbRY / SHRT_MAX };

	if (std::fabsf(sThumbR.x) < deadZone && std::fabsf(sThumbR.y) < deadZone) {
		sThumbR = { 0.0f,0.0f };
	}

	return Vector2(sThumbR);
}

bool Input::XPressJoystick(int32_t keyNumber)
{
	if ((xJoyState_.Gamepad.wButtons & keyNumber) && (xJoyStatePrev_.Gamepad.wButtons & keyNumber)) {
		return true;
	}
	return false;
}

bool Input::XNotPressJoystick(int32_t keyNumber)
{
	if (!(xJoyState_.Gamepad.wButtons & keyNumber) && !(xJoyStatePrev_.Gamepad.wButtons & keyNumber)) {
		return true;
	}
	return false;
}

bool Input::XTriggerJoystick(int32_t keyNumber)
{
	if (xJoyState_.Gamepad.wButtons & keyNumber) {
		return true;
	}
	return false;
}

bool Input::XReleaseJoystick(int32_t keyNumber)
{
	if (!(xJoyState_.Gamepad.wButtons & keyNumber) && (xJoyStatePrev_.Gamepad.wButtons & keyNumber)) {
		return true;
	}
	return false;
}

bool Input::XLTrigger()
{
	if (xJoyState_.Gamepad.bLeftTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD) {
		return true;
	}
	return false;
}

bool Input::XRTrigger()
{
	if (xJoyState_.Gamepad.bRightTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD) {
		return true;
	}
	return false;
}

void Input::XJoystickUpdate()
{

	xJoyStatePrev_ = xJoyState_;

	XJoyConnected();
}

void Input::XJoyConnected()
{
	ZeroMemory(&xJoyState_, sizeof(XINPUT_STATE));

	xJoystick_ = XInputGetState(0, &xJoyState_);

	if (xJoystick_ == ERROR_SUCCESS) {
		isJoystickConnected_ = true;
	}
	else {
		isJoystickConnected_ = false;
	}
}

bool Input::IsPressMouse(int32_t buttonNumber)
{
	return IsPress(mouse_, buttonNumber);
}

bool Input::IsTriggerMouse(int32_t buttonNumber)
{
	// 前回押してなくて、今回押していればトリガー
	bool currentPush = IsPressMouse(buttonNumber);
	return currentPush && !IsPress(preMouse_, buttonNumber);
}

Input::MouseMove Input::GetMouseMove()
{
	MouseMove tmp;
	tmp.lX = mouse_.lX;
	tmp.lY = mouse_.lY;
	tmp.lZ = mouse_.lZ;

	return tmp;
}

const Vector2& Input::GetMousePosition() const
{
	// TODO: return ステートメントをここに挿入します
	return mousePosition_;
}

void Input::MouseUpdate()
{
	dMouse_->Acquire();
	POINT mousePosition;
	// マウス座標
	GetCursorPos(&mousePosition);

	// ウィンドウ座標に変換
	ScreenToClient(hwnd_, &mousePosition);
	mousePosition_.x = static_cast<float>(mousePosition.x);
	mousePosition_.y = static_cast<float>(mousePosition.y);
}
