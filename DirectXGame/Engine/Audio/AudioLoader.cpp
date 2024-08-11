#include "AudioLoader.h"

#include <cassert>

Microsoft::WRL::ComPtr<IXAudio2> AudioLoader::sIxAudio2;
// サウンド格納ディレクトリ
std::string AudioLoader::sDirectoryPath;

void AudioLoader::Initialize(const std::string& directoryPath)
{
	sDirectoryPath = directoryPath;

	HRESULT result;
	IXAudio2MasteringVoice* masterVoice;

	// xAudioエンジンのインスタンスを生成
	result = XAudio2Create(&sIxAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);
	assert(SUCCEEDED(result));

	// マスターボイスを生成
	result = sIxAudio2->CreateMasteringVoice(&masterVoice);
	assert(SUCCEEDED(result));

	//indexSoundData_ = 0u;
	//indexVoice_ = 0u;
}
