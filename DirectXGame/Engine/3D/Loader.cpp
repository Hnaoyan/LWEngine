#include "Loader.h"
#include <fstream>
#include <sstream>
#include <cassert>

Model::ModelData Loader::LoadObj(const std::string& directory, const std::string& fileName)
{
	Model::ModelData modelData;
	std::vector<Vector4> positions;
	std::vector<Vector2> texcoords;
	std::vector<Vector3> normals;

	std::string line;

	// ファイル開く
	std::ifstream file(directory + "/" + fileName);
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
			pos.w = 1.0f;
			positions.push_back(pos);
		}
		// 法線
		else if (identifier == "vn") {
			Vector3 normal = {};
			s >> normal.x >> normal.y >> normal.z;
			normals.push_back(normal);
		}
		// 
		else if (identifier == "vt") {
			Vector2 texcoord = {};
			s >> texcoord.x >> texcoord.y;
			// Y方向反転
			texcoord.y = 1.0f - texcoord.y;
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
				Vector2 texcoord = texcoords[elementIndices[1] - 1];
				Vector3 normal = normals[elementIndices[2] - 1];

				normal.x *= -1.0f;
				triangle[faceVertex] = { position,texcoord,normal };

			}

			modelData.vertices.push_back(triangle[2]);
			modelData.vertices.push_back(triangle[1]);
			modelData.vertices.push_back(triangle[0]);
		}
		// マテリアル
		else if (identifier == "mtllib") {
			std::string materialFilename;

			s >> materialFilename;
			modelData.material = LoadMaterial(directory, materialFilename);
		}

	}

	return modelData;

}

Model::MaterialData Loader::LoadMaterial(const std::string& directory, const std::string& fileName)
{
	Model::MaterialData materialData;
	std::string line;
	std::ifstream file(directory + "/" + fileName);
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

	return materialData;
}
