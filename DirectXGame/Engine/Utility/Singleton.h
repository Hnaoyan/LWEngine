#pragma once

/// <summary>
/// シングルトン用のクラス
/// </summary>
/// <typeparam name="T">対象クラス</typeparam>
template<typename T>
class Singleton
{
public:
	// インスタンス取得
	static T* GetInstance() {
		static T instance;
		return &instance;
	}
protected:
	Singleton() = default;
	virtual ~Singleton() = default;

	// コピー禁止
	Singleton(const Singleton&) = delete;
	Singleton& operator=(const Singleton&) = delete;

};
