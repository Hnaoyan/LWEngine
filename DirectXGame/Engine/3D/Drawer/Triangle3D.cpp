#include "Triangle3D.h"
#include "Engine/Base/DirectXCommon.h"
#include "Engine/3D/ModelUtility/ModelRenderer.h"
#include "Engine/2D/TextureManager.h"

#include<imgui.h>

void Triangle3D::Initialize()
{
	// USER

	// 頂点情報作成
	CreateVertex();
}

void Triangle3D::Update(std::vector<Vector3> controlPoint)
{	
	// 頂点の情報
	UpdateVertex(controlPoint, Vector4(0.0f, 1.0f, 1.0f, 0.85f), 1.0f);
	// ポインタに値を渡す
	memcpy(vertex_.cMap_, vertexData_.data(), sizeof(TriangleData) * vertexData_.size());
	memcpy(index_.cMap_, indices_.data(), sizeof(uint32_t) * indices_.size());
}

void Triangle3D::Draw(ICamera* camera)
{
	ModelRenderer::TriangleDraw(camera, this);
}

void Triangle3D::CreateVertex()
{
	const size_t kMaxSegments = 1024;

	vertexData_.resize((kMaxSegments) * 2);
	indices_.resize((kMaxSegments) * 6);

	// デバイス
	ID3D12Device* device = DirectXCommon::GetInstance()->GetDevice();
	// 頂点作成
	vertex_.CreateConstantBuffer(device, vertexData_.size());
	std::memcpy(vertex_.cMap_, vertexData_.data(), sizeof(TriangleData) * vertexData_.size());
	// バッファービュー作成
	vbView_.BufferLocation = vertex_.cBuffer->GetGPUVirtualAddress();
	vbView_.SizeInBytes = UINT(sizeof(TriangleData) * vertexData_.size());
	vbView_.StrideInBytes = sizeof(TriangleData);

	// indexの作成
	if (indices_.empty()) {
		return;
	}

	index_.CreateConstantBuffer(device, indices_.size());
	std::memcpy(index_.cMap_, indices_.data(), sizeof(uint32_t) * indices_.size());

	ibView_.BufferLocation = index_.cBuffer->GetGPUVirtualAddress();
	ibView_.SizeInBytes = UINT(sizeof(uint32_t) * indices_.size());
	ibView_.Format = DXGI_FORMAT_R32_UINT;
}

void Triangle3D::RefreshVertex()
{
	// デバイス
	ID3D12Device* device = DirectXCommon::GetInstance()->GetDevice();
	// マッピングのし直し
	vertex_.cBuffer->Unmap(0, nullptr);
	vertex_.CreateConstantBuffer(device, vertexData_.size());
	std::memcpy(vertex_.cMap_, vertexData_.data(), sizeof(TriangleData) * vertexData_.size());
	// ビューの再生成
	vbView_.BufferLocation = vertex_.cBuffer->GetGPUVirtualAddress();
	vbView_.SizeInBytes = UINT(sizeof(TriangleData) * vertexData_.size());
	vbView_.StrideInBytes = sizeof(TriangleData);
}

void Triangle3D::GeneratePolygon(const Vector4& color, const float& width)
{
	//size_t numPoints = curvePoints_.size();

	//int32_t vertexId = 0;
	//int32_t indexId = 0;

	//for (size_t i = 0; i < numPoints - 1; ++i) {
	//	// 始点と終点
	//	Vector3 startPoint = curvePoints_[i];
	//	Vector3 endPoint = curvePoints_[i + 1];

	//	// 接戦
	//	Vector3 tangent = endPoint - startPoint;
	//	Vector3 normal = Vector3::Up();

	//	Vector3 right = { -tangent.z,0.0f,tangent.x };
	//	float length = std::sqrtf(right.x * right.x + right.z * right.z);
	//	right.x /= length;
	//	right.z /= length;

	//	// 幅調整
	//	right.x *= width * 0.5f;
	//	right.z *= width * 0.5f;

	//	// 頂点
	//	float alpha = (vertexId + 1.0f) / (float)vertexData_.size();
	//	Vector4 addColor = color;
	//	addColor.w = alpha;
	//	vertexData_[vertexId] = { {startPoint.x - right.x,startPoint.y,startPoint.z - right.z},addColor };
	//	vertexData_[vertexId + 1] = { {startPoint.x + right.x,startPoint.y,startPoint.z + right.z},addColor };
	//
	//	// インデックスの設定
	//	if (i < numPoints - 2) {
	//		indices_[indexId++] = vertexId;
	//		indices_[indexId++] = vertexId + 1;
	//		indices_[indexId++] = vertexId + 2;

	//		indices_[indexId++] = vertexId + 1;
	//		indices_[indexId++] = vertexId + 3;
	//		indices_[indexId++] = vertexId + 2;
	//	}

	//	// 次の頂点に
	//	vertexId += 2;
	//}
	color, width;
}

void Triangle3D::UpdateVertex(std::vector<Vector3> controllPoint,const Vector4& color, const float& width)
{
	size_t numPoints = controllPoint.size();

	// 補間数が足りない場合早期
	if (numPoints < 2) { return; }

	int32_t vertexId = 0;
	int32_t indexId = 0;

	for (size_t i = 0; i < numPoints - 1; ++i) {
		// 始点と終点
		Vector3 startPoint = controllPoint[i];
		Vector3 endPoint = controllPoint[i + 1];

		// 接戦
		Vector3 tangent = endPoint - startPoint;
		Vector3 normal = Vector3::Up();

		Vector3 right = { -tangent.y,tangent.x,0.0f };
		float length = std::sqrtf(right.x * right.x + right.y * right.y);
		right.x /= length;
		right.y /= length;

		// 幅調整
		right.x *= width * 0.5f;
		right.y *= width * 0.5f;

		// ビルボード処理
		if (camera_) {
			Vector3 toCamera = Vector3::Normalize(camera_->transform_.translate - startPoint);
			Vector3 trailDirect = Vector3::Normalize(endPoint - startPoint);
			Vector3 perpendicular = Vector3::Cross(toCamera, trailDirect);

			// 頂点
			vertexData_[vertexId] = { {startPoint.x - perpendicular.x,startPoint.y - perpendicular.y,startPoint.z - perpendicular.z},color };
			vertexData_[vertexId + 1] = { {startPoint.x,startPoint.y,startPoint.z},color };

			//vertexData_[vertexId + 1] = { {startPoint.x + perpendicular.x,startPoint.y + perpendicular.y,startPoint.z + perpendicular.z},color };
		}
		// ビルボードなし
		else {
			// 頂点
			vertexData_[vertexId] = { {startPoint.x - right.x,startPoint.y - right.y,startPoint.z},color };
			vertexData_[vertexId + 1] = { {startPoint.x,startPoint.y,startPoint.z},color };
			//vertexData_[vertexId + 1] = { {startPoint.x + right.x,startPoint.y + right.y,startPoint.z},color };
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
