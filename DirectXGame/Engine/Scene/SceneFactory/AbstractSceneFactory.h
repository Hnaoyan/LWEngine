#pragma once
#include "../IScene.h"
#include <string>

/// <summary>
/// シーンファクトリーインターフェース
/// </summary>
class AbstractSceneFactory
{
public:

	virtual ~AbstractSceneFactory() = default;

	virtual IScene* CreateScene(const std::string& sceneName) = 0;

};

