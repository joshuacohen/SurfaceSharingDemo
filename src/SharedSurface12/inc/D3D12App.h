#include <d3d12.h>
#include <windows.h>
#include <wrl.h>

class D3D12App {
public:
	void Init(HANDLE sharedSurface, HANDLE sharedFence);
	bool Update();
	void Shutdown();

protected:

};