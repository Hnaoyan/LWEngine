#pragma once
#include <vector>
#include <stdint.h>

class IndexAllocator
{
public:	// コンストラクタ・デストラクタ
	IndexAllocator(int32_t maxSize) :indexSlot_(maxSize, false), slotSize_(maxSize) {};
	~IndexAllocator() = default;

public: // 関数
	/// <summary>
	/// 空き番号の検索
	/// </summary>
	/// <returns></returns>
	int32_t AllocateIndex() {
		for (int i = 0; i < slotSize_; ++i) {
			// 空いている場合
			if (!indexSlot_[i]) {
				indexSlot_[i] = true;
				return i;
			}
		}
		// 空き無し
		return -1;
	}

	/// <summary>
	/// 指定番号の開放
	/// </summary>
	/// <param name="index"></param>
	void ReleaseIndex(int32_t index) {
		indexSlot_[index] = false;
	}
	
	void UseIndex(int32_t index) {
		indexSlot_[index] = true;
	}

private: // メンバ
	int32_t slotSize_;
	std::vector<bool> indexSlot_;
};
