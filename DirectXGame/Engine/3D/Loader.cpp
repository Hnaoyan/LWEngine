#include "Loader.h"
#include "../2D/TextureManager.h"
#include <fstream>
#include <sstream>
#include <cassert>

ModelData Loader::LoadObj(const std::string& directory, const std::string& fileName, LoadExtension ex)
{
	ModelData modelData;
	std::vector<Vector4> positions;
	std::vector<Vector2> texcoords;
	std::vector<Vector3> normals;

	std::string line;

	// ファイル開く
	// ファイルまでのパス
	std::string fileDirectory = directory + "/" + fileName;
	// フルパス
	std::string fullPath = fileDirectory + "/" + fileName;

	// 識別子を判断
	switch (ex)
	{
	case LoadExtension::kObj:
		fullPath += ".obj";
		break;
	case LoadExtension::kGltf:
		fullPath += ".obj";
		break;
	}


	std::ifstream file(fullPath);
	assert(file.is_open());

	// 読み込み
	while (std::getline(file, line))
	{
		std::string identifier;
		std::istringstream s(line);

		s >> identifier;

		// 座標
		if (identifier == "v") {
			Vector4 pos = {};
			s >> pos.x >> pos.y >> pos.z;
			//pos.x *= -1.0f;
			pos.w = 1.0f;
			positions.push_back(pos);
		}
		// 法線
		else if (identifier == "vn") {
			Vector3 normal = {};
			s >> normal.x >> normal.y >> normal.z;
			//normal.x *= -1.0f;
			normals.push_back(normal);
		}
		// 
		else if (identifier == "vt") {
			Vector2 texcoord = {};
			s >> texcoord.x >> texcoord.y;
			// Y方向反転
			//texcoord.y = 1.0f - texcoord.y;
			texcoords.push_back(texcoord);
		}
		// 三角形限定
		else if (identifier == "f") {
			//
			VertexData triangle[3] = {};

			for (int32_t faceVertex = 0; faceVertex < 3; ++faceVertex) {
				std::string vertexDefinition;

				s >> vertexDefinition;
				//
				std::istringstream v(vertexDefinition);
				uint32_t elementIndices[3] = {};
				for (int32_t element = 0; element < 3; ++element) {
					std::string index;
					std::getline(v, index, '/');
					elementIndices[element] = std::stoi(index);
				}

				//
				Vector4 position = positions[elementIndices[0] - 1];
				Vector3 normal = normals[elementIndices[2] - 1];
				Vector2 texcoord = texcoords[elementIndices[1] - 1];

				// 反転処理
				position.x *= -1.0f;
				normal.x *= -1.0f;
				texcoord.y = 1.0f - texcoord.y;

				triangle[faceVertex] = { position,normal,texcoord };

			}

			modelData.vertices.push_back(triangle[2]);
			modelData.vertices.push_back(triangle[1]);
			modelData.vertices.push_back(triangle[0]);
		}
		// マテリアル
		else if (identifier == "mtllib") {
			std::string materialFilename;

			s >> materialFilename;
			modelData.material = LoadMaterial(fileDirectory, materialFilename);
		}

	}
	ModelData result = modelData;

	return result;

}

ModelData Loader::LoadAssimp(const std::string& directory, const std::string& fileName)
{
	ModelData modelData;

	
	Assimp::Importer importer;
	// ファイルまでのパス
	std::string fileDirectory = directory + "/" + fileName;
	std::string filePath = fileDirectory + "/" + fileName + ".obj";
	const aiScene* scene = importer.ReadFile(filePath.c_str(), aiProcess_FlipWindingOrder | aiProcess_FlipUVs);
	assert(scene->HasMeshes());


	for (uint32_t meshIndex = 0; meshIndex < scene->mNumMeshes; ++meshIndex) {
		aiMesh* mesh = scene->mMeshes[meshIndex];
		assert(mesh->HasNormals());	// 法線がないMeshは今回は非対応
		assert(mesh->HasTextureCoords(0));	// TexCoordが無い場合は今回は非対応
		// ここからMeshのFaceを解析
		for (uint32_t faceIndex = 0; faceIndex < mesh->mNumFaces; ++faceIndex) {
			aiFace& face = mesh->mFaces[faceIndex];
			assert(face.mNumIndices == 3);	// 三角のみサポート
			// ここからFaceのVertexの解析
			for (uint32_t element = 0; element < face.mNumIndices; ++element) {
				uint32_t vertexIndex = face.mIndices[element];
				aiVector3D& position = mesh->mVertices[vertexIndex];
				aiVector3D& normal = mesh->mNormals[vertexIndex];
				aiVector3D& texcoord = mesh->mTextureCoords[0][vertexIndex];

				VertexData vertex{};

				//VertexData vertex;
				vertex.position = { position.x,position.y,position.z,1.0f };
				vertex.normal = { normal.x,normal.y,normal.z };
				vertex.texcoord = { texcoord.x,texcoord.y };

				vertex.position.x *= -1.0f;
				vertex.normal.x *= -1.0f;
				//vertex.normal.z *= -1.0f;

				// Meshに追加
				modelData.vertices.push_back(vertex);
			}
		}

	}

	for (uint32_t materialIndex = 0; materialIndex < scene->mNumMaterials; ++materialIndex) {
		aiMaterial* material = scene->mMaterials[materialIndex];
		if (material->GetTextureCount(aiTextureType_DIFFUSE) != 0) {
			aiString textureFilePath;
			material->GetTexture(aiTextureType_DIFFUSE, 0, &textureFilePath);
			modelData.material.textureFilename = fileDirectory + "/" + textureFilePath.C_Str();
			modelData.material.textureHandle = TextureManager::Load(modelData.material.textureFilename);
		}

	}
	ModelData result = modelData;
	return result;
}

ModelData Loader::LoadGlTF(const std::string& directory, const std::string& fileName, LoadExtension ex)
{
	ModelData modelData;
	ex;
	Assimp::Importer importer;
	std::string fileDirectory = directory + "/" + fileName;
	std::string filePath = fileDirectory + "/" + fileName + ".gltf";
	const aiScene* scene = importer.ReadFile(filePath.c_str(), aiProcess_FlipWindingOrder | aiProcess_FlipUVs);
	assert(scene->HasMeshes());


	for (uint32_t meshIndex = 0; meshIndex < scene->mNumMeshes; ++meshIndex) {
		aiMesh* mesh = scene->mMeshes[meshIndex];
		assert(mesh->HasNormals());	// 法線がないMeshは今回は非対応
		assert(mesh->HasTextureCoords(0));	// TexCoordが無い場合は今回は非対応
		// ここからMeshのFaceを解析
		for (uint32_t faceIndex = 0; faceIndex < mesh->mNumFaces; ++faceIndex) {
			aiFace& face = mesh->mFaces[faceIndex];
			assert(face.mNumIndices == 3);	// 三角のみサポート
			// ここからFaceのVertexの解析
			for (uint32_t element = 0; element < face.mNumIndices; ++element) {
				uint32_t vertexIndex = face.mIndices[element];
				aiVector3D& position = mesh->mVertices[vertexIndex];
				aiVector3D& normal = mesh->mNormals[vertexIndex];
				aiVector3D& texcoord = mesh->mTextureCoords[0][vertexIndex];

				VertexData vertex{};

				vertex.position = { position.x,position.y,position.z,1.0f };
				vertex.normal = { -normal.x,normal.y,normal.z };
				vertex.texcoord = { texcoord.x,texcoord.y };

				//vertex.position.x *= -1.0f;
				vertex.normal.x *= -1.0f;
				//vertex.texcoord.y = 1.0f - texcoord.y;

				// Meshに追加
				modelData.vertices.push_back(vertex);
			}
		}

	}

	for (uint32_t materialIndex = 0; materialIndex < scene->mNumMaterials; ++materialIndex) {
		aiMaterial* material = scene->mMaterials[materialIndex];
		if (material->GetTextureCount(aiTextureType_DIFFUSE) != 0) {
			aiString textureFilePath;
			material->GetTexture(aiTextureType_DIFFUSE, 0, &textureFilePath);
			modelData.material.textureFilename = fileDirectory + "/" + textureFilePath.C_Str();
			modelData.material.textureHandle = TextureManager::Load(modelData.material.textureFilename);
		}

	}

	// ノード読み込み
	modelData.rootNode = ReadNode(scene->mRootNode);

	ModelData data = modelData;
	return data;
}

MaterialData Loader::LoadMaterial(const std::string& directory, const std::string& fileName)
{
	MaterialData materialData;
	std::string line;
	// フルパス
	std::string fullPath = directory + "/" + fileName;
	
	std::ifstream file(fullPath);
	assert(file.is_open());

	while (std::getline(file, line))
	{
		std::string identifier;
		std::istringstream s(line);
		s >> identifier;

		if (identifier == "map_Kd") {
			std::string textureFilename;
			s >> textureFilename;
			// ファイルパス
			materialData.textureFilename = directory + "/" + textureFilename;
		}

	}

	materialData.textureHandle = TextureManager::Load(materialData.textureFilename);
	// 返す値
	MaterialData result = materialData;

	return result;
}

Node Loader::ReadNode(aiNode* node)
{
	Node result;
	aiMatrix4x4 aiLocalMatrix = node->mTransformation;	// NodeのLocalMatrixを取得
	aiLocalMatrix.Transpose();							// 列ベクトル形式を行ベクトル形式に転置
	//result.localMatrix.m[0][0] = aiLocalMatrix[0][0];	// 他の要素も同様に
	for (uint32_t y = 0; y < 4; ++y) {
		for (uint32_t x = 0; x < 4; ++x) {
			result.localMatrix.m[y][x] = aiLocalMatrix[y][x];
		}
	}

	// ...
	result.name = node->mName.C_Str();
	result.children.resize(node->mNumChildren);
	for (uint32_t childIndex = 0; childIndex < node->mNumChildren; ++childIndex) {
		// 再帰的に読み込み階層構造を作っていく
		result.children[childIndex] = ReadNode(node->mChildren[childIndex]);
	}

	return result;

}