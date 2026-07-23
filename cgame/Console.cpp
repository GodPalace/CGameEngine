#include "Console.h"

#include "mutex"
#include "Screen.h"
#include "thread"
#include "windows.h"
#include "stdexcept"
#include "../cevents/EventBus.h"
#include "../cevents/Events.h"
#include "../cgutils/Logger.h"

// 游戏帧率延迟
long long g_fpsDelay;

// 游戏实际帧率
uint8_t g_fps;

// 帧率计算相关变量
std::chrono::steady_clock::time_point g_lastFpsTime;
unsigned long long g_frameCount = 0;

HANDLE g_hOut = nullptr, g_hBuffer = nullptr;
COORD g_size;
WORD g_attr;

cgame::Screen* g_screen = nullptr;
std::condition_variable g_screenCV;
std::mutex g_screenMutex;

void InitConsoleMode()
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

void HideCursor(HANDLE handle)
{
    CONSOLE_CURSOR_INFO ci;
    GetConsoleCursorInfo(handle, &ci);
    ci.bVisible = FALSE;
    SetConsoleCursorInfo(handle, &ci);
}

[[noreturn]]
void GameLoopThread()
{
    while (true)
    {
        if (g_screen != nullptr)
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
        }
        else
        {
            // 等待屏幕被设置
            std::unique_lock lock(g_screenMutex);
            g_screenCV.wait(lock, [] { return g_screen != nullptr; });
        }

        // 控制帧率
        std::this_thread::sleep_for(std::chrono::milliseconds(g_fpsDelay));
    }
}

void cgame::InitConsole(int fps)
{
    g_fpsDelay = fps == 0 ? 0 : static_cast<long long>(1000 / fps);
    g_lastFpsTime = std::chrono::steady_clock::now();
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

void cgame::SetScreen(Screen* screen)
{
    std::lock_guard lock(g_screenMutex);
    g_screen = screen;
    g_screenCV.notify_one();
}

cgame::Screen* cgame::GetScreen()
{
    return g_screen;
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
}

// 绘制
void cgame::Draw()
{
    if (g_screen == nullptr) return;
    g_screen->Draw(g_hBuffer);
}

// 渲染绘制
void cgame::Render()
{
    SwapBuffer();
}

// 清理后台缓冲区
void cgame::ClearBuffer()
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(g_hBuffer, &csbi);

    DWORD bufSize = csbi.dwSize.X * csbi.dwSize.Y;
    DWORD written;

    FillConsoleOutputCharacter(
        g_hBuffer,
        ' ',
        bufSize,
        {0, 0},
        &written
    );

    FillConsoleOutputAttribute(
        g_hBuffer,
        csbi.wAttributes,
        bufSize,
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
