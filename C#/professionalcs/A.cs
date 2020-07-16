using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Inheritance
{
    class A
    {
        public virtual void p()
        {
            Console.WriteLine("A->p");
        }
    }

    class B : A
    {
        public void p()
        {
            Console.WriteLine("B default->p");
        }
    }

    class C:A
    {
        public new void p()
        {
            Console.WriteLine("C new ->p");
        }
    }

    class D : A
    {
        public override void p()
        {
            Console.WriteLine("D override ->p");
        }
    }

    class E : A
    {

        public void p()
        {
            Console.WriteLine("E ->p");
        }
    }

    class AT
    {
        static public void Test()
        {
            A b = new B();
            b.p();

            A c = new C();
            c.p();

            A d = new D();
            d.p();

            A e = new E();
            e.p();

        }
    }
}
