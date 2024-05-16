#include "Sprite.h"
#include "../TextureManager.h"

ID3D12Device* Sprite::sDevice_ = nullptr;
ID3D12GraphicsCommandList* Sprite::sCommandList_;
Matrix4x4 Sprite::sMatProjection_;

Sprite* Sprite::Create(uint32_t textureHandle, Vector2 position, Vector4 color, Vector2 anchorpoint, bool isFlipX, bool isFlipY)
{
	// 仮のサイズ設定
	Vector2 size = {100.0f,100.0f};
	// テクスチャの設定
	{
		const D3D12_RESOURCE_DESC& rDesc = TextureManager::GetInstance()->GetResourceDesc(textureHandle);
		size = { float(rDesc.Width),float(rDesc.Height) };
	}

	Sprite* sprite =
		new Sprite(textureHandle, position, size, color, anchorpoint, isFlipX, isFlipY);

	
	// nullチェック
	if (sprite == nullptr) {
		return nullptr;
	}

	if (!sprite->Initialize()) {
		delete sprite;
		assert(0);
		return nullptr;
	}

	return sprite;
}

void Sprite::StaticInitialize(ID3D12Device* device, uint32_t width, uint32_t height)
{

	sDevice_ = device;
	width, height;
}

void Sprite::PreDraw(ID3D12GraphicsCommandList* cmdList)
{
	assert(sCommandList_ == nullptr);
	// コマンドリストをセット
	sCommandList_ = cmdList;

}

void Sprite::PostDraw()
{
	// コマンドリストを解除
	sCommandList_ = nullptr;
}

Sprite::Sprite(uint32_t textureHandle, const Vector2& position, const Vector2& size, const Vector4& color, const Vector2& anchorPoint, bool isFlipX, bool isFlipY)
{
	position_ = position;
	size_ = size;
	anchorPoint_ = anchorPoint;
	//matWorld_ = 
	color_ = color;
	textureHandle_ = textureHandle;

	isFlipX_ = isFlipX;
	isFlipY_ = isFlipY;
	isInvisible_ = false;

	texSize_ = size;
}

bool Sprite::Initialize()
{
	HRESULT result = S_FALSE;
	// ハンドルからデスクを取得
	resourceDesc_ = TextureManager::GetInstance()->GetResourceDesc(textureHandle_);

	// 頂点系
	{
		// リソース設定
		vertBuff_ = ResourceLib::CreateBufferResource(sDevice_, ((sizeof(VertexData) + 0xff) & ~0xff) * kVertNum);
		// 頂点バッファビューの設定
		vbView_.BufferLocation = vertBuff_->GetGPUVirtualAddress();
		vbView_.SizeInBytes = ((sizeof(VertexData) + 0xff) & ~0xff) * kVertNum;
		vbView_.StrideInBytes = (sizeof(VertexData) + 0xff) & ~0xff;

		// 頂点バッファのマッピング
		result = vertBuff_->Map(0, nullptr, reinterpret_cast<void**>(&vertData_));
		assert(SUCCEEDED(result));
	}

	// 頂点バッファへの転送
	TransferVertices();

	// インデックス系
	{

	}

	// 定数系
	{
		// バッファー作成
		spriteGPUBuff_ = ResourceLib::CreateBufferResource(sDevice_, (sizeof(SpriteDataForGPU) + 0xff) & ~0xff);
		// マッピング
		result = spriteGPUBuff_->Map(0, nullptr, reinterpret_cast<void**>(&spriteGPUData_));
		assert(SUCCEEDED(result));
	}

	return true;
}
