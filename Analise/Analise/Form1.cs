namespace Analise
{
    using System;
    using System.ComponentModel;
    using System.Drawing;
    using System.Windows.Forms;

    ///////////////////////////////////////////////////////////////////////
    /// <summary>
    /// Defines the <see cref="Form1" />.
    /// </summary>
    public partial class Form1 : Form
    {
        ///////////////////////////////////////////////////////////////////////
        /// <summary>
        /// Defines the VERDADE.
        /// </summary>
        internal const byte VERDADE = 1;

        /// <summary>
        /// Defines the FALSO.
        /// </summary>
        internal const byte FALSO = 0;

        /// <summary>
        /// Defines the MAIS.
        /// </summary>
        internal const byte MAIS = 0;

        /// <summary>
        /// Defines the MENOS.
        /// </summary>
        internal const byte MENOS = 1;

        /// <summary>
        /// Defines the VEZES.
        /// </summary>
        internal const byte VEZES = 2;

        /// <summary>
        /// Defines the DIVIDIR.
        /// </summary>
        internal const byte DIVIDIR = 3;

        /// <summary>
        /// Defines the EXP.
        /// </summary>
        internal const byte EXP = 4;

        /// <summary>
        /// Defines the MODULO.
        /// </summary>
        internal const byte MODULO = 5;

        /// <summary>
        /// Defines the APAREN.
        /// </summary>
        internal const byte APAREN = 6;

        /// <summary>
        /// Defines the FPAREN.
        /// </summary>
        internal const byte FPAREN = 7;

        /// <summary>
        /// Defines the NUM.
        /// </summary>
        internal const byte NUM = 8;

        /// <summary>
        /// Defines the VAR.
        /// </summary>
        internal const byte VAR = 9;

        /// <summary>
        /// Defines the FUNC.
        /// </summary>
        internal const byte FUNC = 10;

        /// <summary>
        /// Defines the EOLN.
        /// </summary>
        internal const byte EOLN = 11;

        /// <summary>
        /// Defines the RUIM.
        /// </summary>
        internal const byte RUIM = 12;

        /// <summary>
        /// Defines the TAMMAXNOMEFUNC.
        /// </summary>
        internal const byte TAMMAXNOMEFUNC = 5;

        /// <summary>
        /// Defines the CALCTAMPILHA.
        /// </summary>
        internal const byte CALCTAMPILHA = 20;

        /// <summary>
        /// Defines the MSGERROPILHA.
        /// </summary>
        internal const string MSGERROPILHA = "\n\rERRO: estouro de pilha.\n\r";

        //const byte MAXCOLS = 1;
        //const byte MAXLINHAS = 1;


        ///////////////////////////////
        /// <summary>
        /// Defines the pilha.
        /// </summary>
        internal REGTOKEN[] pilha = new REGTOKEN[20];//CALCTAMPILHA

        /// <summary>
        /// Defines the token_atual.
        /// </summary>
        internal REGTOKEN token_atual;

        /// <summary>
        /// Defines the topo_pilha, tipo_token.
        /// </summary>
        internal int topo_pilha, tipo_token;

        /// <summary>
        /// Defines the erro.
        /// </summary>
        internal bool erro;

        /// <summary>
        /// Defines the entrada.
        /// </summary>
        internal char[] entrada;

        //char ent;
        /// <summary>
        /// Defines the cnt.
        /// </summary>
        internal int cnt;

        //bool isformula;
        /// <summary>
        /// Defines the fiter.
        /// </summary>
        internal string fiter;

        //int auxptr;
        /// <summary>
        /// Defines the found, pritm.
        /// </summary>
        internal int found = 0, pritm;

        /// <summary>
        /// Defines the xx, yy.
        /// </summary>
        internal double xx, yy;//, zz;

        /// <summary>
        /// Defines the sx, sy, ca.
        /// </summary>
        internal double sx, sy, ca;

        /// <summary>
        /// Defines the prec, scale.
        /// </summary>
        internal double prec, scale;

        /// <summary>
        /// Defines the precalcy.
        /// </summary>
        internal uint[] precalcy = new uint[201];

        /// <summary>
        /// Defines the var.
        /// </summary>
        internal REGVAR var;//MAXCOLS - MAXLINHAS

        /// <summary>
        /// Defines the func.
        /// </summary>
        internal String func;

        /// <summary>
        /// Defines the a, inicio, final, apinic, apfin.
        /// </summary>
        internal double a, inicio, final, apinic, apfin;

        /// <summary>
        /// Defines the bmp.
        /// </summary>
        internal Bitmap bmp;

        internal uint AreaX;
        internal uint AreaY;


        /// <summary>
        /// The ajuste1.
        /// </summary>
        private void ajuste1()
        {
            label3.Visible = false;
            label6.Visible = false;
            label8.Visible = false;
            numIter.Visible = false;
            apinicTxt.Visible = false;
            apfimTxt.Visible = false;
            label4.Text = "Valor inicial:";
            label5.Text = "Valor final:";
        }

        /// <summary>
        /// The ajuste2.
        /// </summary>
        private void ajuste2()
        {
            label3.Visible = true;
            label6.Visible = true;
            label8.Visible = false;
            numIter.Visible = true;
            apinicTxt.Visible = true;
            apfimTxt.Visible = false;
            label4.Text = "Intervalo inicial:";
            label5.Text = "Intervalo final:";
        }

        /// <summary>
        /// The ajuste3.
        /// </summary>
        private void ajuste3()
        {
            label3.Visible = false;
            label6.Visible = true;
            label8.Visible = true;
            numIter.Visible = false;
            apinicTxt.Visible = true;
            apfimTxt.Visible = true;
            label4.Text = "Intervalo inicial:";
            label5.Text = "Intervalo final:";
        }

        /// <summary>
        /// The isalpha.
        /// </summary>
        /// <param name="x">The x<see cref="dynamic"/>.</param>
        /// <returns>The <see cref="bool"/>.</returns>
        internal bool Isalpha(dynamic x)
        {
            if (strchr("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz", x)/*x is string*/)
                return true;

            return false;
        }

        /// <summary>
        /// The isdigit.
        /// </summary>
        /// <param name="x">The x<see cref="dynamic"/>.</param>
        /// <returns>The <see cref="bool"/>.</returns>
        internal bool isdigit(dynamic x)
        {
            if (strchr("0123456789", x))
                return true;

            return false;
        }

        /// <summary>
        /// The formula_inicio.
        /// </summary>
        /// <returns>The <see cref="bool"/>.</returns>
        internal bool formula_inicio()
        {
            int tam, maxtam = 2;
            //char inicio;
            int col;
            int lin;

            if (!Isalpha(entrada[cnt]))
                return (false);

            cnt++;

            if (Isalpha(entrada[cnt]))
                cnt++;

            col = 0;

            if (col >= 1)
                return (false);

            for (tam = 0; tam < maxtam; tam++)
            {
                if (!isdigit(entrada[cnt++]))
                {
                    cnt--;
                    break;
                }
            }

            if (tam == 0)
                return (false);

            lin = 0;

            if ((lin >= 1) || (lin == -1))
                return false;

            return true;
        }

        ///////////////////////////////////////////////////////////////////////
        /// <summary>
        /// The isfunc.
        /// </summary>
        /// <param name="s">The s<see cref="string"/>.</param>
        /// <returns>The <see cref="bool"/>.</returns>
        internal bool isfunc(string s)
        {
            int tam = s.Length;
            string aux = "";
            int i;

            if (tam > entrada.Length) return false;

            if ((cnt + tam - 1) >= entrada.Length) return false;

            for (i = 0; i < tam; i++)
            {
                aux += entrada[cnt + i];
            }

            if (s == aux)
            {
                token_atual.nomefunc = aux;
                token_atual.valor = 0;
                cnt += tam;
                return (true);
            }

            return (false);
        }

        ///////////////////////////////////////////////////////////////////////
        /// <summary>
        /// The strchr.
        /// </summary>
        /// <param name="itens">The itens<see cref="string"/>.</param>
        /// <param name="dest">The dest<see cref="dynamic"/>.</param>
        /// <returns>The <see cref="bool"/>.</returns>
        internal bool strchr(string itens, dynamic dest)
        {
            int i;
            int t;
            char x;
            char y;
            if (dest is string)
            {
                y = dest.ToCharArray()[0];
            }
            else
            {
                if (dest is char)
                {
                    y = dest;
                }
                else
                {
                    y = dest[0];
                }

            }
            t = itens.Length;
            for (i = 0; i < t; i++)
            {
                x = itens.Substring(i, 1).ToCharArray()[0];
                if (x == y)
                    return true;
            }

            return false;
        }

        ///////////////////////////////////////////////////////////////////////
        /// <summary>
        /// The prox_token.
        /// </summary>
        /// <returns>The <see cref="int"/>.</returns>
        internal int prox_token()
        {
            char inicio;
            string numstring;
            int tam, numtam;
            bool decimal1;
            char ent;
            int number;

            if (entrada.Length == cnt)
                return (EOLN);

            if (strchr("0123456789.", entrada[cnt]))
            {
                //inicio = "";

                inicio = entrada[cnt];
                tam = 0;
                decimal1 = false;

                while ((isdigit(entrada[cnt])) || ((entrada[cnt] == '.') && (!decimal1)))
                {
                    if (entrada[cnt] == '.')
                        decimal1 = true;

                    cnt++;
                    tam++;

                    if (entrada.Length == cnt)
                        break;
                }

                if ((tam == 1) && (inicio == '.'))
                    return (RUIM);

                if (entrada.Length != cnt)
                {
                    if (entrada[cnt] == 'E')
                    {
                        cnt++;
                        tam++;

                        if (strchr("+-", entrada[cnt]))
                        {
                            cnt++;
                            tam++;
                        }

                        numtam = 0;

                        while ((isdigit(entrada[cnt])) && (++numtam <= 3))
                        {
                            cnt++;
                            tam++;
                        }
                    }
                }

                numstring = inicio.ToString();
                bool isParsable = Int32.TryParse(numstring, out number);

                if (!isParsable)
                    return (RUIM);
                token_atual.valor = number;

                return (NUM);
            }
            else
            {
                if (Isalpha(entrada[cnt]))
                {
                    if (isfunc("ABS") || isfunc("ACOS") || isfunc("ASEN") ||
                            isfunc("ATAN") || isfunc("COSH") || isfunc("COS") ||
                            isfunc("EA1P") || isfunc("LOG10") || isfunc("LOG") ||
                            isfunc("POT10") || isfunc("ARD") || isfunc("SENH") ||
                            isfunc("SEN") || isfunc("RAIZQ") || isfunc("QUAD") ||
                            isfunc("TANH") || isfunc("TAN") || isfunc("TRUNC"))
                        return (FUNC);

                    if (formula_inicio())
                    {
                        return (VAR);
                    }
                    else
                        return (RUIM);
                }
                else
                {
                    ent = entrada[cnt];
                    cnt++;
                    switch (ent)
                    {
                        case '+':
                            return (MAIS);

                        case '-':
                            return (MENOS);

                        case '*':
                            return (VEZES);

                        case '/':
                            return (DIVIDIR);

                        case '^':
                            return (EXP);

                        case ':':
                            return (MODULO);

                        case '(':
                            return (APAREN);

                        case ')':
                            return (FPAREN);

                        default:
                            return (RUIM);
                    }
                }
            }
        }

        ///////////////////////////////////////////////////////////////////////
        /// <summary>
        /// The copiatoken.
        /// </summary>
        /// <param name="t">The t<see cref="REGTOKEN"/>.</param>
        /// <returns>The <see cref="REGTOKEN"/>.</returns>
        internal REGTOKEN copiatoken(REGTOKEN t)
        {
            REGTOKEN n = new REGTOKEN();
            n.col = t.col;
            n.estado = t.estado;
            n.lin = t.lin;
            n.nomefunc = t.nomefunc;
            n.valor = t.valor;
            return n;
        }

        ///////////////////////////////////////////////////////////////////////
        /// <summary>
        /// The push.
        /// </summary>
        /// <param name="token">The token<see cref="REGTOKEN"/>.</param>
        internal void push(REGTOKEN token)
        {
            if (topo_pilha == CALCTAMPILHA - 1)
            {
                //cprintf(MSGERROPILHA);
                erro = true;
            }
            else
                pilha[++topo_pilha] = copiatoken(token);
        }

        ///////////////////////////////////////////////////////////////////////
        /// <summary>
        /// The pop.
        /// </summary>
        /// <returns>The <see cref="REGTOKEN"/>.</returns>
        internal REGTOKEN pop()
        {
            return (pilha[topo_pilha--]);
        }

        ///////////////////////////////////////////////////////////////////////
        /// <summary>
        /// The vai_pra_estado.
        /// </summary>
        /// <param name="producao">The producao<see cref="int"/>.</param>
        /// <returns>The <see cref="int"/>.</returns>
        internal int vai_pra_estado(int producao)
        {
            int estado = pilha[topo_pilha].estado;

            if (producao <= 3)
            {
                switch (estado)
                {
                    case 0:
                        return (1);

                    case 9:
                        return (19);

                    case 20:
                        return (28);
                }
            }
            else if (producao <= 6)
            {
                switch (estado)
                {
                    case 0:
                    case 9:
                    case 20:
                        return (2);

                    case 12:
                        return (21);

                    case 13:
                        return (22);
                }
            }
            else if (producao <= 8)
            {
                switch (estado)
                {
                    case 0:
                    case 9:
                    case 12:
                    case 13:
                    case 20:
                        return (3);

                    case 14:
                        return (23);

                    case 15:
                        return (24);

                    case 16:
                        return (25);
                }
            }
            else if (producao <= 10)
            {
                switch (estado)
                {
                    case 0:
                    case 9:
                    case 12:
                    case 13:
                    case 14:
                    case 15:
                    case 16:
                    case 20:
                        return (4);
                }
            }
            else if (producao <= 12)
            {
                switch (estado)
                {
                    case 0:
                    case 9:
                    case 12:
                    case 13:
                    case 14:
                    case 15:
                    case 16:
                    case 20:
                        return (6);

                    case 5:
                        return (17);
                }
            }
            else
            {
                switch (estado)
                {
                    case 0:
                    case 5:
                    case 9:
                    case 12:
                    case 13:
                    case 14:
                    case 15:
                    case 16:
                    case 20:
                        return (8);
                }
            }

            return (30);
        }

        ///////////////////////////////////////////////////////////////////////
        /// <summary>
        /// The varvalor.
        /// </summary>
        /// <returns>The <see cref="double"/>.</returns>
        internal double varvalor()
        {
            if (var is null)
                return (0);

            return (var.valor);
        }

        ///////////////////////////////////////////////////////////////////////
        /// <summary>
        /// The desloca.
        /// </summary>
        /// <param name="estado">The estado<see cref="int"/>.</param>
        internal void desloca(int estado)
        {
            token_atual.estado = estado;
            push(token_atual);
            tipo_token = prox_token();
        }

        ///////////////////////////////////////////////////////////////////////
        /// <summary>
        /// The reduza.
        /// </summary>
        /// <param name="reducao">The reducao<see cref="int"/>.</param>
        internal void reduza(int reducao)
        {
            REGTOKEN token1, token2;

            switch (reducao)
            {
                case 1:
                    token1 = pop();
                    pop();
                    token2 = pop();
                    token_atual.valor = token1.valor + token2.valor;
                    break;

                case 2:
                    token1 = pop();
                    pop();
                    token2 = pop();
                    token_atual.valor = token2.valor - token1.valor;
                    break;

                case 4:
                    token1 = pop();
                    pop();
                    token2 = pop();
                    token_atual.valor = token1.valor * token2.valor;
                    break;

                case 5:
                    token1 = pop();
                    pop();
                    token2 = pop();

                    if (Math.Abs(token1.valor) == 0)
                        token_atual.valor = Double.PositiveInfinity;//   HUGE_VAL;
                    else
                        token_atual.valor = token2.valor / token1.valor;

                    break;

                case 7:
                    token1 = pop();
                    pop();
                    token2 = pop();
                    token_atual.valor = Math.Pow(token2.valor, token1.valor);
                    break;

                case 9:
                    token1 = pop();
                    pop();
                    token_atual.valor = -token1.valor;
                    break;

                case 11:
                    token1 = pop();
                    pop();
                    token2 = pop();

                    if (Math.Abs(token1.valor) == 0)
                        token_atual.valor = Double.PositiveInfinity;// HUGE_VAL;
                    else
                        token_atual.valor = (double)((long)token2.valor % (long)token1.valor);

                    break;

                case 13:
                    token_atual = pop();
                    token_atual.valor = varvalor();
                    break;

                case 14:
                    pop();
                    token_atual = pop();
                    pop();
                    break;

                case 16:
                    pop();
                    token_atual = pop();
                    pop();
                    token1 = pop();

                    if (strcmp(token1.nomefunc, "ABS") == 0)
                    {
                        token_atual.valor = Math.Abs(token_atual.valor);
                    }
                    else
                    {
                        if (strcmp(token1.nomefunc, "ACOS") == 0)
                        {
                            token_atual.valor = Math.Acos(token_atual.valor);
                        }
                        else if (strcmp(token1.nomefunc, "ASEN") == 0)
                        {
                            token_atual.valor = Math.Asin(token_atual.valor);
                        }
                        else
                        {
                            if (strcmp(token1.nomefunc, "ATAN") == 0)
                            {
                                token_atual.valor = Math.Atan(token_atual.valor);
                            }
                            else
                            {
                                if (strcmp(token1.nomefunc, "COSH") == 0)
                                {
                                    token_atual.valor = Math.Cosh(token_atual.valor);
                                }
                                else
                                {
                                    if (strcmp(token1.nomefunc, "COS") == 0)
                                    {
                                        token_atual.valor = Math.Cos(token_atual.valor);
                                    }
                                    else
                                    {
                                        if (strcmp(token1.nomefunc, "EA1P") == 0)
                                        {
                                            token_atual.valor = Math.Exp(token_atual.valor);
                                        }
                                        else
                                        {
                                            if (strcmp(token1.nomefunc, "LOG10") == 0)
                                            {
                                                token_atual.valor = Math.Log10(token_atual.valor);
                                            }
                                            else
                                            {
                                                if (strcmp(token1.nomefunc, "LOG") == 0)
                                                {
                                                    token_atual.valor = Math.Log(token_atual.valor);
                                                }
                                                else
                                                {
                                                    if (strcmp(token1.nomefunc, "ARD") == 0)
                                                    {
                                                        token_atual.valor = (int)(token_atual.valor + 0.5);
                                                    }
                                                    else
                                                    {
                                                        if (strcmp(token1.nomefunc, "POT10") == 0)
                                                        {
                                                            token_atual.valor = Math.Pow(token_atual.valor, 10);
                                                        }
                                                        else
                                                        {
                                                            if (strcmp(token1.nomefunc, "SENH") == 0)
                                                            {
                                                                token_atual.valor = Math.Sinh(token_atual.valor);
                                                            }
                                                            else
                                                            {
                                                                if (strcmp(token1.nomefunc, "SEN") == 0)
                                                                {
                                                                    token_atual.valor = Math.Sin(token_atual.valor);
                                                                }
                                                                else
                                                                {
                                                                    if (strcmp(token1.nomefunc, "RAIZQ") == 0)
                                                                    {
                                                                        token_atual.valor = Math.Sqrt(token_atual.valor);
                                                                    }
                                                                    else
                                                                    {
                                                                        if (strcmp(token1.nomefunc, "QUAD") == 0)
                                                                        {
                                                                            token_atual.valor *= token_atual.valor;
                                                                        }
                                                                        else
                                                                        {
                                                                            if (strcmp(token1.nomefunc, "TANH") == 0)
                                                                            {
                                                                                token_atual.valor = Math.Tanh(token_atual.valor);
                                                                            }
                                                                            else
                                                                            {
                                                                                if (strcmp(token1.nomefunc, "TAN") == 0)
                                                                                {
                                                                                    token_atual.valor = Math.Tan(token_atual.valor);
                                                                                }
                                                                                else
                                                                                {
                                                                                    if (strcmp(token1.nomefunc, "TRUNC") == 0)
                                                                                    {
                                                                                        token_atual.valor = Math.Truncate(token_atual.valor);
                                                                                    }
                                                                                }
                                                                            }
                                                                        }
                                                                    }
                                                                }
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }

                    break;

                case 3:
                case 6:
                case 8:
                case 10:
                case 12:
                case 15:
                    token_atual = pop();
                    break;
            }

            token_atual.estado = vai_pra_estado(reducao);
            push(token_atual);
        }

        ///////////////////////////////////////////////////////////////////////
        /// <summary>
        /// The calcula.
        /// </summary>
        /// <param name="s">The s<see cref="string"/>.</param>
        /// <returns>The <see cref="double"/>.</returns>
        internal double calcula(string s)
        {
            REGTOKEN firsttoken = new REGTOKEN();
            bool aceito = false;
            string copia;
            erro = false;
            copia = s.ToUpper().Trim();
            entrada = copia.ToCharArray();
            cnt = 0;
            topo_pilha = -1;
            firsttoken.estado = 0;
            firsttoken.valor = 0;
            push(firsttoken);
            tipo_token = prox_token();

            do
            {
                switch (pilha[topo_pilha].estado)
                {
                    case 0:
                    case 9:
                    case 12:
                    case 13:
                    case 14:
                    case 15:
                    case 16:
                    case 20:
                        if (tipo_token == NUM)
                        {
                            desloca(10);
                        }
                        else
                        {
                            if (tipo_token == VAR)
                            {
                                desloca(7);
                            }
                            else
                            {
                                if (tipo_token == FUNC)
                                {
                                    desloca(11);
                                }
                                else
                                {
                                    if (tipo_token == MENOS)
                                    {
                                        desloca(5);
                                    }
                                    else
                                    {
                                        if (tipo_token == APAREN)
                                        {
                                            desloca(9);
                                        }
                                        else
                                        {
                                            erro = true;
                                        }
                                    }
                                }
                            }
                        }

                        break;

                    case 1:
                        if (tipo_token == EOLN)
                        {
                            aceito = true;
                        }
                        else
                        {
                            if (tipo_token == MAIS)
                            {
                                desloca(12);
                            }
                            else
                            {
                                if (tipo_token == MENOS)
                                {
                                    desloca(13);
                                }
                                else
                                {
                                    erro = true;
                                }
                            }
                        }

                        break;

                    case 2:
                        if (tipo_token == VEZES)
                        {
                            desloca(14);
                        }
                        else
                        {
                            if (tipo_token == DIVIDIR)
                            {
                                desloca(15);
                            }
                            else
                            {
                                reduza(3);
                            }
                        }

                        break;

                    case 3:
                        reduza(6);
                        break;

                    case 4:
                        if (tipo_token == EXP)
                        {
                            desloca(16);
                        }
                        else
                        {
                            reduza(8);
                        }

                        break;

                    case 5:
                        if (tipo_token == NUM)
                        {
                            desloca(10);
                        }
                        else
                        {
                            if (tipo_token == VAR)
                            {
                                desloca(7);
                            }
                            else
                            {
                                if (tipo_token == FUNC)
                                {
                                    desloca(11);
                                }
                                else
                                {
                                    if (tipo_token == APAREN)
                                    {
                                        desloca(9);
                                    }
                                    else
                                    {
                                        erro = true;
                                    }
                                }
                            }
                        }

                        break;

                    case 6:
                        reduza(10);
                        break;

                    case 7:
                        if (tipo_token == MODULO)
                        {
                            desloca(18);
                        }
                        else
                        {
                            reduza(13);
                        }

                        break;

                    case 8:
                        reduza(12);
                        break;

                    case 10:
                        reduza(15);
                        break;

                    case 11:
                        if (tipo_token == APAREN)
                        {
                            desloca(20);
                        }
                        else
                        {
                            erro = true;
                        }

                        break;

                    case 17:
                        reduza(9);
                        break;

                    case 18:
                        if (tipo_token == VAR)
                        {
                            desloca(26);
                        }
                        else
                        {
                            erro = true;
                        }

                        break;

                    case 19:
                        if (tipo_token == MAIS)
                        {
                            desloca(12);
                        }
                        else
                        {
                            if (tipo_token == MENOS)
                            {
                                desloca(13);
                            }
                            else
                            {
                                if (tipo_token == FPAREN)
                                {
                                    desloca(27);
                                }
                                else
                                {
                                    erro = true;
                                }
                            }
                        }

                        break;

                    case 21:
                        if (tipo_token == VEZES)
                        {
                            desloca(14);
                        }
                        else
                        {
                            if (tipo_token == DIVIDIR)
                            {
                                desloca(15);
                            }
                            else
                            {
                                reduza(1);
                            }
                        }

                        break;

                    case 22:
                        if (tipo_token == VEZES)
                        {
                            desloca(14);
                        }
                        else
                        {
                            if (tipo_token == DIVIDIR)
                            {
                                desloca(15);
                            }
                            else
                            {
                                reduza(2);
                            }
                        }

                        break;

                    case 23:
                        reduza(4);
                        break;

                    case 24:
                        reduza(5);
                        break;

                    case 25:
                        reduza(7);
                        break;

                    case 26:
                        reduza(11);
                        break;

                    case 27:
                        reduza(14);
                        break;

                    case 28:
                        if (tipo_token == MAIS)
                        {
                            desloca(12);
                        }
                        else
                        {
                            if (tipo_token == MENOS)
                            {
                                desloca(13);
                            }
                            else
                            {
                                if (tipo_token == FPAREN)
                                {
                                    desloca(29);
                                }
                                else
                                {
                                    erro = true;
                                }
                            }
                        }

                        break;

                    case 29:
                        reduza(16);
                        break;

                    case 30:
                        erro = true;
                        break;
                }
            }
            while ((!aceito) && (!erro));

            if (erro)
                return (0);

            s = copia;
            return (pilha[topo_pilha].valor);
        }

        ///////////////////////////////////////////////////////////////////////
        /// <summary>
        /// The strcmp.
        /// </summary>
        /// <param name="nomefunc">The nomefunc<see cref="string"/>.</param>
        /// <param name="v">The v<see cref="string"/>.</param>
        /// <returns>The <see cref="int"/>.</returns>
        private int strcmp(string nomefunc, string v)
        {
            return nomefunc.CompareTo(v);
        }

        ///////////////////////////////////////////////////////////////////////
        /// <summary>
        /// The ff.
        /// </summary>
        /// <param name="x">The x<see cref="double"/>.</param>
        /// <returns>The <see cref="double"/>.</returns>
        internal double ff(double x)
        {
            double tmp;
            var.valor = x;
            tmp = calcula(func);
            if (double.IsInfinity(tmp))
                tmp = double.MaxValue;
            if (double.IsNaN(tmp))
                tmp = double.MaxValue;
            return tmp;
        }

        ///////////////////////////////////////////////////////////////////////
        /// <summary>
        /// The fi.
        /// </summary>
        /// <param name="x">The x<see cref="double"/>.</param>
        /// <returns>The <see cref="double"/>.</returns>
        internal double fi(double x)
        {
            double tmp;
            var.valor = x;
            tmp = calcula(fiter);
            if (double.IsInfinity(tmp))
                tmp = double.MaxValue;
            if (double.IsNaN(tmp))
                tmp = double.MaxValue;
            return tmp;
        }

        ///////////////////////////////////////////////////////////////////////
        /// <summary>
        /// The sinal.
        /// </summary>
        /// <param name="x">The x<see cref="double"/>.</param>
        /// <returns>The <see cref="int"/>.</returns>
        internal int sinal(double x)
        {
            if (x > 0)
                return 1;

            if (x < 0)
                return -1;

            return 0;
        }

        ///////////////////////////////////////////////////////////////////////
        /// <summary>
        /// The cmpr.
        /// </summary>
        /// <param name="first">The first<see cref="double"/>.</param>
        /// <param name="second">The second<see cref="double"/>.</param>
        /// <returns>The <see cref="int"/>.</returns>
        internal int cmpr(double first, double second)
        {
            double sign;
            sign = first - second;
            sign = Math.Abs(Math.Round(sign, 5));

            if (sign == 0.0)
                return 0;
            else
                return 1;
        }

        ///////////////////////////////////////////////////////////////////////
        /// <summary>
        /// The calc3d.
        /// </summary>
        /// <param name="x">The x<see cref="double"/>.</param>
        /// <param name="y">The y<see cref="double"/>.</param>
        /// <param name="z">The z<see cref="double"/>.</param>
        internal void calc3d(double x, double y, double z)
        {
            x = -x;
            z -= AreaX+30;

            if (z == 0)
                z = 1;

            sx = 1200.0 * x / z;
            sy = 1200.0 * y / z;
            sx += 399.0;
            sy += 299.0;
            sx *= ((AreaX-1.0) / 799.0);
            sy *= ((AreaY-1.0) / 599.0);
        }

        ///////////////////////////////////////////////////////////////////////
        /// <summary>
        /// The putpixel.
        /// </summary>
        /// <param name="x">The x<see cref="int"/>.</param>
        /// <param name="y">The y<see cref="int"/>.</param>
        /// <param name="cor">The cor<see cref="Color"/>.</param>
        internal void putpixel(int x, int y, Color cor)
        {
            //unsigned offst;

            if (((x > 0) && (x < AreaX)) && ((y > 0) && (y < AreaY)))
            {
                bmp.SetPixel(x, y, cor);
            }
        }

        ///////////////////////////////////////////////////////////////////////
        /// <summary>
        /// The compCor.
        /// </summary>
        /// <param name="c1">The c1<see cref="Color"/>.</param>
        /// <param name="c2">The c2<see cref="Color"/>.</param>
        /// <returns>The <see cref="bool"/>.</returns>
        internal bool compCor(Color c1, Color c2)
        {
            if (c1.R == c2.R && c1.G == c2.G && c1.B == c2.B)
            {
                return true;
            }
            return false;
        }

        ///////////////////////////////////////////////////////////////////////
        /// <summary>
        /// The sombra.
        /// </summary>
        /// <param name="x">The x<see cref="int"/>.</param>
        /// <param name="y">The y<see cref="int"/>.</param>
        internal void sombra(int x, int y)
        {
            Color cor;
            x -= (((int)AreaY - y) / 3);
            calc3d(((double)x - ((AreaX/2.0)-1.0)) / 1.5, -180.0, (double)y - AreaY);
            if (sy > (AreaY-1))
                sy = (AreaY-1);
            cor = bmp.GetPixel((int)sx, (int)sy);

            if (compCor(cor, Color.Red))
                putpixel((int)sx, (int)sy, Color.DarkRed);

            if (compCor(cor, Color.Cyan))
                putpixel((int)sx, (int)sy, Color.DarkCyan);
        }

        ///////////////////////////////////////////////////////////////////////
        /// <summary>
        /// The vline.
        /// </summary>
        /// <param name="bock">The bock<see cref="int"/>.</param>
        /// <param name="chis">The chis<see cref="int"/>.</param>
        /// <param name="ipslon">The ipslon<see cref="int"/>.</param>
        /// <param name="cor">The cor<see cref="Color"/>.</param>
        internal void vline(int bock, int chis, int ipslon, Color cor)
        {
            int swapvar;

            if (chis > ipslon)
            {
                swapvar = chis;
                chis = ipslon;
                ipslon = swapvar;
            }

            for (swapvar = chis; swapvar <= ipslon; swapvar++)
            {
                putpixel(bock, swapvar, cor);
                sombra(bock, swapvar);
            }
        }

        ///////////////////////////////////////////////////////////////////////
        /// <summary>
        /// The plotfunc.
        /// </summary>
        /// <param name="x">The x<see cref="double"/>.</param>
        /// <param name="cor">The cor<see cref="Color"/>.</param>
        /// <param name="_">The _<see cref="byte"/>.</param>
        internal void plotfunc(double x, Color cor, byte _)
        {
            double f, tmp;
            //uint glitch;
            f = ff(x * scale);
            tmp = (((AreaY-1.0) - ((f / scale) / 3.0)) - ((AreaY / 2.0) - 1));

            if (Math.Abs(f / 3.0) <= ((AreaY / 2) - 1))
            {
                if (_ == 1)
                    vline((int)x + (((int)AreaX/2)-1), (int)tmp, ((int)AreaY/2), cor);
                else
                {
                    putpixel((int)x + (((int)AreaX / 2) - 1), (int)tmp, cor);
                    sombra((int)x + (((int)AreaX / 2) - 1), (int)tmp);
                }
            }

            putpixel((int)x + (((int)AreaX / 2) - 1), ((int)AreaY /2), Color.FromArgb(255, cor.R / 2, cor.G / 2, cor.B / 2));
        }

        ///////////////////////////////////////////////////////////////////////
        /// <summary>
        /// The raizbisec.
        /// </summary>
        /// <param name="a">The a<see cref="double"/>.</param>
        /// <param name="b">The b<see cref="double"/>.</param>
        /// <returns>The <see cref="double"/>.</returns>
        internal double raizbisec(double a, double b)
        {
            double x, y, q, w, e;

            if (a > b)
            {
                y = a;
                a = b;
                b = y;
            }

            x = (a + b) / 2;
            q = ff(a);
            w = ff(b);
            e = ff(x);
            plotfunc(x / scale, Color.Green, 1);

            if ((cmpr(x, a) == 0) && (Math.Abs(e) <= prec))
            {
                found = 1;
                return x;
            }

            if (cmpr(x, a) == 0)
            {
                found = 0;
                return x;
            }

            if (Math.Abs(e) <= prec)
            {
                found = 1;
                return x;
            }

            if ((sinal(q) != sinal(e)) && (found != 1))
                return raizbisec(a, x);

            if ((sinal(w) != sinal(e)) && (found != 1))
                return raizbisec(x, b);

            if (((sinal(w) == sinal(e)) && (sinal(q) == sinal(e))) && (found != 1))
            {
                y = raizbisec(a, x);

                if (found != 1)
                    y = raizbisec(x, b);

                return y;
            }

            return x;
        }

        ///////////////////////////////////////////////////////////////////////
        /// <summary>
        /// The pos_falsa.
        /// </summary>
        /// <param name="a">The a<see cref="double"/>.</param>
        /// <param name="b">The b<see cref="double"/>.</param>
        /// <returns>The <see cref="double"/>.</returns>
        internal double pos_falsa(double a, double b)
        {
            double x, y, q, w, e;

            if (a > b)
            {
                y = a;
                a = b;
                b = y;
            }

            if (Math.Abs(b - a) < prec)
            {
                found = 1;
                return (a + b) / 2;
            }

            q = ff(a);
            w = ff(b);

            if (Math.Abs(q) < prec)
            {
                found = 1;
                return a;
            }

            if (Math.Abs(w) < prec)
            {
                found = 1;
                return b;
            }

            while (true)
            {
                if (w == q)
                    x = double.MaxValue;//    .PositiveInfinity;
                else
                    x = ((a * w) - (b * q)) / (w - q);

                if (double.IsNaN(x))
                    x = double.MaxValue;

                e = ff(x);
                plotfunc(x / scale, Color.Green, 1);

                if ((cmpr(x, a) == 0) && (Math.Abs(e) < prec))
                {
                    found = 1;
                    return x;
                }

                if (cmpr(x, a) == 0)
                {
                    found = 0;
                    return x;
                }

                if (Math.Abs(e) < prec)
                {
                    found = 1;
                    return x;
                }

                if ((q * e) > 0)
                {
                    a = x;
                    q = ff(a);
                }
                else
                {
                    b = x;
                    w = ff(b);
                }

                if (Math.Abs(b - a) < prec)
                {
                    found = 1;
                    return (a + b) / 2;
                }
            }
        }

        ///////////////////////////////////////////////////////////////////////
        /// <summary>
        /// The pos_falsa_mod.
        /// </summary>
        /// <param name="a">The a<see cref="double"/>.</param>
        /// <param name="b">The b<see cref="double"/>.</param>
        /// <returns>The <see cref="double"/>.</returns>
        internal double pos_falsa_mod(double a, double b)
        {
            double x, y, q, w, e, x0;

            if (a > b)
            {
                y = a;
                a = b;
                b = y;
            }

            q = ff(a);
            w = ff(b);
            x0 = a;

            if (Math.Abs(b - a) < prec)
            {
                found = 1;
                return a;
            }

            if (Math.Abs(q) < prec)
            {
                found = 1;
                return a;
            }

            if (Math.Abs(w) < prec)
            {
                found = 1;
                return b;
            }

            while (!false)
            {
                if (w == q)
                    x = double.MaxValue;// double.PositiveInfinity;
                else
                    x = ((a * w) - (b * q)) / (w - q);

                if (double.IsNaN(x))
                    x = double.MaxValue;

                e = ff(x);
                plotfunc(x / scale, Color.Green, 1);

                if (Math.Abs(b - a) < prec)
                {
                    found = 1;
                    return x;
                }

                if (Math.Abs(e) < prec)
                {
                    found = 1;
                    return x;
                }

                if (cmpr(x, a) == 0)
                {
                    found = 0;
                    return x;
                }

                if ((q * e) > 0)
                {
                    a = x;
                    q = ff(a);

                    if ((q * ff(x0)) > 0)
                        w /= 2;
                }
                else
                {
                    b = x;
                    w = ff(b);

                    if ((w * ff(x0)) > 0)
                        q /= 2;
                }

                x0 = x;
            }
        }

        ///////////////////////////////////////////////////////////////////////
        /// <summary>
        /// The secante.
        /// </summary>
        /// <param name="gx">The gx<see cref="double"/>.</param>
        /// <param name="gy">The gy<see cref="double"/>.</param>
        /// <param name="a">The a<see cref="double"/>.</param>
        /// <param name="b">The b<see cref="double"/>.</param>
        /// <returns>The <see cref="double"/>.</returns>
        internal double secante(double gx, double gy, double a, double b)
        {
            double x, y, q, w, e;

            if (a > b)
            {
                y = a;
                a = b;
                b = y;
            }

            q = ff(a);
            w = ff(b);

            if (Math.Abs(q) < prec)
            {
                found = 1;
                return a;
            }

            if (Math.Abs(w) < prec)
            {
                found = 1;
                return b;
            }

            if (Math.Abs(b - a) < prec)
            {
                found = 1;
                return b;
            }

            while (!false)
            {
                q = ff(a);
                w = ff(b);

                if (w == q)
                    x = double.MaxValue;
                else
                    x = b - (w / (w - q)) * (b - a);

                e = ff(x);

                if ((x > gx) && (x < gy))
                    plotfunc(x / scale, Color.Green, 1);

                if (Math.Abs(x - b) < prec)
                {
                    found = 1;
                    return x;
                }

                if (Math.Abs(e) < prec)
                {
                    found = 1;
                    return x;
                }

                if (cmpr(x, a) == 0)
                {
                    found = 0;
                    return x;
                }

                a = b;
                b = x;
            }
        }

        ///////////////////////////////////////////////////////////////////////
        /// <summary>
        /// The mil.
        /// </summary>
        /// <param name="a">The a<see cref="double"/>.</param>
        /// <param name="olho">The olho<see cref="double"/>.</param>
        /// <param name="nariz">The nariz<see cref="double"/>.</param>
        /// <returns>The <see cref="double"/>.</returns>
        internal double mil(double a, double olho, double nariz)
        {
            double x, y, q;
            int k = 0;
            q = ff(a);

            if (Math.Abs(q) < prec)
            {
                found = 1;
                return a;
            }

            while (true)
            {
                x = fi(a);
                y = ff(x);

                if ((olho < x) && (x < nariz))
                    plotfunc(x / scale, Color.Green, 1);

                if ((Math.Abs(y) < prec) || (Math.Abs(x - a) < prec))
                {
                    found = 1;
                    return x;
                }

                a = x;
                k++;

                if (k > ca)
                {
                    found = 0;
                    return x;
                }
            }
        }

        ///////////////////////////////////////////////////////////////////////
        /// <summary>
        /// The newton.
        /// </summary>
        /// <param name="a">The a<see cref="double"/>.</param>
        /// <param name="olho">The olho<see cref="double"/>.</param>
        /// <param name="nariz">The nariz<see cref="double"/>.</param>
        /// <returns>The <see cref="double"/>.</returns>
        internal double newton(double a, double olho, double nariz)
        {
            double x, y, q, w;
            int k = 0;
            q = ff(a);

            if (Math.Abs(q) < prec)
            {
                found = 1;
                return a;
            }

            while (true)
            {
                w = fi(a);
                q = ff(a);
                x = a - (q / w);
                y = ff(x);

                if ((olho < x) && (x < nariz))
                    plotfunc(x / scale, Color.Green, 1);

                if ((Math.Abs(y) < prec) || (Math.Abs(x - a) < prec))
                {
                    found = 1;
                    return x;
                }

                a = x;
                k++;

                if (k > ca)
                {
                    found = 0;
                    return x;
                }
            }
        }

        ///////////////////////////////////////////////////////////////////////
        /// <summary>
        /// The decplaces.
        /// </summary>
        /// <param name="val">The val<see cref="string"/>.</param>
        /// <returns>The <see cref="int"/>.</returns>
        internal int decplaces(string val)
        {
            int i, j, k, l;
            j = 0;
            k = 0;
            l = 0;
            i = val.Length;

            while (true)
            {
                if (j == i)
                    break;

                if ((j != i) && (k == 1))
                    l++;

                if (val[j] == '.')
                    k = 1;

                j++;
            }

            return l;
        }

        ///////////////////////////////////////////////////////////////////////
        /// <summary>
        /// The drawfunc.
        /// </summary>
        /// <param name="inicio">The inicio<see cref="double"/>.</param>
        /// <param name="final">The final<see cref="double"/>.</param>
        internal void drawfunc(double inicio, double final)
        {
            int j;
            double x, y;
            double startval = -((AreaX / 2.0) - 2.0), endval = (AreaX / 2.0) - 2.0;
            Color floor = Color.Red;
            bool checker = false;
            int hAreaX;
            int hAreaY;
            hAreaX = (int)AreaX / 2;
            hAreaY = (int)AreaY / 2;

            if (inicio > final)
            {
                y = inicio;
                inicio = final;
                final = y;
            }

            if ((sinal(inicio) == -1) && (sinal(final) <= 0))
            {
                startval = -(AreaX / 2.0) - 2.0;
                endval = 0.0;
            }

            if ((sinal(inicio) >= 0) && (sinal(final) == 1))
            {
                startval = 0.0;
                endval = (AreaX/2.0)-2.0;
            }

            for (xx = -(AreaX+40.0); xx <= 0.0; xx++)
            {
                if (((long)((xx + (AreaX/2.0)) / 30.0) % 2) == 0)
                    checker = false;
                else
                    checker = true;

                for (yy = -AreaY*2.0; yy <= (AreaY/2); yy++)
                {
                    if (((long)yy % 40) == 0)
                        checker = !checker;

                    calc3d(xx / 1.5, -180.0, yy - 90.0);

                    if (checker == false)
                        floor = Color.Red;

                    if (checker == true)
                        floor = Color.Cyan;

                    putpixel((int)sx, (int)sy, floor);
                    putpixel((int)AreaX - (int)sx, (int)sy, floor);
                }
            }

            pictureBox1.Refresh();

            for (j = 0; j <= ((int)(AreaY+100) -1); j++)
            {
                putpixel(hAreaX-2, j, Color.DarkGray);
                sombra(hAreaX - 2, j);
                sombra(hAreaX - 2, -j);
                putpixel(hAreaX - 1, j, Color.Gray);
                sombra(hAreaX - 1, j);
                sombra(hAreaX - 1, -j);
                putpixel(hAreaX, j, Color.White);
                sombra(hAreaX, j);
                sombra(hAreaX, -j);
                putpixel(hAreaX +1, j, Color.Gray);
                sombra(hAreaX + 1, j);
                sombra(hAreaX + 1, -j);
                putpixel(hAreaX + 2, j, Color.DarkGray);
                sombra(hAreaX + 2, j);
                sombra(hAreaX + 2, -j);
            }

            for (j = 0; j <= (AreaX-1); j++)
            {
                putpixel(j, hAreaY-2, Color.DarkGray);
                sombra(j, hAreaY - 2);
                putpixel(j, hAreaY - 1, Color.Gray);
                sombra(j, hAreaY - 1);
                putpixel(j, hAreaY, Color.White);
                sombra(j, hAreaY);
                putpixel(j, hAreaY +1, Color.Gray);
                sombra(j, hAreaY + 1);
                putpixel(j, hAreaY + 2, Color.DarkGray);
                sombra(j, hAreaY + 2);
            }

            for (x = startval; x <= endval; x = x + 0.01)
                plotfunc(x, Color.Yellow, 0);

            pictureBox1.Refresh();
        }

        ///////////////////////////////////////////////////////////////////////
        /// <summary>
        /// The wasfound.
        /// </summary>
        /// <param name="achou">The achou<see cref="int"/>.</param>
        /// <param name="a">The a<see cref="double"/>.</param>
        internal void wasfound(int achou, double a)
        {
            double b;
            b = ff(a);

            if (achou == 1)
                foundTxt.Text = "Achei! - " + Math.Round(a, 4).ToString() + " | " + Math.Round(b, 4).ToString();//sprintf(outc, "Achei!");
            else
                foundTxt.Text = "Não achei... " + Math.Round(a, 4).ToString() + " | " + Math.Round(b, 4).ToString();//sprintf(outc, "N„o achei...");
        }

        ///////////////////////////////////////////////////////////////////////
        /// <summary>
        /// The precTxt_TextChanged.
        /// </summary>
        /// <param name="sender">The sender<see cref="object"/>.</param>
        /// <param name="e">The e<see cref="EventArgs"/>.</param>
        private void precTxt_TextChanged(object sender, EventArgs e)
        {
        }

        /// <summary>
        /// The pictureBox1_Click.
        /// </summary>
        /// <param name="sender">The sender<see cref="object"/>.</param>
        /// <param name="e">The e<see cref="EventArgs"/>.</param>
        private void pictureBox1_Click(object sender, EventArgs e)
        {
        }

        /// <summary>
        /// The pictureBox1_Paint.
        /// </summary>
        /// <param name="sender">The sender<see cref="object"/>.</param>
        /// <param name="e">The e<see cref="PaintEventArgs"/>.</param>
        private void pictureBox1_Paint(object sender, PaintEventArgs e)
        {
            pictureBox1.Image = bmp;
        }

        /// <summary>
        /// The precTxt_Validating.
        /// </summary>
        /// <param name="sender">The sender<see cref="object"/>.</param>
        /// <param name="e">The e<see cref="CancelEventArgs"/>.</param>
        private void precTxt_Validating(object sender, CancelEventArgs e)
        {
            double number;
            string prc;
            bool isParsable = double.TryParse(precTxt.Text, out number);
            if (isParsable)
                number = Math.Abs(number);
            else
                number = 1.0;
            prc = number.ToString();
            precTxt.Text = prc;
            pritm = decplaces(prc);
            prec = number;
        }

        /// <summary>
        /// The numIter_ValueChanged.
        /// </summary>
        /// <param name="sender">The sender<see cref="object"/>.</param>
        /// <param name="e">The e<see cref="EventArgs"/>.</param>
        private void numIter_ValueChanged(object sender, EventArgs e)
        {
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="Form1"/> class.
        /// </summary>
        public Form1()
        {
            InitializeComponent();
            var = new REGVAR();
            token_atual = new REGTOKEN();
        }

        /// <summary>
        /// The textBox1_TextChanged.
        /// </summary>
        /// <param name="sender">The sender<see cref="object"/>.</param>
        /// <param name="e">The e<see cref="EventArgs"/>.</param>
        private void textBox1_TextChanged(object sender, EventArgs e)
        {
        }

        /// <summary>
        /// The radioButton1_CheckedChanged.
        /// </summary>
        /// <param name="sender">The sender<see cref="object"/>.</param>
        /// <param name="e">The e<see cref="EventArgs"/>.</param>
        private void radioButton1_CheckedChanged(object sender, EventArgs e)
        {
            ajuste1();
        }

        /// <summary>
        /// The radioButton2_CheckedChanged.
        /// </summary>
        /// <param name="sender">The sender<see cref="object"/>.</param>
        /// <param name="e">The e<see cref="EventArgs"/>.</param>
        private void radioButton2_CheckedChanged(object sender, EventArgs e)
        {
            ajuste1();
        }

        /// <summary>
        /// The radioButton3_CheckedChanged.
        /// </summary>
        /// <param name="sender">The sender<see cref="object"/>.</param>
        /// <param name="e">The e<see cref="EventArgs"/>.</param>
        private void radioButton3_CheckedChanged(object sender, EventArgs e)
        {
            ajuste1();
        }

        /// <summary>
        /// The radioButton4_CheckedChanged.
        /// </summary>
        /// <param name="sender">The sender<see cref="object"/>.</param>
        /// <param name="e">The e<see cref="EventArgs"/>.</param>
        private void radioButton4_CheckedChanged(object sender, EventArgs e)
        {
            ajuste2();
        }

        /// <summary>
        /// The radioButton5_CheckedChanged.
        /// </summary>
        /// <param name="sender">The sender<see cref="object"/>.</param>
        /// <param name="e">The e<see cref="EventArgs"/>.</param>
        private void radioButton5_CheckedChanged(object sender, EventArgs e)
        {
            ajuste2();
        }

        /// <summary>
        /// The radioButton6_CheckedChanged.
        /// </summary>
        /// <param name="sender">The sender<see cref="object"/>.</param>
        /// <param name="e">The e<see cref="EventArgs"/>.</param>
        private void radioButton6_CheckedChanged(object sender, EventArgs e)
        {
            ajuste3();
        }

        /// <summary>
        /// The button1_Click.
        /// </summary>
        /// <param name="sender">The sender<see cref="object"/>.</param>
        /// <param name="e">The e<see cref="EventArgs"/>.</param>
        private void button1_Click(object sender, EventArgs e)
        {
            int i;
            bool isParsable;
            string prc;
            int hAreaX;

            AreaX = 640;
            AreaY = 400;

            hAreaX = ((int)AreaX / 2) - 1;

            bmp = new Bitmap((int)AreaX, (int)AreaY);
            pictureBox1.Image = bmp;
            isParsable = double.TryParse(precTxt.Text, out double number);
            if (isParsable)
                number = Math.Abs(number);
            else
                number = 1.0;
            prc = number.ToString();
            precTxt.Text = prc;
            pritm = decplaces(prc);
            prec = number;

            func = "";
            funcao.Text = funcao.Text.ToUpper().Trim()+"+0";
            funcao.Text = funcao.Text.Replace("+0+0", "+0");
            for (i = 0; i < funcao.Text.Length; i++)
            {
                if (funcao.Text.Substring(i, 1) == "X")
                    func = func + "A1";
                else
                    func = func + funcao.Text.Substring(i, 1);

            }
            isParsable = double.TryParse(inicioTxt.Text, out inicio);
            isParsable = double.TryParse(finalTxt.Text, out final);

            if (inicio == final)
            {
                if (inicio == 0)
                {
                    inicio = 1;
                    final = -1;
                }
                else
                    inicio = -final;
            }

            if (inicio > hAreaX)
                inicio = hAreaX;

            if (inicio < -hAreaX)
                inicio = -hAreaX;

            if (final > hAreaX)
                final = hAreaX;

            if (final < -hAreaX)
                final = -hAreaX;

            if (Math.Abs(inicio) >= Math.Abs(final))
                scale = Math.Abs(inicio / (hAreaX - 1));
            else
                scale = Math.Abs(final / (hAreaX - 1));

            drawfunc(inicio, final);


            if (radioButton4.Checked)
            {
                fiter = func;
                isParsable = double.TryParse(apinicTxt.Text, out apinic);
                isParsable = double.TryParse(numIter.Text, out ca);
                a = mil(apinic, inicio, final);
            }


            if (radioButton5.Checked)
            {
                fiter = func;
                isParsable = double.TryParse(apinicTxt.Text, out apinic);
                isParsable = double.TryParse(numIter.Text, out ca);
                a = newton(apinic, inicio, final);
            }


            if (radioButton1.Checked || radioButton2.Checked || radioButton3.Checked)
            {
                isParsable = double.TryParse(inicioTxt.Text, out inicio);
                isParsable = double.TryParse(finalTxt.Text, out final);

                if (inicio == final)
                {
                    if (inicio == 0)
                    {
                        inicio = 1;
                        final = -1;
                    }
                    else
                        inicio = -final;
                }

                if (inicio > hAreaX)
                    inicio = hAreaX;

                if (inicio < -hAreaX)
                    inicio = -hAreaX;

                if (final > hAreaX)
                    final = hAreaX;

                if (final < -hAreaX)
                    final = -hAreaX;

                if (Math.Abs(inicio) >= Math.Abs(final))
                    scale = Math.Abs(inicio / (hAreaX - 1));
                else
                    scale = Math.Abs(final / (hAreaX - 1));

                drawfunc(inicio, final);

                if (radioButton1.Checked)
                    a = raizbisec(inicio, final);

                if (radioButton2.Checked)
                    a = pos_falsa(inicio, final);

                if (radioButton3.Checked)
                    a = pos_falsa_mod(inicio, final);
            }

            if (radioButton6.Checked)
            {
                isParsable = double.TryParse(inicioTxt.Text, out inicio);
                isParsable = double.TryParse(finalTxt.Text, out final);
                isParsable = double.TryParse(apinicTxt.Text, out apinic);
                isParsable = double.TryParse(apfimTxt.Text, out apfin);

                if (inicio == final)
                {
                    if (inicio == 0)
                    {
                        inicio = 1;
                        final = -1;
                    }
                    else
                        inicio = -final;
                }

                if (inicio > hAreaX)
                    inicio = hAreaX;

                if (inicio < -hAreaX)
                    inicio = -hAreaX;

                if (final > hAreaX)
                    final = hAreaX;

                if (final < -hAreaX)
                    final = -hAreaX;

                if (Math.Abs(inicio) >= Math.Abs(final))
                    scale = Math.Abs(inicio / (hAreaX - 1));
                else
                    scale = Math.Abs(final / (hAreaX - 1));

                drawfunc(inicio, final);

                a = secante(inicio, final, apinic, apfin);
            }
            pictureBox1.Refresh();
            wasfound(found, a);
        }

        /// <summary>
        /// The Form1_Load.
        /// </summary>
        /// <param name="sender">The sender<see cref="object"/>.</param>
        /// <param name="e">The e<see cref="EventArgs"/>.</param>
        private void Form1_Load(object sender, EventArgs e)
        {
            inicio = 0.0;
            final = 0.0;
            apinic = 0.0;
            apfin = 0.0;
        }

        /// <summary>
        /// The funcao_Leave.
        /// </summary>
        /// <param name="sender">The sender<see cref="object"/>.</param>
        /// <param name="e">The e<see cref="EventArgs"/>.</param>
        private void funcao_Leave(object sender, EventArgs e)
        {
            funcao.Text = funcao.Text.ToUpper();
            funcao.Text = funcao.Text.Replace(" ", "");
            funcao.Text = funcao.Text.Trim();
        }
    }
}
/*
ANALISE.C

   A fun‡„o de X pode conter ate' no m ximo 60 caracteres.
   Opera‡”es:
      <expr> + <expr>    Soma
      <expr> - <expr>    Subtra‡„o
      <expr> * <expr>    Multiplica‡„o
      <expr> / <expr>    Divis„o
      <expr> ^ <expr>    Exponencia‡„o
      <expr> : <expr>    Resto da divis„o inteira
      (<expr>)           Parea‡„o
      ABS(<expr>)        M¢dulo
      ACOS(<expr>)       Arco-cosseno
      ARD(<expr>)        Parte inteira (arredonda)
      ASEN(<expr>)       Arco-seno
      ATAN(<expr>)       Arco-tangente
      COS(<expr>)        Cosseno
      COSH(<expr>)       Cosseno hiperb¢lico
      EXP(<expr>)        e(2.71828182845904523536) elevado a <expr>
      LOG10(<expr>)      Logar¡tmo base 10
      LOG(<expr>)        Logar¡tmo base e
      POT10(<expr>)      10 elevado a <expr>
      QUAD(<expr>)       <expr> elevado a 2
      RAIZQ(<expr>)      Raiz quadrada de <expr>
      SEN(<expr>)        Seno
      SENH(<expr>)       Seno hiperb¢lico
      TAN(<expr>)        Tangente
      TANH(<expr>)       Tangente hiperb¢lica
      TRUNC(<expr>)      Parte inteira (trunca)
*/