using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace FindStr
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            var str = this.textBox1.Text;
            var path = this.textBox2.Text;

            List<string> list = new List<string>();

            this.label3.Text = "finding";
            this.Refresh();
            Walk(path, str, ref list);
            if (list.Count > 0)
            {
                for (int i = 0; i < list.Count; i++)
                {
                    this.label3.Text += "\r\n" + list[i];
                }
            }
            else
            {
                this.label3.Text = "not found";
            }
        }

        public void Walk(string path, string target, ref List<string> list)
        {
            if (path == null || target == null)
            {
                return;
            }
            DirectoryInfo di = new DirectoryInfo(path);
            FileInfo[] fis = di.GetFiles();
            for (int i = 0; i < fis.Length; i++)
            {
                string data = readFile(fis[i].FullName);
                if (data.IndexOf(target) > 0)
                {
                    list.Add(fis[i].FullName);
                }
            }
            DirectoryInfo[] dis = di.GetDirectories();
            for (int i = 0; i < dis.Length; i++)
            {
                Walk(dis[i].FullName, target, ref list);
            }
        }

        public string readFile(string path)
        {
            FileStream fs = new FileStream(path, FileMode.Open, FileAccess.Read);
            StreamReader sr = new StreamReader(fs, System.Text.Encoding.Default);
            String str = sr.ReadToEnd();
            
            sr.Close();
            fs.Close();
            return str;
        }
    }
}
