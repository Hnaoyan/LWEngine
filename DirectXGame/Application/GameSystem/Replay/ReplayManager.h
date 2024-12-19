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
	// リプレイの記録準備（スタート
	void RecordSetUp();
	void ReplaySetUp();
	// カウント
	void ReplayCount();
public: // アクセッサ
	size_t GetReplayDataSize() {
		return replayDatas_.size();
	}

	ReplayData GetReplayData(int32_t num) {
		return replayDatas_[num];
	}

	int32_t GetReplayFrame() { return replayNowFrame_; }
	bool IsReplayNow() { return isReplayNow_; }
	bool IsReplayEnd() { return isReplayEnd_; }
private:
	// ディレクトリパス
	std::string directoryPath_;
	// データ用コンテナ
	std::vector<ReplayData> replayDatas_;
	// レコーディング中か
	bool isRecord_ = false;
	// 出力用の名前
	char exportName_[256] = "Replay";
	// 入力用の名前
	char importName_[256] = "Replay";
	// 現在のフレーム番号
	int32_t recordNowFrame_;
	// 現在リプレイを再生しているか？
	bool isReplayNow_ = false;
	bool isReplayEnd_ = false;
	// リプレイ中の現在のフレーム
	int32_t replayNowFrame_ = 0;
};
