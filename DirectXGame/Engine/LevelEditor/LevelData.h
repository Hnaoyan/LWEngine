#pragma once
#include "Engine/Base/CBufferCommon.h"

#include <list>
#include <string>

/// <summary>
/// Blenderから受け取るデータ
/// </summary>
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
	//std::string 

};
