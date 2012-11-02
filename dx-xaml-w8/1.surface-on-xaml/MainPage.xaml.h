//
// MainPage.xaml.h
// Declaration of the MainPage class.
//

#pragma once

#include "MainPage.g.h"

namespace _1_surface_on_xaml
{
	/// <summary>
	/// An empty page that can be used on its own or navigated to within a Frame.
	/// </summary>
	public ref class MainPage sealed
	{
	public:
		MainPage();

	protected:
		virtual void OnNavigatedTo(Windows::UI::Xaml::Navigation::NavigationEventArgs^ e) override;

	private:
		Microsoft::WRL::ComPtr<ISurfaceImageSourceNative> m_sisNative;

		Microsoft::WRL::ComPtr<ID3D11Device>              m_d3dDevice;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext>       m_d3dContext;
		Microsoft::WRL::ComPtr<IDXGIDevice>               m_dxgiDevice;

		Microsoft::WRL::ComPtr<ID2D1Device>               m_d2dDevice;
		Microsoft::WRL::ComPtr<ID2D1DeviceContext>        m_d2dDeviceContext;

		void CreateDeviceIndependentResources();
		void CreateDeviceResources();
		void Draw();
	};
}
