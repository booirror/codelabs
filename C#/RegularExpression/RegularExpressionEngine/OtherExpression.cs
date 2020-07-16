using RegularExpressionEngine.Common;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace RegularExpressionEngine
{
    public class CharacterEscapeExpression : SymbolExpression
    {
        public CharacterEscapeExpression(char symbol)
            : base(symbol, RegularExpressionType.CharacterEscape)
        {
        }

        public override string ToString()
        {
            return "\\" + Symbol;
        }

        internal override T Accept<T>(RegularExpressionConverter<T> converter)
        {
            return converter.ConvertCharacterEscape(this);
        }
    }

    public class IdentityEscapeExpression : SymbolExpression
    {
        public IdentityEscapeExpression(char symbol)
            : base(symbol, RegularExpressionType.IdentityEscape)
        {
        }

        public override string ToString()
        {
            return "\\" + Symbol;
        }

        internal override T Accept<T>(RegularExpressionConverter<T> converter)
        {
            return converter.ConvertIdentityEscape(this);
        }
    }

    public class SubExpression : RegularExpression
    {
        public SubExpression(RegularExpression subExpr, int idx)
            : base(RegularExpressionType.SubExpression)
        {
            SubExpr = subExpr;
            Index = idx;
        }
        public RegularExpression SubExpr { get; private set; }

        public int Index { get; private set; }
        public override string ToString()
        {
            return "(" + SubExpr.ToString() + ")";
        }
        internal override Func<HashSet<char>>[] GetCompactableCharSets()
        {
            return SubExpr.GetCompactableCharSets();
        }

        internal override HashSet<char> GetUncompactableCharSet()
        {
            return SubExpr.GetUncompactableCharSet();
        }
        internal override T Accept<T>(RegularExpressionConverter<T> converter)
        {
            return converter.ConvertSubExpr(this);
        }
    }

    public class QuantiferExpression : RegularExpression
    {
        public QuantiferExpression(RegularExpression innerExp, int min, int max, bool isGreedy)
            : base(RegularExpressionType.Quantifier)
        {
            CodeContract.RequiresArgumentNotNull(innerExp, "innerExp");

            InnerExpression = innerExp;

            this.Min = min;
            this.Max = max;
            this.IsGreedy = isGreedy;
        }

        public int Min { get; private set; }
        public int Max { get; private set; } // -1 表示无穷大

        public bool IsGreedy { get; private set; }
        public RegularExpression InnerExpression { get; private set; }

        public override string ToString()
        {
            return '(' + InnerExpression.ToString() + ')' + "{ " + Min + ", " + Max + "}" + (IsGreedy ? "?" : "");
        }

        internal override Func<HashSet<char>>[] GetCompactableCharSets()
        {
            return InnerExpression.GetCompactableCharSets();
        }

        internal override HashSet<char> GetUncompactableCharSet()
        {
            return InnerExpression.GetUncompactableCharSet();
        }

        internal override T Accept<T>(RegularExpressionConverter<T> converter)
        {
            return converter.ConvertQuantifierExpr(this);
        }
    }

    public class CharacterClassExpression : RegularExpression
    {
        public CharacterClassExpression(List<char> list)
            : this(list, false)
        {
        }

        public CharacterClassExpression(List<char> list, bool notInList)
            : base(RegularExpressionType.CharacterClass)
        {
            CharList = list;
            IsNot = notInList;
        }

        internal override Func<HashSet<char>>[] GetCompactableCharSets()
        {
            return new Func<HashSet<char>>[0];
        }

        internal override HashSet<char> GetUncompactableCharSet()
        {
            return new HashSet<char>(CharList);
        }

        internal override T Accept<T>(RegularExpressionConverter<T> converter)
        {
            return converter.ConvertCharacterClassExpr(this);
        }

        public List<char> CharList { get; private set; }

        public bool IsNot { get; private set; }
    }
}
