using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace StarScrnSvr
{
    internal class UPOINT
    {
        private long _x, _y;

        public long x
        {
            get
            {
                return this._x;
            }
            set
            {
                /*if (value < 0)
                    value = 65535;*/
                this._x = value;
            }
        }


        public long y
        {
            get
            {
                return this._y;
            }
            set
            {
                /*if (value < 0)
                    value = 65535;*/
                this._y = value;
            }
        }
    }
}
