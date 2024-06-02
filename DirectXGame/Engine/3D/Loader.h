#pragma once
#include "Drawer/Model.h"
#include "../Utility/Singleton.h"

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
	static ModelData LoadObj(const std::string& directory, const std::string& fileName);

	/// <summary>
	/// Assimpでの読み込み
	/// </summary>
	/// <param name="directory"></param>
	/// <param name="fileName"></param>
	/// <returns></returns>
	static ModelData LoadAssimp(const std::string& directory, const std::string& fileName);

	/// <summary>
	/// マテリアル読み込み
	/// </summary>
	/// <param name="directory"></param>
	/// <param name="fileName"></param>
	/// <returns></returns>
	static MaterialData LoadMaterial(const std::string& directory, const std::string& fileName);


};