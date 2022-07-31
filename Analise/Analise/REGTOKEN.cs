namespace Analise
{
    /// <summary>
    /// Defines the <see cref="REGTOKEN" />.
    /// </summary>
    internal class REGTOKEN
    {
        /// <summary>
        /// Defines the _estado.
        /// </summary>
        internal int _estado;

        /// <summary>
        /// Defines the _valor.
        /// </summary>
        internal double _valor;

        /// <summary>
        /// Defines the _lin.
        /// </summary>
        internal int _lin;

        /// <summary>
        /// Defines the _col.
        /// </summary>
        internal int _col;

        /// <summary>
        /// Defines the _nomefunc.
        /// </summary>
        internal string _nomefunc;

        /// <summary>
        /// Gets or sets the col.
        /// </summary>
        public int col
        {
            get
            {
                return _col;
            }
            set
            {
                _col = value;
            }
        }

        /// <summary>
        /// Gets or sets the lin.
        /// </summary>
        public int lin
        {
            get
            {
                return _lin;
            }
            set
            {
                _lin = value;
            }
        }

        /// <summary>
        /// Gets or sets the estado.
        /// </summary>
        public int estado
        {
            get
            {
                return _estado;
            }
            set
            {
                _estado = value;
            }
        }

        /// <summary>
        /// Gets or sets the valor.
        /// </summary>
        public double valor
        {
            get
            {
                return _valor;
            }
            set
            {
                _valor = value;
            }
        }

        /// <summary>
        /// Gets or sets the nomefunc.
        /// </summary>
        public string nomefunc
        {
            get
            {
                return _nomefunc;
            }
            set
            {
                _nomefunc = value;
            }
        }
    }
}
