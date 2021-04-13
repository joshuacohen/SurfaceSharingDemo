#pragma once
#include <d3d11_4.h>
#include <windows.h>
#include <wrl.h>

template<class T>
using ComPtr = Microsoft::WRL::ComPtr<T>;

class D3D11App {
public:
	void Init(HWND hwnd);
	bool Update();
	void Shutdown();

	const wchar_t* GetSurface() { return surfaceGuidStr; }
	const wchar_t* GetFence() { return fenceGuidStr; }

protected:
	ComPtr<IDXGIFactory1> factory = nullptr;
	ComPtr<IDXGISwapChain> swapChain = nullptr;
	ComPtr<ID3D11Device5> device = nullptr;
	ComPtr<ID3D11DeviceContext4> context = nullptr;
	
	ComPtr<ID3D11Texture2D> renderTarget = nullptr;	
	ComPtr<ID3D11Texture2D> sharedSurface = nullptr;
	ComPtr<ID3D11Fence> sharedFence = nullptr;

	wchar_t surfaceGuidStr[39];
	wchar_t fenceGuidStr[39];
};