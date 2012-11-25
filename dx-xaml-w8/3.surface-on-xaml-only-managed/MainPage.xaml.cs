using _3_sis_wrapper;
using System;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Media;
using Windows.UI.Xaml.Navigation;

// The Blank Page item template is documented at http://go.microsoft.com/fwlink/?LinkId=234238

namespace _3.surface_on_xaml_only_managed
{
    /// <summary>
    /// An empty page that can be used on its own or navigated to within a Frame.
    /// </summary>
    public sealed partial class MainPage : Page
    {
        SurfaceImageSourceWrapper _sisWrapper = new SurfaceImageSourceWrapper(300, 300, true);

        public MainPage()
        {
            this.InitializeComponent();
        }

        /// <summary>
        /// Invoked when this page is about to be displayed in a Frame.
        /// </summary>
        /// <param name="e">Event data that describes how this page was reached.  The Parameter
        /// property is typically used to configure the page.</param>
        protected override void OnNavigatedTo(NavigationEventArgs e)
        {
            var imageBrush = new ImageBrush();
            imageBrush.ImageSource = _sisWrapper;
            rectangle.Fill = imageBrush;

            _sisWrapper.BeginDraw();

            var context = new SharpDX.Direct2D1.DeviceContext(new IntPtr(_sisWrapper.GetDeviceContext()));
            context.Clear(new SharpDX.Color4(0.4f, 0.3f, 0.15f, 1.0f));

            var random = new Random();
            for (int i = 0; i < 20; ++i)
            {
                var brush = new SharpDX.Direct2D1.SolidColorBrush(context, new SharpDX.Color4(
                    (float)random.NextDouble(),
                    (float)random.NextDouble(),
                    (float)random.NextDouble(),
                    1.0f));

                var left = (float)(250.0f * random.NextDouble());
                var top = (float)(250.0f * random.NextDouble());
                context.FillRectangle(new SharpDX.RectangleF(
                    left, top, left + 50.0f, top + 50.0f),
                    brush);
            }

            _sisWrapper.EndDraw();
        }
    }
}
