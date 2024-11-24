#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <iostream>
#include "Windows.h"
#include "TextureConverter.h"
//$(TargetDir)
enum Argument {
    kApplicationPath,   
    kFilePath,           

    NumArgument
};

int main(int argc, char* argv[]) {
    // 日本語
    for (int i = 0; i < argc; ++i) {
        printf(argv[i]);

        printf("\n");
    }
    printf("Hello,World!\n");
    //assert(argc >= NumArgument);

    if (argc < NumArgument) {
        // 使い方を表示
        TextureConverter::OutputUsage();
        return 0;
    }

    // 初期化処理
    HRESULT hr = S_FALSE;
    hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
    assert(SUCCEEDED(hr));

    // テクスチャのコンバート
    TextureConverter converter;

    // オプション数
    int numOptions = argc - NumArgument;
    // オプション配列
    char** options = argv + NumArgument;

    converter.ConvertTextureWICToDDS(argv[kFilePath], numOptions, options);
    
    // 終了処理
    CoUninitialize();
    // システム的な停止
    //system("Pause");
    return 0;
}