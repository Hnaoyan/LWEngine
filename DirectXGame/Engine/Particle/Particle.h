#pragma once
#include <memory>

#include "Engine/Base/Utility/DxCreateLib.h"

struct ParticleCS {
	Vector3 translate;
	Vector3 scale;
	float lifetime;
	Vector3 velocity;
	float currentTime;
	Vector4 color;
};

class Particle
{
public:
	void CreateCSResource();
	// リソース
	Microsoft::WRL::ComPtr<ID3D12Resource> particleResources_;

};
