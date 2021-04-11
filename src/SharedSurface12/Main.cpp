#ifndef UNICODE
#define UNICODE
#endif 

#include <D3D12App.h>
#include <string>

#include <debugapi.h>

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            EndPaint(hwnd, &ps);
        }
        return 0;

    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{

    // Register the window class.
    const wchar_t CLASS_NAME[]  = L"SharedSurface12";
    
    WNDCLASS wc = { };

    wc.lpfnWndProc   = WindowProc;
    wc.hInstance     = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

	bool breakOnEntry = true;

	std::wstring cmd = pCmdLine;
	HANDLE sharedSurface;
	HANDLE sharedFence;
	{
		size_t end = cmd.find(' ');
		sharedSurface = (HANDLE)wcstoull(cmd.substr(0, end).c_str(), nullptr, 10);
		sharedFence = (HANDLE)wcstoull(cmd.substr(end, cmd.size()).c_str(), nullptr, 10);
	}

	D3D12App app;
	app.Init(sharedSurface, sharedFence);

    // Run the message loop.
    MSG msg = { };
    while (app.Update(), GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

	app.Shutdown();
    return 0;
}
