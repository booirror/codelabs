using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Security.Cryptography;
using System.Text;
using System.Threading.Tasks;

namespace webpToolConsole
{
    class Program
    {
        static void Main(string[] args)
        {
            //var list = new List<string>();
            //list.Add("shareRes");
            //clearDir("ccbResources\\resfolder", null, list);

            //clearDir("ccbResources\\resfolder\\", null, null);

            //var flist = new List<string>();
            //flist.Add("ccbResources");
            //CopyDirectory(@"D:\tag_p5_huopin\hpmj\Resources\res", "ccbResources\\resfolder", null, flist);

            //clearDir("ccbResources\\resfolder\\", null, null);

            string path = "ccbResources";
            if (!Directory.Exists(path))
            {
                Console.WriteLine("请把exe放在 webpTool下");
                return;
            }
            updateSrcAndRes(args);
        }

        static public void updateSrcAndRes(string[] args)
        {
            var timeTick = new Stopwatch();
            timeTick.Start();
            // 清理src, res
            var srcPath = "..\\pro.android_hpmj\\assets\\src";
            var resPath = "..\\pro.android_hpmj\\assets\\res";
            var webpPath = "ccbResources";
            var webpResPath = webpPath + "\\ccbResources";

            Console.WriteLine("begin clear src, res");
            clearDir(srcPath, null, null);
            clearDir(resPath, null, null);
            clearDir(webpPath, null, null);

            Console.WriteLine("copy to asset");
            //copy src 
            CopyDirectory(@"..\Resources\src", srcPath, null, null);

            // copy res 除了ccbResources
            var dirList = new List<string>();
            dirList.Add("ccbResources");
            CopyDirectory(@"..\Resources\res", resPath, null, dirList);

            // copy ccbResources到webp
            CopyDirectory(@"..\Resources\res\ccbResources", webpResPath, null, null);

            //png to webp
            if (RunWebp(args)) {
                //把新ccbResources的拷贝到asset
                Console.WriteLine("begin copy ccbResources to asset");
                CopyDirectory(webpResPath, resPath + "\\ccbResources", null, null);
                Console.WriteLine("完毕，执行成功");
            }
            else
            {
                Console.WriteLine("error 转webp失败");
            }

            timeTick.Stop();
            Console.WriteLine("time:" + timeTick.Elapsed);

            Console.ReadKey(false);
        }

        static bool RunWebp(string[] args)
        {
            List<string> list = new List<string>();
            string path = "ccbResources";
            if (args.Length > 1)
            {
                path = args[1];
            }
            if (!Directory.Exists(path))
            {
                Console.WriteLine("webpTool 目录下ccbResources 不存在");
                return false;
            }

            Walk(path, ref list);

            // print
            //for (int i = 0; i < list.Count; i++)
            //{
            //    System.Console.WriteLine(list[i]);
            //}
            if (list.Count == 0)
            {
                Console.WriteLine("ccbResources没有图片");
                return false;
            }
            var appDataPath = System.Environment.GetFolderPath(System.Environment.SpecialFolder.ApplicationData);

            var webpDir = appDataPath + "\\webptool"; // 用户数据路径
            if (!Directory.Exists(webpDir))
            {
                Directory.CreateDirectory(webpDir);
            }

            var md5InfoFile = webpDir + "\\webpmd5.map";

            Dictionary<string, string> dict = Load(md5InfoFile);

            List<string> needConvertList = new List<string>();

            Dictionary<string, string> needConvert = new Dictionary<string, string>();

            Console.WriteLine("处理中");
            for (int i = 0; i < list.Count; i++)
            {
                string ExistMd5 = "";
                string originPath = list[i];
                string subPath = getSubPath(originPath);
                var isSuccess = dict.TryGetValue(originPath, out ExistMd5);
                var originMd5 = GetMD5HashFromFile(originPath);
                if (isSuccess && ExistMd5 == originMd5)
                {
                    // 上面图片没变，不需要再转换
                    if (!CopyFile(webpDir + "\\" + subPath, originPath))
                    {
                        // 拷贝失败，转webp
                        needConvert.Add(originPath, originMd5);
                    }
                }
                else
                {
                    //转webp
                    needConvert.Add(originPath, originMd5);
                }
            }
            bool isWebpSuccess = true;
            if (needConvert.Count > 0)
            {
                isWebpSuccess = toWebp(needConvert, webpDir, dict);
                Save(md5InfoFile, dict);
                System.Console.WriteLine(string.Format("to webp完毕, 转换了{0}张", needConvert.Count));
            }
            else
            {
                System.Console.WriteLine("to webp完毕，无需转换");
            }
            return isWebpSuccess;
        }

        static bool toWebp(Dictionary<string, string> needConvertList, string webpDir, Dictionary<string, string> dict)
        {
            System.Console.WriteLine("running");
            string errorInfo = InvokeExcute(needConvertList); // webp 转换
            if (errorInfo.Length > 1)
            {
                System.Console.WriteLine("not update webpmd5.map");
                // 说明转换有出问题
                return false;
            }
            else
            {
                System.Console.WriteLine("will update webpmd5.map");
            }

            foreach (var item in needConvertList)
            {
                string subPath = getSubPath(item.Key);
                
                var targetPath = webpDir + "\\" + subPath;
                var fullpath = Path.GetDirectoryName(targetPath);
                Directory.CreateDirectory(fullpath);
                if (!CopyFile(item.Key, targetPath))
                {
                    // 复制出错
                    System.Console.WriteLine(string.Format("error copy file to [{0}]", targetPath));
                }
                else
                {
                    //System.Console.WriteLine(item.Key);
                    if (dict.ContainsKey(item.Key)) {
                        dict[item.Key] = item.Value;
                    }else{
                        dict.Add(item.Key, item.Value);

                    }
                }
            }
            return true;
        }

        public static string InvokeExcute(Dictionary<string, string> needConvertList)
        {
            //Command = Command.Trim().TrimEnd('&') + "&exit";

            Console.WriteLine("needConvertList size=" + needConvertList.Count);
            using (Process p = new Process())
            {
                p.StartInfo.FileName = "cmd.exe";
                p.StartInfo.UseShellExecute = false;        //是否使用操作系统shell启动
                p.StartInfo.RedirectStandardInput = true;   //接受来自调用程序的输入信息
                p.StartInfo.RedirectStandardOutput = true;  //由调用程序获取输出信息
                p.StartInfo.RedirectStandardError = true;   //重定向标准错误输出
                p.StartInfo.CreateNoWindow = false;          //不显示程序窗口

                

                string str = "";
                p.ErrorDataReceived += new DataReceivedEventHandler(delegate(object sender, DataReceivedEventArgs e)
                {
                    if (!string.IsNullOrEmpty(e.Data)) {
                        Console.WriteLine("error: " + e.Data);
                        str = "error";
                    }
                });
                p.OutputDataReceived += new DataReceivedEventHandler(delegate(object sender, DataReceivedEventArgs e)
                {
                    Console.WriteLine(e.Data);
                });
                
                p.Start();//启动程序

                p.BeginErrorReadLine();
                p.BeginOutputReadLine();

                

                //向cmd窗口写入命令
                
                p.StandardInput.WriteLine("@echo off");
                p.StandardInput.AutoFlush = true;
                int idxVal = 0;
                foreach (var item in needConvertList)
                {
                    idxVal++;
                    
                    p.StandardInput.WriteLine(string.Format("bin\\cwebp.exe -quiet -q  80 \"{0}\" -o \"{1}\"", item.Key, item.Key));
                }
                p.StandardInput.WriteLine("exit");

                

                //获取cmd窗口的输出信息
                //StreamReader reader = p.StandardOutput;//截取输出流
                //StreamReader error = p.StandardError;//截取错误信息
                //string str = error.ReadToEnd();

                
                p.WaitForExit();//等待程序执行完退出进程
                p.Close();

                

                return str;
            }
        }

        static string getSubPath(string path)
        {
            var val = path.IndexOf("ccbResources");
            return path.Substring(val);
        }

        static void Walk(string path, ref List<string> list)
        {
            DirectoryInfo di = new DirectoryInfo(path);
            FileInfo[] fis = di.GetFiles();
            for (int i = 0; i < fis.Length; i++)
            {
                if (fis[i].Extension == ".jpg" || fis[i].Extension == ".png" || fis[i].Extension == ".jpeg")
                {
                    list.Add(fis[i].FullName);
                }
            }
            DirectoryInfo[] dis = di.GetDirectories();
            for (int i = 0; i < dis.Length; i++)
            {
                Walk(dis[i].FullName, ref list);
            }
        }

        static void clearDir(string path, List<string> excludeFiles, List<string> excludeFolders)
        {
            DirectoryInfo di = new DirectoryInfo(path);
            FileInfo[] fis = di.GetFiles();
            for (int i = 0; i < fis.Length; i++)
            {
                if (excludeFiles == null || excludeFiles.FindIndex(item => item == fis[i].Name) == -1)
                {
                    fis[i].Delete();
                }
            }
            DirectoryInfo[] dis = di.GetDirectories();
            for (int i = 0; i < dis.Length; i++)
            {
                if (excludeFolders == null || excludeFolders.FindIndex(item => item == dis[i].Name) == -1)
                {
                    dis[i].Delete(true);
                }
            }
        }

        static void deleteFile(string fileName)
        {
            FileInfo fi = new FileInfo(fileName);
            if (fi != null)
            {
                fi.Delete();
            }
        }
        /// <summary>
        /// 拷贝目录内容
        /// </summary>
        /// <param name="source">源目录</param>
        /// <param name="destination">目的目录</param>
        /// <param name="copySubDirs">是否拷贝子目录</param>
        public static void CopyDirectory(string sourcePath, string destinationPath, List<string> excludeFiles, List<string> excludeFolders)
        {
            DirectoryInfo source = new DirectoryInfo(sourcePath);
            DirectoryInfo destination = new DirectoryInfo(destinationPath);

            if (!destination.Exists)
            {
                destination.Create(); //目标目录若不存在就创建
            }
            FileInfo[] files = source.GetFiles();
            foreach (FileInfo file in files)
            {
                if (excludeFiles == null || excludeFiles.FindIndex(item => item == file.Name) == -1)
                {
                    file.CopyTo(Path.Combine(destination.FullName, file.Name), true); //复制目录中所有文件
                }
            }
            var copySubDirs = true;
            if (copySubDirs)
            {
                DirectoryInfo[] dirs = source.GetDirectories();
                foreach (DirectoryInfo dir in dirs)
                {
                    string destinationDir = Path.Combine(destination.FullName, dir.Name);
                    if (excludeFolders == null || excludeFolders.FindIndex(item => item == dir.Name) == -1)
                    {
                        CopyDirectory(dir.FullName, destinationDir, null, null);
                    }
                }
            }
        }

        public static string GetMD5HashFromFile(string fileName)
        {
            try
            {
                FileStream file = new FileStream(fileName, System.IO.FileMode.Open);
                MD5 md5 = new MD5CryptoServiceProvider();
                byte[] retVal = md5.ComputeHash(file);
                file.Close();
                StringBuilder sb = new StringBuilder();
                for (int i = 0; i < retVal.Length; i++)
                {
                    sb.Append(retVal[i].ToString("x2"));
                }
                return sb.ToString();
            }
            catch (Exception ex)
            {
                throw new Exception("GetMD5HashFromFile() fail,error:" + ex.Message);
            }
        }

        public static Dictionary<string, string> Load(string path)
        {
            Dictionary<string, string> dict = new Dictionary<string, string>();
            if (!File.Exists(path)) {
                return dict;
            }
            System.IO.StreamReader s = File.OpenText(path);
            while (s.Peek() != -1)
            {
                string line = s.ReadLine();
                if (!string.IsNullOrEmpty(line))
                {
                    string[] data = line.Split(',');
                    dict.Add(data[0], data[1]);
                }
            }
            s.Close();
            return dict;
        }

        public static void Save(string path, Dictionary<string, string> dict) 
        {
            System.IO.FileStream s = File.Open(path, FileMode.Create);
            StreamWriter writer = new StreamWriter(s);
            foreach (var item in dict)
            {
                writer.WriteLine(item.Key + "," + item.Value);
            }
            writer.Close();
        }

        /// <summary>
        /// 大文件多次复制文件  true：复制成功   false：复制失败
        /// </summary>
        /// <param name="soucrePath">原始文件路径</param>
        /// <param name="targetPath">复制目标文件路径</param>
        /// <returns></returns>
        public static bool CopyFile(string soucrePath, string targetPath)
        {
            //Console.WriteLine("Copy src={0}, dst={1}", soucrePath, targetPath);
            try
            {
                //读取复制文件流
                using (FileStream fsRead = new FileStream(soucrePath, FileMode.Open, FileAccess.Read))
                {
                    //写入文件复制流
                    using (FileStream fsWrite = new FileStream(targetPath, FileMode.Create, FileAccess.Write))
                    {

                        byte[] buffer = new byte[1024 * 1024 * 2]; //每次读取2M
                        //可能文件比较大，要循环读取，每次读取2M
                        while (true)
                        {
                            //每次读取的数据    n：是每次读取到的实际数据大小
                            int n = fsRead.Read(buffer, 0, buffer.Count());
                            //如果n=0说明读取的数据为空，已经读取到最后了，跳出循环
                            if (n == 0)
                            {
                                break;
                            }
                            //写入每次读取的实际数据大小
                            fsWrite.Write(buffer, 0, n);
                        }
                    }
                }
                return true;
            }
            catch (System.Exception ex)
            {
                return false;
            }
        }
    }
}
