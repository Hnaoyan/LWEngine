#include "ReplayManager.h"
#include "../KeyConfigManager.h"
#include "Engine/LevelEditor/CSV/CSVLoader.h"

#include <fstream>
#include <sstream>
#include <filesystem>

ReplayManager::ReplayManager()
{
}

void ReplayManager::RecordFrame(KeyConfigManager* keyManager, int32_t nowFrame)
{
    // 保存処理
    ReplayData saveData{};
    saveData.keyConfigs = keyManager->GetPlayerKey();
    saveData.leftStick = keyManager->GetKeyConfig()->leftStick;
    saveData.rightStick = keyManager->GetKeyConfig()->rightStick;
    saveData.frameNumber = int32_t(nowFrame);
    replayDatas_.push_back(saveData);
}

void ReplayManager::ExportReplay(const std::string& fileName)
{

    std::string folderPath = "Resources/CSVData/";
    
    // ディレクトリがなければ作成
    std::filesystem::path dir(folderPath);
    if (!std::filesystem::exists(folderPath))
    {
        std::filesystem::create_directories(folderPath);
    }

    std::string name = fileName;
    size_t isExt = name.find(".csv");
    if (isExt == std::string::npos) {
        name = name + ".csv";
    }
    std::string fullPath = folderPath + name;

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

    fileName;

}
