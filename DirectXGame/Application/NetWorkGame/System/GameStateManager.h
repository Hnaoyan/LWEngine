#pragma once

class GameStateManager
{
public:
	void Initialize();
	void Update();

public:	// アクセッサ
	void SetIsClear(bool isClear) { isClear_ = isClear; }

private:
	// クリアしたか
	bool isClear_ = false;

};

