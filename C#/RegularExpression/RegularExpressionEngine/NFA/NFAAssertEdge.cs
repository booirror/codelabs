using RegularExpressionEngine.Parser;
using System;


namespace RegularExpressionEngine.NFA
{
    class NFAAssertEdge : NFAEdge
    {
        public NFAAssertEdge(int symbol, NFAState targetState)
            : base(NFAEdgeType.Assertion)
        {
            Symbol = symbol;
            TargetState = targetState;
        }

        public override bool Match(string s, ref int pos)
        {
            if (Symbol == SpecialChar.AssertBegin)
            {
                if (pos == 0 || SpecialChar.IsNewLine(s[pos - 1]))
                    return true;
                return false;
            }
            if (Symbol == SpecialChar.AssertEnd)
            {
                if (pos == s.Length || SpecialChar.IsNewLine(s[pos]))
                    return true;
                return false;
            }
            return false;
        }
    }
}
