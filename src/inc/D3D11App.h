#pragma once
#include <memory>
#include <windows.h>

struct ID3D11Device2;
struct ID3D11DeviceContext;
struct IDXGIFactory1;
struct IDXGISwapChain;
struct ID3D11Texture2D;
struct ID3D11RenderTargetView;

class D3D11App {
public:
void Init(HWND hwnd);
bool Update();
void Shutdown();

protected:
	IDXGIFactory1* factory;
	IDXGISwapChain* swapChain;
	ID3D11Device2* device;
	ID3D11DeviceContext* context;
	ID3D11Texture2D* renderTarget;
	ID3D11RenderTargetView* rtv;
private:

};