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

    HRESULT hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
    assert(SUCCEEDED(hr));

    TextureConverter converter;

    converter.ConvertTextureWICToDDS(argv[kFilePath]);
    argc;
    CoUninitialize();

    system("Pause");
    return 0;
}