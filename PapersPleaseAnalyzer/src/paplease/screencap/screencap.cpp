#include "pch.h"
#include "paplease/screencap/screencap.h"

#include <Windows.h>

namespace paplease {
    namespace screencap {

        static cv::Mat GetMat(HWND hWND)
        {
            HDC deviceContext = GetDC(hWND);
            HDC memoryDeviceContext = CreateCompatibleDC(deviceContext);

            RECT windowRect;
            GetClientRect(hWND, &windowRect);

            int height = windowRect.bottom;
            int width = windowRect.right;

            HBITMAP bitmap = CreateCompatibleBitmap(deviceContext, width, height);

            SelectObject(memoryDeviceContext, bitmap);

            // copy data into bitmap
            BitBlt(memoryDeviceContext, 0, 0, width, height, deviceContext, 0, 0, SRCCOPY);

            // specify format by using bitmapinfoheader!
            BITMAPINFOHEADER bi;
            bi.biSize = sizeof(BITMAPINFOHEADER);
            bi.biWidth = width;
            bi.biHeight = -height;
            bi.biPlanes = 1;
            bi.biBitCount = 24;
            bi.biCompression = BI_RGB;
            bi.biSizeImage = 0; // because no compression
            //bi.biXPelsPerMeter = 1;
            //bi.biYPelsPerMeter = 2;
            //bi.biClrUsed = 3;
            //bi.biClrImportant = 4;

            cv::Mat mat = cv::Mat(height, width, CV_8UC3);

            // transform data
            GetDIBits(memoryDeviceContext, bitmap, 0, height, mat.data, (BITMAPINFO*)&bi, DIB_RGB_COLORS);

            // clean up!
            DeleteObject(bitmap);
            DeleteDC(memoryDeviceContext);
            ReleaseDC(hWND, deviceContext);

            return mat;
        }

        static HWND GetGameWindowHandle()
        {
            auto windowTitle = TEXT("PapersPlease");
            HWND hWND = FindWindow(NULL, windowTitle);
            while (!hWND)
            {
                std::system("cls");
                std::cout << "Start the game..." << std::endl;
                hWND = FindWindow(NULL, windowTitle);
                Sleep(100);
            }
            return hWND;
        }

        static bool IsWindowForeground(HWND hwnd)
        {
            return GetForegroundWindow() == hwnd;
        }

        static bool ResizeGameWindowIfNeeded(HWND hwnd)
        {
            constexpr int TargetWidth = 1142;
            constexpr int TargetHeight = 672;

            constexpr int WidthAddon = TargetWidth - 1128;
            constexpr int HeightAddon = TargetHeight - 665;

            BOOL success = ::SetWindowPos(hwnd, NULL, 0, 0, TargetWidth + WidthAddon, TargetHeight + HeightAddon, SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER);
            assert(success && "Failed to resize game window to 1142x672");
            return success;
        }

        cv::Mat CaptureGameWindow()
        {
            static HWND desktopWindow = GetDesktopWindow();
            HWND gameWindow = GetGameWindowHandle();
            
            static auto result = ResizeGameWindowIfNeeded(gameWindow);

            if (!IsWindowForeground(gameWindow))
            {
                std::cout << "Window is not active!\n";
                while (!IsWindowForeground(gameWindow));
            }

            static auto screenWidth = GetSystemMetrics(SM_CXSCREEN);
            static auto screenHeight = GetSystemMetrics(SM_CYSCREEN);

            RECT windowRect;
            if (GetWindowRect(gameWindow, &windowRect) && (windowRect.left < 0 || windowRect.top < 0 || windowRect.right > screenWidth || windowRect.bottom > screenHeight))
            {
                std::cout << "Window is outside\n";
                while (GetWindowRect(gameWindow, &windowRect) && (windowRect.left < 0 || windowRect.top < 0 || windowRect.right > screenWidth || windowRect.bottom > screenHeight));
            }

            windowRect.left += 7;
            windowRect.right -= 7;
            windowRect.bottom -= 7;
            
            auto windowMat = GetMat(desktopWindow);

            cv::Rect rect(windowRect.left, windowRect.top, windowRect.right - windowRect.left, windowRect.bottom - windowRect.top);

            auto finalWindow = windowMat(rect);

            return finalWindow;
        }

    }
}
