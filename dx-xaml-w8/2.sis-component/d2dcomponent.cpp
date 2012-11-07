#include "pch.h"

#include "d2dcomponent.h"

#pragma comment(lib, "D3D11.lib")
#pragma comment(lib, "d2d1.lib")

using namespace _2_sis_component;
using namespace Platform;
using namespace DX;

D2DComponent::D2DComponent(int pixelWidth, int pixelHeight, bool isOpaque)
	: SurfaceImageSource(pixelWidth, pixelHeight, isOpaque)
	, _width(pixelWidth)
	, _height(pixelHeight)
{
	CreateDeviceIndependentResources();
	CreateDeviceResources();
}

void D2DComponent::CreateDeviceIndependentResources()
{
	IInspectable* sisInspectable = (IInspectable*) reinterpret_cast<IInspectable*>(this);
	ThrowIfFailed(sisInspectable->QueryInterface(__uuidof(ISurfaceImageSourceNative), (void **)&_sisNative));
}

void D2DComponent::CreateDeviceResources()
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

void D2DComponent::SetDpi(float dpi)
{
	_dpi = dpi;
	_d2dDeviceContext->SetDpi(_dpi, _dpi);
}

void D2DComponent::BeginDraw(Windows::Foundation::Rect updateRect)
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

void D2DComponent::EndDraw()
{
	ThrowIfFailed(_d2dDeviceContext->EndDraw());
	ThrowIfFailed(_sisNative->EndDraw());
}

D2D1::ColorF ToNativeColor(Windows::UI::Color color)
{
	return D2D1::ColorF(color.R / 255.0f, color.G / 255.0f, color.B / 255.0f);
}

void D2DComponent::Clear(Windows::UI::Color color)
{
	D2D1::ColorF colorNative(color.R / 255.0f, color.G / 255.0f, color.B / 255.0f);
	_d2dDeviceContext->Clear(colorNative);
}

void D2DComponent::FillRectangle(int x, int y, int width, int height, Windows::UI::Color color)
{
	Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> brush;
	_d2dDeviceContext->CreateSolidColorBrush(ToNativeColor(color), &brush);
	_d2dDeviceContext->FillRectangle(D2D1::RectF(x, y, x + width, y + height), brush.Get());
}