using RegularExpressionEngine.Parser;
using RegularExpressionEngine.Regex;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace RegularExpressionEngine.NFA
{
    class NFACharacterEscapeEdge : NFAEdge
    {
        public NFACharacterEscapeEdge(int symbol, NFAState targetState)
            : base(NFAEdgeType.CharacterEscape)
        {
            Symbol = symbol;
            TargetState = targetState;
        }
    }

    class NFAIdentityEscapeEdge : NFAEdge
    {
        public NFAIdentityEscapeEdge(int symbol, NFAState targetState)
            : base(NFAEdgeType.IdentityEscape)
        {
            Symbol = symbol;
            TargetState = targetState;
        }
    }

    class NFASubExprEdge : NFAEdge
    {
        public NFASubExprEdge(int symbol, int idx, NFAState targetState)
            : base(NFAEdgeType.SubExpression)
        {
            Symbol = symbol;
            TargetState = targetState;
            Index = idx;
        }

        public int Index { get; private set; }
        public override bool Match(string s, ref int pos)
        {
            return true;
        }
    }

    class CharacterClassExprEdge : NFAEdge
    {

        public List<char> CharSet { get; private set; }
        public bool IsNot { get; private set; }
        public CharacterClassExprEdge(List<char> list, bool isNotList, NFAState targetState)
            : base(NFAEdgeType.CharacterClass)
        {
            CharSet = list;
            IsNot = isNotList;
            TargetState = targetState;
            Symbol = '#';
        }

        public override bool Match(string s, ref int pos)
        {
            if (pos >= s.Length)
            {
                return false;
            }
            bool inList = !IsNot;
            foreach (char c in CharSet)
            {
                if (c == s[pos])
                {
                    if (inList)
                    {
                        pos++;
                    }
                    return inList;
                }
            }
            if (IsNot)
            {
                pos++;
            }
            return IsNot;
        }
    }

    class NFAQuantifierEdge : NFAEdge
    {
        public NFAQuantifierEdge(int min, int max, bool isGreedy, NFAState targetState, NFAState tail)
            : base(NFAEdgeType.Quantifier)
        {
            this.Max = max;
            this.Min = min;
            this.IsGreedy = isGreedy;
            this.Num = 0;
            TargetState = targetState;
            TailState = tail;
            _next = targetState;
        }

        public override bool Match(string s, ref int pos)
        {
            Num++;
            return true;
            /*
            if (!IsGreedy && Num >= Min)
            {
                _next = TailState;
                return true;
            }
            if (IsGreedy && )
            */
        }

        public override NFAState NextState()
        {
            return _next;
        }

        //public int MaxPos(string s, int p)
        //{
        //    this.Num = 0;
        //    Match(s, p);
        //}

        /// <summary>
        /// 尝试匹配一圈，成功返回true
        /// </summary>
        /// <param name="s"></param>
        /// <param name="p"></param>
        /// <returns></returns>
        public bool TryMatchCycle(string s, ref int p, Dictionary<int, SubPos> captures)
        {
            //这里只会有一条边
            NFAEdge currEdge = this;
            Stack<Tuple<NFAEdge, int>> stack = new Stack<Tuple<NFAEdge, int>>();
            
            var ess = currEdge.TargetState.OutEdges;
            for (int i = ess.Count - 1; i >= 0; --i)
            {
                stack.Push(new Tuple<NFAEdge, int>(ess[i], p));
            }
            Dictionary<int, int> tempDict = new Dictionary<int,int>();
            while (stack.Count > 0 && p <= s.Length)
            {
                var t = stack.Pop();
                currEdge = t.Item1;
                p = t.Item2;
                if (currEdge == this)
                {
                    return true;
                }
                if (currEdge.Match(s, ref p))
                {
                    if (currEdge is NFASubExprEdge)
                    {
                        NFASubExprEdge edge = currEdge as NFASubExprEdge;
                        if (edge.Symbol == SpecialChar.ParenthesisLeft)
                        {
                            tempDict[edge.Index] = p;
                        }
                        else
                        {
                            captures[edge.Index] = new SubPos(tempDict[edge.Index], p);
                        }
                    }

                    var es = currEdge.TargetState.OutEdges;
                    for (int i = es.Count - 1; i >= 0; --i)
                    {
                        stack.Push(new Tuple<NFAEdge, int>(es[i], p));
                    }
                }
            }
            return false;
        }

        public NFAState TailState { get; private set; }

        public int Max { get; private set; }
        public int Min { get; private set; }

        public bool IsGreedy { get; private set; }

        public int Num { get; set; }

        public NFAState _next;
    }
}
