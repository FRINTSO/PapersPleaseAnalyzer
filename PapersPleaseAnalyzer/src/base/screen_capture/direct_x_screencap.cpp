#include "pch.h"
#include "direct_x_screencap.h"

#include <wrl/client.h>
#include <dxgi1_2.h>
#include <d3d11.h>
#include <opencv2/opencv.hpp>
#include <iostream>

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")

using Microsoft::WRL::ComPtr;

cv::Mat CaptureScreen()
{
    HRESULT hr;

    // Create D3D11 device
    ComPtr<ID3D11Device> d3dDevice;
    ComPtr<ID3D11DeviceContext> d3dContext;
    D3D_FEATURE_LEVEL featureLevel;

    hr = D3D11CreateDevice(
        nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0,
        nullptr, 0, D3D11_SDK_VERSION,
        &d3dDevice, &featureLevel, &d3dContext
    );

    if (FAILED(hr))
    {
        std::cerr << "Failed to create D3D11 device.\n";
        return {};
    }

    // Get DXGI device and output
    ComPtr<IDXGIDevice> dxgiDevice;
    hr = d3dDevice.As(&dxgiDevice);
    if (FAILED(hr)) return {};

    ComPtr<IDXGIAdapter> dxgiAdapter;
    dxgiDevice->GetAdapter(&dxgiAdapter);

    ComPtr<IDXGIOutput> dxgiOutput;
    dxgiAdapter->EnumOutputs(0, &dxgiOutput);

    ComPtr<IDXGIOutput1> dxgiOutput1;
    dxgiOutput.As(&dxgiOutput1);

    // Create duplication
    ComPtr<IDXGIOutputDuplication> deskDupl;
    hr = dxgiOutput1->DuplicateOutput(d3dDevice.Get(), &deskDupl);
    if (FAILED(hr))
    {
        std::cerr << "Failed to duplicate output. Try running as admin or ensure no protected content is active.\n";
        return {};
    }

    DXGI_OUTDUPL_FRAME_INFO frameInfo;
    ComPtr<IDXGIResource> desktopResource;

    hr = deskDupl->AcquireNextFrame(500, &frameInfo, &desktopResource);
    if (FAILED(hr))
    {
        std::cerr << "Failed to acquire next frame.\n";
        return {};
    }

    // Get texture
    ComPtr<ID3D11Texture2D> acquiredTex;
    desktopResource.As(&acquiredTex);

    D3D11_TEXTURE2D_DESC texDesc;
    acquiredTex->GetDesc(&texDesc);

    // Create staging texture for CPU read
    D3D11_TEXTURE2D_DESC cpuTexDesc = texDesc;
    cpuTexDesc.Usage = D3D11_USAGE_STAGING;
    cpuTexDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
    cpuTexDesc.BindFlags = 0;
    cpuTexDesc.MiscFlags = 0;

    ComPtr<ID3D11Texture2D> cpuTex;
    hr = d3dDevice->CreateTexture2D(&cpuTexDesc, nullptr, &cpuTex);
    if (FAILED(hr)) return {};

    d3dContext->CopyResource(cpuTex.Get(), acquiredTex.Get());

    // Map the resource to get data
    D3D11_MAPPED_SUBRESOURCE mapped;
    hr = d3dContext->Map(cpuTex.Get(), 0, D3D11_MAP_READ, 0, &mapped);
    if (FAILED(hr)) return {};

    // Create cv::Mat
    int width = texDesc.Width;
    int height = texDesc.Height;
    int stride = mapped.RowPitch;

    cv::Mat mat(height, width, CV_8UC4);
    memcpy(mat.data, mapped.pData, height * stride);

    d3dContext->Unmap(cpuTex.Get(), 0);
    deskDupl->ReleaseFrame();

    return mat;
}
