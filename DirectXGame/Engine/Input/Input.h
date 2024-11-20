#pragma once
#include <wrl.h>
#include <array>
#include <vector>
#include <Xinput.h>
#define DIRECTINPUT_VERSION		0x0800	// DirectInputのバージョン指定
#include <dinput.h>

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

#include "../Math/MathLib.h"
#include "../Utility/Singleton.h"

/// <summary>
/// 入力関係クラス
/// </summary>
class Input : public Singleton<Input>
{
public: // インナークラス
	struct MouseMove {
		LONG lX;
		LONG lY;
		LONG lZ;
	};

public:
	// パッドのタイプ
	enum class PadType {
		DirectInput,
		XInput,
	};
	// 状態
	union State {
		XINPUT_STATE xInput_;
		DIJOYSTATE2 directInput_;
	};

public:	// メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

public: // キーボード
	/// <summary>
	/// 更新
	/// </summary>
	void KeyboardUpdate();
	/// <summary>
	/// 押した瞬間True
	/// </summary>
	/// <param name="keyNumber"></param>
	/// <returns></returns>
	bool TriggerKey(BYTE keyNumber) const;

	/// <summary>
	/// 押している間True
	/// </summary>
	/// <param name="keyNumber"></param>
	/// <returns></returns>
	bool PressKey(BYTE keyNumber) const;

	/// <summary>
	/// 離した瞬間True
	/// </summary>
	/// <param name="keyNumber"></param>
	/// <returns></returns>
	bool ReleaseKey(BYTE keyNumber) const;

public:	// コントローラー系
	// スティック
	Vector2 XGetLeftJoystick(float deadZone = 0.2f);
	Vector2 XGetRightJoystick(float deadZone = 0.2f);
	// スティック押し込み
	bool XPressJoystick(int32_t keyNumber);
	bool XNotPressJoystick(int32_t keyNumber);
	bool XReleaseJoystick(int32_t keyNumber);
	bool XTriggerJoystick(int32_t keyNumber);
	// スティック押し
	bool XLTrigger();
	bool XRTrigger();


private:
	/// <summary>
	/// xJoyの更新
	/// </summary>
	void XJoystickUpdate();
	/// <summary>
	/// 接続確認
	/// </summary>
	void XJoyConnected();

public: // マウス系

	const std::array<BYTE, 256>& GetAllKey() const { return key_; }

	bool IsPressMouse(int32_t buttonNumber);

	bool IsTriggerMouse(int32_t buttonNumber);

	/// <summary>
	/// マウスの移動量
	/// </summary>
	/// <returns></returns>
	MouseMove GetMouseMove();

	/// <summary>
	/// マウスの座標
	/// </summary>
	/// <returns></returns>
	const Vector2& GetMousePosition() const;

	/// <summary>
	/// マウスの更新
	/// </summary>
	void MouseUpdate();

public:
	~Input() override;

private:
	// デバイスのポインタ
	Microsoft::WRL::ComPtr<IDirectInput8> directInput_;
	Microsoft::WRL::ComPtr<IDirectInputDevice8> dJoystick_;
	Microsoft::WRL::ComPtr<IDirectInputDevice8> dKeyBoard_;
	Microsoft::WRL::ComPtr<IDirectInputDevice8> dMouse_;

	std::array<BYTE, 256> key_ = {};
	std::array<BYTE, 256> preKey_ = {};

	DIMOUSESTATE2 mouse_ = {};
	DIMOUSESTATE2 preMouse_ = {};

	DWORD xJoystick_{};
	XINPUT_STATE xJoyState_{};
	XINPUT_STATE xJoyStatePrev_{};
	// 接続確認フラグ
	bool isJoystickConnected_ = false;	

	HWND hwnd_ = {};
	// マウスの座標
	Vector2 mousePosition_ = {};
};

