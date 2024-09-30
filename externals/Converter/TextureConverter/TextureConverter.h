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
    /// <summary>
    /// 文字列変換
    /// </summary>
    /// <param name="mString"></param>
    /// <returns></returns>
    static std::wstring ConvertMultiByteStringToWideString(const std::string& mString);
    /// <summary>
    /// パスの分離
    /// </summary>
    /// <param name="filePath"></param>
    void SeparateFilePath(const std::wstring& filePath);
    /// <summary>
    /// DDSテクスチャとしてファイル書き出し
    /// </summary>
    void SaveDDSTextureToFile();
private:
    // 画像情報
    DirectX::TexMetadata metaData_;
    // 画像イメージのコンテナ
    DirectX::ScratchImage scratchImage_;

    // ディレクトリパス
    std::wstring directoryPath_;
    // ファイル名
    std::wstring fileName_;
    // 拡張子
    std::wstring fileExt_;

};