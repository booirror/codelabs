
using System.Diagnostics;
namespace RegularExpressionEngine.NFA
{
    [DebuggerDisplay("{Symbol.HasValue?(\"\"+(char)Symbol):\"ε\"}")]
    public class NFAEdge
    {
        public NFAEdge(int symbol, NFAState targetState)
            : this(NFAEdgeType.Default)
        {
            Symbol = symbol;
            TargetState = targetState;
        }

        public NFAEdge(NFAState targetState):this(NFAEdgeType.Default)
        {
            TargetState = targetState;
        }

        public NFAEdge(NFAEdgeType type)
        {
            Type = type;
        }

        public virtual bool Match(string s, ref int pos)
        {
            if (IsEmpty)
            {
                return true;
            }
            else if (pos < s.Length && s[pos] == Symbol)
            {
                pos++;
                return true;
            }
            return false;
        }

        public virtual NFAState NextState()
        {
            return TargetState;
        }

        public int? Symbol {get; protected set;}

        public NFAState TargetState {get; protected set;}

        public NFAEdgeType Type { get; set; }

        public bool IsEmpty
        {
            get
            {
                return !Symbol.HasValue;
            }
        }
    }
}
