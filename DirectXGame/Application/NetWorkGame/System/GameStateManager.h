#pragma once

class GameTimerSystem;

class GameStateManager
{
public:
	void Initialize();
	void Update();

public:	// アクセッサ
	void SetIsClear(bool isClear) { isClear_ = isClear; }
	void SetGameTimerSystem(GameTimerSystem* system) { timer_ = system; }
private:
	// クリアしたか
	bool isClear_ = false;
	GameTimerSystem* timer_ = nullptr;

	bool resetRequest_ = false;
};

