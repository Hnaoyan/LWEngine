#include "AudioManager.h"
#include "Audio.h"

void AudioManager::LoadAudio(const std::string& tag, const std::string& filePath)
{
	std::unordered_map<std::string, uint32_t>::iterator it = soundContainer_.find(tag);
	// あれば早期
	if (it != soundContainer_.end()) {
		return;
	}
	// なければ作成
	else {
		uint32_t audio = Audio::GetInstance()->LoadWave(filePath);
		soundContainer_.emplace(tag, audio);
	}
}

void AudioManager::LoadMP3(const std::string& tag, const std::string& filePath)
{
	std::unordered_map<std::string, uint32_t>::iterator it = soundContainer_.find(tag);
	// あれば早期
	if (it != soundContainer_.end()) {
		return;
	}
	// なければ作成
	else {
		uint32_t audio = Audio::GetInstance()->LoadMP3(filePath);
		soundContainer_.emplace(tag, audio);
	}
}

void AudioManager::PlayAudio(const std::string& tag, float volume, bool isLoop)
{
	std::unordered_map<std::string, uint32_t>::iterator it = soundContainer_.find(tag);
	// なければ早期
	if (it == soundContainer_.end()) {
		return;
	}
	if (!Audio::GetInstance()->IsPlaying(soundContainer_[tag])) {
		// 再生
		soundContainer_[tag] = Audio::GetInstance()->PlayWave(soundContainer_[tag], isLoop, volume);
	}
}

void AudioManager::StopAudio(const std::string& tag)
{
	std::unordered_map<std::string, uint32_t>::iterator it = soundContainer_.find(tag);
	// なければ早期
	if (it == soundContainer_.end()) {
		return;
	}
	// 停止
	Audio::GetInstance()->StopWave(soundContainer_[tag]);
}
