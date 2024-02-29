#pragma once
#include <Windows.h>
#include <cstdint>
#include <format>

#include "../Utility/Singleton.h"

class WindowAPI : public Singleton<WindowAPI>
{
public:
	// クライアント領域のサイズ
	static const int32_t kWindowWidth = 1280;
	static const int32_t kWindowHeight = 720;
	// ウインドウクラス名
	static const wchar_t kWindowClassName[];

public: // 静的メンバ関数
	/// <summary>
	/// ウィンドウプロシージャ
	/// </summary>
	/// <param name="hwnd"></param>
	/// <param name="msg"></param>
	/// <param name="wparam"></param>
	/// <param name="lparam"></param>
	/// <returns></returns>
	static LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);


private:// メンバ変数
	// Window関連
	HWND hwnd_ = nullptr;		// ウィンドウハンドル
	WNDCLASS windowClass_{};	// ウィンドウクラス

};

