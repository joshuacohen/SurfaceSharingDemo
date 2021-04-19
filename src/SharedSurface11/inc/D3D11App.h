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

	const std::wstring GetSurface() { return renderTargetGuidStr; }
	const std::wstring GetDepthBuffer() { return depthBufferGuidStr; }
	const std::wstring GetFence() { return sharedFenceGuidStr; }

	static std::wstring MakeGuidStr();

protected:
	ComPtr<IDXGIFactory1> factory = nullptr;
	ComPtr<IDXGISwapChain> swapChain = nullptr;
	ComPtr<ID3D11Device5> device = nullptr;
	ComPtr<ID3D11DeviceContext4> context = nullptr;
	
	ComPtr<ID3D11Texture2D> backBuffer = nullptr;	
	ComPtr<ID3D11Texture2D> sharedRenderTarget = nullptr;
	ComPtr<ID3D11Texture2D> sharedDepthBuffer = nullptr;
	ComPtr<ID3D11RenderTargetView> rtv = nullptr;
	ComPtr<ID3D11Fence> sharedFence = nullptr;

	std::wstring renderTargetGuidStr = MakeGuidStr();
	std::wstring depthBufferGuidStr = MakeGuidStr();
	std::wstring sharedFenceGuidStr = MakeGuidStr();

	unsigned int monotonicCounter = 0;
};