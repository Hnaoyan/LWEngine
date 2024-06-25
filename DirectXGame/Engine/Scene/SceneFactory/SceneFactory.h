#pragma once

#include "AbstractSceneFactory.h"
#include "../SceneList.h"

class SceneFactory : public AbstractSceneFactory
{
public:
	/// <summary>
	/// シーン作成
	/// </summary>
	/// <param name="sceneName"></param>
	/// <returns></returns>
	IScene* CreateScene(const std::string& sceneName) override;

};

