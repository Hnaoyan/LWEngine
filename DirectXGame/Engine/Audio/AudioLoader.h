#pragma once
#include "../Utility/Singleton.h"
#include <stdint.h>
#include <string>
#include <wrl.h>
#include <array>
#include <set>
#include <string>
#include <xaudio2.h>

#pragma comment(lib,"xaudio2.lib")

class AudioLoader : public Singleton<AudioLoader>
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="directoryPath"></param>
	static void Initialize(const std::string& directoryPath = "Resources/Music/");
	//static uint32_t LoadWave(const std::string& filename);

private:
	// XAudio2のインスタンス
	static Microsoft::WRL::ComPtr<IXAudio2> sIxAudio2;
	// サウンド格納ディレクトリ
	static std::string sDirectoryPath;

};
