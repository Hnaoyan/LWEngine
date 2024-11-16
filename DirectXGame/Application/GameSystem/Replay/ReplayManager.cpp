#include "ReplayManager.h"
#include "../KeyConfigManager.h"
#include "Engine/LevelEditor/CSV/CSVLoader.h"

#include <fstream>
#include <sstream>
#include <filesystem>
#include <imgui.h>

ReplayManager::ReplayManager()
{
    directoryPath_ = "Resources/ReplayData/";
}

void ReplayManager::RecordFrame(KeyConfigManager* keyManager)
{
    // 保存するか
    if (!isRecord_) {
        return;
    }
    // 保存処理
    ReplayData saveData{};
    saveData.keyConfigs = keyManager->GetPlayerKey();
    saveData.leftStick = keyManager->GetKeyConfig()->leftStick;
    saveData.rightStick = keyManager->GetKeyConfig()->rightStick;
    saveData.frameNumber = int32_t(nowFrame_);
    replayDatas_.push_back(saveData);

    nowFrame_++;
}

void ReplayManager::ExportReplay(const std::string& fileName)
{
    // パス作成
    std::string fullPath = CSVLoader::CreateFullPath(directoryPath_, fileName);
    // 書き込み用ファイル
    std::ofstream file;
    // 書き込み用に開く
    file.open(fullPath);

    if (!file.is_open()) {
        return;
    }

    file << "// フレーム番号, 左スティックx.y, 右スティックx.y, キー\n";
    file << "// ブースト, ダッシュ, ジャンプ, 長押しジャンプ, ロックオン, 通常射撃, 誘導射撃\n";

    for (std::vector<ReplayData>::iterator it = replayDatas_.begin(); it != replayDatas_.end(); ++it) 
    {
        // 値の入手
        int32_t frame = (*it).frameNumber;
        PlayerKey<bool> configs = (*it).keyConfigs;
        Vector2 leftStick = (*it).leftStick;
        Vector2 rightStick = (*it).rightStick;
        // 書き込み
        file << "FrameData," << frame << ", "
            << leftStick.x << ", " << leftStick.y << ", " // 左スティック
            << rightStick.x << ", " << rightStick.y << ", " // 右
            << configs.boost << ", " << configs.quickBoost << ", "  // ダッシュ
            << configs.jump << ", " << configs.pressJump << ", "    // ジャンプ
            << configs.lockon << ", "   // ロックオン
            << configs.shot << ", " << configs.homingShot << ", "   // 射撃系
            << "\n";    // 改行
    }
    // ファイルを閉じる
    file.close();

}

void ReplayManager::ImportReplay(const std::string& fileName)
{
    // 念の為にコンテナのリセット
    ClearReplayData();

    // パス作成
    std::string fullPath = CSVLoader::CreateFullPath(directoryPath_, fileName);
    // データ取得
    std::stringstream popCommands = CSVLoader::LoadCSVFile(fullPath);
    // 解析
    std::string line;

    while (std::getline(popCommands,line))
    {
        // 1行の文字列を解析
        std::istringstream line_stream(line);
        // ワード
        std::string word;
        // 取得
        std::getline(line_stream, word, ',');
        //---ここから受け取り部分---//
        // コメントアウトはスキップ
        if (word.find("//") == 0) {
            continue;
        }

        if (word.find("FrameData") == 0) {
            ReplayData newData{};
            std::getline(line_stream, word, ',');
            newData.frameNumber = (int32_t)std::atoi(word.c_str()); // フレーム番号
            std::getline(line_stream, word, ',');
            newData.leftStick.x = (float)std::atof(word.c_str()); // 左スティック
            std::getline(line_stream, word, ',');
            newData.leftStick.y = (float)std::atof(word.c_str()); // 左スティック
            std::getline(line_stream, word, ',');
            newData.rightStick.x = (float)std::atof(word.c_str()); // 右スティック
            std::getline(line_stream, word, ',');
            newData.rightStick.y = (float)std::atof(word.c_str()); // 右スティック
            std::getline(line_stream, word, ',');
            newData.keyConfigs.boost = (bool)std::atoi(word.c_str()); // ブースト
            std::getline(line_stream, word, ',');
            newData.keyConfigs.quickBoost = (bool)std::atoi(word.c_str()); // ダッシュ
            std::getline(line_stream, word, ',');
            newData.keyConfigs.jump = (bool)std::atoi(word.c_str()); // ジャンプ
            std::getline(line_stream, word, ',');
            newData.keyConfigs.pressJump = (bool)std::atoi(word.c_str()); // 長押し
            std::getline(line_stream, word, ',');
            newData.keyConfigs.lockon = (bool)std::atoi(word.c_str()); // ロックオン
            std::getline(line_stream, word, ',');
            newData.keyConfigs.shot = (bool)std::atoi(word.c_str()); // 通常射撃
            std::getline(line_stream, word, ',');
            newData.keyConfigs.homingShot = (bool)std::atoi(word.c_str()); // 追従射撃
            // データを渡す
            replayDatas_.push_back(newData);
        }
    }
}

void ReplayManager::ClearReplayData()
{
    // 初期化
    replayDatas_.clear();
}

void ReplayManager::ImGuiDraw()
{
    // 保存中かのフラグ
    ImGui::Checkbox("RecordingNow", &isRecord_);
    // 書き出し名
    ImGui::InputText("ExportName", exportName_, IM_ARRAYSIZE(exportName_));
    // 書き込み名
    ImGui::InputText("ImportName", importName_, IM_ARRAYSIZE(importName_));
    // 書き出し
    if (ImGui::Button("ExportReplayData")) {
        ExportReplay(exportName_);
    }
    // 書き込み
    if (ImGui::Button("ImportReplayData")) {
        ImportReplay(importName_);
    }
    // データ削除
    if (ImGui::Button("ClearReplayData")) {
        ClearReplayData();
    }
}
