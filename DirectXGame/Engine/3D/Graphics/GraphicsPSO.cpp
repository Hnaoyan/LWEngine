#include "GraphicsPSO.h"

void GraphicsPSO::CreatePipeline()
{
	GraphicsPSO* graphics = GraphicsPSO::GetInstance();

	graphics->CreateModelPSO();
	graphics->CreateParticlePSO();

}

void GraphicsPSO::CreateModelPSO()
{
}

void GraphicsPSO::CreateParticlePSO()
{
}
