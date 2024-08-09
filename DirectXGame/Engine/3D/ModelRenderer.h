#pragma once
#include<d3d12.h>

//namespace Model {
//	class Renderer {
//
//	};
//}

class ModelRenderer
{
public:
	// コマンドリスト
	static ID3D12GraphicsCommandList* sCommandList_;
public:
	/// <summary>
	/// 描画前
	/// </summary>
	static void PreDraw(ID3D12GraphicsCommandList* cmdList);
	/// <summary>
	/// 描画後
	/// </summary>
	static void PostDraw();


};
