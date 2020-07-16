using Newtonsoft.Json.Linq;
using RegularExpressionEngine;
using RegularExpressionEngine.Parser;
using RegularExpressionEngine.Regex;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Net;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading.Tasks;

namespace RegularExpressionTest
{
    
    class Program
    {
        public static readonly string ZH = "zh";
        public static readonly string EN = "en";

        static void Main(string[] args)
        {
            /*
            RegexParser parser = new RegexParser("ab|mc*");
            RegularExpression regular = parser.Parse();
            Console.WriteLine(regular.ToString());
            */
            /*
            Regex regex = new Regex("^cd\\.e$");
            //Console.WriteLine(regex.Regexpr.ToString());
            MatchResult m = regex.Match("cd.e");
            if (m.Success)
            {
                Console.WriteLine(m.Value);
            }
            else
            {
                Console.WriteLine("failure");
            }
             */

            
            // System Regex
            //var re = new System.Text.RegularExpressions.Regex("(ab|ada)*");
            //var m = re.Match("adaadaabcd");
            //Console.WriteLine(m.Value);
            //Console.WriteLine(m.Groups[1]);

            
            // My Regex
            

            //Dictionary<int, int> dict = new Dictionary<int, int>();
            //dict[3] = 3;
            //Console.WriteLine(dict[3]);

            //TestRegex("abc", "((a)|(ab))((c)|(bc))");
            //TestRegex("abc", "ab|abc");
            //show_matches("abc", "abc|");
            //show_matches("abc", "|abc");
            //show_matches("abcdefghi", "a[a-z]{2,4}");
            //show_matches("abcdefghi", "a[a-z]{2,4}?");
            //show_matches("aabaac", "(aa|aabaac|ba|b|c)*");
            //不支持反向引用
            //show_matches("aaaaaaaaaa,aaaaaaaaaaaaaaa", "^(a+)\\1*,\\1+$");
            //未通过
            //show_matches("zaaabbbcac", "(z)((a+)?(b+)?(c))*");
            //show_matches("aaa", "a$");
            //未实现matches the o at the end of the first word
            //show_matches("moo goo gai pan", "o\\b");
            //未实现
            //show_matches("baaabac", "(?=(a+))");


            //TestSystemRegex("mm33abc33", "\\d+([a-z]*)\\d*");
            /*
            var reg = new RegularExpressionEngine.Regex.Regex("\\w*=(\\w*)");
            //var m = reg.Match("lo");
            //Console.WriteLine("[" + m.Value+ "]");
            string s = reg.Replace("ab=hello\r\nbc=world", m =>
            {
                Console.WriteLine("{0},{1}, 1={2},{3}, {4}", m.Index, m.Length, m.Captures[1].Index, m.Captures[1].Length, m.Captures[1].Value);
                //return m.Value.Replace(m.Captures[1].Value, "fuck");
                return m.Value.Substring(0, m.Captures[1].Index - m.Index) + "fuck" + m.Value.Substring(m.Captures[1].Index - m.Index + m.Captures[1].Length);
            });
            Console.WriteLine(s);
            */
            //Console.WriteLine("min={0}, max={1}", (int)char.MinValue, (int)char.MaxValue);
            //PerformanceTest();
            /*
            var s = Translate("我们", ZH, EN);
            Console.WriteLine(s);
             */
            //FileTranslateBatch();
            //FileTranslate();

            //var reg = new RegularExpressionEngine.Regex.Regex("[a-zA-Z0-9_]*");
            //var m = reg.Match("loZ_9");
            //Console.WriteLine("[" + m.Value + "]");
        }

        public static void PerformanceTest()
        {
            StreamReader reader = new StreamReader("string_zh.lua");
            string data = reader.ReadToEnd();
            reader.Close();

            var regex = new RegularExpressionEngine.Regex.Regex("^\\s*[^-][^-][^\r\n]*=\\s*\"(.*)\".*$");
            var ms = regex.Matches(data);
            /*
            foreach(var m in ms)
            {
                string mm = m.Value;
                Console.WriteLine("["+m.Captures[1]+"]");
            }
            */
        }

        public static void FileTranslate()
        {
            StreamReader reader = new StreamReader("string_zh.lua");
            string data = reader.ReadToEnd();
            reader.Close();

            var regex = new RegularExpressionEngine.Regex.Regex("^\\s*[^-][^-][^\r\n]*=\\s*\"(.*)\".*$");
            /*
            var ms = regex.Matches(data);
            foreach(var m in ms)
            {
                string mm = m.Value;
                Console.WriteLine("["+m.Captures[1]+"]");
            }
            */
            //var s = regex.Replace(data, "haha", 1);
            var s = regex.Replace(data, x => Translate(x, ZH, EN), 1);
            StreamWriter writer = new StreamWriter("string_zh_x.lua");
            writer.Write(s);
            writer.Close();
            
        }

        public static void FileTranslateBatch()
        {
            StreamReader reader = new StreamReader("string_zh.lua");
            string data = reader.ReadToEnd();
            reader.Close();

            var regex = new RegularExpressionEngine.Regex.Regex("^\\s*[^-][^-][^\r\n]*=\\s*\"(.*)\".*$");

            var list = regex.Matches(data);
            List<string> strlist =new List<string>();
            foreach(var m in list)
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
            
            StreamWriter writer = new StreamWriter("string_zh_x.lua");
            writer.Write(s);
            writer.Close();

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
            sb.Append("from="+from + "&to=" + to + "&transtype=translang&simple_means_flag=3&query=" + src);
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

        public static void show_matches(string src, string pattern)
        {
            var re = new RegularExpressionEngine.Regex.Regex(pattern);
            var m = re.Match(src);
            if (m.Success)
            {
                Console.WriteLine("src=" + src);
                Console.WriteLine("pattern=" + pattern);
                Console.WriteLine("prefix=" + m.Prefix);
                Console.WriteLine("value=" + m.Value);
                Console.WriteLine("suffix=" + m.Suffix);
                Console.WriteLine("-----------------------");
                foreach (var s in m.Captures)
                {
                    Console.WriteLine("[" + s.Value + "]");
                }
            }
            else
            {
                Console.WriteLine("Failed");
            }
        }

        public static void TestSystemRegex(string src, string pattern)
        {
            var re = new System.Text.RegularExpressions.Regex(pattern);
            var m = re.Match(src);
            if (m.Success)
            {
                Console.WriteLine("src=" + src);
                Console.WriteLine("pattern=" + pattern);
                Console.WriteLine("prefix=" + src.Substring(0, m.Index));
                Console.WriteLine("value=" + m.Value);
                Console.WriteLine("suffix=" + src.Substring(m.Index + m.Length));
                Console.WriteLine("-----------------------");
                foreach (var s in m.Captures)
                {
                    Console.WriteLine(s);
                }
                Console.WriteLine("-----------------------");
                foreach (Group s in m.Groups)
                {
                    Console.WriteLine("["+s+"] {0},{1}", s.Index, s.Length);
                }
            }
            else
            {
                Console.WriteLine("Failed");
            }
        }
    }
}
