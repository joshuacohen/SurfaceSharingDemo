#include <d3d12.h>
#include <windows.h>
#include <wrl.h>

template<class T>
using ComPtr = Microsoft::WRL::ComPtr<T>;

class D3D12App {
public:
	void Init(HANDLE sharedSurface, HANDLE sharedFence);
	bool Update();
	void Shutdown();

protected:
	ComPtr<ID3D12Device> device;
	ComPtr<ID3D12Resource> sharedSurface;
	ComPtr<ID3D12Fence> sharedFence;
};