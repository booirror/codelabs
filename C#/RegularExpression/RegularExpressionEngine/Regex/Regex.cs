using RegularExpressionEngine.NFA;
using RegularExpressionEngine.Parser;
using System;
using System.Linq;
using System.Collections.Generic;
using System.Text;
using System.Diagnostics;

namespace RegularExpressionEngine.Regex
{
    public class Regex
    {
        public NFAModel NFA {get; private set;}
        public RegularExpression Regexpr { get; private set; }

        public Regex(string pattern)
        {
            RegexParser rp = new RegexParser(pattern);
            Regexpr = rp.Parse();

            NFAConverter converter = new NFAConverter();
            this.NFA = converter.Convert(Regexpr);
            this.NFA.CaptureNum = rp.SubNum;
        }

        public string Replace(string input, Func<MatchResult, string> matchEvaluator)
        {
            StringBuilder sb = new StringBuilder();
            int idx = 0;
            while (true)
            {
                MatchResult result = Match(input, idx);
                if (result.Success)
                {
                    sb.Append(input.Substring(idx, result.Index -idx));
                    sb.Append(matchEvaluator(result));
                    idx = result.Index + result.Length;
                }
                else
                {
                    sb.Append(input.Substring(idx));
                    break;
                }
            }
            return sb.ToString();
        }
        /// <summary>
        /// 
        /// </summary>
        /// <param name="input"></param>
        /// <param name="replace"></param>
        /// <param name="captureIdx">捕获的索引号，从左到右，从1开始</param>
        /// <returns></returns>
        public string Replace(string input, string replace, int captureIdx)
        {
            return Replace(input, m =>
            {
                return m.Value.Substring(0, m.Captures[captureIdx].Index - m.Index) + replace + m.Value.Substring(m.Captures[captureIdx].Index - m.Index + m.Captures[captureIdx].Length);
            });
        }

        public string Replace(string input, Func<string, string> matchEvaluator, int captureIdx)
        {
            return Replace(input, m =>
            {
                return m.Value.Substring(0, m.Captures[captureIdx].Index - m.Index) + matchEvaluator(m.Captures[captureIdx].Value) + m.Value.Substring(m.Captures[captureIdx].Index - m.Index + m.Captures[captureIdx].Length);
            });
        }

        public List<MatchResult> Matches(string src)
        {
            List<MatchResult> list = new List<MatchResult>();
            int idx = 0;
            while (true)
            {
                MatchResult result = Match(src, idx);
                if (result.Success)
                {
                    list.Add(result);
                    idx = result.Index + result.Length;
                }
                else
                {
                    break;
                }
            }
            return list;
        }

        public MatchResult Match(string src)
        {
            return Match(src, 0);
        }

        public MatchResult Match(string src, int start)
        {
            for (int i = start; i < src.Length; i++)
            {
                int pos = i;
                var captures = new Dictionary<int, SubPos>();
                var val = Match(src, i, ref pos, NFA.EntryEdge, captures);
                if (val.Success)
                {
                    val.AddCapture(new Capture(src, val.Index, val.Length));
                    for (int idx = 0; idx < NFA.CaptureNum; idx++)
                    {
                        if (captures.ContainsKey(idx) && captures[idx].Begin <= captures[idx].End)
                        {
                            var kv = captures[idx];
                            Capture c = new Capture(src, kv.Begin, kv.End - kv.Begin);
                            val.AddCapture(c);
                        }
                        else
                        {
                            val.AddCapture(new Capture());
                            //val.AddCapture(Capture);
                        }
                    }
                    return val;
                }
            }
            return MatchResult.Empty;
        }

        private MatchResult Match(string src, int idx, ref int pos, NFAEdge startEdge, Dictionary<int, SubPos> captures)
        {
            NFAEdge currEdge = startEdge;
            while ((pos < src.Length) 
                || (pos == src.Length && currEdge is NFAAssertEdge)
                || (pos == src.Length && currEdge is NFASubExprEdge)
                || (pos == src.Length && currEdge.IsEmpty))
            {
                if (currEdge.Match(src, ref pos))
                {
                    if (currEdge is NFASubExprEdge)
                    {
                        NFASubExprEdge edge = currEdge as NFASubExprEdge;
                        if (edge.Symbol == SpecialChar.ParenthesisLeft)
                        {
                            if (!captures.ContainsKey(edge.Index))
                            {
                                captures.Add(edge.Index, new SubPos(pos));
                            }
                            else
                            {
                                captures[edge.Index].Begin = pos;
                            }
                        }
                        else
                        {
                            captures[edge.Index].End = pos;
                        }
                    }
                    if (currEdge.NextState() == NFA.TailState)
                    {
                        return new MatchResult(src, idx, pos - idx);
                    }
                    else
                    {
                        if (currEdge is NFAQuantifierEdge)
                        {
                            NFAQuantifierEdge edge = currEdge as NFAQuantifierEdge;
                            int tp = pos;
                            int num = 0;
                            List<int> cp = new List<int>();
                            cp.Add(tp);

                            while (edge.TryMatchCycle(src, ref tp, captures) && (edge.Max == -1 || num < edge.Max))
                            {
                                num++;
                                cp.Add(tp);
                            }

                            if (num < edge.Min)
                            {
                                return MatchResult.Empty;
                            }

                            if (edge.IsGreedy)
                            {
                                for (int n = num; n >= edge.Min; n--)
                                {
                                    var edges = edge.TailState.OutEdges;
                                    foreach (var e in edges)
                                    {
                                        pos = cp[n];
                                        var result = Match(src, idx, ref pos, e, captures);
                                        if (result.Success)
                                        {
                                            return result;
                                        }
                                    }
                                }
                                return MatchResult.Empty;
                            }
                            else
                            {
                                for (int n = edge.Min; n <= num; n++)
                                {
                                    pos = cp[n];
                                    var edges = edge.TailState.OutEdges;
                                    foreach (var e in edges)
                                    {
                                        var result = Match(src, idx, ref pos, e, captures);
                                        if (result.Success)
                                        {
                                            return result;
                                        }
                                    }
                                }
                                return MatchResult.Empty;
                            }
                        }
                        else
                        {
                            var edges = currEdge.NextState().OutEdges;
                            foreach (var e in edges)
                            {
                                int p = pos;
                                var result = Match(src, idx, ref p, e, captures);
                                if (result.Success)
                                {
                                    return result;
                                }
                            }
                            return MatchResult.Empty;
                        }
                    }
                }
                else
                {
                    return MatchResult.Empty;
                }
            }
            return MatchResult.Empty;
        }
    }
}
