using RegularExpressionEngine.Parser;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace RegularExpressionEngine.NFA
{
    class NFAConverter : RegularExpressionConverter<NFAModel>
    {
        public NFAConverter()
        {
        }

        public override NFAModel ConvertAlternation(AlternationExpression exp)
        {
            var nfa1 = Convert(exp.Expression1);
            var nfa2 = Convert(exp.Expression2);

            NFAState head = new NFAState();
            NFAState tail = new NFAState();

            head.AddEdge(nfa1.EntryEdge);
            head.AddEdge(nfa2.EntryEdge);

            nfa1.TailState.AddEmptyEdgeTo(tail);
            nfa2.TailState.AddEmptyEdgeTo(tail);

            NFAModel alternationNfa = new NFAModel();
            alternationNfa.AddState(head);
            alternationNfa.AddStates(nfa1.States);
            alternationNfa.AddStates(nfa2.States);
            alternationNfa.AddState(tail);

            alternationNfa.EntryEdge = new NFAEdge(head);
            alternationNfa.TailState = tail;

            return alternationNfa;
        }

        public override NFAModel ConvertSymbol(SymbolExpression exp)
        {
            NFAState tail = new NFAState();
            NFAEdge entryEdge = new NFAEdge(exp.Symbol, tail);
            
            NFAModel symbolNfa = new NFAModel();

            symbolNfa.AddState(tail);
            symbolNfa.TailState = tail;
            symbolNfa.EntryEdge = entryEdge;

            return symbolNfa;
        }

        public override NFAModel ConvertEmpty(EmptyExpression exp)
        {
            NFAState tail = new NFAState();
            NFAEdge entryEdge = new NFAEdge(tail);
            NFAModel emptyNfa = new NFAModel();
            emptyNfa.AddState(tail);
            emptyNfa.TailState = tail;
            emptyNfa.EntryEdge = entryEdge;

            return emptyNfa;
        }

        public override NFAModel ConvertConcatenation(ConcatenationExpression exp)
        {
            var leftNfa = Convert(exp.Left);
            var rightNfa = Convert(exp.Right);

            leftNfa.TailState.AddEdge(rightNfa.EntryEdge);

            var concatNfa = new NFAModel();
            concatNfa.AddStates(leftNfa.States);
            concatNfa.AddStates(rightNfa.States);

            concatNfa.TailState = rightNfa.TailState;
            concatNfa.EntryEdge = leftNfa.EntryEdge;

            return concatNfa;
        }

        public override NFAModel ConvertAlternationCharSet(AlternationCharSetExpression exp)
        {
            NFAState head = new NFAState();
            NFAState tail = new NFAState();

            NFAModel charSetNfa = new NFAModel();

            charSetNfa.AddState(head);

            foreach (var symbol in exp.CharSet)
            {
                var symbolEdge = new NFAEdge(symbol, tail);
                head.AddEdge(symbolEdge);
            }
            charSetNfa.AddState(tail);
            charSetNfa.EntryEdge = new NFAEdge(head);
            charSetNfa.TailState = tail;

            return charSetNfa;
        }

        public override NFAModel ConvertStringLiteral(StringLiteralExpression exp)
        {
            NFAModel literalNfa = new NFAModel();
            NFAState lastState = null;

            foreach (var symbol in exp.Literal)
            {
                var symbolState = new NFAState();
                var symbolEdge = new NFAEdge(symbol, symbolState);

                if (lastState != null)
                {
                    lastState.AddEdge(symbolEdge);
                }
                else
                {
                    literalNfa.EntryEdge = symbolEdge;
                }
                lastState = symbolState;
                literalNfa.AddState(symbolState);
            }
            literalNfa.TailState = lastState;
            return literalNfa;
        }

        public override NFAModel ConvertKleeneStar(KleeneStarExpression exp)
        {
            var innerDfa = Convert(exp.InnerExpression);
            var newTail = new NFAState();
            var entry = new NFAEdge(newTail);

            innerDfa.TailState.AddEmptyEdgeTo(newTail);
            newTail.AddEdge(innerDfa.EntryEdge);

            var kleeneStarNfa = new NFAModel();
            kleeneStarNfa.AddStates(innerDfa.States);
            kleeneStarNfa.AddState(newTail);
            kleeneStarNfa.EntryEdge = entry;
            kleeneStarNfa.TailState = newTail;
            
            return kleeneStarNfa;
        }

        public override NFAModel ConvertAssertion(AssertionExpression exp)
        {
            NFAState tail = new NFAState();
            NFAEdge entryEdge = new NFAAssertEdge(exp.Symbol, tail);

            NFAModel assertionNfa = new NFAModel();

            assertionNfa.AddState(tail);
            assertionNfa.TailState = tail;
            assertionNfa.EntryEdge = entryEdge;

            return assertionNfa;
        }

        public override NFAModel ConvertAnyChar(AnyCharExpression exp)
        {
            NFAState tail = new NFAState();
            NFAEdge entryEdge = new NFAAnyCharEdge(exp.Symbol, tail);

            NFAModel anyNFA = new NFAModel();

            anyNFA.AddState(tail);
            anyNFA.TailState = tail;
            anyNFA.EntryEdge = entryEdge;

            return anyNFA;
        }

        public override NFAModel ConvertCharacterEscape(CharacterEscapeExpression exp)
        {
            NFAState tail = new NFAState();
            NFAEdge entryEdge = new NFACharacterEscapeEdge(exp.Symbol, tail);

            NFAModel nfa = new NFAModel();

            nfa.AddState(tail);
            nfa.TailState = tail;
            nfa.EntryEdge = entryEdge;
            return nfa;
        }

        public override NFAModel ConvertIdentityEscape(IdentityEscapeExpression exp)
        {
            NFAState tail = new NFAState();
            NFAEdge entryEdge = new NFAIdentityEscapeEdge(exp.Symbol, tail);

            NFAModel nfa = new NFAModel();

            nfa.AddState(tail);
            nfa.TailState = tail;
            nfa.EntryEdge = entryEdge;
            return nfa;
        }

        public override NFAModel ConvertSubExpr(SubExpression exp)
        {
            var subNFA = Convert(exp.SubExpr);
            NFAState head = new NFAState();
            NFAState tail = new NFAState();
            //NFAState subState = new NFAState();

            head.AddEdge(subNFA.EntryEdge);
            var endSubEdge = new NFASubExprEdge(SpecialChar.ParenthesisRight, exp.Index, tail);
            subNFA.TailState.AddEdge(endSubEdge);
            var beginSubEdge = new NFASubExprEdge(SpecialChar.ParenthesisLeft, exp.Index, head);
            //subState.AddEdge(beginSubEdge);

            NFAModel nfa = new NFAModel();
            nfa.AddState(head);
            nfa.AddStates(subNFA.States);
            nfa.AddState(tail);
            nfa.TailState = tail;
            nfa.EntryEdge = beginSubEdge;

            return nfa;
        }

        public override NFAModel ConvertQuantifierExpr(QuantiferExpression exp)
        {
            var innerDfa = Convert(exp.InnerExpression);
            var endTail = new NFAState();
            var newTail = new NFAState();
            var midState = new NFAState();
            var headState = new NFAState();

            var entry = new NFAEdge(headState);

            var quantifierEdge = new NFAQuantifierEdge(exp.Min, exp.Max, exp.IsGreedy, midState, newTail);
            headState.AddEdge(quantifierEdge);
            midState.AddEdge(innerDfa.EntryEdge);
            innerDfa.TailState.AddEmptyEdgeTo(headState);

            //innerDfa.TailState.AddEmptyEdgeTo(newTail);
            //newTail.AddEdge(innerDfa.EntryEdge);
            newTail.AddEmptyEdgeTo(endTail);

            var nfa = new NFAModel();
            nfa.AddState(headState);
            nfa.AddState(midState);
            nfa.AddStates(innerDfa.States);
            nfa.AddState(newTail);
            nfa.AddState(endTail);
            nfa.EntryEdge = entry;
            nfa.TailState = endTail;

            return nfa;
        }

        public override NFAModel ConvertCharacterClassExpr(CharacterClassExpression exp)
        {
            NFAState tail = new NFAState();
            NFAEdge entryEdge = new CharacterClassExprEdge(exp.CharList, exp.IsNot, tail);

            NFAModel NFA = new NFAModel();

            NFA.AddState(tail);
            NFA.TailState = tail;
            NFA.EntryEdge = entryEdge;
            return NFA;
        }
    }
}
