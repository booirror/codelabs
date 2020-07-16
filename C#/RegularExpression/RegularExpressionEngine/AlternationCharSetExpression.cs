
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;

namespace RegularExpressionEngine
{
    public sealed class AlternationCharSetExpression : RegularExpression
    {
        private List<char> _charSet;

        public AlternationCharSetExpression(IEnumerable<char> charset)
            : base(RegularExpressionType.AlternationCharSet)
        {
            _charSet = new List<char>(charset);
        }

        public new ReadOnlyCollection<char> CharSet
        {
            get
            {
                return _charSet.AsReadOnly();
            }
        }

        public override string ToString()
        {
            if (_charSet.Count == 0)
            {
                return String.Empty;
            }

            return '[' + new String(_charSet.ToArray()) + ']';
        }

        internal override Func<HashSet<char>>[] GetCompactableCharSets()
        {
            return new Func<HashSet<char>>[] { () => new HashSet<char>(_charSet) };
        }

        internal override HashSet<char> GetUncompactableCharSet()
        {
            return new HashSet<char>();
        }

        internal override T Accept<T>(RegularExpressionConverter<T> converter)
        {
            return converter.ConvertAlternationCharSet(this);
        }
    }
}
