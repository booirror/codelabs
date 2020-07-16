using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Calculator
{
    class Token
    {
        public char Kind { get; private set; }
        public double Value {get; private set;}
        public Token(char k, double v)
        {
            Kind = k;
            Value = v;
        }

        public Token(char k)
        {
            Kind = k;
        }
    }
}
