#pragma once
#include "../Math/MathLib.h"

#include <wrl.h>
#include <d3d12.h>

class WorldTransform
{
public:
	Vector3 scale_ = {};
	Vector3 rotate_ = {};
	Vector3 translate_ = {};

	// 定数バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffer_;


	// 親子ポインタ
	const WorldTransform* parent_ = nullptr;

};