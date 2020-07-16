using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace RegularExpressionEngine.NFA
{
    public class CompactCharSetManager
    {
        private readonly ushort _maxIndex;
        private readonly ushort _minIndex = 1;

        private ushort[] _compactCharTable;

        public CompactCharSetManager(ushort[] compactCharTable, ushort maxIndex)
        {
            _compactCharTable = compactCharTable;
            Debug.Assert(maxIndex >= _maxIndex);
            _maxIndex = maxIndex;
        }

        public int MaxClassIndex
        {
            get
            {
                return _maxIndex;
            }
        }

        public int MinClassIndex
        {
            get
            {
                return _minIndex;
            }
        }

        public int GetCompactClass(char c)
        {
            ushort compactClass = _compactCharTable[(int)c];
            Debug.Assert(compactClass >= _minIndex);
            return (int)compactClass;
        }

        public bool HasCompactClass(char c)
        {
            return _compactCharTable[c] >= _minIndex;
        }

        public HashSet<char>[] CreateCompactCharMapTable()
        {
            HashSet<char>[] result = new HashSet<char>[_maxIndex + 1];
            for (int i = 0; i <= _maxIndex; i++)
            {
                result[i] = new HashSet<char>();
            }
            for (int i = Char.MinValue; i < Char.MaxValue; i++)
            {
                int index = _compactCharTable[i];
                result[index].Add((char)i);
            }
            return result;
        }
    }
}
