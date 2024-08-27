#include "SpriteManager.h"
#include <cassert>

std::unordered_map<std::string, std::unique_ptr<Sprite>> SpriteManager::sSprites;

void SpriteManager::LoadSprite(const std::string& tag, uint32_t texture)
{
	// イテレータ取得
	std::unordered_map<std::string, std::unique_ptr<Sprite>>::iterator it = sSprites.find(tag);
	// あれば早期
	if (it != sSprites.end()) {
		return;
	}
	// なければ作成
	else {
		std::unique_ptr<Sprite> instance;
		instance.reset(Sprite::Create(texture, { 0,0 }, { 0.5f,0.5f }));
		sSprites.emplace(tag, std::move(instance));
	}
	return;
}

Sprite* SpriteManager::GetSprite(const std::string& tag)
{
	// イテレータ取得
	std::unordered_map<std::string, std::unique_ptr<Sprite>>::iterator it = sSprites.find(tag);
	// なければエラー
	assert(it != sSprites.end());
	// 見つけたイテレータからポインタを取得
	return it->second.get();
}
