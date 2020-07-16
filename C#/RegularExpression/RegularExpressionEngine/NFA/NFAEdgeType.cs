using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace RegularExpressionEngine.NFA
{
    public enum NFAEdgeType
    {
        Default,
        Assertion,
        AnyChar,
        CharacterEscape,
        IdentityEscape,
        SubExpression,
        Quantifier,
        CharacterClass,
    }
}
