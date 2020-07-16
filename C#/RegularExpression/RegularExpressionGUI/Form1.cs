using Newtonsoft.Json.Linq;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Net;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace RegularExpressionGUI
{
    public enum ShowWindowCommands : int
    {

        SW_HIDE = 0,
        SW_SHOWNORMAL = 1,    //用最近的大小和位置显示，激活
        SW_NORMAL = 1,
        SW_SHOWMINIMIZED = 2,
        SW_SHOWMAXIMIZED = 3,
        SW_MAXIMIZE = 3,
        SW_SHOWNOACTIVATE = 4,
        SW_SHOW = 5,
        SW_MINIMIZE = 6,
        SW_SHOWMINNOACTIVE = 7,
        SW_SHOWNA = 8,
        SW_RESTORE = 9,
        SW_SHOWDEFAULT = 10,
        SW_MAX = 10
    }
    public partial class Form1 : Form
    {
        public static readonly string ZH = "zh";
        public static readonly string EN = "en";

        [DllImport("shell32.dll")]
        public static extern IntPtr ShellExecute(
            IntPtr hwnd,
            string lpszOp,
            string lpszFile,
            string lpszParams,
            string lpszDir,
            ShowWindowCommands FsShowCmd
            );
        public Form1()
        {
            InitializeComponent();
            this.textBox3.Text = @"^\\s*[^-][^-][^\r\n]*=\\s*\""(.*)\"".*$";
        }

        private void textBox2_TextChanged(object sender, EventArgs e)
        {

        }

        private void Form1_Resize(object sender, EventArgs e)
        {
            int width = this.Width;
            int btnwidth = 75;
            int textwidth = (width - btnwidth - 60*2) / 2;
            int height = this.Height;
            int textheight = height - 65 - 50;

            textBox1.Top = 65;
            textBox1.Left = 40;
            textBox1.Width = textwidth;
            textBox1.Height = textheight;

            this.translate.Top = textBox1.Top;
            this.translate.Left = textBox1.Left + textBox1.Width + 20;
            this.translate.Width = btnwidth;
            this.translate.Height = textheight;

            textBox2.Top = this.translate.Top;
            textBox2.Left = this.translate.Left + this.translate.Width + 20;
            textBox2.Width = textwidth;
            textBox2.Height = textheight;

            this.textBox3.Width = width - 80;
        }

        private string ToReExpression(string s)
        {
            Dictionary<char, char> map = new Dictionary<char, char>();
            map.Add('\\', '\\');
            map.Add('r', '\r');
            map.Add('n', '\n');
            map.Add('"', '\"');
            StringBuilder sb = new StringBuilder();
            for (int i = 0; i < s.Length; i++)
            {
                if (s[i] == '\\')
                {
                    sb.Append(map[s[i + 1]]);
                    i++;
                    continue;
                }
                sb.Append(s[i]);
            }
            return sb.ToString();
        }

        private void translate_Click(object sender, EventArgs e)
        {
            string tg = "^\\s*[^-][^-][^\r\n]*=\\s*\"(.*)\".*$";
            string reraw = this.textBox3.Text;
            string src = this.textBox1.Text;

            string re = ToReExpression(reraw);
            re = re.Trim();
            if (string.IsNullOrEmpty(src)) { return; }
            if (string.IsNullOrEmpty(re))
            {
                string dst = Translate(src, ZH, EN);
                this.textBox2.Text = dst;
            }
            else
            {
                string fileName = "temp.text";
                int idx = fileName.LastIndexOf('.');
                string resultName = fileName.Substring(0, idx) + "_x" + fileName.Substring(idx);

                StreamWriter writer = new StreamWriter(fileName);
                writer.Write(src);
                writer.Close();

                string dst = FileTranslateBatch(fileName, resultName, re);
                this.textBox2.Text = dst;
                showResult(fileName, resultName);
            }
        }

        public static string FileTranslateBatch(string fileName, string resultName, string re)
        {
            StreamReader reader = new StreamReader(fileName);
            string data = reader.ReadToEnd();
            reader.Close();

            var regex = new RegularExpressionEngine.Regex.Regex(re);

            var list = regex.Matches(data);
            List<string> strlist = new List<string>();
            foreach (var m in list)
            {
                strlist.Add(m.Captures[1].Value);
            }
            var translist = Translate(strlist, ZH, EN);
            int start = 0;
            int end = 0;
            StringBuilder sbb = new StringBuilder();
            for (int i = 0; i < list.Count; i++)
            {
                end = list[i].Captures[1].Index;
                sbb.Append(data.Substring(start, end - start));
                sbb.Append(translist[i]);
                start = list[i].Captures[1].Index + list[i].Captures[1].Length;
            }
            sbb.Append(data.Substring(start));
            string s = sbb.ToString();

            StreamWriter writer = new StreamWriter(resultName);
            writer.Write(s);
            writer.Close();
            return s;
        }

        public static string Translate(string src, string from, string to)
        {
            return BaiduTranslate(src, from, to)[0];
        }
        public static List<string> Translate(List<string> list, string from, string to)
        {
            StringBuilder sb = new StringBuilder();
            foreach (var ss in list)
            {
                sb.Append(ss);
                sb.Append("\n");
            }
            return BaiduTranslate(sb.ToString(), from, to);
        }


        private static List<string> BaiduTranslate(string src, string from, string to)
        {
            StringBuilder sb = new StringBuilder();
            sb.Append("from=" + from + "&to=" + to + "&transtype=translang&simple_means_flag=3&query=" + src);
            string url = "http://fanyi.baidu.com/v2transapi";
            HttpWebRequest httpWebRequest = (HttpWebRequest)HttpWebRequest.Create(url);
            httpWebRequest.CookieContainer = new CookieContainer();
            httpWebRequest.ContentType = "application/x-www-form-urlencoded; charset=UTF-8";
            httpWebRequest.Referer = "http://fanyi.baidu.com/?aldtype=16047";
            httpWebRequest.Accept = "*/*";
            httpWebRequest.UserAgent = "Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/55.0.2883.87 Safari/537.36";
            httpWebRequest.Method = "POST";
            string postData = sb.ToString();
            byte[] postdatabyte = Encoding.UTF8.GetBytes(postData);
            Stream stream = httpWebRequest.GetRequestStream();
            stream.Write(postdatabyte, 0, postdatabyte.Length);
            stream.Close();
            HttpWebResponse httpWebResponse = (HttpWebResponse)httpWebRequest.GetResponse();
            Stream responseStream = httpWebResponse.GetResponseStream();
            StreamReader streamReader = new StreamReader(responseStream, Encoding.UTF8);
            string responseData = streamReader.ReadToEnd();
            responseStream.Close();
            httpWebResponse.Close();
            JObject obj = JObject.Parse(responseData);
            var result = obj["trans_result"];
            JArray data = result["data"] as JArray;

            List<string> list = new List<string>();
            for (int i = 0; i < data.Count; i++)
            {
                list.Add(data[i]["dst"].ToString());
            }
            return list;
        }

        private string GetDiffProgram()
        {
            string[] array = new string[] {
                @"C:\Program Files (x86)\Beyond Compare 4\BCompare.exe",
                @"C:\Program Files (x86)\Beyond Compare 2\BC2.exe",
                @"D:\Program Files (x86)\Beyond Compare 4\BCompare.exe",
                @"D:\Program Files (x86)\Beyond Compare 2\BC2.exe",
            };
            foreach (var p in array)
            {
                if (System.IO.File.Exists(p))
                {
                    return p;
                }
            }
            return null;
        }

        private void showResult(string input, string output)
        {
            string diff = GetDiffProgram();
            if (diff != null)
            {
                ShellExecute(IntPtr.Zero, "open", diff, (input + " " + output), null, ShowWindowCommands.SW_SHOWNORMAL);
            }
            else
            {
                System.Diagnostics.Process.Start("explorer.exe", output);
            }
        }
    }
}
