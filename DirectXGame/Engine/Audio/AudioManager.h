#pragma once
#include "Engine/Utility/Singleton.h"
#include "Audio.h"
#include <unordered_map>

/// <summary>
/// ゲームシーンなどで使うものを管理するクラス
/// </summary>
class AudioManager : public Singleton<AudioManager>
{
public:
	void LoadAudio(const std::string& tag, const std::string& filePath);

	void PlayAudio(const std::string& tag);

	void StopAudio(const std::string& tag);

private:
	// サウンドコンテナ
	std::unordered_map<std::string, uint32_t> soundContainer_;
};