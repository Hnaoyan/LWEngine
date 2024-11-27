#pragma once
#include "../GameSystemStructs.h"
#include <vector>
#include <string>

class KeyConfigManager;

/// <summary>
/// リプレイ管理クラス
/// </summary>
class ReplayManager
{
public:
	ReplayManager();

public: // メンバ
	// 更新処理
	void RecordFrame(KeyConfigManager* keyManager);
	// 外部へ書き出し
	void ExportReplay(const std::string& fileName);
	void ExportReplay();
	// 外部から書き込み
	void ImportReplay();
	// コンテナの初期化
	void ClearReplayData();
	// ImGui
	void ImGuiDraw();

	size_t GetReplayDataSize() {
		return replayDatas_.size();
	}

	ReplayData GetReplayData(int32_t num) {
		return replayDatas_[num];
	}

	// リプレイの記録準備（スタート
	void RecordSetUp();

private:
	// ディレクトリパス
	std::string directoryPath_;
	// データ用コンテナ
	std::vector<ReplayData> replayDatas_;
	// レコーディング中か
	bool isRecord_ = false;
	// 出力用の名前
	char exportName_[256] = "";
	// 入力用の名前
	char importName_[256] = "";
	// 現在のフレーム番号
	uint32_t nowFrame_;

};
