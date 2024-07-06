#pragma once
#include "../Utility/Singleton.h"

#include <string>

class LevelLoader : public Singleton<LevelLoader>
{
public:
	const std::string kDefaultDirectoryPath = std::string("Resources") + ("/") + ("LevelData");

public:
	void LoadSceneData(const std::string& filename);

};
