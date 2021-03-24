// dear imgui: standalone example application for DirectX 9
// If you are new to dear imgui, see examples/README.txt and documentation at the top of imgui.cpp.
#pragma warning(disable : 4995)
#include "CH5_External.h"

#pragma comment(lib, "urlmon.lib")
#include <urlmon.h>
#include <sstream>
#include <cstring>

// Data
LPDIRECT3D9              g_pD3D = NULL;
LPDIRECT3DDEVICE9        g_pd3dDevice = NULL;
D3DPRESENT_PARAMETERS    g_d3dpp = {};
// Window
HWND g_HackWnd = NULL;

const char* szWindowTitleClass = "Crianosfera v5";

DECLSPEC_NOINLINE bool CreateDeviceD3D(HWND hWnd)
{
    bool bRetValue = false;
    if ((g_pD3D = Direct3DCreate9(D3D_SDK_VERSION)))
    {
        ZeroMemory(&g_d3dpp, sizeof(g_d3dpp));
        g_d3dpp.Windowed = TRUE;
        g_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
        g_d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;//D3DFMT_UNKNOWN;
        g_d3dpp.EnableAutoDepthStencil = TRUE;
        g_d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
        g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;           // Present with vsync
        //g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;   // Present without vsync, maximum unthrottled framerate
        if (g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &g_d3dpp, &g_pd3dDevice) < 0)
            bRetValue = false;
        else
            bRetValue = true;
    }
    return bRetValue;
}

DECLSPEC_NOINLINE void CleanupDeviceD3D()
{
    if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = NULL; }
    if (g_pD3D) { g_pD3D->Release(); g_pD3D = NULL; }
}

void ResetDevice()
{
    ImGui_ImplDX9_InvalidateDeviceObjects();
    HRESULT hr = g_pd3dDevice->Reset(&g_d3dpp);
    if (hr == D3DERR_INVALIDCALL)
        IM_ASSERT(0);
    ImGui_ImplDX9_CreateDeviceObjects();
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_SIZE:
        if (g_pd3dDevice != NULL && wParam != SIZE_MINIMIZED)
        {
            g_d3dpp.BackBufferWidth = LOWORD(lParam);
            g_d3dpp.BackBufferHeight = HIWORD(lParam);
            ResetDevice();
        }
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
            return 0;
        break;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    }
    return ::DefWindowProc(hWnd, msg, wParam, lParam);
}

DECLSPEC_NOINLINE void MainLoop()
{
    // Our state
    ImVec4 clear_color = ImVec4(0.0f, 0.0f, 0.0f, 0.00f);
    // Main loop
    bool bHackEnabled = true;
    MSG msg;
    ZeroMemory(&msg, sizeof(msg));
    while (msg.message != WM_QUIT)
    {
        if (::PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
            continue;
        }
        CH5::Hooks::Update();

        // Start the Dear ImGui frame
        ImGui_ImplDX9_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        CH5::Interface::BackGround();
        CH5::Interface::Windows(&bHackEnabled);

        // Rendering
        ImGui::EndFrame();
        g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
        g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
        g_pd3dDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);
        D3DCOLOR clear_col_dx = D3DCOLOR_RGBA((int)(clear_color.x * 255.0f), (int)(clear_color.y * 255.0f), (int)(clear_color.z * 255.0f), (int)(clear_color.w * 255.0f));
        g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, clear_col_dx, 1.0f, 0);
        if (g_pd3dDevice->BeginScene() >= 0)
        {
            ImGui::Render();
            ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
            g_pd3dDevice->EndScene();
        }
        HRESULT result = g_pd3dDevice->Present(NULL, NULL, NULL, NULL);

        // Handle loss of D3D9 device
        if (result == D3DERR_DEVICELOST && g_pd3dDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
            ResetDevice();

        if (!bHackEnabled)
            msg.message = WM_QUIT;
    }
}

DECLSPEC_NOINLINE int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    WNDCLASSEX wc;
    MARGINS MARGIN = { 0, 0, CH5::Engine::Width, CH5::Engine::Height };
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.cbClsExtra = NULL;
    wc.cbWndExtra = NULL;
    wc.hCursor = LoadCursor(0, IDC_ARROW);
    wc.hIcon = LoadIcon(0, IDI_APPLICATION);
    wc.hIconSm = LoadIcon(0, IDI_APPLICATION);
    wc.hbrBackground = (HBRUSH)::CreateSolidBrush(RGB(0, 0, 0));
    wc.lpfnWndProc = WndProc;
    wc.lpszClassName = szWindowTitleClass;
    wc.lpszMenuName = szWindowTitleClass;
    wc.style = CS_VREDRAW | CS_HREDRAW;

    ::RegisterClassEx(&wc);
    g_HackWnd = ::CreateWindowEx(WS_EX_TOPMOST | WS_EX_TRANSPARENT, wc.lpszClassName, wc.lpszMenuName, WS_POPUP, 0, 0, CH5::Engine::Width, CH5::Engine::Height, 0, 0, 0, 0);

    SetLayeredWindowAttributes(g_HackWnd, 0, 255, LWA_ALPHA);
    SetLayeredWindowAttributes(g_HackWnd, RGB(0, 0, 0), 0, ULW_COLORKEY);

    BOOL bCompositor;
    DwmIsCompositionEnabled(&bCompositor);
    if (!bCompositor)
    {
        MessageBox(g_HackWnd,
            /*Compositor is disabled. Click OK to enable it*/XorStr<0x32, 46, 0xA498E701>("\x71\x5C\x59\x45\x59\x44\x51\x4D\x55\x49\x1C\x54\x4D\x1F\x24\x28\x31\x22\x26\x29\x23\x23\x66\x69\x09\x27\x25\x2E\x25\x6F\x1F\x1A\x72\x27\x3B\x75\x33\x39\x39\x3B\x36\x3E\x7C\x34\x2A" + 0xA498E701).s,
            /*Crianosfera Hack v5 - External*/XorStr<0xD5, 31, 0xDB4B7D49>("\x96\xA4\xBE\xB9\xB7\xB5\xA8\xBA\xB8\xAC\xBE\xC0\xA9\x83\x80\x8F\xC5\x90\xD2\xC8\xC4\xCA\xAE\x94\x99\x8B\x9D\x9E\x90\x9E" + 0xDB4B7D49).s,
            MB_ICONINFORMATION);
        DwmEnableComposition(DWM_EC_ENABLECOMPOSITION);
    }
    DwmExtendFrameIntoClientArea(g_HackWnd, &MARGIN);

    // Initialize Direct3D
    if (CreateDeviceD3D(g_HackWnd))
    {
        ::ShowWindow(g_HackWnd, SW_SHOWDEFAULT);
        ::UpdateWindow(g_HackWnd);
        ::SetWindowPos(g_HackWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
        // Show the window

        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;

        // Setup Dear ImGui style
        ImGui::StyleColorsDark();
        //ImGui::StyleColorsClassic();
        //ImGui::StyleColorsLight();

        ImGuiStyle& styles = ImGui::GetStyle();
        // Disable transparency
        /*for (int i = 0; i < ImGuiCol_COUNT; i++)
            styles.Colors[i].w = 1.0f;*/
            //styles.Colors[ImGuiCol_WindowBg].w = 1.0f;

            //Center titles
        styles.WindowTitleAlign = ImVec2(0.5f, 0.5f);

        // Setup Platform/Renderer bindings
        ImGui_ImplWin32_Init(g_HackWnd);
        ImGui_ImplDX9_Init(g_pd3dDevice);

        // Load Fonts
        // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
        // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
        // - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
        // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
        // - Read 'docs/FONTS.md' for more instructions and details.
        // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
        //io.Fonts->AddFontDefault();
        //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
        //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
        //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
        //io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f);
        //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
        //IM_ASSERT(font != NULL);



        // Hack Threading
        /*HANDLE hUpdateThread =*/ //CreateThread(0, 0, (LPTHREAD_START_ROUTINE)CH5::Hooks::Update, 0, 0, 0);

        // Initialize Lua
        CH5::Lua::Initialize();
        CH5::Lua::ReloadScripts();

        CH5::Settings::Initialize();
        CH5::Settings::Load();

        MainLoop();

        ImGui_ImplDX9_Shutdown();
        ImGui_ImplWin32_Shutdown();
        ImGui::DestroyContext();

        CleanupDeviceD3D();
        ::DestroyWindow(g_HackWnd);
        ::UnregisterClass(wc.lpszClassName, wc.hInstance);
    }
    else
    {
        CleanupDeviceD3D();
        ::UnregisterClass(wc.lpszClassName, wc.hInstance);
    }
    return 0;
}