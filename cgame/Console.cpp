#include "Console.h"

#include "mutex"
#include "thread"
#include "windows.h"
#include "stdexcept"
#include "../cevent/EventBus.h"
#include "../cevent/Events.h"
#include "../cgutil/Logger.h"

// 帧率
static uint8_t g_fps;
static std::chrono::steady_clock::time_point g_lastFpsTime;
static std::chrono::steady_clock::time_point g_lastFrameTime;
static unsigned long long g_frameCount = 0;
static unsigned long long g_targetFrameTimeUs;

// 绘制
static HANDLE g_hOut = nullptr, g_hBuffer = nullptr;
static ccontrol::Graphics* g_graphicsOut = nullptr;
static ccontrol::Graphics* g_graphicsBuffer = nullptr;

// 窗口
static COORD g_size;
static WORD g_attr;

// Root Control
static ccontrol::Control* g_rootControl = nullptr;
static std::condition_variable g_rootControlCV;
static std::mutex g_rootControlMutex;

static void InitConsoleMode()
{
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD mode = 0;
    GetConsoleMode(hOut, &mode);
    SetConsoleMode(hOut,
        mode |
        ENABLE_VIRTUAL_TERMINAL_PROCESSING |
        DISABLE_NEWLINE_AUTO_RETURN |
        ENABLE_MOUSE_INPUT |
        ENABLE_WINDOW_INPUT |
        ENABLE_EXTENDED_FLAGS &
        ~ENABLE_QUICK_EDIT_MODE);
}

static void HideCursor(HANDLE handle)
{
    CONSOLE_CURSOR_INFO ci;
    GetConsoleCursorInfo(handle, &ci);
    ci.bVisible = FALSE;
    SetConsoleCursorInfo(handle, &ci);
}

[[noreturn]]
static void GameLoopThread()
{
    while (true)
    {
        if (g_rootControl != nullptr)
        {
            cevent::GameTickEvent event;
            event.atTickFront = true;
            cevent::DispatchEvent(&event);

            // 处理输入事件
            cevent::ProcessEvent();

            // 渲染
            cgame::ClearBuffer();
            cevent::RenderEvent renderEvent;
            cevent::DispatchEvent(&renderEvent);
            cgame::Draw();
            cgame::Render();

            // 更新帧率
            g_frameCount++;
            auto currentTime = std::chrono::steady_clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - g_lastFpsTime).count();
            if (elapsed >= 1000)
            {
                g_fps = static_cast<uint8_t>(g_frameCount * 1000 / elapsed);
                g_frameCount = 0;
                g_lastFpsTime = currentTime;
            }

            event.atTickFront = false;
            cevent::DispatchEvent(&event);

            // 控制帧率
            if (g_targetFrameTimeUs != 0)
            {
                auto frameEndTime = std::chrono::steady_clock::now();
                auto frameTimeUs = std::chrono::duration_cast<std::chrono::microseconds>(frameEndTime - g_lastFrameTime).count();
                if (frameTimeUs < g_targetFrameTimeUs)
                {
                    std::this_thread::sleep_for(std::chrono::microseconds(g_targetFrameTimeUs - frameTimeUs));
                }

                g_lastFrameTime = frameEndTime;
            }
        }
        else
        {
            // 等待屏幕被设置
            std::unique_lock lock(g_rootControlMutex);
            g_rootControlCV.wait(lock, [] { return g_rootControl != nullptr; });
        }
    }
}

void cgame::InitConsole(uint32_t fps)
{
    g_targetFrameTimeUs = fps != 0 ? 1000000 / fps : 0;
    g_lastFpsTime = std::chrono::steady_clock::now();
    g_lastFrameTime = std::chrono::steady_clock::now();
    g_frameCount = 0;
    g_fps = 0;

    InitConsoleMode();

    // 设置代码页
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);

    g_hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (g_hOut == INVALID_HANDLE_VALUE)
    {
        throw std::runtime_error("Failed to get output handle");
    }
    g_graphicsOut = new ccontrol::Graphics(g_hOut);

    // 获取窗口大小
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(g_hOut, &csbi);
    g_size = csbi.dwSize;

    // 创建缓冲区
    g_hBuffer = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, nullptr, CONSOLE_TEXTMODE_BUFFER, nullptr);
    if (g_hBuffer == INVALID_HANDLE_VALUE)
    {
        throw std::runtime_error("Failed to create buffer");
    }
    g_graphicsBuffer = new ccontrol::Graphics(g_hBuffer);

    // 关闭光标
    HideCursor(g_hOut);
    HideCursor(g_hBuffer);

    // 设置后台缓冲区大小
    SetConsoleScreenBufferSize(g_hBuffer, g_size);

    // 缓存属性
    g_attr = csbi.wAttributes;

    // 注册游戏循环线程
    std::thread eventThread(GameLoopThread);
    eventThread.detach();
}

void cgame::DestroyConsole()
{
    if (g_hBuffer) CloseHandle(g_hBuffer);
    if (g_hOut) CloseHandle(g_hOut);
    g_hBuffer = nullptr;
    g_hOut = nullptr;

    delete g_graphicsOut;
    delete g_graphicsBuffer;
    g_graphicsOut = nullptr;
    g_graphicsBuffer = nullptr;
}

short cgame::GetConsoleWidth()
{
    return g_size.X;
}

short cgame::GetConsoleHeight()
{
    return g_size.Y;
}

WORD cgame::GetConsoleAttribute()
{
    return g_attr;
}

void cgame::SetConsoleRootControl(ccontrol::Control* control)
{
    std::lock_guard lock(g_rootControlMutex);
    g_rootControl = control;
    g_rootControlCV.notify_one();
}

ccontrol::Control* cgame::GetConsoleRootControl()
{
    return g_rootControl;
}

void cgame::SetConsoleBgColor(uint32_t color)
{
    g_attr = g_attr & 0x00FF | color << 4;
    SetConsoleTextAttribute(g_hOut, g_attr);
}

void cgame::SetConsoleFgColor(uint32_t color)
{
    g_attr = g_attr & 0xFF00 | color;
    SetConsoleTextAttribute(g_hOut, g_attr);
}

// 交换缓冲区
void SwapBuffer()
{
    SetConsoleActiveScreenBuffer(g_hBuffer);
    std::swap(g_hOut, g_hBuffer);
    std::swap(g_graphicsOut, g_graphicsBuffer);
}

// 绘制
void cgame::Draw()
{
    if (g_rootControl == nullptr) return;
    g_rootControl->Draw(g_graphicsBuffer);
}

// 渲染绘制
void cgame::Render()
{
    SwapBuffer();
}

// 清理后台缓冲区
void cgame::ClearBuffer()
{
    DWORD size = g_size.X * g_size.Y;
    DWORD written;

    FillConsoleOutputCharacter(
        g_hBuffer,
        ' ',
        size,
        {0, 0},
        &written
    );

    FillConsoleOutputAttribute(
        g_hBuffer,
        g_attr,
        size,
        {0, 0},
        &written
    );
}

void cgame::RenderAndClearBuffer()
{
    Render();
    ClearBuffer();
}

uint8_t cgame::GetFps()
{
    return g_fps;
}

// 重置缓冲区
void OnResize(short width, short height)
{
    g_size = {width, height};
    CloseHandle(g_hBuffer);
    g_hBuffer = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, nullptr, CONSOLE_TEXTMODE_BUFFER, nullptr);

    SetConsoleScreenBufferSize(g_hOut, g_size);
    SetConsoleScreenBufferSize(g_hBuffer, g_size);

    cgame::ClearBuffer();
    HideCursor(g_hBuffer);
    SwapBuffer();
}
