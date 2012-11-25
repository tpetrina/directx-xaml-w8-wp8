#pragma once

namespace _3_sis_wrapper
{
	public ref class SurfaceImageSourceWrapper sealed : Windows::UI::Xaml::Media::Imaging::SurfaceImageSource
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
		SurfaceImageSourceWrapper(int pixelWidth, int pixelHeight, bool isOpaque);

		void BeginDraw(Windows::Foundation::Rect updateRect);
		void BeginDraw()
		{
			BeginDraw(Windows::Foundation::Rect(0, 0, (float)_width, (float)_height));
		}
		void EndDraw();

		void SetDpi(float dpi);

		int GetDeviceContext() { return ((Platform::IntPtr)_d2dDeviceContext.Get()).ToInt32(); }
	};
}