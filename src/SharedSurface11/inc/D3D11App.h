#pragma once
#include <d3d11_4.h>
#include <windows.h>
#include <wrl.h>
#include <string>

template<class T>
using ComPtr = Microsoft::WRL::ComPtr<T>;

class D3D11App {
public:
	void Init(HWND hwnd);
	bool Update();
	void Shutdown();

	const HANDLE GetSurface() { return rtHandle; }
	const HANDLE GetDepthBuffer() { return dbHandle; }
	const HANDLE GetFence() { return fenceHandle; }

protected:
	ComPtr<IDXGIFactory1> factory = nullptr;
	ComPtr<IDXGISwapChain> swapChain = nullptr;
	ComPtr<ID3D11Device5> device = nullptr;
	ComPtr<ID3D11DeviceContext4> context = nullptr;
	
	ComPtr<ID3D11Texture2D> backBuffer = nullptr;	
	ComPtr<ID3D11Texture2D> sharedRenderTarget = nullptr;
	ComPtr<ID3D11Texture2D> sharedDepthBuffer = nullptr;
	ComPtr<ID3D11Texture2D> ownedDepthBuffer = nullptr;
	ComPtr<ID3D11RenderTargetView> rtv = nullptr;
	ComPtr<ID3D11Fence> sharedFence = nullptr;

	HANDLE rtHandle = nullptr;
	HANDLE dbHandle = nullptr;
	HANDLE fenceHandle = nullptr;

	unsigned int monotonicCounter = 0;
};