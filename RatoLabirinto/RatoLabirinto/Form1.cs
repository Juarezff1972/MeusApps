using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace RatoLabirinto
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        int ratoX;
        int ratoY;
        int saidaX;
        int saidaY;
        bool primR;
        bool primS;
        byte[,] mapa;

        Bitmap bmpParede;
        Bitmap bmpCaminho;
        Bitmap bmpMarcado;
        Bitmap bmpRato;
        Bitmap bmpSaida;

        Bitmap bmpNorte;
        Bitmap bmpSul;
        Bitmap bmpLeste;
        Bitmap bmpOeste;

        int npx;
        int npy;


        private void Form1_Load(object sender, EventArgs e)
        {
            int i;
            int j;
            npx = pictureBox1.Width / 11;
            npy = pictureBox1.Height / 11;
            mapa = new byte[npx + 1, npy + 1];
            for (i = 0; i < npx; i++)
            {
                for (j = 0; j < npy; j++)
                {
                    mapa[i, j] = 0;
                }
            }
            bmpParede = new Bitmap(RatoLabirinto.Properties.Resources.parede);
            bmpCaminho = new Bitmap(RatoLabirinto.Properties.Resources.caminho);
            bmpMarcado = new Bitmap(RatoLabirinto.Properties.Resources.marcado);
            bmpRato = new Bitmap(RatoLabirinto.Properties.Resources.rato);
            bmpSaida = new Bitmap(RatoLabirinto.Properties.Resources.saida);
            bmpNorte = new Bitmap(RatoLabirinto.Properties.Resources.norte);
            bmpSul = new Bitmap(RatoLabirinto.Properties.Resources.sul);
            bmpLeste = new Bitmap(RatoLabirinto.Properties.Resources.leste);
            bmpOeste = new Bitmap(RatoLabirinto.Properties.Resources.oeste);
            primR = false;
            primS = false;
        }

        private void Form1_Shown(object sender, EventArgs e)
        {
            AboutBox1 ab;
            ab = new AboutBox1();
            ab.Show();
            foundTXT.Text = "Inicio";
            foundTXT.Refresh();
        }

        private void editBtn_Click(object sender, EventArgs e)
        {
            int i;
            int j;
            paredeBtn.Enabled = true;
            caminhoBtn.Enabled = true;
            ratoBtn.Enabled = true;
            saidaBtn.Enabled = true;
            gerarBtn.Enabled = true;
            foundTXT.Text = "Editando";
            foundTXT.Refresh();
            for (i = 0; i < npx; i++)
            {
                for (j = 0; j < npy; j++)
                {
                    if (mapa[i, j] < 3 || mapa[i, j] > 40) mapa[i, j] = 0;
                }
            }
            if (primR) mapa[ratoX, ratoY] = 15;
            pictureBox1.Refresh();
        }

        private void procBtn_Click(object sender, EventArgs e)
        {
            byte z;
            int i;
            int j;
            paredeBtn.Enabled = false;
            caminhoBtn.Enabled = false;
            ratoBtn.Enabled = false;
            saidaBtn.Enabled = false;
            gerarBtn.Enabled = false;

            for (i = 0; i < npx; i++)
            {
                mapa[i, 0] = 9;
                mapa[i, npy - 1] = 9;
            }
            for (i = 0; i < npy; i++)
            {
                mapa[0, i] = 9;
                mapa[npx - 1, i] = 9;
            }
            foundTXT.Text = "Procurando...";
            foundTXT.Refresh();
            z = Procurar(ratoX, ratoY, 1);
            if (z == 2)
                foundTXT.Text = "Encontrado.";
            else
                foundTXT.Text = "Não encontrado.";
            foundTXT.Refresh();
        }

        private void pictureBox1_MouseUp(object sender, MouseEventArgs e)
        {

        }

        private void pictureBox1_MouseDown(object sender, MouseEventArgs e)
        {
            int x = 11 * (e.X / 11);
            int y = 11 * (e.Y / 11);
            if (paredeBtn.Checked)
            {
                mapa[x / 11, y / 11] = 9;
            }
            if (caminhoBtn.Checked)
            {
                mapa[x / 11, y / 11] = 0;
            }
            if (ratoBtn.Checked)
            {
                if (primR) mapa[ratoX, ratoY] = 0;
                ratoX = x / 11;
                ratoY = y / 11;
                mapa[ratoX, ratoY] = 15;
                primR = true;

            }
            if (saidaBtn.Checked)
            {
                if (primS) mapa[saidaX, saidaY] = 0;
                saidaX = x / 11;
                saidaY = y / 11;
                mapa[saidaX, saidaY] = 11;
                primS = true;
            }
            pictureBox1.Refresh();

            if (primR && primS) procBtn.Enabled = true;
        }

        private void pictureBox1_MouseMove(object sender, MouseEventArgs e)
        {
            MouseButtons b;
            b = e.Button;
            if (b == MouseButtons.Left)
            {
                pictureBox1_MouseDown(sender, e);
            }
        }

        private void paredeBtn_CheckedChanged(object sender, EventArgs e)
        {
            if (paredeBtn.Checked)
            {
                caminhoBtn.Checked = false;
                ratoBtn.Checked = false;
                saidaBtn.Checked = false;
            }
        }

        private void caminhoBtn_CheckedChanged(object sender, EventArgs e)
        {
            if (caminhoBtn.Checked)
            {
                paredeBtn.Checked = false;
                ratoBtn.Checked = false;
                saidaBtn.Checked = false;
            }
        }

        private void ratoBtn_CheckedChanged(object sender, EventArgs e)
        {
            if (ratoBtn.Checked)
            {
                paredeBtn.Checked = false;
                caminhoBtn.Checked = false;
                saidaBtn.Checked = false;
            }
        }

        private void saidaBtn_CheckedChanged(object sender, EventArgs e)
        {
            if (saidaBtn.Checked)
            {
                paredeBtn.Checked = false;
                caminhoBtn.Checked = false;
                ratoBtn.Checked = false;
            }
        }

        private void pictureBox1_Paint(object sender, PaintEventArgs e)
        {
            int i;
            int j;
            //bool d;
            for (i = 0; i < npx; i++)
            {
                for (j = 0; j < npy; j++)
                {
                    //d = false;
                    if (mapa[i, j] == 9)
                    {
                        //d = true;
                        e.Graphics.DrawImage(bmpParede, new Point(i * 11, j * 11));
                    }
                    if (mapa[i, j] == 0 || mapa[i, j] == 2)
                    {
                        //d = true;
                        e.Graphics.DrawImage(bmpCaminho, new Point(i * 11, j * 11));
                    }
                    if (mapa[i, j] == 1)
                    {
                        //d = true;
                        e.Graphics.DrawImage(bmpMarcado, new Point(i * 11, j * 11));
                    }
                    if (mapa[i, j] == 15)
                    {
                        //d = true;
                        e.Graphics.DrawImage(bmpRato, new Point(i * 11, j * 11));
                    }
                    if (mapa[i, j] == 11)
                    {
                        //d = true;
                        e.Graphics.DrawImage(bmpSaida, new Point(i * 11, j * 11));
                    }
                    if (mapa[i, j] == 50)
                    {
                        //d = true;
                        e.Graphics.DrawImage(bmpNorte, new Point(i * 11, j * 11));
                    }
                    if (mapa[i, j] == 51)
                    {
                        //d = true;
                        e.Graphics.DrawImage(bmpLeste, new Point(i * 11, j * 11));
                    }
                    if (mapa[i, j] == 52)
                    {
                        //d = true;
                        e.Graphics.DrawImage(bmpSul, new Point(i * 11, j * 11));
                    }
                    if (mapa[i, j] == 53)
                    {
                        //d = true;
                        e.Graphics.DrawImage(bmpOeste, new Point(i * 11, j * 11));
                    }
                    //if (d == true) e.Graphics.DrawImage(bmp, new Point(i * 11, j * 11));

                    //mapa[i, j] = 0;
                }
            }
        }

        private void GeraLabirinto()
        {
            int x;
            int y;
            int r;
            int r1;
            int r2;
            int i;

            for (x = 0; x < npx; x++)
            {
                for (y = 0; y < npy; y++)
                {
                    mapa[x, y] = 0;
                }
            }

            Random rnd = new();

            for (x = 0; x < npx - 1; x += 2)
            {
                for (y = 0; y < npy - 1; y += 2)
                {
                    /*r1 = rnd.Next(0, 10);
                    r2 = rnd.Next(0, 100);

                    if (r2 < 10 || r2 > 90) continue;

                    if (r1 < 4)
                    {
                        mapa[x, y] = 9;
                    }*/
                    r = rnd.Next(0, 2) + 1;
                    if (x == npx - 1 && r == 1)
                    {
                        r = 2;
                    }
                    if (y == npy - 1 && r == 2)
                    {
                        r = 1;
                    }
                    mapa[x, y] = 9;
                    r1 = 0;
                    r2 = 0;
                    if (r == 1)
                    {
                        r1 = 1;
                    }
                    if (r == 2)
                    {
                        r2 = 1;
                    }
                    mapa[x + r1, y + r2] = 9;
                }
            }

            for (x = 0; x < npx - 1; x += 2)
            {
                for (y = 0; y < npy - 1; y += 2)
                {
                    r1 = rnd.Next(0, 10);
                    r2 = rnd.Next(0, 100);

                    if (r2 < 10 || r2 > 90) continue;

                    if (r1 < 4)
                    {
                        mapa[x, y] = 0;
                    }
                }
            }

            for (i = 0; i < npx; i++)
            {
                mapa[i, 0] = 9;
                mapa[i, npy - 1] = 9;
            }
            for (i = 0; i < npy; i++)
            {
                mapa[0, i] = 9;
                mapa[npx - 1, i] = 9;
            }
            pictureBox1.Refresh();
        }

        public byte Procurar(int x, int y, byte dir)
        {
            byte aux;
            byte rtncode = 0;

            mapa[x, y] = 1;

            if (dir == 0) mapa[x, y] = 50;
            if (dir == 1) mapa[x, y] = 51;
            if (dir == 2) mapa[x, y] = 52;
            if (dir == 3) mapa[x, y] = 53;
            aux = mapa[x + 1, y];
            //delay(50);
            pictureBox1.Refresh();
            Task.Delay(50);
            if ((aux == 0) || (aux == 11))
            {
                if (aux == 11)
                    return 2;
                mapa[x, y] = 1;
                rtncode = Procurar(x + 1, y, 1);
                if (rtncode == 2)
                    return rtncode;
                if (dir == 0) mapa[x, y] = 50;
                if (dir == 1) mapa[x, y] = 51;
                if (dir == 2) mapa[x, y] = 52;
                if (dir == 3) mapa[x, y] = 53;
            }
            aux = mapa[x, y + 1];
            if ((aux == 0) || (aux == 11))
            {
                if (aux == 11)
                    return 2;
                mapa[x, y] = 1;
                rtncode = Procurar(x, y + 1, 2);
                if (rtncode == 2)
                    return rtncode;
                if (dir == 0) mapa[x, y] = 50;
                if (dir == 1) mapa[x, y] = 51;
                if (dir == 2) mapa[x, y] = 52;
                if (dir == 3) mapa[x, y] = 53;
            }
            aux = mapa[x - 1, y];
            if ((aux == 0) || (aux == 11))
            {
                if (aux == 11)
                    return 2;
                mapa[x, y] = 1;
                rtncode = Procurar(x - 1, y, 3);
                if (rtncode == 2)
                    return rtncode;
                if (dir == 0) mapa[x, y] = 50;
                if (dir == 1) mapa[x, y] = 51;
                if (dir == 2) mapa[x, y] = 52;
                if (dir == 3) mapa[x, y] = 53;
            }
            aux = mapa[x, y - 1];
            if ((aux == 0) || (aux == 11))
            {
                if (aux == 11)
                    return 2;
                mapa[x, y] = 1;
                rtncode = Procurar(x, y - 1, 0);
                if (rtncode == 2)
                    return rtncode;
                if (dir == 0) mapa[x, y] = 50;
                if (dir == 1) mapa[x, y] = 51;
                if (dir == 2) mapa[x, y] = 52;
                if (dir == 3) mapa[x, y] = 53;
            }
            pictureBox1.Refresh();
            Task.Delay(50);
            mapa[x, y] = 2;
            return rtncode;
        }

        private void gerarBtn_Click(object sender, EventArgs e)
        {
            GeraLabirinto();
        }
    }
}
