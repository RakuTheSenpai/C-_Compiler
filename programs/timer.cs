using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Windows.Threading;

namespace MinesweeperTest
{
    public partial class Timer : UserControl
    {
        DispatcherTimer timer = new DispatcherTimer();
        Label lblDescription = new Label();
        Label lblTime = new Label();
        public int Time { get; set; }

        public Timer()
        {
            InitializeComponent();
            timer.Interval = TimeSpan.FromSeconds(1);
            timer.Tick += Timer_Tick;
            Width = 70;
            Height = 48;
            Time = 0;
        }

        private void Timer_Tick(object sender, EventArgs e)
        {
            Time++;
            lblTime.Content = Time.ToString();
        }

        private void elCanvas_Loaded(object sender, RoutedEventArgs e)
        {
            lblDescription.Height = elCanvas.Height / 4;
            lblDescription.Width = elCanvas.Width;
            lblDescription.Foreground = new SolidColorBrush(Colors.Black);
            lblDescription.Content = "Time:";
            elCanvas.Children.Add(lblDescription);
            Canvas.SetLeft(lblDescription, 0);
            Canvas.SetTop(lblDescription, 0);

            lblTime.Height = elCanvas.Height - lblDescription.Height;
            lblTime.Width = elCanvas.Width;
            lblTime.Foreground = new SolidColorBrush(Colors.Black);
            lblTime.FontSize = 30;
            lblTime.Content = Time.ToString();
            elCanvas.Children.Add(lblTime);
            Canvas.SetLeft(lblTime, 0);
            Canvas.SetTop(lblTime, 12);

        }

        public void Start()
        {
            timer.Start();
        }

        public void Stop()
        {
            timer.Stop();
        }
    }
}
