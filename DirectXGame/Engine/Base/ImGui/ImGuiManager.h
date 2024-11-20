#pragma once
#include "../../Utility/Singleton.h"

class DirectXCommon;
class WindowAPI;

/// <summary>
/// ImGui関係管理クラス
/// </summary>
class ImGuiManager : public Singleton<ImGuiManager>
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="dxCommon"></param>
	/// <param name="winApp"></param>
	void Initialize(DirectXCommon* dxCommon,WindowAPI* winApp);
	/// <summary>
	/// 開始
	/// </summary>
	void Begin();
	/// <summary>
	/// 終了
	/// </summary>
	void End();
	/// <summary>
	/// 描画
	/// </summary>
	void Draw();
	/// <summary>
	/// 解放
	/// </summary>
	void Finalize();

private:
	DirectXCommon* dxCommon_ = nullptr;

};
