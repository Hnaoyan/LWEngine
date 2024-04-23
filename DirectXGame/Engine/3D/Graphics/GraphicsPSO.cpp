#include "GraphicsPSO.h"

using namespace Microsoft::WRL;
using namespace Pipeline;

void GraphicsPSO::CreatePipeline()
{
	GraphicsPSO* graphics = GraphicsPSO::GetInstance();

	graphics->CreateModelPSO();
	graphics->CreateParticlePSO();

}

void GraphicsPSO::CreateModelPSO()
{
	HRESULT result = S_FALSE;

	ComPtr<IDxcBlob> vsBlob;
	ComPtr<IDxcBlob> psBlob;
	ComPtr<ID3DBlob> errorBlob;
	ComPtr<ID3DBlob> rootSigBlob;


}

void GraphicsPSO::CreateParticlePSO()
{
}
