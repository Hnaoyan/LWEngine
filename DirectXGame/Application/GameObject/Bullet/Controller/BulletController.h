#pragma once
#include "Engine/LwLib/LwLibLists.h"

class BulletCluster;

class BulletController
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="cluster"></param>
	BulletController(BulletCluster* cluster) { cluster_ = cluster; };
	BulletController() {};
	
	/// <summary>
	/// 更新
	/// </summary>
	void Update();

private:
	// 弾のリスト枠
	BulletCluster* cluster_ = nullptr;
	// タイマー
	FrameTimer straghtTimer_;
};
