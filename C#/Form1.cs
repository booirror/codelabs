using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Calculator
{
    public partial class Form1 : Form
    {
        TokenStream ts;
        public Form1()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            ts = new TokenStream(this.textBox1.Text);
            double d = expression();
            this.textBox2.Text = "" + d;
        }

        public double primary()
        {
            Token t = ts.Get();
            switch (t.Kind)
            {
                case '(':
                    double d = expression();
                    t = ts.Get();
                    return d;
                case '8':
                    return t.Value;
                default:
                    ts.PutBack(t);
                    return 0;
            }
        }

        public double term()
        {
            double left = primary();
            Token t = ts.Get();
            
            while (true)
            {
                if (t == null)
                {
                    return left;
                }
                switch (t.Kind)
                {
                    case '*':
                        left *= primary();
                        t = ts.Get();
                        break;
                    case '/':
                        left /= primary();
                        t = ts.Get();
                        break;
                    default:
                        ts.PutBack(t);
                        return left;
                }
            }
        }

        public double expression()
        {
            double left = term();
            Token t = ts.Get();
            
            while (true)
            {
                if (t == null)
                {
                    return left;
                }
                switch (t.Kind)
                {
                    case '+':
                        left += term();
                        t = ts.Get();
                        break;
                    case '-':
                        left -= term();
                        t = ts.Get();
                        break;
                    default:
                        ts.PutBack(t);
                        return left;
                }
            }
        }
    }
}
