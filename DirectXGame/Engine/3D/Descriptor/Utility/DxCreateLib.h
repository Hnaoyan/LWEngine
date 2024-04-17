#pragma once
#include <d3d12.h>
#include <cstdint>

namespace DxCreateLib
{
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

	};

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

}