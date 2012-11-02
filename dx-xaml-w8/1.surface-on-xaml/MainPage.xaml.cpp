//
// MainPage.xaml.cpp
// Implementation of the MainPage class.
//

#include "pch.h"
#include "MainPage.xaml.h"

using namespace _1_surface_on_xaml;

using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;

// The Blank Page item template is documented at http://go.microsoft.com/fwlink/?LinkId=234238

#pragma comment(lib, "D3D11.lib")
#pragma comment(lib, "d2d1.lib")

MainPage::MainPage()
{
	InitializeComponent();

	CreateDeviceResources();
	CreateDeviceIndependentResources();

	DX::ThrowIfFailed(
		m_sisNative->SetDevice(m_dxgiDevice.Get())
		);

	Draw();
}

/// <summary>
/// Invoked when this page is about to be displayed in a Frame.
/// </summary>
/// <param name="e">Event data that describes how this page was reached.  The Parameter
/// property is typically used to configure the page.</param>
void MainPage::OnNavigatedTo(NavigationEventArgs^ e)
{
	(void) e;	// Unused parameter
}

void MainPage::CreateDeviceIndependentResources()
{
	using namespace Windows::UI::Xaml::Media::Imaging;
	SurfaceImageSource^ surfaceImageSource = ref new SurfaceImageSource(400, 300);

	// extract backing native interface
	IInspectable* sisInspectable = (IInspectable*) reinterpret_cast<IInspectable*>(surfaceImageSource);
	sisInspectable->QueryInterface(__uuidof(ISurfaceImageSourceNative), (void **)&m_sisNative); 

	ImageBrush^ brush = ref new ImageBrush();
	brush->ImageSource = surfaceImageSource;
	ellipse->Fill = brush;
	rectangle->Fill = brush;
}

void MainPage::CreateDeviceResources()
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

#if defined(_DEBUG)flag.
	creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	DX::ThrowIfFailed(D3D11CreateDevice(
		NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		creationFlags,
		featureLevels,
		ARRAYSIZE(featureLevels),
		D3D11_SDK_VERSION,
		&m_d3dDevice,
		NULL,
		&m_d3dContext
		));

	DX::ThrowIfFailed(m_d3dDevice.As(&m_dxgiDevice));

	DX::ThrowIfFailed(
		D2D1CreateDevice(m_dxgiDevice.Get(), NULL, &m_d2dDevice)
		);
	DX::ThrowIfFailed(
		m_d2dDevice->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, &m_d2dDeviceContext)
		);

	m_d2dDeviceContext->SetUnitMode(D2D1_UNIT_MODE_PIXELS);
	float dpi = Windows::Graphics::Display::DisplayProperties::LogicalDpi;
	m_d2dDeviceContext->SetDpi(dpi, dpi);
	m_d2dDeviceContext->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_GRAYSCALE);
}

void MainPage::Draw()
{
	using namespace Microsoft::WRL;
	using namespace D2D1;
	using namespace DX;

	ComPtr<IDXGISurface> surface;
	RECT updateRect = {0,0,400,300};
	POINT offset;

	HRESULT beginDrawHR = m_sisNative->BeginDraw(updateRect, &surface, &offset);
	if (beginDrawHR == DXGI_ERROR_DEVICE_REMOVED || beginDrawHR == DXGI_ERROR_DEVICE_RESET)
	{
		// device changed
		// recreate device resources and try again
	}
	else
	{
		ThrowIfFailed(beginDrawHR);
	}

	ComPtr<ID2D1Bitmap1> bitmap;
	ThrowIfFailed(
		m_d2dDeviceContext->CreateBitmapFromDxgiSurface(surface.Get(), nullptr, &bitmap)
		);

	m_d2dDeviceContext->SetTarget(bitmap.Get());

	m_d2dDeviceContext->BeginDraw();

	m_d2dDeviceContext->Clear(ColorF(ColorF::Green));

	for (int i = 0; i < 25; ++i)
	{
		ComPtr<ID2D1SolidColorBrush> brush;
		Windows::UI::Color color;
		color.R = rand() % 255 + 1;
		color.G = rand() % 255 + 1;
		color.B = rand() % 255 + 1;
		ThrowIfFailed(m_d2dDeviceContext->CreateSolidColorBrush(ColorF(color.R / 255.0, color.G / 255.0, color.B / 255.0, 1.0), &brush));

		int x = rand() % 350;
		int y = rand() % 250;
		m_d2dDeviceContext->FillRectangle(RectF(x, y, x + 50, y + 50), brush.Get());
	}

	m_d2dDeviceContext->EndDraw();

	m_sisNative->EndDraw();
}