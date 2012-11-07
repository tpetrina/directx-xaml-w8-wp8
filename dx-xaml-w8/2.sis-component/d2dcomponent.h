#pragma once

namespace _2_sis_component
{
	
	public ref class D2DComponent sealed : Windows::UI::Xaml::Media::Imaging::SurfaceImageSource
    {
	private protected:
		Microsoft::WRL::ComPtr<ISurfaceImageSourceNative>	_sisNative;
		
		Microsoft::WRL::ComPtr<ID3D11Device>				_d3dDevice;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext>			_d3dContext;
		Microsoft::WRL::ComPtr<IDXGIDevice>					_dxgiDevice;
	
		Microsoft::WRL::ComPtr<ID2D1Device>					_d2dDevice;
		Microsoft::WRL::ComPtr<ID2D1DeviceContext>			_d2dDeviceContext;

		int		_height;
		int		_width;
		float	_dpi;

		void CreateDeviceIndependentResources();
		void CreateDeviceResources();

    public:
		D2DComponent(int pixelWidth, int pixelHeight, bool isOpaque);

		void BeginDraw(Windows::Foundation::Rect updateRect);
		void BeginDraw()
		{
			BeginDraw(Windows::Foundation::Rect(0, 0, (float)_width, (float)_height));
		}
		void EndDraw();

		void FillRectangle(int x, int y, int width, int height, Windows::UI::Color color);

		void SetDpi(float dpi);

		void Clear(Windows::UI::Color color);
    };
}