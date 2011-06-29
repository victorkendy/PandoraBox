
#include <string>
#include <iostream>

#if defined (WIN32) || defined (_WIN32)
    #include <windows.h>
	#include <windowsx.h>
    #include <atlstr.h>
    const std::string _windowClassName = "pbge_window_class";
    const CString windowClassName = CString(_windowClassName.c_str());
#else 
    #error "system not supported"
#endif

#include "pbge/core/Window.h"
#include "pbge/core/KeyboardEventHandler.h"
#include "pbge/core/MouseEventHandler.h"

#include "pbge/gfx/GraphicAPI.h"
#include "pbge/gfx/Renderer.h"
#include "pbge/gfx/SceneGraph.h"
#include "pbge/gfx/SceneInitializer.h"


#if defined (WIN32) || defined (_WIN32)

namespace {
    class WGLContext : public pbge::GraphicContext {
    public:
        WGLContext(HDC hDC) {
            hdc = hDC;
            makePixelFormatDescriptor();
            context = wglCreateContext(hdc);
        }

        ~WGLContext() {
            wglMakeCurrent(hdc, NULL);
            wglDeleteContext(context);
        }

        void makeCurrent() {
            wglMakeCurrent(hdc, context);
        }

        void swapBuffers() {
            SwapBuffers(hdc);
        }

        void release() {
            wglMakeCurrent(hdc, NULL);
            wglDeleteContext(context);
        }

        void getSystemGraphicContext(void * p_context) {
            HGLRC * p_hglrc = reinterpret_cast<HGLRC*>(p_context);
            *p_hglrc = context;
        }

        void getSystemDeviceContext(void * p_dc) {
            HDC * p_device = reinterpret_cast<HDC*>(p_dc);
            *p_device = hdc;
        }

    private:
        void makePixelFormatDescriptor() {
            PIXELFORMATDESCRIPTOR pfd = {
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
            int nPixelFormat = ChoosePixelFormat(hdc, &pfd);
            SetPixelFormat(hdc, nPixelFormat, &pfd);
        }

        HGLRC context;

        HDC hdc;
    };

    void messageLoop(HWND hwnd) {
        BOOL code;
        bool done = false;
        MSG msg;
        while((code = GetMessage(&msg, hwnd, 0,0)) != 0 && !done) {
            if (code < 0 || msg.message == WM_QUIT)
                done = true;
            else {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
    }

    int isRepeatedKey(LPARAM lParam) {
        return (lParam >> 30) & 1;
    }

    LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam){
        pbge::Window * window;
        pbge::GraphicAPI * ogl;
        if(msg == WM_CREATE) {
            CREATESTRUCT * cs = (CREATESTRUCT*)lParam;
            window = reinterpret_cast<pbge::Window*>(cs->lpCreateParams);
            SetWindowLongPtr(hwnd, GWLP_USERDATA, (ULONG)window);
            ogl = window->getGraphicAPI();
            ogl->setContext(new WGLContext(GetDC(hwnd)));
            if(window->getSceneInitializer() != NULL && window->getScene() == NULL) {
                pbge::SceneInitializer * initializer = window->getSceneInitializer();
                window->setScene((*initializer)(ogl, window));
                window->getRenderer()->setScene(window->getScene());
            }
            ogl->enableDrawBuffer(GL_BACK);
            ogl->clearColor(0,0,0,1);
        } else {
            window = reinterpret_cast<pbge::Window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
            switch(msg) {
                case WM_KEYDOWN:
                    window->getEventHandler()->handleKeyDown((char)wParam);
                    break;
                case WM_KEYUP:
                    window->getEventHandler()->handleKeyUp((char)wParam);
                    break;
				case WM_LBUTTONDOWN:
					window->getEventHandler()->handleButtonDown(pbge::L_MOUSE_BUTTON, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
					break;
			    case WM_LBUTTONUP:
					window->getEventHandler()->handleButtonUp(pbge::L_MOUSE_BUTTON, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
					break;
				case WM_RBUTTONDOWN:
					window->getEventHandler()->handleButtonDown(pbge::R_MOUSE_BUTTON, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
					break;
				case WM_RBUTTONUP:
					window->getEventHandler()->handleButtonUp(pbge::R_MOUSE_BUTTON, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
					break;
	            case WM_MOUSEMOVE:
					window->getEventHandler()->handleMove(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
					break;
                case WM_CLOSE:
                    PostQuitMessage(0);
                    break;
                case WM_DESTROY:
                    ogl = window->getGraphicAPI();
                    ogl->releaseContext();
                    PostQuitMessage(0);
                    // find somewhere to delete OpenGL
                    break;
                case WM_SIZE:
                    window->getEventHandler()->handleResize(LOWORD(lParam), HIWORD(lParam));
                    break;
                case WM_PAINT:
                    ogl = window->getGraphicAPI();
                    ogl->clear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
                    window->getRenderer()->render();
                    ogl->swapBuffers();
                    return 0;
                default:
                    break;
            }

        }
        return (DefWindowProc(hwnd, msg, wParam, lParam));
    }
}
#endif

using namespace pbge;

void Window::displayWindow() {
    this->renderer = new Renderer(this->getGraphicAPI());

    #if defined (WIN32) || defined (_WIN32) // code for win32 system

    HINSTANCE hInstance = static_cast<HINSTANCE>(GetModuleHandle(NULL));
    // will register a windows class for the window that will be created
    WNDCLASS wc;
    wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wc.lpfnWndProc = static_cast<WNDPROC>(WndProc);
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = NULL;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = NULL;
    wc.lpszMenuName = NULL;
    wc.lpszClassName = windowClassName;
    // TODO: store atom somewhere so i don't have to register the window class everytime
    RegisterClass(&wc);
    // TODO: make the style dynamic to cope with fullscreen rendering
    DWORD dwStyle = WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;

    HWND hwnd = CreateWindow(windowClassName, CString(this->windowTitle.c_str()), dwStyle,
                             this->positionX, this->positionY, this->width, this->height, NULL, NULL, hInstance, (LPVOID)this);
    ShowWindow(hwnd, SW_SHOW);
    UpdateWindow(hwnd);
    messageLoop(hwnd);
    UnregisterClass(windowClassName, hInstance);
    #endif
}

Window::~Window() {
    delete initializer;
    delete scene;
    delete renderer;
}

