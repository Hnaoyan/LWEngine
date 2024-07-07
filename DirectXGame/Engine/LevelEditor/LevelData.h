#pragma once
#include "../3D/CBuffer.h"

#include <list>

class LevelData
{
public:
	struct ObjectData
	{
		Vector3 translation;
		Vector3 rotation;
		Vector3 scaling;
		std::string filename;
	};

	std::list<ObjectData> objects;

	EulerTransform transform_;

};
