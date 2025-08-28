#pragma once

class GameTimerSystem;
class LoginToken;

class GameStateManager
{
public:
	void Initialize();
	void Update();

public:	// アクセッサ
	void SetIsClear(bool isClear) { isClear_ = isClear; }
	void SetGameTimerSystem(GameTimerSystem* system) { timer_ = system; }
	void SetLoginToken(LoginToken* token) { login_ = token; }
	bool IsClear() { return isClear_; }
private:
	// クリアしたか
	bool isClear_ = false;
	bool isAccept_ = false;

	GameTimerSystem* timer_ = nullptr;
	LoginToken* login_ = nullptr;

	bool resetRequest_ = false;
};

