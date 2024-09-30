#include "TextureConverter.h"
#include <Windows.h>

void TextureConverter::ConvertTextureWICToDDS(const std::string& filePath)
{
	// テクスチャファイルを読み込む
	LoadWICTextureFromFile(filePath);

	// DDS形式に変換

	filePath;
}

void TextureConverter::LoadWICTextureFromFile(const std::string& filePath)
{
	// ファイルパスをワイド文字列に
	std::wstring wFilePath = ConvertMultiByteStringToWideString(filePath);
	
	// テクスチャ読み込み

	// WICのロード
	HRESULT result = S_FALSE;
}

std::wstring TextureConverter::ConvertMultiByteStringToWideString(const std::string& mString)
{
	//if (mString.empty()) {
	//	return std::wstring();
	//}
	//auto sizeNeeded = MultiByteToWideChar(CP_UTF8, 0, reinterpret_cast<const char*>(&mString[0]), static_cast<int>(mString.size()), NULL, 0);
	//if (sizeNeeded == 0) {
	//	return std::wstring();
	//}
	//std::wstring result(sizeNeeded, 0);
	//MultiByteToWideChar(CP_UTF8, 0, reinterpret_cast<const char*>(&mString[0]), static_cast<int>(mString.size()), &result[0], sizeNeeded);
	//return std::wstring(result);

	int bufferSize = MultiByteToWideChar(CP_ACP, 0, mString.c_str(), -1, nullptr, 0);
	// 
	std::wstring wString;
	wString.resize(bufferSize);

	//
	MultiByteToWideChar(CP_ACP, 0, mString.c_str(), -1, &wString[0], bufferSize);

	return std::wstring(wString);
}
