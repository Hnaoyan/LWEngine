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

void Sprite::TransferVertices()
{
	// 4頂点
	enum { LB, LT, RB, RT };

	// 左右上下
	float left = (0.0f - anchorPoint_.x) * size_.x;
	float right = (1.0f - anchorPoint_.x) * size_.x;
	float top = (0.0f - anchorPoint_.y) * size_.y;
	float bottom = (1.0f - anchorPoint_.y) * size_.y;

	if (isFlipX_) {	// 左右反転
		left = -left;
		right = -right;
	}
	if (isFlipY_) {	// 上下反転
		top = -top;
		bottom = -bottom;
	}

	// 頂点情報
	VertexData vertices[kVertNum] = {};

	vertices[LB].position = { left,bottom,0.0f,0.0f };	// 左下
	vertices[LT].position = { left,top,0.0f,0.0f };		// 左上
	vertices[RB].position = { right,bottom,0.0f,0.0f };	// 右下
	vertices[RT].position = { right,top,0.0f,0.0f };		// 右上

	// テクスチャ情報取得
	{
		float tex_left = texBase_.x / resourceDesc_.Width;
		float tex_right = (texBase_.x + texSize_.x) / resourceDesc_.Width;
		float tex_top = texBase_.y / resourceDesc_.Height;
		float tex_bottom = (texBase_.y + texSize_.y) / resourceDesc_.Height;

		vertices[LB].texcoord = { tex_left, tex_bottom };  // 左下
		vertices[LT].texcoord = { tex_left, tex_top };     // 左上
		vertices[RB].texcoord = { tex_right, tex_bottom }; // 右下
		vertices[RT].texcoord = { tex_right, tex_top };    // 右上
	}

	memcpy(vertData_, vertices, sizeof(vertices));
}