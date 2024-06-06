#pragma once
#include "Drawer/Model.h"
#include "../Animation/AnimationData.h"
#include "../Utility/Singleton.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

/// <summary>
/// モデルなどの読み込み関数用クラス
/// </summary>
class Loader : public Singleton<Loader>
{
public:
	/// <summary>
	/// obj読み込み
	/// </summary>
	/// <param name="directory"></param>
	/// <param name="fileName"></param>
	/// <returns></returns>
	static ModelData LoadObj(const std::string& directory, const std::string& fileName, LoadExtension ex);

	/// <summary>
	/// Assimpでの読み込み
	/// </summary>
	/// <param name="directory"></param>
	/// <param name="fileName"></param>
	/// <returns></returns>
	static ModelData LoadAssimp(const std::string& directory, const std::string& fileName);

	static ModelData LoadGlTF(const std::string& directory, const std::string& fileName, LoadExtension ex = LoadExtension::kGltf);

	/// <summary>
	/// マテリアル読み込み
	/// </summary>
	/// <param name="directory"></param>
	/// <param name="fileName"></param>
	/// <returns></returns>
	static MaterialData LoadMaterial(const std::string& directory, const std::string& fileName);

public:	// アニメーション関係
	/// <summary>
	/// アニメーションデータの読み込み
	/// </summary>
	/// <param name="directoryPath"></param>
	/// <param name="fileName"></param>
	/// <returns></returns>
	static AnimationData LoadAnimationFile(const std::string& directoryPath, const std::string& fileName);

	/// <summary>
	/// ノード読み込み
	/// </summary>
	/// <param name="node"></param>
	/// <returns></returns>
	static ModelNode ReadNode(aiNode* node);

	static Skeleton CreateSkeleton(const ModelNode& rootNode);

	static int32_t CreateJoint(const ModelNode& node, const std::optional<int32_t>& parent, std::vector<Joint>& joints);

};