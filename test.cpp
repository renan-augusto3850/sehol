// cl main.c

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#define Assert(c) do { if (!(c)) __debugbreak(); } while (0)

#pragma comment (lib, "user32.lib")

LRESULT CALLBACK win_proc(HWND window, UINT msg, 
                          WPARAM w_param, LPARAM l_param) {
    switch(msg) {
        case WM_KEYDOWN: {
            switch(w_param) {
                case 'O': { 
                    DestroyWindow(window); 
                } break;
            }
        } break;
        
        case WM_DESTROY: {
            PostQuitMessage(0);
        } break;
        
        default: {
            return DefWindowProcW(window, msg, w_param, l_param);
        }
    }
    
    return 0;
}

int WINAPI WinMain(HINSTANCE instance, HINSTANCE prev_instance, 
                   PSTR cmd, int cmd_show) {
    
    WNDCLASSW wc = {
        .lpszClassName = L"MyWindowClass",
        .lpfnWndProc = win_proc,
        .hInstance = instance,
        .hCursor = LoadCursor(0, IDC_CROSS)
    };
    
    ATOM atom = RegisterClassW(&wc);
    Assert(atom && "Failed to register a window");
    
    HWND window = CreateWindowW(wc.lpszClassName, 
                                L"Title", 
                                WS_OVERLAPPEDWINDOW, 
                                CW_USEDEFAULT, CW_USEDEFAULT, 
                                CW_USEDEFAULT, CW_USEDEFAULT, 
                                NULL, NULL, instance, NULL);
    Assert(window && "Failed to create a window");
    
    ShowWindow(window, cmd_show);
    
    for(;;) {
        MSG msg;
        if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            if(msg.message == WM_QUIT) break;
            TranslateMessage(&msg);
            DispatchMessage(&msg);
            continue;
        }    
    }
    return 0;
}
