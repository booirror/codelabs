using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace RegularExpressionEngine.Regex
{
    public class SubPos
    {
        public int Begin;
        public int End;

        public SubPos(int begin)
        {
            this.Begin = begin;
        }

        public SubPos(int begin, int end)
        {
            this.Begin = begin;
            this.End = end;
        }
    }
    public class Capture
    {
        public Capture(string src, int idx, int len)
        {
            this.Index = idx;
            this.Length = len;
            this.Value = src.Substring(idx, len);
            this.Success = true;
        }

        public Capture()
        {
            Success = false;
            this.Value = "";
            this.Length = 0;
        }

        public bool Success { get; private set; }
        public int Index { get; private set; }

        public int Length { get; private set; }

        public string Value { get; private set; }

        public override string ToString()
        {
            return Value;
        }
    }
}
