using RegularExpressionEngine.Parser;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace RegularExpressionEngine.NFA
{
    class NFAAnyCharEdge : NFAEdge
    {
        public NFAAnyCharEdge(int symbol, NFAState targetState)
            : base(NFAEdgeType.AnyChar)
        {
            Symbol = symbol;
            TargetState = targetState;
        }

        public override bool Match(string s, ref int pos)
        {
            if (SpecialChar.IsNewLine(s[pos]))
            {
                return false;
            }
            else
            {
                pos++;
                return true;
            }
        }
    }
}
