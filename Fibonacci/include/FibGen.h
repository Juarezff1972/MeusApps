#ifndef FIBGEN_H
#define FIBGEN_H


class FibGen
{
    public:
        /** Default constructor */
        FibGen();

        /** Access m_Valor
         * \return The current value of m_Valor
         */
        unsigned int GetValor() { return m_Valor; }
        /** Set m_Valor
         * \param val New value to set
         */
        void SetValor(unsigned int val) { m_Valor = val; }

    protected:

    private:
        unsigned int m_Valor; //!< Member variable "m_Valor"
};

#endif // FIBGEN_H
