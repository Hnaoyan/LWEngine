#include "Trail3D.h"
#include "Engine/Base/DirectXCommon.h"
#include "Engine/3D/ModelUtility/ModelRenderer.h"
#include "Engine/2D/TextureManager.h"
#include "Engine/LwLib/LwLibLists.h"

#include<imgui.h>

Trail3D::Trail3D()
{
	// USER
	isBillBoard_ = true;
	width_ = 1.0f;
	minWidth_ = width_ / 2.0f;

	maxAlpha_ = 0.75f;
	color_ = { 1.0f,1.0f,1.0f,maxAlpha_ };
	// テクスチャ
	texture_ = TextureManager::Load("Resources/default/uvChecker.png");

	// 頂点とindexのサイズ設定
	const size_t kMaxSegments = 1024 * 2;
	vertexData_.resize((kMaxSegments) * 2);
	indices_.resize((kMaxSegments) * 6);
	CreateVertex();

	// マテリアル
	material_ = std::make_unique<TrailMaterial>();
	material_->CreateMaterial();
}

void Trail3D::Update()
{
	// 頂点送信
	TransferVertex();
	// マテリアルの更新
	material_->Update();
}

void Trail3D::TransferVertex()
{
	// ポインタに値を渡す
	memcpy(vertex_.cMap_, vertexData_.data(), sizeof(TriangleData) * vertexData_.size());
	memcpy(index_.cMap_, indices_.data(), sizeof(uint32_t) * indices_.size());
}

void Trail3D::CreateVertex()
{
	// 中身のエラーチェック
	assert(vertexData_.size());
	assert(indices_.size());

	// デバイス
	ID3D12Device* device = DirectXCommon::GetInstance()->GetDevice();
	// 頂点作成
	vertex_.CreateConstantBuffer(device, vertexData_.size());
	index_.CreateConstantBuffer(device, indices_.size());
	// 頂点の送信
	TransferVertex();

	//---ビューの作成---//
	// Vertex
	vbView_.BufferLocation = vertex_.cBuffer->GetGPUVirtualAddress();
	vbView_.SizeInBytes = UINT(sizeof(TriangleData) * vertexData_.size());
	vbView_.StrideInBytes = sizeof(TriangleData);
	// index
	ibView_.BufferLocation = index_.cBuffer->GetGPUVirtualAddress();
	ibView_.SizeInBytes = UINT(sizeof(uint32_t) * indices_.size());
	ibView_.Format = DXGI_FORMAT_R32_UINT;
}

void Trail3D::BuildVertexFromPoints(const std::vector<Vector3>& points)
{
	// 座標数
	size_t numPoints = points.size();
	// 補間数が足りない場合早期
	if (numPoints < 2) { return; }
	// 頂点用のIndex
	int32_t vertexId = 0;
	// インデックス用のIndex
	int32_t indexId = 0;

	for (size_t i = 0; i < numPoints - 1; ++i) {
		// 始点と終点
		Vector3 startPoint = points[i];
		Vector3 endPoint = points[i + 1];

		// テクスチャ座標のUV座標設定
		// Uの座標
		float uStart = (float)i / (float)(numPoints - 1);
		float uEnd = (float)(i + 1) / (float)(numPoints - 1);
		// Vの座標
		float vLeft = 0.0f;   // 左側の頂点
		float vRight = 1.0f;  // 右側の頂点

		// アルファ値を頂点の場所に応じて
		Vector4 newColor = color_;
		newColor.w = ((float)i + 1.0f) / (float)points.size();
		newColor.w = std::clamp(newColor.w, 0.0f, maxAlpha_);

		// ビルボード処理
		if (isBillBoard_ && camera_.has_value()) {
			assert(camera_);
			// カメラから始点へのベクトル
			Vector3 toCamera = Vector3::Normalize(camera_.value()->transform_.translate - startPoint);
			// 軌跡のベクトル
			Vector3 trailDirect = Vector3::Normalize(endPoint - startPoint);
			// 外積
			Vector3 perpendicular = Vector3::Cross(toCamera, trailDirect);
			// 外積を幅分
			perpendicular = Vector3::Normalize(perpendicular);
			perpendicular *= width_;

			// 頂点
			vertexData_[vertexId] = { {startPoint.x - perpendicular.x,startPoint.y - perpendicular.y,startPoint.z - perpendicular.z},newColor,{uStart,vLeft} };
			//vertexData_[vertexId + 1] = { {startPoint.x,startPoint.y,startPoint.z},color };
			vertexData_[vertexId + 1] = { {startPoint.x + perpendicular.x,startPoint.y + perpendicular.y,startPoint.z + perpendicular.z},newColor ,{uEnd,vRight} };
		}
		// ビルボードなし
		else {
			// 接戦
			Vector3 tangent = endPoint - startPoint;
			Vector3 normal = Vector3::Up();
			Vector3 right = { -tangent.y,tangent.x,0.0f };
			float length = std::sqrtf(right.x * right.x + right.y * right.y);
			right.x /= length;
			right.y /= length;
			// 幅調整
			right.x *= width_ * 0.5f;
			right.y *= width_ * 0.5f;
			// 頂点
			vertexData_[vertexId] = { {startPoint.x - right.x,startPoint.y - right.y,startPoint.z},newColor,{uStart,vLeft} };
			//vertexData_[vertexId + 1] = { {startPoint.x,startPoint.y,startPoint.z},color };
			vertexData_[vertexId + 1] = { {startPoint.x + right.x,startPoint.y + right.y,startPoint.z},newColor,{uEnd,vRight} };
		}

		// インデックスの設定
		if (i < numPoints - 2) {
			indices_[indexId++] = vertexId;
			indices_[indexId++] = vertexId + 1;
			indices_[indexId++] = vertexId + 2;

			indices_[indexId++] = vertexId + 1;
			indices_[indexId++] = vertexId + 3;
			indices_[indexId++] = vertexId + 2;
		}
		// 次の頂点に
		vertexId += 2;
	}
}

void Trail3D::LerpWidthVertex(const std::vector<Vector3>& points)
{
	// 座標数
	size_t numPoints = points.size();
	// 補間数が足りない場合早期
	if (numPoints < 2) { return; }
	// 頂点用のIndex
	int32_t vertexId = 0;
	// インデックス用のIndex
	int32_t indexId = 0;

	float widthRatio = 0.0f;

	for (size_t i = 0; i < numPoints - 1; ++i) {
		// 幅の割合
		widthRatio = (float)(i + 1) / (float)numPoints;
		// 始点と終点
		Vector3 startPoint = points[i];
		Vector3 endPoint = points[i + 1];

		// テクスチャ座標のUV座標設定
		// Uの座標
		float uStart = (float)i / (float)(numPoints - 1);
		float uEnd = (float)(i + 1) / (float)(numPoints - 1);
		// Vの座標
		float vLeft = 0.0f;   // 左側の頂点
		float vRight = 1.0f;  // 右側の頂点

		// アルファ値を頂点の場所に応じて
		Vector4 newColor = color_;
		newColor.w = ((float)i + 1.0f) / (float)points.size();
		newColor.w = std::clamp(newColor.w, 0.0f, maxAlpha_);
		if (newColor.w != 0.0f) {
			newColor.w *= 0.65f;
		}

		// ビルボード処理
		if (isBillBoard_ && camera_.has_value()) {
			assert(camera_);
			// カメラから始点へのベクトル
			Vector3 toCamera = Vector3::Normalize(camera_.value()->transform_.translate - startPoint);
			// 軌跡のベクトル
			Vector3 trailDirect = Vector3::Normalize(endPoint - startPoint);
			// 外積
			Vector3 perpendicular = Vector3::Cross(toCamera, trailDirect);
			// 外積を幅分
			perpendicular = Vector3::Normalize(perpendicular);

			float widthMin = minWidth_;
			float widthMax = width_;
			float lerpWidth = LwLib::Lerp(widthMin, widthMax, widthRatio);

			perpendicular *= lerpWidth * 0.5f;

			// 頂点
			vertexData_[vertexId] = { {startPoint.x - perpendicular.x,startPoint.y - perpendicular.y,startPoint.z - perpendicular.z},newColor,{uStart,vLeft} };
			//vertexData_[vertexId + 1] = { {startPoint.x,startPoint.y,startPoint.z},color };
			vertexData_[vertexId + 1] = { {startPoint.x + perpendicular.x,startPoint.y + perpendicular.y,startPoint.z + perpendicular.z},newColor ,{uEnd,vRight} };
		}
		// ビルボードなし
		else {
			// 接戦
			Vector3 tangent = endPoint - startPoint;
			Vector3 normal = Vector3::Up();
			Vector3 right = { -tangent.y,tangent.x,0.0f };
			float length = std::sqrtf(right.x * right.x + right.y * right.y);
			right.x /= length;
			right.y /= length;
			// 幅調整
			right.x *= width_ * 0.5f;
			right.y *= width_ * 0.5f;
			// 頂点
			vertexData_[vertexId] = { {startPoint.x - right.x,startPoint.y - right.y,startPoint.z},newColor,{uStart,vLeft} };
			//vertexData_[vertexId + 1] = { {startPoint.x,startPoint.y,startPoint.z},color };
			vertexData_[vertexId + 1] = { {startPoint.x + right.x,startPoint.y + right.y,startPoint.z},newColor,{uEnd,vRight} };
		}

		// インデックスの設定
		if (i < numPoints - 2) {
			indices_[indexId++] = vertexId;
			indices_[indexId++] = vertexId + 1;
			indices_[indexId++] = vertexId + 2;

			indices_[indexId++] = vertexId + 1;
			indices_[indexId++] = vertexId + 3;
			indices_[indexId++] = vertexId + 2;
		}
		// 次の頂点に
		vertexId += 2;
	}

}

void Trail3D::FadeWidthVertex(const std::vector<Vector3>& points)
{
	// 座標数
	size_t numPoints = points.size();
	// 補間数が足りない場合早期
	if (numPoints < 2) { return; }
	// 頂点用のIndex
	int32_t vertexId = 0;
	// インデックス用のIndex
	int32_t indexId = 0;
	// 幅の割合
	float widthRatio = 0.0f;

	for (size_t i = 0; i < numPoints - 1; ++i) {
		// 幅の割合
		widthRatio = (float)(i + 1) / (float)numPoints;
		// 始点と終点
		Vector3 startPoint = points[i];
		Vector3 endPoint = points[i + 1];

		// テクスチャ座標のUV座標設定
		// Uの座標
		float uStart = (float)i / (float)(numPoints - 1);
		float uEnd = (float)(i + 1) / (float)(numPoints - 1);
		// Vの座標
		float vLeft = 0.0f;   // 左側の頂点
		float vRight = 1.0f;  // 右側の頂点

		// アルファ値を頂点の場所に応じて
		Vector4 newColor = color_;
		newColor.w = ((float)i + 1.0f) / (float)points.size();
		newColor.w = std::clamp(newColor.w, 0.0f, maxAlpha_);
		if (newColor.w != 0.0f) {
			newColor.w *= 0.65f;
		}

		// ビルボード処理
		if (isBillBoard_ && camera_.has_value()) {
			assert(camera_);
			// カメラから始点へのベクトル
			Vector3 toCamera = Vector3::Normalize(camera_.value()->transform_.translate - startPoint);
			// 軌跡のベクトル
			Vector3 trailDirect = Vector3::Normalize(endPoint - startPoint);
			// 外積
			Vector3 perpendicular = Vector3::Cross(toCamera, trailDirect);
			// 外積を幅分
			perpendicular = Vector3::Normalize(perpendicular);

			float widthMin = minWidth_;
			float widthMax = width_;
			float lerpWidth = LwLib::Lerp(widthMin, widthMax, widthRatio);

			perpendicular *= lerpWidth * 0.5f;

			// 頂点
			vertexData_[vertexId] = { {startPoint.x - perpendicular.x,startPoint.y - perpendicular.y,startPoint.z - perpendicular.z},newColor,{uStart,vLeft} };
			vertexData_[vertexId + 1] = { {startPoint.x + perpendicular.x,startPoint.y + perpendicular.y,startPoint.z + perpendicular.z},newColor ,{uEnd,vRight} };
		}
		// ビルボードなし
		else {
			// 接戦
			Vector3 tangent = endPoint - startPoint;
			Vector3 normal = Vector3::Up();
			Vector3 right = { -tangent.y,tangent.x,0.0f };
			float length = std::sqrtf(right.x * right.x + right.y * right.y);
			right.x /= length;
			right.y /= length;
			// 幅調整
			right.x *= width_ * 0.5f;
			right.y *= width_ * 0.5f;
			// 頂点
			vertexData_[vertexId] = { {startPoint.x - right.x,startPoint.y - right.y,startPoint.z},newColor,{uStart,vLeft} };
			vertexData_[vertexId + 1] = { {startPoint.x + right.x,startPoint.y + right.y,startPoint.z},newColor,{uEnd,vRight} };
		}

		// インデックスの設定
		if (i < numPoints - 2) {
			indices_[indexId++] = vertexId;
			indices_[indexId++] = vertexId + 1;
			indices_[indexId++] = vertexId + 2;

			indices_[indexId++] = vertexId + 1;
			indices_[indexId++] = vertexId + 3;
			indices_[indexId++] = vertexId + 2;
		}
		// 次の頂点に
		vertexId += 2;
	}
}
