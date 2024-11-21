#pragma once

/// <summary>
/// ポインタの削除クラス（仮
/// </summary>
/// <typeparam name="T"></typeparam>
/// <param name="p"></param>
template<class T> inline void SafeDelete(T*& classPtr) {
	delete classPtr;
	classPtr = nullptr;
}
