#include <D3D11App.h>
#include <d3d11_2.h>
#include <DirectXColors.h>

inline void ThrowOnError(HRESULT hr) {
	if (FAILED(hr)) {
		throw std::exception();
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

	D3D_FEATURE_LEVEL features[] = {
		D3D_FEATURE_LEVEL_9_1,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_11_1,
	};

	ThrowOnError(CreateDXGIFactory1(IID_PPV_ARGS(&factory)));

	ID3D11Device* device0;

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
		&context));

	ThrowOnError(device0->QueryInterface<ID3D11Device2>(&device));
	ThrowOnError(swapChain->GetBuffer(0, IID_PPV_ARGS(&renderTarget)));

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

	ThrowOnError(device->CreateTexture2D(&ss, nullptr, &sharedSurface));

	IDXGIResource1* sharedResource = nullptr;
	ThrowOnError(sharedSurface->QueryInterface(IID_PPV_ARGS(&sharedResource)));

	ThrowOnError(sharedResource->CreateSharedHandle(
		nullptr, 
		DXGI_SHARED_RESOURCE_READ | DXGI_SHARED_RESOURCE_WRITE,
		L"Shared Surface",
		&sharedSurfaceHandle
	));

	//ThrowOnError(device->CreateRenderTargetView(renderTarget, nullptr, &rtv));
}

bool D3D11App::Update() {
	context->ClearRenderTargetView(rtv, DirectX::Colors::CornflowerBlue);
	swapChain->Present(1, 0);
	return true;
}

void D3D11App::Shutdown() {
	
}