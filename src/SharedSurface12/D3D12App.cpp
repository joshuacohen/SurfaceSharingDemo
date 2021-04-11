#include <D3D12App.h>

#include <stdexcept>
#include <comdef.h>

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
	HANDLE h;
	ThrowOnError(D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_12_1, IID_PPV_ARGS(&device)));

	ThrowOnError(device->OpenSharedHandleByName(L"Shared Surface", GENERIC_ALL, &h));
	ThrowOnError(device->OpenSharedHandle(h, IID_PPV_ARGS(&sharedSurface)));

	ThrowOnError(device->OpenSharedHandleByName(L"Shared Fence", GENERIC_ALL, &h));
	ThrowOnError(device->OpenSharedHandle(h, IID_PPV_ARGS(&sharedFence)));
}

bool D3D12App::Update() {
	return false;
}

void D3D12App::Shutdown() {

}
