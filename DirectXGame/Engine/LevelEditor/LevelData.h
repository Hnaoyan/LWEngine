#pragma once
#include "../3D/CBuffer.h"

#include <list>

class LevelData
{
public:
	struct ObjectData
	{
		EulerTransform transform;
		std::string filename;
	};

	std::list<ObjectData> objects;

	EulerTransform transform_;

};
