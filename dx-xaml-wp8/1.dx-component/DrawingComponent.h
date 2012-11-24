#pragma once

namespace _1_dx_component
{
	public delegate void RequestAdditionalFrameHandler();
	public delegate void RecreateSynchronizedTextureHandler();

	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class DrawingComponent sealed
	{
		// Content provider
	public:
		Windows::Phone::Graphics::Interop::IDrawingSurfaceContentProvider^ GetContentHelper(); 

		event RequestAdditionalFrameHandler^ RequestAdditionalFrame;
		event RecreateSynchronizedTextureHandler^ RecreateSynchronizedTexture;

		property Windows::Foundation::Size WindowBounds;
		property Windows::Foundation::Size RenderResolution;

	internal:
		HRESULT STDMETHODCALLTYPE Connect(_In_ IDrawingSurfaceRuntimeHostNative* host);
		void STDMETHODCALLTYPE Disconnect();
		HRESULT STDMETHODCALLTYPE PrepareResources(_In_ const LARGE_INTEGER* presentTargetTime, _Out_ BOOL* contentDirty);
		HRESULT STDMETHODCALLTYPE GetTexture(_In_ const DrawingSurfaceSizeF* size, _Out_ IDrawingSurfaceSynchronizedTextureNative** synchronizedTexture, _Out_ DrawingSurfaceRectF* textureSubRectangle);

		ID3D11Texture2D* GetTexture() {return m_renderTarget.Get(); }

	private:
		void CreateDeviceResources();
		void CreateWindowSizeDependentResources();
		void Render();

		// Direct3D Objects.
		Microsoft::WRL::ComPtr<ID3D11Device1> m_d3dDevice;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext1> m_d3dContext;
		Microsoft::WRL::ComPtr<ID3D11Texture2D> m_renderTarget;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_renderTargetView;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_depthStencilView;

		// Cached renderer properties.
		D3D_FEATURE_LEVEL m_featureLevel;
		Windows::Foundation::Size m_renderTargetSize;
		Windows::Foundation::Rect m_windowBounds;
	};
};