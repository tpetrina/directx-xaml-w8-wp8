#pragma once

namespace _1_dx_component
{
	[Windows::Foundation::Metadata::WebHostHidden]
    public ref class DrawingSurfaceManipulationHandler sealed : public Windows::Phone::Input::Interop::IDrawingSurfaceManipulationHandler
	{
    public:
        DrawingSurfaceManipulationHandler();

		virtual void SetManipulationHost(Windows::Phone::Input::Interop::DrawingSurfaceManipulationHost^ manipulationHost);
    };
}