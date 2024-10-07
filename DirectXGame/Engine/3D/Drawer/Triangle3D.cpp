#include "Triangle3D.h"
#include "Engine/Base/DirectXCommon.h"
#include "Engine/3D/ModelUtility/ModelRenderer.h"
#include "Engine/2D/TextureManager.h"

#include<imgui.h>

void Triangle3D::Initialize()
{
	// USER
	isBillBoard_ = true;
	color_ = { 0.0f,0.0f,0.0f,1.0f };
	width_ = 1.0f;
	// 頂点情報作成
	CreateVertex();
}

void Triangle3D::Update(std::vector<Vector3> controlPoint)
{	
	// 頂点の情報
	UpdateVertex(controlPoint, color_, width_);
	// ポインタに値を渡す
	memcpy(vertex_.cMap_, vertexData_.data(), sizeof(TriangleData) * vertexData_.size());
	memcpy(index_.cMap_, indices_.data(), sizeof(uint32_t) * indices_.size());
}

void Triangle3D::CreateVertex()
{
	const size_t kMaxSegments = 1024 * 2;

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

void Triangle3D::UpdateVertex(std::vector<Vector3> controllPoint,const Vector4& color, const float& width)
{
	size_t numPoints = controllPoint.size();
	size_t alphaIndex = controllPoint.size();

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

		Vector4 newColor = color;

		newColor.w = ((float)i + 1.0f) / (float)controllPoint.size();
		//newColor.w = ((float)alphaIndex) / (float)controllPoint.size();
		newColor.w = std::clamp(newColor.w, 0.0f, 1.0f);

		// ビルボード処理
		if (isBillBoard_ && camera_.has_value()) {
			assert(camera_);
			Vector3 toCamera = Vector3::Normalize(camera_.value()->transform_.translate - startPoint);
			Vector3 trailDirect = Vector3::Normalize(endPoint - startPoint);
			Vector3 perpendicular = Vector3::Cross(toCamera, trailDirect);

			perpendicular = Vector3::Normalize(perpendicular);
			perpendicular *= width * 0.5f;

			// 頂点
			vertexData_[vertexId] = { {startPoint.x - perpendicular.x,startPoint.y - perpendicular.y,startPoint.z - perpendicular.z},newColor };
			//vertexData_[vertexId + 1] = { {startPoint.x,startPoint.y,startPoint.z},color };
			vertexData_[vertexId + 1] = { {startPoint.x + perpendicular.x,startPoint.y + perpendicular.y,startPoint.z + perpendicular.z},newColor };
		}
		// ビルボードなし
		else {
			// 頂点
			vertexData_[vertexId] = { {startPoint.x - right.x,startPoint.y - right.y,startPoint.z},newColor };
			//vertexData_[vertexId + 1] = { {startPoint.x,startPoint.y,startPoint.z},color };
			vertexData_[vertexId + 1] = { {startPoint.x + right.x,startPoint.y + right.y,startPoint.z},newColor };
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
		alphaIndex--;
	}

}
