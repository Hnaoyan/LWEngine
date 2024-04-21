#pragma once
#include <d3d12.h>
#include <cstdint>
#include <cassert>

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
			resourceDesc.Width = sizeInBytes;	// リソースのサイズ
			// バッファの場合はこれらは1にする決まり
			resourceDesc.Height = 1;
			resourceDesc.DepthOrArraySize = 1;
			resourceDesc.MipLevels = 1;
			resourceDesc.SampleDesc.Count = 1;
			// バッファの場合はこれにする決まり
			resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

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
	/// Descriptor関係の関数
	/// </summary>
	class DescriptorLib {
	public:
		inline static D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandle(ID3D12DescriptorHeap* descriptorHeap, uint32_t descriptorSize, uint32_t index) 
		{
			D3D12_CPU_DESCRIPTOR_HANDLE handleCPU = descriptorHeap->GetCPUDescriptorHandleForHeapStart();
			handleCPU.ptr += (descriptorSize * index);
			return handleCPU;
		}

		inline static D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandle(ID3D12DescriptorHeap* descriptorHeap, uint32_t descriptorSize, uint32_t index)
		{
			D3D12_GPU_DESCRIPTOR_HANDLE handleCPU = descriptorHeap->GetGPUDescriptorHandleForHeapStart();
			handleCPU.ptr += (descriptorSize * index);
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

}