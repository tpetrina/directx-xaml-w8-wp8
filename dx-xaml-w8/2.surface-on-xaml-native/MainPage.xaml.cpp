//
// MainPage.xaml.cpp
// Implementation of the MainPage class.
//

#include "pch.h"
#include "MainPage.xaml.h"

using namespace _2_surface_on_xaml_native;

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

MainPage::MainPage()
{
	InitializeComponent();
}

/// <summary>
/// Invoked when this page is about to be displayed in a Frame.
/// </summary>
/// <param name="e">Event data that describes how this page was reached.  The Parameter
/// property is typically used to configure the page.</param>
void MainPage::OnNavigatedTo(NavigationEventArgs^ e)
{
	(void) e;	// Unused parameter

	auto imageBrush = ref new ImageBrush();
	auto _d2dComponent = ref new _2_sis_component::D2DComponent(300, 300, true);
	imageBrush->ImageSource = _d2dComponent;
	rectangle->Fill = imageBrush;

	_d2dComponent->BeginDraw();
	_d2dComponent->Clear(Windows::UI::Colors::Bisque);

	for (int i = 0; i < 20; ++i)
	{
		Windows::UI::Color color;
		color.R = rand() % 255 + 1;
		color.G = rand() % 255 + 1;
		color.B = rand() % 255 + 1;
		_d2dComponent->FillRectangle(
			rand() % 251,
			rand() % 251,
			50, 50,
			color);
	}

	_d2dComponent->EndDraw();
}
