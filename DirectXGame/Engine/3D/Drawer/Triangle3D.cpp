#include "Triangle3D.h"
#include "Engine/Base/DirectXCommon.h"
#include "Engine/3D/ModelUtility/ModelRenderer.h"
#include "Engine/2D/TextureManager.h"

#include<imgui.h>

void Triangle3D::Initialize()
{
	//vertexData_.resize(size_t(30));

	//vertexData_[0].color = { 1.0f,0.0f,0.0f,1.0f };
	//vertexData_[1].color = { 0.0f,0.0f,1.0f,1.0f };
	//vertexData_[2].color = { 0.0f,1.0f,0.0f,1.0f };
	//vertexData_[3].color = { 1.0f,0.0f,0.0f,1.0f };

	//vertexData_[4] = vertexData_[0];
	//vertexData_[5] = vertexData_[2];

	//Vector3 head{};
	//Vector3 tail{};

	//head = { 3.0f,0.0f,0.0f };
	//tail = { -3.0f,0.0f,0.0f };
	//float offset = 2.0f;

	//vertexData_[0].position = head;
	//vertexData_[0].position.y += offset;
	//vertexData_[1].position = head;
	//vertexData_[1].position.y -= offset;
	//vertexData_[2].position = tail;
	//vertexData_[2].position.y += offset;

	//vertexData_[3].position = tail;
	//vertexData_[3].position.y += offset;
	//vertexData_[4].position = tail;
	//vertexData_[4].position.y -= offset;
	//vertexData_[5].position = head;
	//vertexData_[5].position.y -= offset;	

	texture_ = TextureManager::Load("Resources/default/uvChecker.png");

	//head[0] ={}
	Vector3 point = {};
	for (int i = 0; i < 10; i++) {
		curvePoints_.push_back(point);
		point += Vector3(1.0f, 0.5f, 0.0f);
	}
	CreateVertex();

	GeneratePolygon({ 0.0f,1.0f,1.0f,0.85f }, 1.0f);

}

void Triangle3D::Update()
{

	ImGui::Begin("Triangle");

	std::string name;
	int num = 0;
	//for (auto it = vertexData_.begin(); it != vertexData_.end(); ++it) {
	//	name = "Pos" + std::to_string(num);
	//	ImGui::DragFloat3(name.c_str(), &(*it).position.x, 0.01f);
	//	name = "Color" + std::to_string(num);
	//	ImGui::ColorEdit4(name.c_str(), &(*it).color.x);
	//	num++;

	//	ImGui::Separator();
	//}

	num = 0;
	for (auto it = curvePoints_.begin(); it != curvePoints_.end(); ++it) {
		name = "CurvePoint" + std::to_string(num);
		ImGui::DragFloat3(name.c_str(), &(*it).x, 0.01f);
		ImGui::Separator();
		num++;
	}

	ImGui::End();
	
	UpdateVertex(Vector4(0.0f, 1.0f, 1.0f, 0.85f), 1.0f);

	// ポインタに値を渡す
	memcpy(vertex_.cMap_, vertexData_.data(), sizeof(TriangleData) * vertexData_.size());
}

void Triangle3D::Draw(ICamera* camera)
{
	ModelRenderer::TriangleDraw(camera, this);
}

void Triangle3D::CreateVertex()
{
	const size_t kMaxSegments = 60;

	vertexData_.resize(kMaxSegments * 2);
	indices_.resize(kMaxSegments * 6);

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

void Triangle3D::GenerateVertices()
{
	float width = 0.5f;
	for (size_t i = 1; i < curvePoints_.size() - 1; ++i) {
		Vector3 tangent = Vector3::Normalize(curvePoints_[i + 1] - curvePoints_[i - 1]);
		Vector3 normal = { 0.0f,1.0f,0.0f };

		Vector3 right = Vector3::Cross(normal, tangent);

		vertexData_[i - 1].position = curvePoints_[i] - right * (width * 0.5f);

	}

}

void Triangle3D::GeneratePolygon(const Vector4& color, const float& width)
{
	size_t numPoints = curvePoints_.size();

	int32_t vertexId = 0;
	int32_t indexId = 0;

	for (size_t i = 0; i < numPoints - 1; ++i) {
		// 始点と終点
		Vector3 startPoint = curvePoints_[i];
		Vector3 endPoint = curvePoints_[i + 1];

		// 接戦
		Vector3 tangent = endPoint - startPoint;
		Vector3 normal = Vector3::Up();

		Vector3 right = { -tangent.z,0.0f,tangent.x };
		float length = std::sqrtf(right.x * right.x + right.z * right.z);
		right.x /= length;
		right.z /= length;

		// 幅調整
		right.x *= width * 0.5f;
		right.z *= width * 0.5f;

		// 頂点
		vertexData_[vertexId] = { {startPoint.x - right.x,startPoint.y,startPoint.z - right.z},color };
		vertexData_[vertexId + 1] = { {startPoint.x + right.x,startPoint.y,startPoint.z + right.z},color };
	
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

void Triangle3D::UpdateVertex(const Vector4& color, const float& width)
{
	size_t numPoints = curvePoints_.size();

	// 補間数が足りない場合早期
	if (numPoints < 2) { return; }

	int32_t vertexId = 0;
	int32_t indexId = 0;

	for (size_t i = 0; i < numPoints - 1; ++i) {
		// 始点と終点
		Vector3 startPoint = curvePoints_[i];
		Vector3 endPoint = curvePoints_[i + 1];

		// 接戦
		Vector3 tangent = endPoint - startPoint;
		Vector3 normal = Vector3::Up();

		Vector3 right = { -tangent.z,0.0f,tangent.x };
		float length = std::sqrtf(right.x * right.x + right.z * right.z);
		right.x /= length;
		right.z /= length;

		// 幅調整
		right.x *= width * 0.5f;
		right.z *= width * 0.5f;

		// 頂点
		vertexData_[vertexId] = { {startPoint.x - right.x,startPoint.y,startPoint.z - right.z},color };
		vertexData_[vertexId + 1] = { {startPoint.x + right.x,startPoint.y,startPoint.z + right.z},color };

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
