using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Drawing.Imaging;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace _2Bmp
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        public string path;
        public Point selectAreaPosAfterScale = new Point(0, 0);
        public Point selectAreaPos = new Point(0, 0);
        public Size selectAreaSize = new Size(100, 100);
        public bool isSelected = false;
        public Point prevPos = new Point(0, 0);

        private void button1_Click(object sender, EventArgs e)
        {
            OpenFileDialog Ofd = new OpenFileDialog();
            Ofd.AddExtension = true;
            Ofd.Filter = "Jpg(*.jpg)|*.jpg|png(*.png)|*.png|所有文件(*.*)|*.*";
            if (Ofd.ShowDialog() == System.Windows.Forms.DialogResult.OK)
            {
                Image image = Image.FromFile(Ofd.FileName);
                int min = Math.Min(image.Width, image.Height);
                path = Ofd.FileName;
                this.pictureBox1.Image = image;
                DrawSelectArea(selectAreaPos);
            }
            else
            {
                path = null;
            }
        }

        // start 是相对于图片原尺寸的
        private void DrawSelectArea(Point start)
        {
            this.pictureBox1.Refresh();
            Graphics g = this.pictureBox1.CreateGraphics();
            double scale = GetScaleVal();
            Size offset = GetMarginSize();
            Size scaledSize = GetScaledSize();

            int x = (int)Math.Round(start.X * scale, 0);
            int y = (int)Math.Round(start.Y * scale, 0);

            int width = (int)(selectAreaSize.Width * scale);
            int height = (int)(selectAreaSize.Height * scale);

            if (x < 0)
            {
                x = 0;
            }
            if (y < 0)
            {
                y = 0;
            }

            if (x + width > scaledSize.Width)
            {
                x = scaledSize.Width - width;
            }

            if (y + height > scaledSize.Height)
            {
                y = scaledSize.Height - height;
            }
            selectAreaPosAfterScale = new Point(x, y);
            selectAreaPos = start;
            g.DrawRectangle(Pens.White, offset.Width + x, offset.Height + y, width, height);
        }

        private bool IsInSelectArea(int x, int y)
        {
            Size offset = GetMarginSize();
            x -= offset.Width;
            y -= offset.Height;
            double scale = GetScaleVal();
            if (selectAreaPosAfterScale.X > x ||
                selectAreaPosAfterScale.Y > y ||
                selectAreaPosAfterScale.X + selectAreaSize.Width <= x ||
                selectAreaPosAfterScale.Y + selectAreaSize.Height <= y)
            {
                return false;
            }
            return true;
        }

        private Size GetScaledSize()
        {
            int originalWidth = this.pictureBox1.Image.Width;
            int originalHeight = this.pictureBox1.Image.Height;

            PropertyInfo rectangleProperty = this.pictureBox1.GetType().GetProperty("ImageRectangle", BindingFlags.Instance | BindingFlags.NonPublic);
            Rectangle rectangle = (Rectangle)rectangleProperty.GetValue(this.pictureBox1, null);

            int cw = rectangle.Width;
            int ch = rectangle.Height;
            return new Size(cw, ch);
        }

        private double GetScaleVal()
        {
            int originalWidth = this.pictureBox1.Image.Width;
            int originalHeight = this.pictureBox1.Image.Height;

            PropertyInfo rectangleProperty = this.pictureBox1.GetType().GetProperty("ImageRectangle", BindingFlags.Instance | BindingFlags.NonPublic);
            Rectangle rectangle = (Rectangle)rectangleProperty.GetValue(this.pictureBox1, null);

            int currentWidth = rectangle.Width;
            int currentHeight = rectangle.Height;

            double rate = (double)currentHeight / (double)originalHeight;
            return rate;
        }

        private Size GetMarginSize()
        {
            int originalWidth = this.pictureBox1.Image.Width;
            int originalHeight = this.pictureBox1.Image.Height;

            PropertyInfo rectangleProperty = this.pictureBox1.GetType().GetProperty("ImageRectangle", BindingFlags.Instance | BindingFlags.NonPublic);
            Rectangle rectangle = (Rectangle)rectangleProperty.GetValue(this.pictureBox1, null);

            int currentWidth = rectangle.Width;
            int currentHeight = rectangle.Height;

            double rate = (double)currentHeight / (double)originalHeight;

            int black_left_width = (currentWidth == this.pictureBox1.Width) ? 0 : (this.pictureBox1.Width - currentWidth) / 2;
            int black_top_height = (currentHeight == this.pictureBox1.Height) ? 0 : (this.pictureBox1.Height - currentHeight) / 2;
            
            return new Size(black_left_width, black_top_height);
        }

        private Point GetRelativePos(int ex, int ey)
        {
            int originalWidth = this.pictureBox1.Image.Width;
            int originalHeight = this.pictureBox1.Image.Height;

            PropertyInfo rectangleProperty = this.pictureBox1.GetType().GetProperty("ImageRectangle", BindingFlags.Instance | BindingFlags.NonPublic);
            Rectangle rectangle = (Rectangle)rectangleProperty.GetValue(this.pictureBox1, null);

            int currentWidth = rectangle.Width;
            int currentHeight = rectangle.Height;

            double rate = (double)currentHeight / (double)originalHeight;

            int black_left_width = (currentWidth == this.pictureBox1.Width) ? 0 : (this.pictureBox1.Width - currentWidth) / 2;
            int black_top_height = (currentHeight == this.pictureBox1.Height) ? 0 : (this.pictureBox1.Height - currentHeight) / 2;

            int zoom_x = ex - black_left_width;
            int zoom_y = ey - black_top_height;

            double original_x = (double)zoom_x / rate;
            double original_y = (double)zoom_y / rate;

            int x = (int)Math.Round(original_x, 0);
            int y = (int)Math.Round(original_y, 0);

            return new Point(x, y);
        }

        private void pictureBox1_MouseMove(object sender, MouseEventArgs e)
        {
            if (path == null || this.pictureBox1.Image == null) return;
            var p = GetRelativePos(e.X, e.Y);
            int x = p.X;
            int y = p.Y;
            this.label1.Text = "origin(" + x + ", " + y + ")";

            if (isSelected)
            {
                int dx = e.X - prevPos.X;
                int dy = e.Y - prevPos.Y;
                prevPos = new Point(e.X, e.Y);
                double r = GetScaleVal();
                DrawSelectArea(new Point((int)(dx / r) + selectAreaPos.X, (int)(dy / r) + selectAreaPos.Y));
            }
        }

        private void pictureBox1_MouseDown(object sender, MouseEventArgs e)
        {
            if (IsInSelectArea(e.X, e.Y))
            {
                isSelected = true;
                prevPos = new Point(e.X, e.Y);
            }
            DrawSelectArea(selectAreaPos);
        }

        private void pictureBox1_MouseUp(object sender, MouseEventArgs e)
        {
            isSelected = false;
        }

        private void button3_Click(object sender, EventArgs e)
        {
            Bitmap bmp = new Bitmap(selectAreaSize.Width, selectAreaSize.Height);
            Graphics g = Graphics.FromImage(bmp);
            Image img = Image.FromFile(path);
            g.DrawImage(img, 0, 0, new Rectangle(selectAreaPos.X, selectAreaPos.Y, selectAreaSize.Width, selectAreaSize.Height), GraphicsUnit.Pixel);
            g.Dispose();
            string fname = "pic-" + string.Format("{0}{1}{2}{3}{4}{5}", DateTime.Now.Year, DateTime.Now.Month, DateTime.Now.Day, DateTime.Now.Hour, DateTime.Now.Minute, DateTime.Now.Second) + ".bmp";
            bmp.Save(fname, ImageFormat.Bmp);
        }

        private void button2_Click(object sender, EventArgs e)
        {
            string s = this.textBox1.Text;
            int idx = s.IndexOfAny(new char[] { 'X', 'x' });
            string x = s.Substring(0, idx);
            string y = s.Substring(idx+1);
            selectAreaSize = new Size(int.Parse(x), int.Parse(y));
            DrawSelectArea(selectAreaPos);
        }
    }
}
