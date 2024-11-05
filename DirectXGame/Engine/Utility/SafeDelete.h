#pragma once

/// <summary>
/// ポインタを安全に削除するクラス
/// </summary>
/// <typeparam name="T"></typeparam>
/// <param name="p"></param>
template<class T> inline void SafeDelete(T*& p) {
	delete p;
	p = nullptr;
}
