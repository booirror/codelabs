using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace RegularExpressionEngine.Regex
{
    public class MatchResult : Capture
    {
        public MatchResult(string src, int idx, int len): base(src, idx, len)
        {
            Captures = new List<Capture>();
            Prefix = src.Substring(0, idx);
            Suffix = src.Substring(idx + len);
        }

        public MatchResult()
        {
        }

        public void AddCapture(Capture c)
        {
            Captures.Add(c);
        }

        public String Prefix { get; private set; }
        public String Suffix { get; private set; }

        public static readonly MatchResult Empty = new MatchResult();

        public List<Capture> Captures { private set; get; }
    }
}
