#include <d3d12.h>
#include <windows.h>
#include <wrl.h>
#include <string>

template<class T>
using ComPtr = Microsoft::WRL::ComPtr<T>;

class D3D12App {
public:
	void Init(std::wstring surfaceGuidStr, std::wstring fenceGuidStr);
	bool Update();
	void Shutdown();

protected:
	void InitRenderer();
	void InitShaders();
	void InitBuffers();
	void InitTextures(std::wstring surfaceGuidStr, std::wstring fenceGuidStr);
	void InitPipeline();

	ComPtr<ID3D12Device> device;
	ComPtr<ID3D12CommandQueue> commandQueue;
	ComPtr<ID3D12CommandAllocator> commandAllocator;
	ComPtr<ID3D12DescriptorHeap> rtvHeap;
	ComPtr<ID3D12DescriptorHeap> dsvHeap;

	ComPtr<ID3DBlob> vertexShader;
	ComPtr<ID3DBlob> pixelShader;

	ComPtr<ID3D12Resource> vertBuffer;
	D3D12_VERTEX_BUFFER_VIEW vertBufferView;

	ComPtr<ID3D12Resource> sharedSurface;
	ComPtr<ID3D12Resource> depthBuffer;
	ComPtr<ID3D12Fence> sharedFence;
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle;
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle;

	ComPtr<ID3D12RootSignature> rootSig;
	ComPtr<ID3D12GraphicsCommandList> commandList;
	HANDLE cpuFenceEvent;
};