#pragma once

#include <wrl/client.h>
#include <wrl/module.h>
#include <Windows.Phone.Graphics.Interop.h>
#include <DrawingSurfaceNative.h>

namespace _1_dx_component
{
	template <typename T>
	class DrawingSurfaceContentProvider :
		public Microsoft::WRL::RuntimeClass<
		Microsoft::WRL::RuntimeClassFlags<Microsoft::WRL::WinRtClassicComMix>,
		ABI::Windows::Phone::Graphics::Interop::IDrawingSurfaceContentProvider,
		IDrawingSurfaceContentProviderNative>
	{
	public:
		DrawingSurfaceContentProvider(T^ controller)
		{
			m_controller = controller;
			m_controller->RequestAdditionalFrame += ref new RequestAdditionalFrameHandler([=] ()
			{
				if (m_host)
					m_host->RequestAdditionalFrame();
			});

			m_controller->RecreateSynchronizedTexture += ref new RecreateSynchronizedTextureHandler([=] ()
			{
				if (m_host)
					m_host->CreateSynchronizedTexture(m_controller->GetTexture(), &m_synchronizedTexture);
			});
		}

	public:
		HRESULT STDMETHODCALLTYPE Connect(_In_ IDrawingSurfaceRuntimeHostNative* host)
		{
			m_host = host;
			return m_controller->Connect(host);
		}

		void STDMETHODCALLTYPE Disconnect()
		{
			m_controller->Disconnect();
			m_host = nullptr;
			m_synchronizedTexture = nullptr;
		}

		HRESULT STDMETHODCALLTYPE PrepareResources(_In_ const LARGE_INTEGER* presentTargetTime, _Out_ BOOL* contentDirty)
		{
			return m_controller->PrepareResources(presentTargetTime, contentDirty);
		}

		HRESULT STDMETHODCALLTYPE GetTexture(_In_ const DrawingSurfaceSizeF* size, _Out_ IDrawingSurfaceSynchronizedTextureNative** synchronizedTexture, _Out_ DrawingSurfaceRectF* textureSubRectangle)
		{
			HRESULT hr = S_OK;

			if (!m_synchronizedTexture)
				hr = m_host->CreateSynchronizedTexture(m_controller->GetTexture(), &m_synchronizedTexture);

			// Set output parameters.
			textureSubRectangle->left = 0.0f;
			textureSubRectangle->top = 0.0f;
			textureSubRectangle->right = static_cast<FLOAT>(size->width);
			textureSubRectangle->bottom = static_cast<FLOAT>(size->height);

			m_synchronizedTexture.CopyTo(synchronizedTexture);

			// Draw to the texture.
			if (SUCCEEDED(hr))
			{
				hr = m_synchronizedTexture->BeginDraw();

				if (SUCCEEDED(hr))
					hr = m_controller->GetTexture(size, synchronizedTexture, textureSubRectangle);

				m_synchronizedTexture->EndDraw();
			}

			return hr;
		}

	private:
		T^													m_controller;
		Microsoft::WRL::ComPtr<IDrawingSurfaceRuntimeHostNative>			m_host;
		Microsoft::WRL::ComPtr<IDrawingSurfaceSynchronizedTextureNative>	m_synchronizedTexture;
	};
}