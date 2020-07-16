using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace RegularExpressionEngine.Parser
{

    public class CCData
    {
        public List<char> Charset;
        public bool IsNot;

        public CCData(List<char> c, bool b)
        {
            Charset = c;
            IsNot = b;
        }

        public CCData(char c)
            : this(new List<char> { c}, false)
        {
        }
        
        public Func<char, bool> ToPredicate()
        {
            return x=>{
                foreach(var c in Charset) {
                    if (c == x) return !IsNot;
                }
                return IsNot;
            };
        }
    }

    public enum AtomType {
        kSource,
        kCharClass,
    }
    public class Atom
    {
        public AtomType Type;
        public char Symbol;
        public Atom(AtomType type, char c)
        {
            Type = type;
            Symbol = c;
        }
    }

    public class RegexParser
    {
        private string _regex;
        private int _idx;
        private int _subIdx = 0;
        private Stack<int> _stack;
        public RegexParser(string pattern)
        {
            _regex = pattern;
            _idx = 0;
            _stack = new Stack<int>();
        }
        public RegularExpression Parse()
        {
            return Disjunction();
        }

        public int SubNum { get { return _subIdx; }}
        private RegularExpression Disjunction()
        {
            //http://en.cppreference.com/w/cpp/regex/ecmascript
            //A regular expression pattern is sequence of one or more Alternatives
            //Disjunction ::
            //Alternative
            //Alternative | Disjunction
            var alterExpr = Alternative();
            if (!IsEnd() && PeekNextChar() == '|')
            {
                this.Consume();
                var expr = Disjunction();
                if (alterExpr == null)
                {
                    return expr;
                }
                else
                {
                    return new AlternationExpression(alterExpr, expr);
                }
            }
            else
            {
                return alterExpr;
            }
        }

        private RegularExpression Alternative()
        {
            //Each Alternative is either empty or is a sequence of Terms (with no separators between the Terms)
            //Alternative ::
            //[empty]
            //Alternative Term
            if (IsEnd())
            {
                return EmptyExpression.Instance;
            }
            else
            {
                var term = Term();
                if (term == null)
                {
                    return null;
                }
                var expr = Alternative();
                if (expr == null)
                {
                    return term;
                }
                else
                {
                    return term.Concat(expr);
                }
            }
        }

        private RegularExpression Term()
        {
            //Term ::
            //Assertion
            //Atom
            //Atom Quantifier
            var expr = Assertion();
            if (expr != null)
            {
                return expr;
            }
            expr = Atom();
            if (expr != null)
            {
                if (IsEnd())
                {
                    return expr;
                }
                else
                {
                    if (SpecialChar.Quantifiers.Contains(PeekNextChar()))
                    {
                        int min, max;
                        Quantifiers(out min, out max);
                        bool isGreedy = true;
                        if (!IsEnd() && PeekNextChar() == '?')
                        {
                            this.Consume();
                            isGreedy = false;
                        }
                        return new QuantiferExpression(expr, min, max, isGreedy);
                    }
                    else
                    {
                        return expr;
                    }
                }
            }
            if (SpecialChar.StopChar.Contains(PeekNextChar()))
            {
                return null;
            }
            throw new NotImplementedException(ErrorInfo("Can't parse this"));
        }

        private void Quantifiers(out int min, out int max)
        {
            min = 0;
            max = 0;
            var c = NextChar();
            if (c == SpecialChar.Star)
            {
                max = -1;
            }
            else if (c == SpecialChar.Plus)
            {
                min = 1;
                max = -1;
            }
            else if (c == SpecialChar.Question)
            {
                max = 1;
            }
            else
            {
                int end = _regex.IndexOf('}');
                if (end == -1)
                {
                    throw new Exception(ErrorInfo("Syntax Error"));
                }
                else
                {
                    int dot = _regex.IndexOf(',');
                    if (dot != -1 && dot < end)
                    {
                        var num1 = _regex.Substring(_idx, dot - _idx);
                        var num2 = _regex.Substring(dot+1, end - dot - 1);
                        if (num2.Length == 0)
                        {
                            min = int.Parse(num1);
                            max = -1;
                        }
                        else
                        {
                            min = int.Parse(num1);
                            max = int.Parse(num2);
                        }
                    }
                    else
                    {
                        var num = _regex.Substring(_idx, end - _idx);
                        min = max = int.Parse(num);
                    }
                    _idx = end + 1;
                }
            }
        }

        private RegularExpression SubExpr()
        {
            if (!Expect(SpecialChar.ParenthesisLeft))
            {
                return null;
            }
            this.Consume();
            var subexpr = Disjunction();
            if (!Expect(SpecialChar.ParenthesisRight))
            {
                throw new Exception(ErrorInfo("Expect ')' here"));
            }
            this.Consume();
            return new SubExpression(subexpr, _stack.Peek());
        }

        private RegularExpression Atom()
        {
            //Atom ::
            //PatternCharacter
            //.
            //\ AtomEscape
            //CharacterClass
            //( Disjunction )
            //( ? : Disjunction )
            //where AtomEscape ::
            //DecimalEscape
            //CharacterEscape
            //CharacterClassEscape
            //Different kinds of atoms evaluate differently.

            
            if (!SpecialChar.NoPatternChar.Contains(PeekNextChar()))
            {
                ////PatternCharacter
                return new SymbolExpression(NextChar());
            }
            else if (PeekNextChar() == SpecialChar.Any)
            {
                //.
                return new AnyCharExpression(NextChar());
            }
            else if (PeekNextChar() == SpecialChar.BracketLeft)
            {
                //CharacterClass
                return CharacterClass();
            }
            else if (PeekNextChar() == SpecialChar.Escape)
            {
                //DecimalEscape
                //CharacterEscape
                //CharacterClassEscape
                this.Consume();
                var c = PeekNextChar();
                int cidx = SpecialChar.ControlEscape.IndexOf(c);
                if (cidx != -1)
                {
                    this.Consume();
                    return new CharacterEscapeExpression(SpecialChar.ControlEscapeStr[cidx]);
                } else if (SpecialChar.IdentityEscape.Contains(c))
                {
                    //IdentityEscape can be any non-alphanumeric character
                    this.Consume();
                    return new IdentityEscapeExpression(c);
                }
                else if (SpecialChar.CharClassEscape.Contains(c))
                {
                    this.Consume();
                    var ccdata = PreCharacterClassEscape(c);
                    return CCData2Expr(ccdata);
                }
                else
                {
                    return null;
                }
            }
            else if (Expect(SpecialChar.ParenthesisLeft)) {
                _stack.Push(_subIdx++);
                var expr = SubExpr();
                _stack.Pop();
                return expr;
            }
            else
            {
                return null;
            }
        }

        private RegularExpression CCData2Expr(CCData ccdata)
        {
            if ((ccdata.Charset.Count > Char.MaxValue / 2 && !ccdata.IsNot)
                || (ccdata.Charset.Count < Char.MaxValue / 2 && ccdata.IsNot))
            {
                //当字符超过一半时，进行优化
                List<char> newList = new List<char>();
                bool[] isin = new bool[char.MaxValue - char.MinValue];
                foreach(char c in ccdata.Charset)
                {
                    isin[char.MinValue + c] = true;
                }
                for (int i = char.MinValue; i < char.MaxValue; i++)
                {
                    if (!isin[i])
                    {
                        newList.Add((char)i);
                    }
                }
                return new CharacterClassExpression(newList, !ccdata.IsNot);
            }
            return RegularExpression.CharsOf(ccdata.ToPredicate());
        }

        private RegularExpression CharacterClass()
        {
            //CharacterClass ::
            //[ [ lookahead ∉ {^}] ClassRanges ]
            //[ ^ ClassRanges ]
            
            //ClassAtom - ClassAtom ClassRanges
            this.Consume(); // '['
            bool isNot = false;
            if (PeekNextChar() == SpecialChar.Not)
            {
                this.Consume();
                isNot = true;
            }
            List<CCData> list = ClassRanges();
            if (!Expect(SpecialChar.BracketRight))
            {
                throw new Exception(ErrorInfo("Expect ]"));
            }
            this.Consume();
            var set = MergeData(list);
            CCData ccd = new CCData(set, isNot);
            return CCData2Expr(ccd);
        }

        private List<char> MergeData(List<CCData> list)
        {
            List<char> charSet = new List<char>();
            foreach (var data in list)
            {
                var predicate = data.ToPredicate();
                for (int i = Char.MinValue; i < Char.MaxValue; i++)
                {
                    if (predicate((char)i))
                    {
                        charSet.Add((char)i);
                    }
                }
            }
            return charSet;
        }

        private List<CCData> ClassRanges()
        {
            //ClassRanges ::
            //[empty]
            //NonemptyClassRanges
            //这段代码其实可以合到NonemptyClassRanges里
            List<CCData> set = new List<CCData>();
            if (PeekNextChar() == SpecialChar.BracketRight)
            {
                //empty
                return set;
            }
            return NonemptyClassRanges();
        }

        private List<CCData> NonemptyClassRanges()
        {
            //NonemptyClassRanges ::
            //ClassAtom
            //ClassAtom NonemptyClassRangesNoDash
            //ClassAtom - ClassAtom ClassRanges
            List<CCData> set = new List<CCData>();
            while (PeekNextChar() != SpecialChar.BracketRight)
            {
                Atom atom = ClassAtom();
                if (PeekNextChar() == SpecialChar.Dash && atom.Type != AtomType.kCharClass)
                {
                    this.Consume();
                    Atom atom2 = ClassAtom();
                    if (atom2.Type == AtomType.kCharClass)
                    {
                        throw new Exception(ErrorInfo("expect source char"));
                    }
                    if (atom2.Symbol < atom.Symbol)
                    {
                        throw new Exception(ErrorInfo("[x-y] x, y should swap location"));
                    }
                    List<char> list = new List<char>();
                    for (int i = atom.Symbol; i <= atom2.Symbol; i++)
                    {
                        list.Add((char)i);
                    }
                    set.Add(new CCData(list, false));
                }
                else
                {
                    if (atom.Type == AtomType.kCharClass)
                    {
                        var data = PreCharacterClassEscape(atom.Symbol);
                        set.Add(data);
                    }
                    else
                    {
                        CCData data = new CCData(atom.Symbol);
                        set.Add(data);
                    }
                }
            }
            return set;
        }

        private CCData PreCharacterClassEscape(char c)
        {
            if (c == 'd' || c == 'D')
            {
                var list = GetPreCharacterClassEscapeSet('d');
                return new CCData(list, c == 'D');
            }
            if (c == 'w' || c == 'W')
            {
                var list = GetPreCharacterClassEscapeSet('w');
                return new CCData(list, c == 'W');
            }
            if (c == 's' || c == 'S')
            {
                var list = GetPreCharacterClassEscapeSet('s');
                return new CCData(list, c == 'S');
            }
            throw new Exception("Error");
        }

        private List<char> GetPreCharacterClassEscapeSet(char c)
        {
            List<char> list = new List<char>();
            if (c == 'd')
            {
                for (int i = '0'; i < '9'; i++)
                {
                    list.Add((char)i);
                }
            }
            else if (c == 's')
            {
                list.AddRange(" \f\n\r\t\v");
            }
            else if (c == 'w')
            {
                for (int i = '0'; i < '9'; i++)
                {
                    list.Add((char)i);
                }
                for (int i = 'a'; i < 'z'; i++)
                {
                    list.Add((char)i);
                }
                for (int i = 'A'; i < 'Z'; i++)
                {
                    list.Add((char)i);
                }
                list.Add('-');
            }
            return list;
        }

        private Atom ClassAtom()
        {
            if (PeekNextChar() == SpecialChar.Escape)
            {
                this.Consume();
                var c = this.NextChar();
                if (SpecialChar.CharClassEscape.Contains(c))
                {
                    return new Atom(AtomType.kCharClass, c);
                }
                else
                {
                    return new Atom(AtomType.kSource, c);
                }
            }
            else
            {
                return new Atom(AtomType.kSource, NextChar());
            }
        }

        private RegularExpression Assertion()
        {
            //Assertions match conditions, rather than substrings of the input string. They never consume any characters from the input. 
            //Each Assertion is one of the following
            //Assertion ::
            //^
            //$
            //\ b
            //\ B
            //( ? = Disjunction )
            //( ? ! Disjunction )
            //这里只实现^ $
            if (Expect(SpecialChar.AssertBegin) || Expect(SpecialChar.AssertEnd))
            {
                char c = NextChar();
                return new AssertionExpression(c);
            }
            else
            {
                return null;
            }
        }

        private bool IsEnd()
        {
            return _idx == _regex.Length;
        }

        private bool Expect(char c)
        {
            return _regex[_idx] == c;
        }

        private void Consume()
        {
            ++_idx;
        }

        private char NextChar()
        {
            return _regex[_idx++];
        }

        private char PeekNextChar()
        {
            return _regex[_idx];
        }

        private string ErrorInfo(string info)
        {
            int start = Math.Max(_idx - 2, 0);
            int idx = Math.Min(_idx, 2);
            int len = Math.Min(6, _regex.Length - start);
            StringBuilder sb = new StringBuilder();
            sb.Append(info);
            sb.Append(":\r\n");
            sb.Append(_regex.Substring(start, len));
            sb.Append("\r\n");
            sb.Append(' ', idx);
            sb.Append('^');
            return sb.ToString();
        }
    }
}
