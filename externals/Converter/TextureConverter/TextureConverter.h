#pragma once
#include <string>
#include "../../DirectXTex/DirectXTex.h"

class TextureConverter
{
public:
    /// <summary>
    /// WICからDDSに変換
    /// </summary>
    /// <param name="filePath"></param>
    void ConvertTextureWICToDDS(const std::string& filePath);
    /// <summary>
    /// テクスチャファイル読み込み
    /// </summary>
    /// <param name="filePath"></param>
    void LoadWICTextureFromFile(const std::string& filePath);

private:
    static std::wstring ConvertMultiByteStringToWideString(const std::string& mString);

private:
    // 画像情報
    DirectX::TexMetadata metaData_;
    // 画像イメージのコンテナ
    DirectX::ScratchImage scratchImage_;

};