using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace StarScrnSvr
{
    internal class g
    {
        private STAR[] _Stars;
        private int _NumStars;
        private UPOINT _Screen;
        private int _Speed;

        public STAR[] Stars
        {
            get
            {
                return this._Stars;
            }
            set
            {
                this._Stars = value;
            }
        }

        public int NumStars
        {
            get
            {
                return this._NumStars;
            }
            set
            {
                this._NumStars = value;
            }
        }

        public UPOINT Screen
        {
            get
            {
                return this._Screen;
            }
            set
            {
                this._Screen = value;
            }
        }

        public int Speed
        {
            get
            {
                return this._Speed;
            }
            set
            {
                this._Speed = value;
            }
        }

        public g()
        {
            _Stars = new STAR[300];
            _Screen = new UPOINT();
        }
    }
}
