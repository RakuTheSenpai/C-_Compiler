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
    public partial class BombCounter : UserControl
    {
        public int bombsLeft { get; set; }
        Label lblDescription = new Label();
        Label lblBombsLeft = new Label();

        public BombCounter(int bombs)
        {
            InitializeComponent();
            Width = 280 / 3;
            Height = 48;
            bombsLeft = bombs;
        }

        private void Canvas_Loaded(object sender, RoutedEventArgs e)
        {
            lblDescription.Height = elCanvas.Height / 4;
            lblDescription.Width = elCanvas.Width;
            lblDescription.Foreground = new SolidColorBrush(Colors.Black);
            lblDescription.Content = "Bombs left:";
            elCanvas.Children.Add(lblDescription);
            Canvas.SetLeft(lblDescription, 0);
            Canvas.SetTop(lblDescription, 0);

            lblBombsLeft.Height = elCanvas.Height - lblDescription.Height;
            lblBombsLeft.Width = elCanvas.Width;
            lblBombsLeft.Foreground = new SolidColorBrush(Colors.Black);
            lblBombsLeft.FontSize = 30;
            lblBombsLeft.Content = bombsLeft.ToString();
            elCanvas.Children.Add(lblBombsLeft);
            Canvas.SetLeft(lblBombsLeft, 0);
            Canvas.SetTop(lblBombsLeft, 12);

        }

        public void SetCounter(int bombsLeft)
        {
            this.bombsLeft = bombsLeft;
            lblBombsLeft.Content = this.bombsLeft.ToString();
        }
    }
}
