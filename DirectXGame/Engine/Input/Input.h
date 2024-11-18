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

class Input : public Singleton<Input>
{
private:
	// ComPtrのテンプレート
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public: // インナークラス
	struct MouseMove {
		LONG lX;
		LONG lY;
		LONG lZ;
	};

public:

	enum class PadType {
		DirectInput,
		XInput,
	};

	union State {
		XINPUT_STATE xInput_;
		DIJOYSTATE2 directInput_;
	};

	//struct JoyStick {
	//	ComPtr<IDirectInputDevice8> device_;
	//	int32_t deadZoneL_;
	//	int32_t deadZoneR_;
	//	PadType type_;
	//	State state_;
	//	State statePre_;
	//};


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

	Vector2 XGetLeftJoystick(float deadZone = 0.2f);

	Vector2 XGetRightJoystick(float deadZone = 0.2f);

	bool XPressJoystick(int32_t keyNumber);
	bool XNotPressJoystick(int32_t keyNumber);
	bool XReleaseJoystick(int32_t keyNumber);
	bool XTriggerJoystick(int32_t keyNumber);

	bool XLTrigger();
	bool XRTrigger();


private:
	/// <summary>
	/// xJoyの更新
	/// </summary>
	void XJoystickUpdate();

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

	void MouseUpdate();

public:
	~Input() override;

private:

	ComPtr<IDirectInput8> directInput_;
	ComPtr<IDirectInputDevice8> dJoystick_;
	ComPtr<IDirectInputDevice8> dKeyBoard_;
	ComPtr<IDirectInputDevice8> dMouse_;

	//std::vector<JoyStick> devJoysticks_;
	std::array<BYTE, 256> key_ = {};
	std::array<BYTE, 256> preKey_ = {};

	DIMOUSESTATE2 mouse_ = {};
	DIMOUSESTATE2 preMouse_ = {};

	DWORD xJoystick_{};
	XINPUT_STATE xJoyState_{};
	XINPUT_STATE xJoyStatePrev_{};

	bool isJoystickConnected_ = false;

	HWND hwnd_ = {};

	Vector2 mousePosition_ = {};
};

