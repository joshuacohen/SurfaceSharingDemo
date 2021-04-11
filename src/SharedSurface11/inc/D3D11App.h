#pragma once
#include <windows.h>

struct ID3D11Device5;
struct ID3D11DeviceContext;
struct IDXGIFactory1;
struct IDXGISwapChain;
struct ID3D11Texture2D;
struct ID3D11RenderTargetView;
struct ID3D11Fence;

class D3D11App {
public:
	void Init(HWND hwnd);
	bool Update();
	void Shutdown();

protected:
	IDXGIFactory1* factory = nullptr;
	IDXGISwapChain* swapChain = nullptr;
	ID3D11Device5* device = nullptr;
	ID3D11DeviceContext* context = nullptr;
	
	ID3D11Texture2D* renderTarget = nullptr;	
	ID3D11Texture2D* sharedSurface = nullptr;
	ID3D11Fence* sharedFence = nullptr;

	HANDLE sharedSurfaceHandle;
	HANDLE sharedFenceHandle;
};