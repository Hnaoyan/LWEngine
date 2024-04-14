#pragma once
#include "../Base/DirectXCommon.h"
#include "../Input/Input.h"

class IScene
{
public: // 仮想関数
	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize();
	/// <summary>
	/// 更新処理
	/// </summary>
	virtual void Update() = 0;
	/// <summary>
	/// 描画処理
	/// </summary>
	virtual void Draw() = 0;

	/// <summary>
	/// ImGuiの処理
	/// </summary>
	virtual void ImGuiDraw() = 0;

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~IScene() = default;

public: // 取得・設定
	/// <summary>
	/// シーン番号取得
	/// </summary>
	/// <returns></returns>
	int GetSceneNum() { return sceneNum; }

protected:
	/// <summary>
	/// シーン番号
	/// </summary>
	static int sceneNum;

	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;

};

