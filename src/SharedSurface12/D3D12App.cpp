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

void D3D12App::InitRenderer() {

	D3D12_COMMAND_QUEUE_DESC commandQueueDesc {
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		D3D12_COMMAND_QUEUE_PRIORITY_NORMAL,
		D3D12_COMMAND_QUEUE_FLAG_NONE,
		0
	};

	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc {
		D3D12_DESCRIPTOR_HEAP_TYPE_RTV,
		1,
		D3D12_DESCRIPTOR_HEAP_FLAG_NONE,
		0
	};

	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc {
		D3D12_DESCRIPTOR_HEAP_TYPE_DSV,
		1,
		D3D12_DESCRIPTOR_HEAP_FLAG_NONE,
		0
	};

	ComPtr<ID3D12Debug> debugLayer;
	ThrowOnError(D3D12GetDebugInterface(IID_PPV_ARGS(&debugLayer)));
	debugLayer->EnableDebugLayer();

	ThrowOnError(D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_12_1, IID_PPV_ARGS(&device)));
	ThrowOnError(device->CreateCommandQueue(&commandQueueDesc, IID_PPV_ARGS(&commandQueue)));
	ThrowOnError(device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&commandAllocator)));
	ThrowOnError(device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&rtvHeap)));
	ThrowOnError(device->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&dsvHeap)));
}

void D3D12App::InitShaders() {

}

void D3D12App::InitBuffers() {

}

void D3D12App::InitTextures(std::wstring surfaceGuidStr, std::wstring fenceGuidStr) {
	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc {
		DXGI_FORMAT_B8G8R8A8_UNORM,
		D3D12_RTV_DIMENSION_TEXTURE2D,
		0
	};

	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc {
		DXGI_FORMAT_D24_UNORM_S8_UINT,
		D3D12_DSV_DIMENSION_TEXTURE2D,
		D3D12_DSV_FLAG_READ_ONLY_STENCIL,
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

	HANDLE h;
	ThrowOnError(device->OpenSharedHandleByName(surfaceGuidStr.c_str(), GENERIC_ALL, &h));
	ThrowOnError(device->OpenSharedHandle(h, IID_PPV_ARGS(&sharedSurface)));

	ThrowOnError(device->OpenSharedHandleByName(fenceGuidStr.c_str(), GENERIC_ALL, &h));
	ThrowOnError(device->OpenSharedHandle(h, IID_PPV_ARGS(&sharedFence)));

	ThrowOnError(device->CreateCommittedResource(
		&implicitHeapProperties,
		D3D12_HEAP_FLAG_NONE,
		&depthBufferDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,
		&depthBufferClearVal,
		IID_PPV_ARGS(&depthBuffer)
	));

	rtvHandle = rtvHeap->GetCPUDescriptorHandleForHeapStart();
	device->CreateRenderTargetView(sharedSurface.Get(), &rtvDesc, rtvHandle);

	dsvHandle = dsvHeap->GetCPUDescriptorHandleForHeapStart();
	device->CreateDepthStencilView(depthBuffer.Get(), &dsvDesc, dsvHandle);
}

void D3D12App::InitPipeline() {
	// We are going to serialize an empty root signature 
	D3D12_ROOT_SIGNATURE_DESC rsd {
		0,
		nullptr,
		0,
		nullptr,
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT
	};

	ComPtr<ID3DBlob> sig, err;
	ThrowOnError(D3D12SerializeRootSignature(&rsd, D3D_ROOT_SIGNATURE_VERSION_1, &sig, &err));
	ThrowOnError(device->CreateRootSignature(0, sig->GetBufferPointer(), sig->GetBufferSize(), IID_PPV_ARGS(&rootSig)));

}

void D3D12App::Init(std::wstring surfaceGuidStr, std::wstring fenceGuidStr) {
	InitRenderer();
	InitShaders();
	InitBuffers();
	InitTextures(surfaceGuidStr, fenceGuidStr);
	InitPipeline();

	ThrowOnError(device->CreateCommandList(
		0,
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		commandAllocator.Get(),
		nullptr,
		IID_PPV_ARGS(&commandList)
	));

	commandList->OMSetRenderTargets(1, &rtvHandle, true, &dsvHandle);
	commandList->ClearRenderTargetView(rtvHandle, DirectX::Colors::CornflowerBlue, 0, nullptr);
	commandList->Close();

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
