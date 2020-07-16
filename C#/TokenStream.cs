using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Calculator
{
    class TokenStream
    {
        private BinaryReader _br;
        private MemoryStream stream;
        public bool IsFull = false;
        private Token buffer;

        public TokenStream(string s)
        {

            byte[] array = Encoding.ASCII.GetBytes(s);
            stream = new MemoryStream(array);             //convert stream 2 string      
            //StreamReader reader = new StreamReader(stream);
            _br = new BinaryReader(stream);
            
        }

        ~TokenStream()
        {
            if (_br != null)
            {
                _br.Close();
                stream.Close();
            }
        }

        public Token Get()
        {
            if (IsFull)
            {
                IsFull = false;
                return buffer;
            }
            int ch = _br.PeekChar();
            switch (ch)
            {
                case '(':
                case ')':
                case '+':
                case '-':
                case '*':
                case '/':
                    char c = _br.ReadChar();
                    return new Token(c);
                case '0':case '1':case '2':case '3':case '4':
                case '5':case '6':case '7':case '8':case '9':
                    StringBuilder sb = new StringBuilder();
                    while (ch >= '0' && ch <= '9')
                    {

                        sb.Append(_br.ReadChar());
                        ch = _br.PeekChar();
                    }
                    var d = double.Parse(sb.ToString());
                    return new Token('8', d);
                default:
                    return null;
            }
        }

        public bool PutBack(Token token)
        {
            if (IsFull) return false;
            IsFull = true;
            buffer = token;
            return true;
        }


    }
}
