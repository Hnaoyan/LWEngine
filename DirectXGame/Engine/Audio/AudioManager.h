#pragma once
#include "Engine/Utility/Singleton.h"
#include <unordered_map>
#include <stdint.h>
#include <string>

/// <summary>
/// ゲームシーンなどで使うものを管理するクラス
/// </summary>
class AudioManager : public Singleton<AudioManager>
{
public:
	/// <summary>
	/// 読み込み
	/// </summary>
	/// <param name="tag"></param>
	/// <param name="filePath"></param>
	void LoadAudio(const std::string& tag, const std::string& filePath);
	void LoadMP3(const std::string& tag, const std::string& filePath);
	/// <summary>
	/// 再生
	/// </summary>
	/// <param name="tag"></param>
	void PlayAudio(const std::string& tag, float volume = 0.5f, bool isLoop = false);
	/// <summary>
	/// 停止
	/// </summary>
	/// <param name="tag"></param>
	void StopAudio(const std::string& tag);

private:
	// サウンドコンテナ
	std::unordered_map<std::string, uint32_t> soundContainer_;
};