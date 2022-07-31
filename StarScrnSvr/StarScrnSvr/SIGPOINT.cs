using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace StarScrnSvr
{
    internal class SIGPOINT
    {
        private int _x, _y;

        public int x
        {
            get
            {
                return this._x;
            }
            set
            {
                this._x = value;
            }
        }


        public int y
        {
            get
            {
                return this._y;
            }
            set
            {
                this._y = value;
            }
        }
    }
}

