#include "pch.h"

#include "SurfaceImageSourceWrapper.h"

#pragma comment(lib, "D3D11.lib")
#pragma comment(lib, "d2d1.lib")

using namespace _3_sis_wrapper;
using namespace Platform;
using namespace DX;

SurfaceImageSourceWrapper::SurfaceImageSourceWrapper(int pixelWidth, int pixelHeight, bool isOpaque)
	: SurfaceImageSource(pixelWidth, pixelHeight, isOpaque)
	, _width(pixelWidth)
	, _height(pixelHeight)
{
	CreateDeviceIndependentResources();
	CreateDeviceResources();
}

void SurfaceImageSourceWrapper::CreateDeviceIndependentResources()
{
	IInspectable* sisInspectable = (IInspectable*) reinterpret_cast<IInspectable*>(this);
	ThrowIfFailed(sisInspectable->QueryInterface(__uuidof(ISurfaceImageSourceNative), (void **)&_sisNative));
}

void SurfaceImageSourceWrapper::CreateDeviceResources()
{
	D3D_FEATURE_LEVEL featureLevels[] = 
	{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_1
	};

	UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

#if defined(_DEBUG)
	creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	ThrowIfFailed(D3D11CreateDevice(
		NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		creationFlags,
		featureLevels,
		ARRAYSIZE(featureLevels),
		D3D11_SDK_VERSION,
		&_d3dDevice,
		NULL,
		&_d3dContext
		));

	ThrowIfFailed(_d3dDevice.As(&_dxgiDevice));

	ThrowIfFailed(D2D1CreateDevice(_dxgiDevice.Get(), NULL, &_d2dDevice));
	ThrowIfFailed(_d2dDevice->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, &_d2dDeviceContext));

	SetDpi(Windows::Graphics::Display::DisplayProperties::LogicalDpi);

	_d2dDeviceContext->SetUnitMode(D2D1_UNIT_MODE_PIXELS);
	_d2dDeviceContext->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_GRAYSCALE);

	ThrowIfFailed(_sisNative->SetDevice(_dxgiDevice.Get()));
}

void SurfaceImageSourceWrapper::SetDpi(float dpi)
{
	_dpi = dpi;
	_d2dDeviceContext->SetDpi(_dpi, _dpi);
}

void SurfaceImageSourceWrapper::BeginDraw(Windows::Foundation::Rect updateRect)
{
	Microsoft::WRL::ComPtr<IDXGISurface> surface;
	POINT offset;
	RECT updateRectNative =
	{
		(LONG)updateRect.Left,
		(LONG)updateRect.Top,
		(LONG)updateRect.Right,
		(LONG)updateRect.Bottom
	};

	HRESULT beginDrawHR = _sisNative->BeginDraw(updateRectNative, &surface, &offset);
	if (beginDrawHR == DXGI_ERROR_DEVICE_REMOVED || beginDrawHR == DXGI_ERROR_DEVICE_RESET)
	{
		CreateDeviceResources();
		BeginDraw(updateRect);
	}
	else
	{
		ThrowIfFailed(beginDrawHR);
	}


	Microsoft::WRL::ComPtr<ID2D1Bitmap1> bitmap;
	ThrowIfFailed(_d2dDeviceContext->CreateBitmapFromDxgiSurface(surface.Get(), nullptr, &bitmap));
	_d2dDeviceContext->BeginDraw();
	_d2dDeviceContext->SetTarget(bitmap.Get());
}

void SurfaceImageSourceWrapper::EndDraw()
{
	ThrowIfFailed(_d2dDeviceContext->EndDraw());
	ThrowIfFailed(_sisNative->EndDraw());
}