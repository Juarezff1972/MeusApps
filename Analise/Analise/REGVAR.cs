namespace Analise
{
    /// <summary>
    /// Defines the <see cref="REGVAR" />.
    /// </summary>
    public class REGVAR
    {
        /// <summary>
        /// Defines the _texto.
        /// </summary>
        private string _texto;

        /// <summary>
        /// Defines the _valor.
        /// </summary>
        private double _valor;

        /// <summary>
        /// Gets or sets the texto.
        /// </summary>
        public string texto
        {
            get
            {
                return _texto;
            }
            set
            {
                _texto = value;
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
    }
}
