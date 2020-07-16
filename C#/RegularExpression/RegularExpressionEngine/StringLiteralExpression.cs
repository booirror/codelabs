
using System;
using System.Collections.Generic;

namespace RegularExpressionEngine
{
    public sealed class StringLiteralExpression : RegularExpression
    {
        public StringLiteralExpression(string literal)
            : base(RegularExpressionType.StringLiteral)
        {
            Literal = literal;
        }

        public new string Literal { get; private set; }

        public override string ToString()
        {
            return Literal;
        }

        internal override Func<HashSet<char>>[] GetCompactableCharSets()
        {
            return new Func<HashSet<char>>[0];
        }

        internal override HashSet<char> GetUncompactableCharSet()
        {
            return new HashSet<char>(Literal);
        }

        internal override T Accept<T>(RegularExpressionConverter<T> converter)
        {
            return converter.ConvertStringLiteral(this);
        }
    }
}
