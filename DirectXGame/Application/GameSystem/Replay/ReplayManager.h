#pragma once
#include "../GameSystemStructs.h"
#include <vector>
#include <string>

class KeyConfigManager;

class ReplayManager
{
public:
	ReplayManager();

public: // メンバ
	void RecordFrame(KeyConfigManager* keyManager, int32_t nowFrame);
	void ExportReplay(const std::string& fileName);
	void ImportReplay(const std::string& fileName);

private:
	// データ用コンテナ
	std::vector<ReplayData> replayDatas_;
};
