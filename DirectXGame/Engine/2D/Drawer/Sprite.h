//#pragma once
//#include "../../Math/MathLib.h"
//
//#include <wrl.h>
//#include <d3d12.h>
//
//class Sprite
//{
//public:
//	~Sprite() = default;
//	Sprite() {};
//	Sprite(uint32_t textureHandle, const Vector2& position,
//		const Vector2& size, const Vector4& color, const Vector2& anchorPoint);
//
//	bool Initialize();
//
//	void Draw();
//
//	void TransferVertices() {};
//
//private:
//	Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff_;
//	Microsoft::WRL::ComPtr<ID3D12Resource> constBuff_;
//
//private:
//	uint32_t textureHandle_ = 0u;
//
//	Vector2 position_ = {};
//	Vector2 size_ = {};
//	Vector2 anchorPoint_ = {};
//
//	Vector4 color_ = {};
//
//
//};