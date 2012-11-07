using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using Windows.Foundation;
using Windows.Foundation.Collections;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Controls.Primitives;
using Windows.UI.Xaml.Data;
using Windows.UI.Xaml.Input;
using Windows.UI.Xaml.Media;
using Windows.UI.Xaml.Navigation;

// The Blank Page item template is documented at http://go.microsoft.com/fwlink/?LinkId=234238

namespace _2.surface_on_xaml
{
    /// <summary>
    /// An empty page that can be used on its own or navigated to within a Frame.
    /// </summary>
    public sealed partial class MainPage : Page
    {
        private _2_sis_component.D2DComponent _d2dComponent;

        public MainPage()
        {
            this.InitializeComponent();

            _d2dComponent = new _2_sis_component.D2DComponent(300, 300, true);
        }

        /// <summary>
        /// Invoked when this page is about to be displayed in a Frame.
        /// </summary>
        /// <param name="e">Event data that describes how this page was reached.  The Parameter
        /// property is typically used to configure the page.</param>
        protected override void OnNavigatedTo(NavigationEventArgs e)
        {
            var imageBrush = new ImageBrush();
            imageBrush.ImageSource = _d2dComponent;
            rectangle.Fill = imageBrush;

            _d2dComponent.BeginDraw();
            _d2dComponent.Clear(Windows.UI.Colors.Bisque);
            _d2dComponent.FillRectangle(50, 50, 50, 50, Windows.UI.Colors.Aqua);
            _d2dComponent.EndDraw();
        }
    }
}
