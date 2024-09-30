#include <cstdio>
#include <cstdlib>
#include "Windows.h"
#include <cassert>
#include "TextureConverter.h"

enum Argument {
    kApplicationPath,   
    kFilePath,           

    NumArgument
};

int main(int argc, char* argv[]) {
    // 日本語
    for (int i = 0; i < 2; ++i) {
        printf(argv[i]);

        printf("\n");
    }
    printf("Hello,World!\n");

    assert(argc >= NumArgument);

    // 初期化処理
    HRESULT hr = S_FALSE;
    hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
    assert(SUCCEEDED(hr));

    // テクスチャのコンバート
    TextureConverter converter;
    converter.ConvertTextureWICToDDS(argv[kFilePath]);
    
    // 終了処理
    CoUninitialize();
    // システム的な停止
    system("Pause");
    return 0;
}