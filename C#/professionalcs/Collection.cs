using System;
using System.Collections;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace _05Generics
{
    class Collection
    {
        public static void TestPerformance()
        {
            TestCollection();
            TestGenericCollection();
        }

        static void TestCollection()
        {
            Stopwatch sw = new Stopwatch();
            sw.Start();
            var list = new ArrayList();
            for (int i = 0; i < 900000; i++)
            {
                list.Add(i);
            }
            int sum = 0;
            foreach(int i2 in list)
            {
                sum += i2;
            }
            sw.Stop();
            Console.WriteLine("TestCollection: {0}", sw.ElapsedMilliseconds);
        }

        static void TestGenericCollection()
        {
            Stopwatch sw = new Stopwatch();
            sw.Start();
            var list = new List<int>();
            for (int i = 0; i < 900000; i++)
            {
                list.Add(i);
            }
            int sum = 0;
            foreach (int i2 in list)
            {
                sum += i2;
            }
            sw.Stop();
            Console.WriteLine("TestGenericCollection: {0}", sw.ElapsedMilliseconds);
        }
    }
}
