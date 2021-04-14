#include <D3D12App.h>

#include <stdexcept>
#include <comdef.h>
#include <DirectXColors.h>

inline void ThrowOnError(HRESULT hr) {
	if (FAILED(hr)) {
		throw std::runtime_error(_com_error(hr).ErrorMessage());
	}
}

template <class T> 
void SafeRelease(T **ppT) {
	if (*ppT) {
		(*ppT)->Release();
		*ppT = NULL;
	}
}

void D3D12App::Init(std::wstring surfaceGuidStr, std::wstring fenceGuidStr) {
	D3D12_COMMAND_QUEUE_DESC cq {
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		D3D12_COMMAND_QUEUE_PRIORITY_NORMAL,
		D3D12_COMMAND_QUEUE_FLAG_NONE,
		0
	};

	D3D12_DESCRIPTOR_HEAP_DESC dh_rtv {
		D3D12_DESCRIPTOR_HEAP_TYPE_RTV,
		1,
		D3D12_DESCRIPTOR_HEAP_FLAG_NONE,
		0
	};

	D3D12_DESCRIPTOR_HEAP_DESC dh_dsv {
		D3D12_DESCRIPTOR_HEAP_TYPE_RTV,
		1,
		D3D12_DESCRIPTOR_HEAP_FLAG_NONE,
		0
	};

	D3D12_RENDER_TARGET_VIEW_DESC rtvd {
		DXGI_FORMAT_B8G8R8A8_UNORM,
		D3D12_RTV_DIMENSION_TEXTURE2D,
		0
	};

	D3D12_RESOURCE_DESC depthBufferDesc {
		D3D12_RESOURCE_DIMENSION_TEXTURE2D,
		0,
		800,
		600,
		1,
		0,
		DXGI_FORMAT_D24_UNORM_S8_UINT,
		DXGI_SAMPLE_DESC {
			1,
			0
		},
		D3D12_TEXTURE_LAYOUT_UNKNOWN,
		D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL
	};

	D3D12_HEAP_PROPERTIES implicitHeapProperties {
		D3D12_HEAP_TYPE_DEFAULT,
		D3D12_CPU_PAGE_PROPERTY_UNKNOWN,
		D3D12_MEMORY_POOL_UNKNOWN,
		0,
		0
	};

	D3D12_CLEAR_VALUE depthBufferClearVal {
		DXGI_FORMAT_D24_UNORM_S8_UINT,
		1.0f,
		0
	};

	ComPtr<ID3D12Debug> debugLayer;
	ThrowOnError(D3D12GetDebugInterface(IID_PPV_ARGS(&debugLayer)));
	debugLayer->EnableDebugLayer();

	ThrowOnError(D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_12_1, IID_PPV_ARGS(&device)));
	ThrowOnError(device->CreateCommandQueue(&cq, IID_PPV_ARGS(&commandQueue)));
	ThrowOnError(device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&commandAllocator)));
	ThrowOnError(device->CreateDescriptorHeap(&dh_rtv, IID_PPV_ARGS(&rtvHeap)));
	ThrowOnError(device->CreateDescriptorHeap(&dh_dsv, IID_PPV_ARGS(&dsvHeap)));

	ThrowOnError(device->CreateCommandList(
		0,
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		commandAllocator.Get(),
		nullptr,
		IID_PPV_ARGS(&commandList)
	));


	rtvHandle = rtvHeap->GetCPUDescriptorHandleForHeapStart();
	device->CreateRenderTargetView(sharedSurface.Get(), &rtvd, rtvHandle);
	dsvHandle = dsvHeap->GetCPUDescriptorHandleForHeapStart();

	ThrowOnError(device->CreateCommittedResource(
		&implicitHeapProperties,
		D3D12_HEAP_FLAG_NONE,
		&depthBufferDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,
		&depthBufferClearVal,
		IID_PPV_ARGS(&depthBuffer)
	));

	commandList->OMSetRenderTargets(1, &rtvHandle, true, nullptr);
	commandList->ClearRenderTargetView(rtvHandle, DirectX::Colors::CornflowerBlue, 0, nullptr);
	commandList->Close();

	HANDLE h;
	ThrowOnError(device->OpenSharedHandleByName(surfaceGuidStr.c_str(), GENERIC_ALL, &h));
	ThrowOnError(device->OpenSharedHandle(h, IID_PPV_ARGS(&sharedSurface)));

	ThrowOnError(device->OpenSharedHandleByName(fenceGuidStr.c_str(), GENERIC_ALL, &h));
	ThrowOnError(device->OpenSharedHandle(h, IID_PPV_ARGS(&sharedFence)));

	cpuFenceEvent = CreateEventEx(NULL, NULL, 0, EVENT_ALL_ACCESS);
}

bool D3D12App::Update() {
	unsigned long long counter = sharedFence->GetCompletedValue();

	ID3D12CommandList* commandLists[] = {commandList.Get()};
	commandQueue->ExecuteCommandLists(1, commandLists);

	ThrowOnError(commandQueue->Signal(sharedFence.Get(), 1 + counter));
	ThrowOnError(commandQueue->Wait(sharedFence.Get(), 2 + counter));

	ThrowOnError(sharedFence->SetEventOnCompletion(2 + counter, cpuFenceEvent));
	WaitForSingleObject(cpuFenceEvent, INFINITE);

	return true;
}

void D3D12App::Shutdown() {

}
