#pragma once
#include "Drawer/Model.h"
#include "../Utility/Singleton.h"

/// <summary>
/// モデルなどの読み込み関数用クラス
/// </summary>
class Loader : public Singleton<Loader>
{
public:
	static Model::ModelData LoadObj(const std::string& directory, const std::string& fileName);

	static Model::MaterialData LoadMaterial(const std::string& directory, const std::string& fileName);

};