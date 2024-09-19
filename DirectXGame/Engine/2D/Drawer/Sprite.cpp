#include "Sprite.h"
#include "../TextureManager.h"

ID3D12Device* Sprite::sDevice_ = nullptr;
ID3D12GraphicsCommandList* Sprite::sCommandList_;
Matrix4x4 Sprite::sMatProjection_;
BlendPipeline Sprite::sPipeline_;

Sprite* Sprite::Create(uint32_t textureHandle, Vector2 position, Vector2 anchorpoint, Vector4 color, bool isFlipX, bool isFlipY)
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
	assert(device);
	sDevice_ = device;
	// 射影行列
	sMatProjection_ = Matrix4x4::MakeOrthographicMatrix(0.0f, 0.0f, (float)width, (float)height, 0.0f, 1.0f);
}

void Sprite::PreDraw(ID3D12GraphicsCommandList* cmdList)
{
	assert(Sprite::sCommandList_ == nullptr);
	// コマンドリストをセット
	sCommandList_ = cmdList;
	sPipeline_ = std::get<BlendPipeline>(GraphicsPSO::sPipelines_[size_t(Pipeline::Order::kSpirte)]);
	// ルートシグネチャの設定
	sCommandList_->SetGraphicsRootSignature(sPipeline_.rootSignature.Get());

	// 形状を設定。PSOに設定しているものとはまた別。同じものを設定すると考えておけば良い
	sCommandList_->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

}

void Sprite::PostDraw()
{
	// コマンドリストを解除
	Sprite::sCommandList_ = nullptr;
}

Sprite::Sprite(uint32_t textureHandle, const Vector2& position, const Vector2& size, const Vector4& color, const Vector2& anchorPoint, bool isFlipX, bool isFlipY)
{
	position_ = position;
	size_ = size;
	anchorPoint_ = anchorPoint;
	matWorld_ = Matrix4x4::MakeIdentity4x4();
	color_ = color;
	textureHandle_ = textureHandle;

	isFlipX_ = isFlipX;
	isFlipY_ = isFlipY;
	isInvisible_ = false;

	texSize_ = size;
	uvTransform_ = {
		{1.0f,1.0f,1.0f},
		{0.0f,0.0f,0.0f},
		{0.0f,0.0f,0.0f},
	};
}

bool Sprite::Initialize()
{
	HRESULT result = S_FALSE;
	// ハンドルからデスクを取得
	resourceDesc_ = TextureManager::GetInstance()->GetResourceDesc(textureHandle_);

	// 頂点系
	{
		// リソース設定
		vertBuff_ = ResourceLib::CreateBufferResource(sDevice_, sizeof(SpriteVertexData) * kVertNum);

		// 頂点バッファのマッピング
		result = vertBuff_->Map(0, nullptr, reinterpret_cast<void**>(&vertData_));
		assert(SUCCEEDED(result));

		// 頂点バッファビューの設定
		vbView_.BufferLocation = vertBuff_->GetGPUVirtualAddress();
		vbView_.SizeInBytes = sizeof(SpriteVertexData) * kVertNum;
		vbView_.StrideInBytes = sizeof(SpriteVertexData);
	}

	// 頂点バッファへの転送
	TransferVertices();

	// インデックス系
	{

		// リソース設定
		ibBuff_ = ResourceLib::CreateBufferResource(sDevice_, sizeof(uint32_t) * kVertNum);

		// マッピング
		result = ibBuff_->Map(0, nullptr, reinterpret_cast<void**>(&ibData_));
		assert(SUCCEEDED(result));

		ibData_[0] = 0;
		ibData_[1] = 1;
		ibData_[2] = 2;
		ibData_[3] = 1;
		ibData_[4] = 3;
		ibData_[5] = 2;

		ibView_.BufferLocation = ibBuff_->GetGPUVirtualAddress();
		ibView_.SizeInBytes = sizeof(uint32_t) * kVertNum;
		ibView_.Format = DXGI_FORMAT_R32_UINT;

	}

	// 定数系
	{
		// バッファー作成
		spriteGPUBuff_ = ResourceLib::CreateBufferResource(sDevice_, (sizeof(SpriteDataForGPU) + 0xff) & ~0xff);

		// マッピング
		result = spriteGPUBuff_->Map(0, nullptr, reinterpret_cast<void**>(&spriteGPUData_));
		assert(SUCCEEDED(result));

	}
	// ブレンドモードの初期化
	blendMode_ = BlendMode::kAlpha;

	return true;
}

void Sprite::Draw()
{
	Update();
	// 非表示処理
	if (isInvisible_) {
		return;
	}
	// パイプラインの設定
	sCommandList_->SetPipelineState(sPipeline_.pipelineStates[size_t(blendMode_)].Get());

	// 頂点バッファ
	sCommandList_->IASetVertexBuffers(0, 1, &vbView_);
	// インデックスバッファ
	sCommandList_->IASetIndexBuffer(&ibView_);

	// マテリアルCbufferの設定
	sCommandList_->SetGraphicsRootConstantBufferView(0, spriteGPUBuff_->GetGPUVirtualAddress());
	// シェーダリソースビューの設定
	TextureManager::GetInstance()->SetGraphicsRootDescriptorTable(sCommandList_, 1, textureHandle_);
	// 描画
	sCommandList_->DrawIndexedInstanced(kVertNum, 1, 0, 0, 0);

}

void Sprite::Update()
{
	TransferVertices();

	// ワールド行列の更新
	matWorld_ = Matrix4x4::MakeIdentity4x4();
	matWorld_ = Matrix4x4::Multiply(matWorld_, Matrix4x4::MakeRotateZMatrix(rotation_));
	matWorld_ = Matrix4x4::Multiply(matWorld_, Matrix4x4::MakeTranslateMatrix(Vector3(position_.x, position_.y, 0)));

	spriteGPUData_->color = color_;
	spriteGPUData_->WVP = sMatProjection_;
	spriteGPUData_->world = matWorld_;

	spriteGPUData_->uvTransform = Matrix4x4::MakeUvTransformMatirx(uvTransform_.scale, uvTransform_.rotate, uvTransform_.translate);
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
	SpriteVertexData vertices[kVertNum] = {};

	vertices[LB].position = { left,bottom,0.0f };	// 左下
	vertices[LT].position = { left,top,0.0f };		// 左上
	vertices[RB].position = { right,bottom,0.0f };	// 右下
	vertices[RT].position = { right,top,0.0f };		// 右上

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
