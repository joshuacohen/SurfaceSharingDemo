#include <D3D11App.h>
#include <DirectXColors.h>
#include <comdef.h>
#include <stdexcept>

#include <string>

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

void D3D11App::Init(HWND hwnd) {
	DXGI_SWAP_CHAIN_DESC scd = {
		DXGI_MODE_DESC {
			800,
			600,
			0,
			1,
			DXGI_FORMAT_B8G8R8A8_UNORM,
			DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED,
			DXGI_MODE_SCALING_UNSPECIFIED
		},
		DXGI_SAMPLE_DESC {
			1,
			0
		},
		DXGI_USAGE_RENDER_TARGET_OUTPUT,
		1,
		hwnd,
		true
	};

	D3D11_TEXTURE2D_DESC ss {
		800,
		600,
		1,
		1,
		DXGI_FORMAT_B8G8R8A8_UNORM,
		DXGI_SAMPLE_DESC {
			1,
			0
		},
		D3D11_USAGE_DEFAULT,
		D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET,
		0,
		D3D11_RESOURCE_MISC_SHARED_KEYEDMUTEX | D3D11_RESOURCE_MISC_SHARED_NTHANDLE
	};

	D3D_FEATURE_LEVEL features[] = {
		D3D_FEATURE_LEVEL_9_1,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_11_1,
	};

	ID3D11Device* device0 = nullptr;
	IDXGIResource1* sharedResource = nullptr;
	ID3D11DeviceContext* context0 = nullptr;

	ThrowOnError(CreateDXGIFactory1(IID_PPV_ARGS(&factory)));

	ThrowOnError(D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		D3D11_CREATE_DEVICE_DEBUG,
		features,
		7,
		D3D11_SDK_VERSION,
		&scd,
		&swapChain,
		&device0,
		nullptr,
		&context0
	));

	ThrowOnError(device0->QueryInterface<ID3D11Device5>(&device));
	ThrowOnError(context0->QueryInterface<ID3D11DeviceContext4>(&context));
	ThrowOnError(swapChain->GetBuffer(0, IID_PPV_ARGS(&renderTarget)));
	ThrowOnError(device->CreateTexture2D(&ss, nullptr, &sharedSurface));
	ThrowOnError(sharedSurface->QueryInterface(IID_PPV_ARGS(&sharedResource)));

	ThrowOnError(device->CreateFence(
		0,
		D3D11_FENCE_FLAG_SHARED,
		IID_PPV_ARGS(&sharedFence)
	));

	GUID surfaceGuid, fenceGuid;
	HANDLE surfaceHandle, fenceHandle;

	CoCreateGuid(&surfaceGuid);
	CoCreateGuid(&fenceGuid);

	StringFromGUID2(surfaceGuid, surfaceGuidStr, sizeof(surfaceGuidStr));
	StringFromGUID2(fenceGuid, fenceGuidStr, sizeof(fenceGuidStr));

	ThrowOnError(sharedResource->CreateSharedHandle(
		nullptr, 
		DXGI_SHARED_RESOURCE_READ | DXGI_SHARED_RESOURCE_WRITE,
		surfaceGuidStr,
		&surfaceHandle
	));

	ThrowOnError(sharedFence->CreateSharedHandle(
		nullptr, 
		DXGI_SHARED_RESOURCE_READ | DXGI_SHARED_RESOURCE_WRITE,
		fenceGuidStr,
		&fenceHandle
	));
}

bool D3D11App::Update() {
	unsigned long long before = sharedFence->GetCompletedValue();

	ThrowOnError(context->Wait(sharedFence.Get(), 1 + before));
	ThrowOnError(context->Signal(sharedFence.Get(), 2 + before));
	ThrowOnError(swapChain->Present(1, 0)); //Flushes the queue?

	return true;
}

void D3D11App::Shutdown() {

}