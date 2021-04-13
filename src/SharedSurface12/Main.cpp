#ifndef UNICODE
#define UNICODE
#endif 

#include <D3D12App.h>
#include <string>
#include <debugapi.h>

#define WAIT_FOR_ATTACH while (!IsDebuggerPresent()) { Sleep(100); } __debugbreak()

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

    // Create the window.
    HWND hwnd = CreateWindowEx(
        0,                     // Optional window styles.
        CLASS_NAME,            // Window class
        L"SharedSurface12",    // Window text
        WS_OVERLAPPEDWINDOW,   // Window style

        // Size and position
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

        NULL,       // Parent window    
        NULL,       // Menu
        hInstance,  // Instance handle
        NULL        // Additional application data
        );

    if (hwnd == NULL)
    {
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);

    std::wstring cmd = pCmdLine;
    std::wstring surfaceGuidStr, fenceGuidStr;
    {
        size_t end = cmd.find(' ');
        surfaceGuidStr = cmd.substr(0, end);
        fenceGuidStr = cmd.substr(end + 1, cmd.size());
    }

    D3D12App app;
    app.Init(surfaceGuidStr, fenceGuidStr);

    // Run the message loop.
    MSG msg = { };
    while (app.Update() && GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    app.Shutdown();
    return 0;
}
