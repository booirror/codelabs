
namespace RegularExpressionEngine
{
    public abstract class RegularExpressionConverter<T>
    {
        protected RegularExpressionConverter() { }

        public T Convert(RegularExpression expression)
        {
            if (expression == null)
            {
                return default(T);
            }
            return expression.Accept(this);
        }

        public abstract T ConvertAlternation(AlternationExpression exp);

        public abstract T ConvertSymbol(SymbolExpression exp);

        public abstract T ConvertEmpty(EmptyExpression exp);

        public abstract T ConvertConcatenation(ConcatenationExpression exp);

        public abstract T ConvertAlternationCharSet(AlternationCharSetExpression exp);

        public abstract T ConvertStringLiteral(StringLiteralExpression exp);

        public abstract T ConvertKleeneStar(KleeneStarExpression exp);

        public abstract T ConvertAssertion(AssertionExpression exp);

        public abstract T ConvertAnyChar(AnyCharExpression exp);

        public abstract T ConvertCharacterEscape(CharacterEscapeExpression exp);

        public abstract T ConvertIdentityEscape(IdentityEscapeExpression exp);

        public abstract T ConvertSubExpr(SubExpression exp);

        public abstract T ConvertQuantifierExpr(QuantiferExpression exp);

        public abstract T ConvertCharacterClassExpr(CharacterClassExpression exp);
    }
}
