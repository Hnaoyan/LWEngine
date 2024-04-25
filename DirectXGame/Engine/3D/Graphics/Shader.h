#pragma once
#include <wrl.h>
#include <string>

#include <d3d12.h>
#include <dxcapi.h>
#pragma comment(lib, "dxcompiler.lib")

#include "../../Utility/Singleton.h"

class Shader : public Singleton<Shader>
{
private:
	static Microsoft::WRL::ComPtr<IDxcIncludeHandler> includeHandler_;
	static Microsoft::WRL::ComPtr<IDxcUtils> dxcUtils_;
	static Microsoft::WRL::ComPtr<IDxcCompiler3> dxcCompiler_;
public:
	/// <summary>
	/// DXCの初期化
	/// </summary>
	static void Initialize();

	/// <summary>
	/// Shaderのコンパイル
	/// </summary>
	/// <param name="filePath"></param>
	/// <param name="profile"></param>
	/// <returns></returns>
	static IDxcBlob* Compile(const std::wstring& filePath, const wchar_t* profile);

	inline static D3D12_SHADER_BYTECODE ShaderByteCode(IDxcBlob* blob) {
		D3D12_SHADER_BYTECODE shaderByte = { blob->GetBufferPointer(),blob->GetBufferSize() };
		return shaderByte;
	}

private:
	static const std::wstring kBaseDirectory;

};