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
using System.Windows.Shapes;

namespace MinesweeperTest
{
    /// <summary>
    /// Lógica de interacción para Menu.xaml
    /// </summary>
    public partial class Menu : Window
    {
        public Menu()
        {
            InitializeComponent();

            Random rng = new Random();
            int width = rng.Next(9, 51);
            int height = rng.Next(9, 31);
            int mine = rng.Next(1, width * height / 2);

            txtMines.Text = mine.ToString();
            txtWidth.Text = width.ToString();
            txtHeight.Text = height.ToString();
        }

        private void button_Click(object sender, RoutedEventArgs e)
        {
            Minesweeper p = new Minesweeper(10, 9, 9);
            p.Show();
            Close();

        }

        private void button1_Click(object sender, RoutedEventArgs e)
        {
            Minesweeper p = new Minesweeper(40, 16, 16);
            p.Show();
            Close();

        }

        private void button2_Click(object sender, RoutedEventArgs e)
        {
            Minesweeper p = new Minesweeper(99, 30, 16);
            p.Show();
            Close();
        }

        private void Window_Closing(object sender, System.ComponentModel.CancelEventArgs e)
        {

        }

        private void btnCustom_Click(object sender, RoutedEventArgs e)
        {
            int mines;
            int width;
            int height;

            bool mineParsed;
            bool widthParsed;
            bool heightParsed;
            bool parsed;

            mineParsed = int.TryParse(txtMines.Text, out mines);
            widthParsed = int.TryParse(txtWidth.Text, out width);
            heightParsed = int.TryParse(txtHeight.Text, out height);
            parsed = mineParsed && widthParsed && heightParsed;

            string errors = "Errors: \n";

            if (!mineParsed)
            {
                errors += "Invalid mines \n";
                txtMines.Clear();
            }
            if (!widthParsed)
            {
                errors += "Invalid width \n";
                txtWidth.Clear();
            }
            if (!heightParsed)
            {
                errors += "Invalid height \n";
                txtHeight.Clear();
            }

            if (parsed)
            {
                if (mines < height * width && mines > 0)
                {
                    if (height >= 9 && height <= 30 && width >= 9 && width <= 50)
                    {
                        Minesweeper p = new Minesweeper(mines, width, height);
                        p.Show();
                        Close();
                    }
                    else
                    {
                        MessageBox.Show("The valid width and height are specified in parenthesis.");
                    }

                }
                else
                {
                    MessageBox.Show("Number of mines must be between 1 and " + (width * height - 1).ToString() + " given the width and height described.");
                }
            }
            else
            {
                MessageBox.Show(errors);
            }

        }
    }
}
