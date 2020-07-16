using System;
using System.Collections.Generic;
using System.Globalization;

namespace RegularExpressionEngine
{
    public class SymbolExpression : RegularExpression
    {
        public SymbolExpression(char symbol):base(RegularExpressionType.Symbol)
        {
            Symbol = symbol;
        }

        public SymbolExpression(char symbol, RegularExpressionType type)
            : base(type)
        {
            Symbol = symbol;
        }

        public new char Symbol { get; private set; }

        public override string ToString()
        {
            return Symbol.ToString(CultureInfo.InvariantCulture);
        }

        internal override Func<HashSet<char>>[] GetCompactableCharSets()
        {
            return new Func<HashSet<char>>[0];
        }

        internal override HashSet<char> GetUncompactableCharSet()
        {
            return new HashSet<char> { Symbol };
        }

        internal override T Accept<T>(RegularExpressionConverter<T> converter)
        {
            return converter.ConvertSymbol(this);
        }
    }
}
