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
public:
	/// <summary>
	/// ゲームウィンドウの作成
	/// </summary>
	/// <param name="title">ウィンドウのタイトル</param>
	/// <param name="windowStyle"></param>
	/// <param name="clientWidth">ウィンドウのクライアント領域の横幅</param>
	/// <param name="clientHeight">ウィンドウのクライアント領	域の縦幅</param>
	void CreateGameWindow(const wchar_t* title = L"DirectXTitle", UINT windowStyle = WS_OVERLAPPEDWINDOW,
		int32_t clientWidth = kWindowWidth, int32_t clientHeight = kWindowHeight);

	/// <summary>
	/// メッセージの処理
	/// </summary>
	/// <returns>終了を確認</returns>
	bool ProcessMessage();

public: // アクセッサ
	/// <summary>
	/// HWNDの取得
	/// </summary>
	/// <returns></returns>
	HWND GetHwnd() const { return hwnd_; }
	/// <summary>
	/// HInstanceの取得
	/// </summary>
	/// <returns></returns>
	HINSTANCE GetHInstance() const { return windowClass_.hInstance; }

private:// メンバ変数
	// Window関連
	HWND hwnd_ = nullptr;		// ウィンドウハンドル
	WNDCLASS windowClass_{};	// ウィンドウクラス

};

