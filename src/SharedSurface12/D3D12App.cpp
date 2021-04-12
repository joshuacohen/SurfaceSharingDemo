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

void D3D12App::Init(HANDLE sharedSurfaceHandle, HANDLE sharedFenceHandle) {
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

	D3D12_RENDER_TARGET_VIEW_DESC rtvd {
		DXGI_FORMAT_B8G8R8A8_UNORM,
		D3D12_RTV_DIMENSION_TEXTURE2D,
		0
	};

	ThrowOnError(D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_12_1, IID_PPV_ARGS(&device)));
	ThrowOnError(device->CreateCommandQueue(&cq, IID_PPV_ARGS(&commandQueue)));
	ThrowOnError(device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&commandAllocator)));
	ThrowOnError(device->CreateDescriptorHeap(&dh_rtv, IID_PPV_ARGS(&rtvHeap)));

	rtvHandle = rtvHeap->GetCPUDescriptorHandleForHeapStart();

	ThrowOnError(device->CreateCommandList(
		0,
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		commandAllocator.Get(),
		nullptr,
		IID_PPV_ARGS(&commandList)
	));

	device->CreateRenderTargetView(sharedSurface.Get(), &rtvd, rtvHandle);

	HANDLE h;
	ThrowOnError(device->OpenSharedHandleByName(L"Shared Surface", GENERIC_ALL, &h));
	ThrowOnError(device->OpenSharedHandle(h, IID_PPV_ARGS(&sharedSurface)));

	ThrowOnError(device->OpenSharedHandleByName(L"Shared Fence", GENERIC_ALL, &h));
	ThrowOnError(device->OpenSharedHandle(h, IID_PPV_ARGS(&sharedFence)));
}

bool D3D12App::Update() {
	unsigned long long counter = sharedFence->GetCompletedValue();
	ThrowOnError(commandQueue->Signal(sharedFence.Get(), 1 + counter));

	commandList->OMSetRenderTargets(1, &rtvHandle, true, nullptr);
	commandList->ClearRenderTargetView(rtvHandle, DirectX::Colors::CornflowerBlue,0, nullptr);
	commandList->Close();

	ID3D12CommandList* commandLists[] = {commandList.Get()};
	ThrowOnError(commandQueue->Wait(sharedFence.Get(), 2 + counter));
	commandQueue->ExecuteCommandLists(1, commandLists);

	return true;
}

void D3D12App::Shutdown() {

}
