#pragma once

class Player;
class Input;

class OparationManager
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Player* player);
	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();
	/// <summary>
	/// 入力更新
	/// </summary>
	void InputUpdate();

	/// <summary>
	/// 落下処理
	/// </summary>
	void GravityUpdate();
private:
	// 親
	Player* player_ = nullptr;
	// 入力クラス
	Input* input_ = nullptr;
};