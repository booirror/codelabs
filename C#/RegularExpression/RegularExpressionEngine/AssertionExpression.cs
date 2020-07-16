
using System;
using System.Collections.Generic;
using System.Globalization;
namespace RegularExpressionEngine
{
    public class AssertionExpression : SymbolExpression
    {
        public AssertionExpression(char symbol)
            : base(symbol, RegularExpressionType.Assertion)
        {
        }

        internal override T Accept<T>(RegularExpressionConverter<T> converter)
        {
            return converter.ConvertAssertion(this);
        }
    }
}
