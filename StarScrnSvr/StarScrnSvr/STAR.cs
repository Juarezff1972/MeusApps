using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Drawing;

namespace StarScrnSvr
{
    internal class STAR
    {
        private UPOINT _Loc;
        private double _Dist;
        private SIGPOINT _Dir;
        private int _Size;
        private Color _Color;

        public STAR()
        {
            _Loc = new UPOINT();
            _Dir = new SIGPOINT();
        }

        public UPOINT Loc
        {
            get
            {
                return this._Loc;
            }
            set
            {
                this._Loc = value;
            }
        }

        public double Dist
        {
            get
            {
                return this._Dist;
            }
            set
            {
                this._Dist = value;
            }
        }

        public SIGPOINT Dir
        {
            get
            {
                return this._Dir;
            }
            set
            {
                this._Dir = value;
            }
        }

        public int Size
        {
            get
            {
                return this._Size;
            }
            set
            {
                this._Size = value;
            }
        }

        public Color Clr
        {
            get
            {
                return this._Color;
            }
            set
            {
                this._Color = value;
            }
        }
    }

}
