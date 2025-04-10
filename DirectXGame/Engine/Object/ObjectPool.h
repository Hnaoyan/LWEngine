#pragma once
#include <vector>
#include <memory>
#include <functional>

template <typename T, size_t Size>
class ObjectPool
{
public:

	struct Slot
	{
		bool isActive = false;
		std::unique_ptr<T> object;
	};

	ObjectPool() {
		// コンテナのサイズ設定
		slots_.resize(Size);

		// オブジェクトの初期化
		for (auto& it : slots_) {
			it.object = std::make_unique<T>();
			it.isActive = false;
		}

	}

	T* GetInactiveObject()
	{
		for (auto& it : slots_) {
			if (!it.isActive) {
				it.isActive = true;
				return it.object.get();
			}
		}
		return nullptr;
	}

	bool ReleaseObject(T* obj, std::function<void()> exit) {
		auto it = std::remove_if(slots_.begin(), slots_.end(),
			[obj](const Slot& slot) {
				return slot.object.get() == obj;
			});
		if (it != slots_.end()) {
			exit();
			slots_.erase(it);
			return true;
		}
		return false;
	}

private:
	// オブジェクトリスト
	std::vector<Slot> slots_;

};