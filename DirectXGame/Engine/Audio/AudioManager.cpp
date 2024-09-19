#include "AudioManager.h"
#include "Audio.h"

void AudioManager::LoadAudio(const std::string& tag, const std::string& filePath)
{
	// オーディオ
	Audio* audio = Audio::GetInstance();
	// イテレータ
	std::unordered_map<std::string, uint32_t>::iterator it = soundContainer_.find(tag);
	// あれば早期
	if (it != soundContainer_.end()) {
		return;
	}
	// なければ作成
	else {
		uint32_t handle = audio->LoadWave(filePath);
		soundContainer_.emplace(tag, handle);
	}
}

void AudioManager::LoadMP3(const std::string& tag, const std::string& filePath)
{
	// オーディオ
	Audio* audio = Audio::GetInstance();
	// イテレータ
	std::unordered_map<std::string, uint32_t>::iterator it = soundContainer_.find(tag);
	// あれば早期
	if (it != soundContainer_.end()) {
		return;
	}
	// なければ作成
	else {
		uint32_t handle = audio->LoadMP3(filePath);
		soundContainer_.emplace(tag, handle);
	}
}

void AudioManager::PlayAudio(const std::string& tag, float volume, bool isLoop)
{
	// オーディオ
	Audio* audio = Audio::GetInstance();
	// イテレータ
	std::unordered_map<std::string, uint32_t>::iterator it = soundContainer_.find(tag);
	// なければ早期
	if (it == soundContainer_.end()) {
		return;
	}
	if (!audio->IsPlaying(soundContainer_[tag])) {
		// 再生
		soundContainer_[tag] = audio->PlayWave(soundContainer_[tag], isLoop, volume);
	}
}

void AudioManager::StopAudio(const std::string& tag)
{
	// オーディオ
	Audio* audio = Audio::GetInstance();
	// イテレータ
	std::unordered_map<std::string, uint32_t>::iterator it = soundContainer_.find(tag);
	// なければ早期
	if (it == soundContainer_.end()) {
		return;
	}
	// 停止
	audio->StopWave(soundContainer_[tag]);
}
