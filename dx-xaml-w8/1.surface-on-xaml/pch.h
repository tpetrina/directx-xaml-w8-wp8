//
// pch.h
// Header for standard system include files.
//

#pragma once

#include <collection.h>
#include "App.xaml.h"

#include <D3D11.h>
#include <d2d1_1.h>
#include <windows.ui.xaml.media.dxinterop.h>
 
// Helper utilities to make DX APIs work with exceptions in the samples apps.
namespace DX
{
    inline void ThrowIfFailed(HRESULT hr)
    {
        if (FAILED(hr))
        {
            // Set a breakpoint on this line to catch DX API errors.
            throw Platform::Exception::CreateException(hr);
        }
    }
}