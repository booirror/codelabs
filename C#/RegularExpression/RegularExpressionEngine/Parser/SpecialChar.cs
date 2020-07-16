using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace RegularExpressionEngine.Parser
{
    public static class SpecialChar
    {
        public const char Any = '.';
        public const char Not = '^';
        public const char AssertBegin = '^';
        public const char AssertEnd = '$';
        public const char Escape = '\\';
        public const char ParenthesisLeft = '(';
        public const char ParenthesisRight = ')';
        public const char BraceLeft = '{';
        public const char BraceRight = '}';
        public const char Star = '*';
        public const char Plus = '+';
        public const char Question = '?';
        public const char BracketLeft = '[';
        public const char BracketRight = ']';
        public const char Dash = '-';

        public const string NoPatternChar = "^$\\.*+?()[]{}|";

        public const string ControlEscape = "fnrtv";
        public const string ControlEscapeStr = "\f\n\r\t\v";

        public const string IdentityEscape = ".\\";

        public const string Quantifiers = "*+?{";

        public const string StopChar = "|)*";

        public const string CharClassEscape = "dDsSwW";

        public static bool IsWhiteSpace()
        {
            throw new NotImplementedException();
        }

        public static bool IsNewLine(char c)
        {
            return c == '\n' || c == '\r';
        }
    }
}
