
namespace RegularExpressionEngine
{
    public enum RegularExpressionType
    {
        Empty,
        Symbol,
        StringLiteral,
        Alternation,
        AlternationCharSet,
        Concatenation,
        KleeneStar,
        Assertion,
        AnyChar,
        CharacterEscape,
        IdentityEscape,
        SubExpression,
        Quantifier,
        CharacterClass,
    }
}
