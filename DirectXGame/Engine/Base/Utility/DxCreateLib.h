#pragma once
#include <d3d12.h>
#include <cstdint>
#include <cassert>
#include <wrl.h>

#include "../../Math/MathLib.h"

namespace DxCreateLib
{
	/// <summary>
	/// Heap関係の関数
	/// </summary>
	class HeapLib {
	public:
		/// <summary>
		/// HeapDesc
		/// </summary>
		/// <param name="type"></param>
		/// <param name="numDescriptor"></param>
		/// <param name="shaderVisible"></param>
		/// <returns></returns>
		inline static D3D12_DESCRIPTOR_HEAP_DESC CreateDescriptorHeapDesc(D3D12_DESCRIPTOR_HEAP_TYPE type, UINT numDescriptor, bool shaderVisible) 
		{
			D3D12_DESCRIPTOR_HEAP_DESC heapDesc{};
			heapDesc.Type = type;
			heapDesc.NumDescriptors = numDescriptor;
			heapDesc.Flags = shaderVisible ? D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE : D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
			return heapDesc;
		}

		/// <summary>
		/// HeapProps
		/// </summary>
		/// <param name="type"></param>
		/// <returns></returns>
		inline static D3D12_HEAP_PROPERTIES CreateHeapProps(D3D12_HEAP_TYPE type) 
		{
			// 利用するHeapの設定
			D3D12_HEAP_PROPERTIES heapProperties{};
			heapProperties.Type = type;
			heapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
			heapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
			heapProperties.CreationNodeMask = 1;
			heapProperties.VisibleNodeMask = 1;
			return heapProperties;
		}


	};

	/// <summary>
	/// Resource関係の関数
	/// </summary>
	class ResourceLib {
	public:
		inline static D3D12_RESOURCE_DESC CreateResourceDesc(DXGI_FORMAT format, D3D12_RESOURCE_DIMENSION dimension,
			D3D12_RESOURCE_FLAGS flags, uint32_t width, uint32_t height) 
		{
			D3D12_RESOURCE_DESC resourceDesc{};

			resourceDesc.Width = width;		// テクスチャの幅
			resourceDesc.Height = height;	// テクスチャの高さ
			resourceDesc.MipLevels = 1;		// mipMapの数
			resourceDesc.DepthOrArraySize = 1;	// 奥行き or 配列Textureの配列数
			resourceDesc.Format = format;	// DepthStencilとして両可能なフォーマット
			resourceDesc.SampleDesc.Count = 1;	// サンプリングカウント。1固定
			resourceDesc.Dimension = dimension;	// 2次元
			resourceDesc.Flags = flags;	// DepthStencilとして使う通知

			return resourceDesc;
		}

		inline static D3D12_RESOURCE_BARRIER GetResourceBarrier(ID3D12Resource* backBuffer, D3D12_RESOURCE_STATES stateBefore, D3D12_RESOURCE_STATES stateAfter) {
			// TransitionBarrierの設定
			D3D12_RESOURCE_BARRIER barrier{};
			// 今回のバリアはTransition
			barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
			// Noneにしておく
			barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
			// バリアを張る対象のリソース。
			barrier.Transition.pResource = backBuffer;
			// 遷移前（現在）のResourceState
			barrier.Transition.StateBefore = stateBefore;
			// 遷移後のState
			barrier.Transition.StateAfter = stateAfter;
			return barrier;
		}

		inline static ID3D12Resource* CreateBufferResource(ID3D12Device* device, const size_t& sizeInBytes) {
			ID3D12Resource* resultResource = nullptr;

			// 頂点リソース用のヒープの設定
			D3D12_HEAP_PROPERTIES uploadHeapProperties{};
			uploadHeapProperties.Type = D3D12_HEAP_TYPE_UPLOAD;	// UploadHeapを使う
			// 頂点リソースの設定
			D3D12_RESOURCE_DESC resourceDesc{};
			// バッファリソース。テクスチャの場合はまた別の設定をする
			resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
			resourceDesc.Alignment = 0;
			resourceDesc.Width = sizeInBytes;	// リソースのサイズ
			// バッファの場合はこれらは1にする決まり
			resourceDesc.Height = 1;
			resourceDesc.DepthOrArraySize = 1;
			resourceDesc.MipLevels = 1;
			resourceDesc.Format = DXGI_FORMAT_UNKNOWN;
			resourceDesc.SampleDesc.Count = 1;
			resourceDesc.SampleDesc.Quality = 0;
			// バッファの場合はこれにする決まり
			resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
			resourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

			// 実際に頂点リソースを作る
			HRESULT hr = S_FALSE;

			hr = device->CreateCommittedResource(&uploadHeapProperties, D3D12_HEAP_FLAG_NONE,
				&resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
				IID_PPV_ARGS(&resultResource));
			assert(SUCCEEDED(hr));

			return resultResource;
		}

	};

	/// <summary>
	/// DSV関係の関数
	/// </summary>
	class DSVLib {
	public:
		/// <summary>
		/// ClearValue
		/// </summary>
		/// <param name="format"></param>
		/// <param name="depth"></param>
		/// <returns></returns>
		inline static D3D12_CLEAR_VALUE CreateClearValue(DXGI_FORMAT format, FLOAT depth)
		{
			// 深度値のクリア設定
			D3D12_CLEAR_VALUE depthClearValue{};
			depthClearValue.DepthStencil.Depth = depth;
			depthClearValue.Format = format;
			return depthClearValue;
		}
	};

	/// <summary>
	/// RenderTexture生成関数
	/// </summary>
	class RenderTextureLib
	{
	public:
		inline static Microsoft::WRL::ComPtr<ID3D12Resource> CreateRenderTextureResource(ID3D12Device* device, uint32_t width, uint32_t height, DXGI_FORMAT format, const Vector4& clearColor)
		{
			D3D12_RESOURCE_DESC resourceDesc{};
			// RenderTarget
			resourceDesc.Width = width;
			resourceDesc.Height = height;
			resourceDesc.MipLevels = 1;		// mipMapの数
			resourceDesc.DepthOrArraySize = 1;	// 奥行き or 配列Textureの配列数
			resourceDesc.Format = format;	// DepthStencilとして両可能なフォーマット
			resourceDesc.SampleDesc.Count = 1;	// サンプリングカウント。1固定
			resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;	// 2次元
			resourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;

			D3D12_HEAP_PROPERTIES heapProperties{};
			// VRAM上に
			heapProperties.Type = D3D12_HEAP_TYPE_DEFAULT;

			D3D12_CLEAR_VALUE clearValue;
			clearValue.Format = format;
			clearValue.Color[0] = clearColor.x;
			clearValue.Color[1] = clearColor.y;
			clearValue.Color[2] = clearColor.z;
			clearValue.Color[3] = clearColor.w;

			Microsoft::WRL::ComPtr<ID3D12Resource> resultResource;
			device->CreateCommittedResource(
				&heapProperties,
				D3D12_HEAP_FLAG_NONE,
				&resourceDesc,
				D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
				&clearValue,
				IID_PPV_ARGS(&resultResource)
			);
			return Microsoft::WRL::ComPtr<ID3D12Resource>(resultResource);
		}
	};

	/// <summary>
	/// Descriptor関係の関数
	/// </summary>
	class DescriptorLib {
	public:
		inline static D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandle(ID3D12DescriptorHeap* descriptorHeap, uint32_t descriptorSize, uint32_t index) 
		{
			D3D12_CPU_DESCRIPTOR_HANDLE handleCPU = descriptorHeap->GetCPUDescriptorHandleForHeapStart();
			handleCPU.ptr += ((static_cast<long long int>(descriptorSize)) * index);
			return handleCPU;
		}

		inline static D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandle(ID3D12DescriptorHeap* descriptorHeap, uint32_t descriptorSize, uint32_t index)
		{
			D3D12_GPU_DESCRIPTOR_HANDLE handleCPU = descriptorHeap->GetGPUDescriptorHandleForHeapStart();
			handleCPU.ptr += ((static_cast<long long int>(descriptorSize)) * index);
			return handleCPU;
		}

	};

	class ViewLib {
	public:
		inline static D3D12_VIEWPORT CreateViewport(FLOAT width, FLOAT height, FLOAT topLeftX, FLOAT topLeftY, FLOAT minDepth, FLOAT maxDepth) {
			D3D12_VIEWPORT viewport{};
			viewport.Width = width;
			viewport.Height = height;
			viewport.TopLeftX = topLeftX;
			viewport.TopLeftY = topLeftY;
			viewport.MinDepth = minDepth;
			viewport.MaxDepth = maxDepth;
			return viewport;
		}

		inline static D3D12_RECT CreateScissorRect(FLOAT left, FLOAT right, FLOAT top, FLOAT bottom) {
			D3D12_RECT scissorRect{};
			scissorRect.left = LONG(left);
			scissorRect.right = LONG(right);
			scissorRect.top = LONG(top);
			scissorRect.bottom = LONG(bottom);
			return scissorRect;
		}

	};
	//class Common {
	//public:
	//	inline static D3D12_RESOURCE_BARRIER GetBarrier
	//};

	class PSOLib {
	public:
		inline static D3D12_INPUT_ELEMENT_DESC SetInputLayout(const char* semanticName, DXGI_FORMAT format)
		{
			D3D12_INPUT_ELEMENT_DESC inputLayout = {
				semanticName,0,format,0,D3D12_APPEND_ALIGNED_ELEMENT,
				D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
			};

			return inputLayout;
		}

		inline static D3D12_BLEND_DESC SetBlendDesc(D3D12_BLEND srcBlend, D3D12_BLEND_OP blendOp, D3D12_BLEND destBlend)
		{
			D3D12_BLEND_DESC blendDesc{};
			// 初期設定
			blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
			blendDesc.RenderTarget[0].BlendEnable = TRUE;

			// 基本設定
			blendDesc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
			blendDesc.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
			blendDesc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
			// 種類別設定
			blendDesc.RenderTarget[0].SrcBlend = srcBlend;
			blendDesc.RenderTarget[0].BlendOp = blendOp;
			blendDesc.RenderTarget[0].DestBlend = destBlend;
			return blendDesc;
		}

		inline static D3D12_RASTERIZER_DESC SetRasterizerState(D3D12_FILL_MODE fillMode, D3D12_CULL_MODE cullMode)
		{
			D3D12_RASTERIZER_DESC rasterizer{};
			rasterizer.FillMode = fillMode;
			rasterizer.CullMode = cullMode;
			rasterizer.FrontCounterClockwise = FALSE;
			rasterizer.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;
			rasterizer.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
			rasterizer.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
			rasterizer.DepthClipEnable = TRUE;
			rasterizer.MultisampleEnable = FALSE;
			rasterizer.AntialiasedLineEnable = FALSE;
			rasterizer.ForcedSampleCount = 0;
			rasterizer.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;
			return rasterizer;
		}

		inline static D3D12_STATIC_SAMPLER_DESC SetSamplerDesc(UINT shaderRegister, D3D12_FILTER filter)
		{
			D3D12_STATIC_SAMPLER_DESC staticSampler{};
			staticSampler.ShaderRegister = shaderRegister;
			staticSampler.Filter = filter;
			staticSampler.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
			staticSampler.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
			staticSampler.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
			staticSampler.MipLODBias = 0;
			staticSampler.MaxAnisotropy = 16;
			staticSampler.ComparisonFunc = D3D12_COMPARISON_FUNC_ALWAYS;
			staticSampler.BorderColor = D3D12_STATIC_BORDER_COLOR_OPAQUE_WHITE;
			staticSampler.MinLOD = 0.f;
			staticSampler.MaxLOD = D3D12_FLOAT32_MAX;
			staticSampler.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
			staticSampler.RegisterSpace = 0;
			return staticSampler;
		}

		inline static D3D12_DESCRIPTOR_RANGE InitDescpritorRange(D3D12_DESCRIPTOR_RANGE_TYPE rangeType, UINT numDescriptors, UINT baseShaderRegister)
		{
			D3D12_DESCRIPTOR_RANGE ranges{};
			ranges.RangeType = rangeType;
			ranges.NumDescriptors = numDescriptors;
			ranges.BaseShaderRegister = baseShaderRegister;
			ranges.RegisterSpace = 0;
			ranges.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
			return ranges;
		}

		inline static D3D12_ROOT_PARAMETER InitAsConstantBufferView(UINT shaderRegister, UINT registerSpace, D3D12_SHADER_VISIBILITY visibility)
		{
			D3D12_ROOT_PARAMETER rootParam{};
			rootParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
			rootParam.ShaderVisibility = visibility;
			rootParam.Descriptor.RegisterSpace = registerSpace;
			rootParam.Descriptor.ShaderRegister = shaderRegister;
			return rootParam;
		}

		inline static D3D12_ROOT_PARAMETER InitAsDescriptorTable(UINT numDescriptorRanges, const D3D12_DESCRIPTOR_RANGE* pDescritorRange, D3D12_SHADER_VISIBILITY visibility)
		{
			D3D12_ROOT_PARAMETER rootParam{};
			rootParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
			rootParam.ShaderVisibility = visibility;
			rootParam.DescriptorTable.NumDescriptorRanges = numDescriptorRanges;
			rootParam.DescriptorTable.pDescriptorRanges = pDescritorRange;
			return rootParam;
		}

	};

}