using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Navigation;
using Microsoft.Phone.Controls;
using Microsoft.Phone.Shell;
using PhoneXamlDirect3DAppNavigationProblemComp;

namespace PhoneXamlDirect3DAppNavigationProblem
{
    public partial class MainPage : PhoneApplicationPage
    {
        private Direct3DInterop m_d3dInterop = new Direct3DInterop();
        private object m_d3dContentProvider = null;

        // Constructor
        public MainPage()
        {
            InitializeComponent();
        }

        private void DrawingSurface_Loaded(object sender, RoutedEventArgs e)
        {
            // Set window bounds in dips
            m_d3dInterop.WindowBounds = new Windows.Foundation.Size(
                (float)DrawingSurface.ActualWidth,
                (float)DrawingSurface.ActualHeight
                );

            // Set native resolution in pixels
            m_d3dInterop.NativeResolution = new Windows.Foundation.Size(
                (float)Math.Floor(DrawingSurface.ActualWidth * Application.Current.Host.Content.ScaleFactor / 100.0f + 0.5f),
                (float)Math.Floor(DrawingSurface.ActualHeight * Application.Current.Host.Content.ScaleFactor / 100.0f + 0.5f)
                );

            // Set render resolution to the full native resolution
            m_d3dInterop.RenderResolution = m_d3dInterop.NativeResolution;

            // Hook-up native component to DrawingSurface
            if (m_d3dContentProvider == null)
                m_d3dContentProvider = m_d3dInterop.CreateContentProvider();
            DrawingSurface.SetContentProvider(m_d3dContentProvider);
            DrawingSurface.SetManipulationHandler(m_d3dInterop);
        }

        private void _btnHighScores_Click(object sender, RoutedEventArgs e)
        {
            NavigationService.Navigate(new Uri("/HighScoresPage.xaml", UriKind.Relative));
        }
    }
}