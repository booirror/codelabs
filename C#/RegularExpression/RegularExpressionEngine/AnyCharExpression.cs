using System;
using System.Collections.Generic;

namespace RegularExpressionEngine
{
    public class AnyCharExpression : SymbolExpression
    {
        public AnyCharExpression(char c):base (c, RegularExpressionType.AnyChar)
        {
        }

        internal override T Accept<T>(RegularExpressionConverter<T> converter)
        {
            return converter.ConvertAnyChar(this);
        }
    }
}
