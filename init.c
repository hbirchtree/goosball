#include <stdio.h>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <glad/glad.h>

extern void Init();
extern void Update();

/* WGL initialization: https://www.khronos.org/opengl/wiki/Creating_an_OpenGL_Context_(WGL) */

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

HWND win_handle;
HDC ourWindowHandleToDeviceContext;
HGLRC ourOpenGLRenderingContext;

int WinMain( __in HINSTANCE hInstance, __in_opt HINSTANCE hPrevInstance, __in_opt LPSTR lpCmdLine, __in int nShowCmd )
{
    MSG msg          = {0};
    WNDCLASS wc      = {0};
    wc.lpfnWndProc   = WndProc;
    wc.hInstance     = hInstance;
    wc.hbrBackground = (HBRUSH)(COLOR_BACKGROUND);
    wc.lpszClassName = L"Goosball";
    wc.style = CS_OWNDC;
    if( !RegisterClass(&wc) )
        return 1;
    win_handle = CreateWindowW(wc.lpszClassName,L"Goosball",WS_OVERLAPPEDWINDOW|WS_VISIBLE,0,0,800,600,0,0,hInstance,0);

    while( GetMessage( &msg, NULL, 0, 0 ) > 0 )
    {
        DispatchMessage( &msg );
        Update();
        SwapWindow();
    }

    return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch(message)
    {
    case WM_CREATE:
        {
        PIXELFORMATDESCRIPTOR pfd =
        {
            sizeof(PIXELFORMATDESCRIPTOR),
            1,
            PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,    //Flags
            PFD_TYPE_RGBA,            //The kind of framebuffer. RGBA or palette.
            32,                        //Colordepth of the framebuffer.
            0, 0, 0, 0, 0, 0,
            0,
            0,
            0,
            0, 0, 0, 0,
            24,                        //Number of bits for the depthbuffer
            8,                        //Number of bits for the stencilbuffer
            0,                        //Number of Aux buffers in the framebuffer.
            PFD_MAIN_PLANE,
            0,
            0, 0, 0
        };

        ourWindowHandleToDeviceContext = GetDC(hWnd);

        int  letWindowsChooseThisPixelFormat;
        letWindowsChooseThisPixelFormat = ChoosePixelFormat(ourWindowHandleToDeviceContext, &pfd);
        SetPixelFormat(ourWindowHandleToDeviceContext,letWindowsChooseThisPixelFormat, &pfd);

        ourOpenGLRenderingContext = wglCreateContext(ourWindowHandleToDeviceContext);
        wglMakeCurrent (ourWindowHandleToDeviceContext, ourOpenGLRenderingContext);

        gladLoadGL();

        ShowWindow(win_handle, SW_SHOW);
        Init();

        }
        break;
    case WM_SIZING:
        LPRECT rec = (LPRECT)lParam;
        int w = rec->right - rec->left;
        int h = rec->bottom - rec->top;
        glViewport(0, 0, w, h);
        break;
    case WM_DESTROY:
        wglDeleteContext(ourOpenGLRenderingContext);
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;

}

int SwapWindow()
{
    SwapBuffers(ourWindowHandleToDeviceContext);
    return 1;
}

void GetWindowSize(LONG* width, LONG* height)
{
    RECT rect;
    GetWindowRect(win_handle, &rect);
    *width = rect.right - rect.left;
    *height = rect.bottom - rect.top;
}
