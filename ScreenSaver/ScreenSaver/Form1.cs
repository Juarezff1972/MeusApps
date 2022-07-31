using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Runtime.InteropServices;
using System.IO;


//using System.Diagnostics;

namespace ScreenSaver
{


    public partial class ScreenSaverForm : Form
    {
        #region Preview Window Declarations

        // Changes the parent window of the specified child window
        [DllImport( "user32.dll" )]
        private static extern IntPtr SetParent(IntPtr hWndChild, IntPtr hWndNewParent);

        // Changes an attribute of the specified window
        [DllImport( "user32.dll" )]
        private static extern int SetWindowLong(IntPtr hWnd, int nIndex, IntPtr dwNewLong);

        // Retrieves information about the specified window
        [DllImport( "user32.dll", SetLastError = true )]
        private static extern int GetWindowLong(IntPtr hWnd, int nIndex);

        // Retrieves the coordinates of a window's client area
        [DllImport( "user32.dll" )]
        private static extern bool GetClientRect(IntPtr hWnd, out Rectangle lpRect);

        #endregion

        #region Private Members

        // Graphics object to use for drawing
        //private Graphics graphics;

        // Random object for randomizing drawings
        //private Random random;

        // Settings object which contains the screensaver settings
        private Settings settings;

        // Flag used to for initially setting mouseMove location
        private bool active;

        // Used to determine if the Mouse has actually been moved
        private Point mouseLocation;

        // Used to indicate if screensaver is in Preview Mode
        private bool previewMode = false;

        private List<string> folderFile;

        //private Form auxform;
        private Bitmap bmp1;
        private Bitmap bmp2;
        private int cnt, antcnt;
        //private vetor[] vect;
        private vetor2[] c;
        private int[] d;
        private int[] b;
        //private int max;
        private Image img1;
        private Image img2;
        private Bitmap screenshot;
        bool firstrun = true;
        int algo = 0;

        #endregion

        public ScreenSaverForm()
        {
            InitializeComponent();
            PrtScrn();
        }

        public ScreenSaverForm(IntPtr previewHandle)
        {
            InitializeComponent();
            PrtScrn();

            // Set the preview window of the screen saver selection 
            // dialog in Windows as the parent of this form.
            SetParent( this.Handle, previewHandle );

            // Set this form to a child form, so that when the screen saver selection 
            // dialog in Windows is closed, this form will also close.
            SetWindowLong( this.Handle, -16, new IntPtr( GetWindowLong( this.Handle, -16 ) | 0x40000000 ) );

            // Set the size of the screen saver to the size of the screen saver 
            // preview window in the screen saver selection dialog in Windows.
            Rectangle ParentRect;
            GetClientRect( previewHandle, out ParentRect );
            this.Size = ParentRect.Size;

            this.Location = new Point( 0, 0 );

            this.previewMode = true;
        }

        private void PrtScrn()
        {
            Graphics g;
            Rectangle bounds;
            bounds = Screen.PrimaryScreen.Bounds;
            screenshot = new Bitmap( bounds.Width, bounds.Height, System.Drawing.Imaging.PixelFormat.Format32bppArgb );
            g = Graphics.FromImage( screenshot );
            g.CopyFromScreen( 0, 0, 0, 0, bounds.Size, CopyPixelOperation.SourceCopy );
        }

        private void lerPastas(string pasta)
        {
            try
            {
                string[] part1 = null, part2 = null, part3 = null;
                string[] p;
                int i;
                part1 = Directory.GetFiles( pasta, "*.jpg" );
                part2 = Directory.GetFiles( pasta, "*.jpeg" );
                part3 = Directory.GetFiles( pasta, "*.bmp" );
                folderFile.AddRange( part1 );
                folderFile.AddRange( part2 );
                folderFile.AddRange( part3 );

                p = Directory.GetDirectories( pasta, "*.*" );
                if (p.Length > 0)
                {
                    for (i = 0; i < p.Length; i++)
                    {
                        lerPastas( p[i] );
                    }
                }
            }
            catch (Exception)
            {

            }

        }

        Image LerImagem(string arq)
        {
            Image im;
            byte retries = 0;
            bool ok = false;
            im = null;
            do
            {
                try
                {
                    im = Image.FromFile( arq );
                    ok = true;
                }
                catch
                {
                    ok = false;
                    retries++;
                    if (retries > 10)
                        Application.Exit();
                }
            } while (!ok);
            return im;
        }

        void ResizeArray<T>(ref T[,] original, int newCoNum, int newRoNum)
        {
            var newArray = new T[newCoNum, newRoNum];
            int columnCount = original.GetLength( 1 );
            int columnCount2 = newRoNum;
            int columns = original.GetUpperBound( 0 );
            for (int co = 0; co <= columns; co++)
                Array.Copy( original, co * columnCount, newArray, co * columnCount2, columnCount );
            original = newArray;
        }

        private void ScreenSaverForm_Load(object sender, EventArgs e)
        {
            int t, i, j, k, a;
            int mc;
            List<int>[] conta;

            //try
            //{
            // Initialise private members
            //this.random = new Random();
            this.settings = new Settings();
            this.settings.LoadSettings();
            this.active = false;

            folderFile = new List<string>( 5 );
            lerPastas( settings.ImagesFolder );

            Random random = new();

            //cnt++;
            //cnt = random.Next(folderFile.Count);

            Size s = new( this.Width, this.Height );
            img1 = screenshot; //Image.FromFile(folderFile[0]);

            img2 = LerImagem( folderFile[random.Next( folderFile.Count )] );


            //bmp1 = new Bitmap(img1, s);

            pictureBox1.Image = screenshot;
            pictureBox1.SizeMode = PictureBoxSizeMode.StretchImage;
            pictureBox1.Top = 0;
            pictureBox1.Left = 0;
            pictureBox1.Width = this.Width;
            pictureBox1.Height = this.Height;

            bmp1 = new Bitmap( img1, s );
            bmp2 = new Bitmap( img2, s );

            cnt = 0;
            t = this.Width * this.Height;
            c = new vetor2[t + 1];
            d = new int[t + 1];
            random = new();

            k = 1;
            c[0] = new vetor2();
            conta = new List<int>[t + 1];
            mc = 0;
            for (i = 0; i < this.Width; i++)
            {
                for (j = 0; j < this.Height; j++)
                {
                    c[k] = new vetor2();
                    c[k].x = i;
                    c[k].y = j;
                    c[k].vlr = random.Next( 0, t ) + 1;
                    if (conta[c[k].vlr] == null)
                        conta[c[k].vlr] = new List<int>();
                    conta[c[k].vlr].Add( k );
                    //cnt[c[k].vlr]++;
                    if (mc < c[k].vlr)
                        mc = c[k].vlr;
                    k++;
                    Application.DoEvents();
                }
            }

            a = 0;
            b = new int[t + 1];
            for (i = 1; i <= mc; i++)
            {
                //cnt[i] = cnt[i] + cnt[i - 1];
                if (conta[i] != null)
                {
                    k = conta[i].Count;
                    for (j = 0; j < k; j++)
                    {
                        if (a >= b.Count())
                            Array.Resize( ref b, a + 1 );
                        b[a] = conta[i].ToArray()[j];
                        a++;
                    }
                }
            }

            Array.Clear( d, 0, d.Length );

            // Hide the cursor
            Cursor.Hide();

            // Create the Graphics object to use when drawing.
            //this.graphics = this.CreateGraphics();



            // Enable the timer.
            tmrMain.Enabled = true;
            //}
            //catch (Exception ex)
            //{
            //MessageBox.Show(string.Format("Error loading screen saver! {0}", ex.Message), "Screen Saver", MessageBoxButtons.OK, MessageBoxIcon.Error);
            //}
        }

        private void ScreenSaverForm_MouseMove(object sender, MouseEventArgs e)
        {
            if (!this.previewMode)
            {
                // If the mouseLocation still points to 0,0, move it to its actual 
                // location and save the location. Otherwise, check to see if the user
                // has moved the mouse at least 10 pixels.
                if (!this.active)
                {
                    this.mouseLocation = new Point( e.X, e.Y );
                    this.active = true;
                }
                else
                {
                    if (( Math.Abs( e.X - this.mouseLocation.X ) > 10 ) ||
                        ( Math.Abs( e.Y - this.mouseLocation.Y ) > 10 ))
                    {
                        // Exit the screensaver
                        Application.Exit();
                    }
                }
            }
        }

        private void ScreenSaverForm_MouseDown(object sender, MouseEventArgs e)
        {
            if (!this.previewMode)
            {
                // Exit the screensaver if not in preview mode
                Application.Exit();
            }
        }



        private void randPixels()
        {
            int k, t;
            int a;
            Color clr;
            t = this.Width * this.Height;
            for (k = 0; k <= t; k++)
            {
                a = b[k];
                clr = bmp2.GetPixel( c[a].x, c[a].y );
                bmp1.SetPixel( c[a].x, c[a].y, clr );
                if (( k % 1000 ) == 0)
                {
                    pictureBox1.Image = bmp1;
                    pictureBox1.Refresh();
                    Application.DoEvents();
                }
            }
            pictureBox1.Image = bmp1;
            pictureBox1.Refresh();
            Application.DoEvents();
        }

        private void randV()
        {
            int i, j;
            Color clr;
            Random random = new();
            vetor aux = new();
            vetor[] vect = new vetor[this.Width];
            for (i = 0; i < this.Width; i++)
            {
                vect[i] = new vetor();
                vect[i].idx = i;
                vect[i].vlr = random.Next( 0, this.Width ) + 1;
            }
            for (i = 0; i < this.Width; i++)
            {
                for (j = this.Width - 1; j > i; j--)
                {
                    if (vect[i].vlr > vect[j].vlr)
                    {
                        aux = vect[i];
                        vect[i] = vect[j];
                        vect[j] = aux;
                    }
                }
            }
            for (i = 0; i < this.Width; i++)
            {
                for (j = 0; j < this.Height; j++)
                {
                    clr = bmp2.GetPixel( vect[i].idx, j );
                    bmp1.SetPixel( vect[i].idx, j, clr );
                }
                pictureBox1.Image = bmp1;
                pictureBox1.Refresh();
                Application.DoEvents();
            }
        }

        private void randH()
        {
            int i, j;
            Color clr;
            Random random = new();
            vetor aux = new();
            vetor[] vect = new vetor[this.Height];
            for (i = 0; i < this.Height; i++)
            {
                vect[i] = new vetor();
                vect[i].idx = i;
                vect[i].vlr = random.Next( 0, this.Height ) + 1;
            }
            for (i = 0; i < this.Height; i++)
            {
                for (j = this.Height - 1; j > i; j--)
                {
                    if (vect[i].vlr > vect[j].vlr)
                    {
                        aux = vect[i];
                        vect[i] = vect[j];
                        vect[j] = aux;
                    }
                }
            }
            for (i = 0; i < this.Height; i++)
            {
                for (j = 0; j < this.Width; j++)
                {
                    clr = bmp2.GetPixel( j, vect[i].idx );
                    bmp1.SetPixel( j, vect[i].idx, clr );
                }
                pictureBox1.Image = bmp1;
                pictureBox1.Refresh();
                Application.DoEvents();
            }
        }

        private void wipeTB()
        {
            int x, y;
            Color tcol2;
            for (y = 0; y < this.Height / 2; y++)
            {
                for (x = 0; x < this.Width; x++)
                {
                    tcol2 = bmp2.GetPixel( x, y );
                    bmp1.SetPixel( x, y, tcol2 );
                    tcol2 = bmp2.GetPixel( x, ( this.Height - 1 ) - y );
                    bmp1.SetPixel( x, ( this.Height - 1 ) - y, tcol2 );
                }
                pictureBox1.Image = bmp1;
                pictureBox1.Refresh();
                Application.DoEvents();
            }
        }

        private void wipeBT()
        {
            int x, y;
            Color tcol2;
            for (y = this.Height / 2; y >= 0; y--)
            {
                for (x = 0; x < this.Width; x++)
                {
                    tcol2 = bmp2.GetPixel( x, y );
                    bmp1.SetPixel( x, y, tcol2 );
                    tcol2 = bmp2.GetPixel( x, ( this.Height - 1 ) - y );
                    bmp1.SetPixel( x, ( this.Height - 1 ) - y, tcol2 );
                }
                pictureBox1.Image = bmp1;
                pictureBox1.Refresh();
                Application.DoEvents();
            }
        }

        private void wipeL()
        {
            int w;
            int h;
            int x;
            int y;
            Color clr;
            w = this.Width;
            h = this.Height;

            for (x = 0; x < w; x++)
            {
                for (y = 0; y < h; y++)
                {
                    clr = bmp2.GetPixel( x, y );
                    bmp1.SetPixel( x, y, clr );
                }
                pictureBox1.Image = bmp1;
                pictureBox1.Refresh();
                Application.DoEvents();
            }
        }

        private void wipeR()
        {
            int w;
            int h;
            int x;
            int y;
            Color clr;
            w = this.Width;
            h = this.Height;

            for (x = w - 1; x >= 0; x--)
            {
                for (y = 0; y < h; y++)
                {
                    clr = bmp2.GetPixel( x, y );
                    bmp1.SetPixel( x, y, clr );
                }
                pictureBox1.Image = bmp1;
                pictureBox1.Refresh();
                Application.DoEvents();
            }
        }

        private void wipeLR()
        {
            int w;
            int h;
            int x;
            int y;
            Color clr;
            w = this.Width;
            h = this.Height;

            for (x = 0; x < w / 2; x++)
            {
                for (y = 0; y < h; y++)
                {
                    clr = bmp2.GetPixel( x, y );
                    bmp1.SetPixel( x, y, clr );
                    clr = bmp2.GetPixel( ( w - 1 ) - x, y );
                    bmp1.SetPixel( ( w - 1 ) - x, y, clr );
                }
                pictureBox1.Image = bmp1;
                pictureBox1.Refresh();
                Application.DoEvents();
            }
        }

        private void wipeRL()
        {
            int w;
            int h;
            int x;
            int y;
            Color clr;
            w = this.Width;
            h = this.Height;

            for (x = ( w - 1 ) / 2; x >= 0; x--)
            {
                for (y = 0; y < h; y++)
                {
                    clr = bmp2.GetPixel( x, y );
                    bmp1.SetPixel( x, y, clr );
                    clr = bmp2.GetPixel( ( w - 1 ) - x, y );
                    bmp1.SetPixel( ( w - 1 ) - x, y, clr );
                }
                pictureBox1.Image = bmp1;
                pictureBox1.Refresh();
                Application.DoEvents();
            }
        }

        private void diagTL()
        {
            int x, y, x1, y1;
            Color tcol2;
            for (x = 0; x < this.Width; x++)
            {
                y = 0;
                for (x1 = x; x1 >= 0; x1--)
                {
                    tcol2 = bmp2.GetPixel( x1, y );
                    bmp1.SetPixel( x1, y, tcol2 );
                    y++;
                    if (y >= this.Height)
                    {
                        y = 0;
                        x1 = -1;
                    }
                }
                pictureBox1.Image = bmp1;
                pictureBox1.Refresh();
                Application.DoEvents();
            }
            for (y = 0; y < this.Height; y++)
            {
                x1 = this.Width - 1;
                for (y1 = y; y1 < this.Height; y1++)
                {
                    tcol2 = bmp2.GetPixel( x1, y1 );
                    bmp1.SetPixel( x1, y1, tcol2 );
                    x1--;
                }
                pictureBox1.Image = bmp1;
                pictureBox1.Refresh();
                Application.DoEvents();
            }
        }

        private void diagTR()
        {
            int x, y, x1, y1;
            Color tcol2;
            for (x = this.Width; x >= 0; x--)
            {
                y = 0;
                for (x1 = x; x1 < this.Width; x1++)
                {
                    tcol2 = bmp2.GetPixel( x1, y );
                    bmp1.SetPixel( x1, y, tcol2 );
                    y++;
                    if (y >= this.Height)
                    {
                        y = 0;
                        x1 = this.Width;
                    }
                }
                pictureBox1.Image = bmp1;
                pictureBox1.Refresh();
                Application.DoEvents();
            }
            for (y = 0; y < this.Height; y++)
            {
                x1 = 0;
                for (y1 = y; y1 < this.Height; y1++)
                {
                    tcol2 = bmp2.GetPixel( x1, y1 );
                    bmp1.SetPixel( x1, y1, tcol2 );
                    x1++;
                }
                pictureBox1.Image = bmp1;
                pictureBox1.Refresh();
                Application.DoEvents();
            }
        }

        private void diagBR()
        {
            int x, y, x1, y1;
            Color tcol2;
            for (x = this.Width; x >= 0; x--)
            {
                y = this.Height - 1;
                for (x1 = x; x1 < this.Width; x1++)
                {
                    tcol2 = bmp2.GetPixel( x1, y );
                    bmp1.SetPixel( x1, y, tcol2 );
                    y--;
                    if (y < 0)
                    {
                        y = 0;
                        x1 = this.Width;
                    }
                }
                pictureBox1.Image = bmp1;
                pictureBox1.Refresh();
                Application.DoEvents();
            }
            for (y = this.Height - 1; y >= 0; y--)
            {
                x1 = 0;
                for (y1 = y; y1 >= 0; y1--)
                {
                    tcol2 = bmp2.GetPixel( x1, y1 );
                    bmp1.SetPixel( x1, y1, tcol2 );
                    x1++;
                }
                pictureBox1.Image = bmp1;
                pictureBox1.Refresh();
                Application.DoEvents();
            }
        }

        private void diagBL()
        {
            int x, y, x1, y1;
            Color tcol2;
            for (x = 0; x < this.Width; x++)
            {
                y = this.Height - 1;
                for (x1 = x; x1 >= 0; x1--)
                {
                    tcol2 = bmp2.GetPixel( x1, y );
                    bmp1.SetPixel( x1, y, tcol2 );
                    y--;
                    if (y < 0)
                    {
                        y = 0;
                        x1 = -1;
                    }
                }
                pictureBox1.Image = bmp1;
                pictureBox1.Refresh();
                Application.DoEvents();
            }
            for (y = this.Height - 1; y >= 0; y--)
            {
                x1 = this.Width - 1;
                for (y1 = y; y1 >= 0; y1--)
                {
                    tcol2 = bmp2.GetPixel( x1, y1 );
                    bmp1.SetPixel( x1, y1, tcol2 );
                    x1--;
                }
                pictureBox1.Image = bmp1;
                pictureBox1.Refresh();
                Application.DoEvents();
            }
        }

        private void slide()
        {
            int r, p, d, x, y;
            Color tcol2;
            Random random = new();
            r = random.Next( 0, 10 ) + 2;
            p = this.Height / r;
            for (x = 0; x < this.Width; x++)
            {
                d = 1;
                for (y = 0; y < this.Height; y++)
                {
                    if (y % p == 0)
                        d = -d;
                    if (d == 1)
                    {
                        tcol2 = bmp2.GetPixel( x, y );
                        bmp1.SetPixel( x, y, tcol2 );
                    }
                    else
                    {
                        tcol2 = bmp2.GetPixel( ( this.Width - 1 ) - x, y );
                        bmp1.SetPixel( ( this.Width - 1 ) - x, y, tcol2 );
                    }
                }
                pictureBox1.Image = bmp1;
                pictureBox1.Refresh();
                Application.DoEvents();
            }
        }

        private void Espiral()
        {
            int p = this.Width * this.Height;
            int c = 0;
            int dx = 1;
            int dy = 0;
            int x1 = 0;
            int y1 = 0;
            int xMax = this.Width;
            int yMax = this.Height;
            int xMin = 0;
            int yMin = 0;
            int a;
            Color tcol2;
            Graphics g;
            Random random = new();
            a = random.Next( 0, 2 );
            //a = 1;
            g = Graphics.FromImage( bmp1 );
            for (c = 0; c < p; c++)
            {
                x1 += dx;
                y1 += dy;
                if (( x1 == xMax - 1 ) && ( dx == 1 ))
                {
                    dx = 0;
                    dy = 1;
                    xMax--;
                }
                if (( y1 == yMax - 1 ) && ( dy == 1 ))
                {
                    dx = -1;
                    dy = 0;
                    yMax--;
                }
                if (( x1 == xMin + 1 ) && ( dx == -1 ))
                {
                    dx = 0;
                    dy = -1;
                    xMin++;
                }
                if (( y1 == yMin + 1 ) && ( dy == -1 ))
                {
                    dx = 1;
                    dy = 0;
                    yMin++;
                }
                tcol2 = bmp2.GetPixel( x1, y1 );
                if (a == 1)
                {
                    if (( yMin != yMax ) && ( xMin != xMax ))
                    {
                        g.DrawLine( new Pen( tcol2, 1 ), x1, y1, this.Width / 2, this.Height / 2 );
                    }
                }
                bmp1.SetPixel( x1, y1, tcol2 );
                if (( c % 1000 ) == 0)
                {
                    pictureBox1.Image = bmp1;
                    pictureBox1.Refresh();
                    Application.DoEvents();
                }

            }
            g.Dispose();
        }

        private void splitEvenH()
        {
            int x;
            int y;
            Color tcol2;
            for (x = 0; x < this.Width; x++)
            {
                for (y = 0; y < this.Height; y++)
                {
                    if (( y % 2 ) == 0)
                    {
                        tcol2 = bmp2.GetPixel( this.Width - 1 - x, y );
                        bmp1.SetPixel( this.Width - 1 - x, y, tcol2 );
                    }
                    else
                    {
                        tcol2 = bmp2.GetPixel( x, y );
                        bmp1.SetPixel( x, y, tcol2 );
                    }
                }
                pictureBox1.Image = bmp1;
                pictureBox1.Refresh();
                Application.DoEvents();
            }
        }

        private void splitEvenV()
        {
            int x, y;
            Color tcol2;
            for (y = this.Height - 1; y >= 0; y--)
            {
                for (x = 0; x < this.Width; x++)
                {
                    if (( x % 2 ) == 0)
                    {
                        tcol2 = bmp2.GetPixel( x, y );
                        bmp1.SetPixel( x, y, tcol2 );
                    }
                    else
                    {
                        tcol2 = bmp2.GetPixel( x, this.Height - 1 - y );
                        bmp1.SetPixel( x, this.Height - 1 - y, tcol2 );
                    }
                }
                pictureBox1.Image = bmp1;
                pictureBox1.Refresh();
                Application.DoEvents();
            }
        }
        private void circular()
        {
            int a, ax, ay, c, x1, y1, tx, ty;
            double xc, yc, d, r, ag, b, x, y, dx, dy;

            Color tcol2;
            Graphics g;
            Random random = new();
            g = Graphics.FromImage( bmp1 );
            a = random.Next( 0, 2 );
            c = random.Next( 0, 2 );
            d = Math.Sqrt( this.Width * this.Width + this.Height * this.Height );
            xc = ( this.Width / 2 ) - 1;
            yc = ( this.Height / 2 ) - 1;
            ax = -1;
            ay = -1;

            if (c == 0)
            {
                for (r = 0; r <= d / 2; r++)
                {
                    for (ag = 0; ag <= 90; ag += 0.03)
                    {
                        b = ag * Math.PI / 180;
                        x = xc + r * Math.Sin( b );
                        y = yc + r * Math.Cos( b );
                        x1 = Convert.ToInt32( x );
                        y1 = Convert.ToInt32( y );
                        if (( x1 >= 0 ) && ( x1 < this.Width ) && ( y1 >= 0 ) && ( y1 < this.Height ))
                        {
                            if (( x1 != ax ) || ( y1 != ay ))
                            {
                                dx = x - xc;
                                dy = y - yc;
                                tx = Convert.ToInt32( xc - dx );
                                ty = Convert.ToInt32( yc - dy );
                                tcol2 = bmp2.GetPixel( x1, y1 );
                                bmp1.SetPixel( x1, y1, tcol2 );

                                if (ty >= 0 && ty <= this.Height)
                                {
                                    tcol2 = bmp2.GetPixel( x1, ty );
                                    bmp1.SetPixel( x1, ty, tcol2 );
                                }

                                if (ty >= 0 && ty <= this.Height && tx >= 0 && tx <= this.Width)
                                {
                                    tcol2 = bmp2.GetPixel( tx, ty );
                                    bmp1.SetPixel( tx, ty, tcol2 );
                                }

                                if (tx >= 0 && tx <= this.Width)
                                {
                                    tcol2 = bmp2.GetPixel( tx, y1 );
                                    bmp1.SetPixel( tx, y1, tcol2 );
                                }
                            }
                            ax = x1;
                            ay = y1;
                        }
                    }
                    pictureBox1.Image = bmp1;
                    pictureBox1.Refresh();
                    Application.DoEvents();
                }
            }
            else
            {
                for (r = d / 2; r >= 0; r--)
                {
                    for (ag = 0; ag <= 90; ag += 0.03)
                    {
                        b = ag * Math.PI / 180;
                        x = xc + r * Math.Sin( b );
                        y = yc + r * Math.Cos( b );
                        x1 = Convert.ToInt32( x );
                        y1 = Convert.ToInt32( y );
                        if (( x1 >= 0 ) && ( x1 < this.Width ) && ( y1 >= 0 ) && ( y1 < this.Height ))
                        {
                            if (( x != ax ) || ( y != ay ))
                            {
                                dx = x - xc;
                                dy = y - yc;
                                tx = Convert.ToInt32( xc - dx );
                                ty = Convert.ToInt32( yc - dy );
                                tcol2 = bmp2.GetPixel( x1, y1 );
                                bmp1.SetPixel( x1, y1, tcol2 );
                                if (a == 1)
                                    g.DrawLine( new Pen( tcol2, 1 ), x1, y1, this.Width / 2, this.Height / 2 );

                                if (ty >= 0 && ty <= this.Height)
                                {
                                    tcol2 = bmp2.GetPixel( x1, ty );
                                    bmp1.SetPixel( x1, ty, tcol2 );
                                    if (a == 1)
                                        g.DrawLine( new Pen( tcol2, 1 ), x1, ty, this.Width / 2, this.Height / 2 );
                                }

                                if (ty >= 0 && ty <= this.Height && tx >= 0 && tx <= this.Width)
                                {
                                    tcol2 = bmp2.GetPixel( tx, ty );
                                    bmp1.SetPixel( tx, ty, tcol2 );
                                    if (a == 1)
                                        g.DrawLine( new Pen( tcol2, 1 ), tx, ty, this.Width / 2, this.Height / 2 );
                                }

                                if (tx >= 0 && tx <= this.Width)
                                {
                                    tcol2 = bmp2.GetPixel( tx, y1 );
                                    bmp1.SetPixel( tx, y1, tcol2 );
                                    if (a == 1)
                                        g.DrawLine( new Pen( tcol2, 1 ), tx, y1, this.Width / 2, this.Height / 2 );
                                }
                            }
                            ax = Convert.ToInt32( x );
                            ay = Convert.ToInt32( y );
                        }
                    }
                    pictureBox1.Image = bmp1;
                    pictureBox1.Refresh();
                    Application.DoEvents();
                }
            }
            g.Dispose();
        }

        private void opCode()
        {
            int a, r, x, y;
            double ratio, yr;
            Graphics g;
            g = Graphics.FromImage( bmp1 );
            Random random = new();
            ratio = ( (double)this.Height / (double)this.Width ) * 5;
            a = random.Next( 1, 12 );
            //a = 11;
            switch (a)
            {
                case 1:
                    for (x = this.Width - 1; x >= 0; x -= 5)
                    {
                        g.DrawImage( img1, 0, 0, x, this.Height );
                        g.DrawImage( img2, x, 0, this.Width - x, this.Height );
                        pictureBox1.Image = bmp1;
                        pictureBox1.Refresh();
                        Application.DoEvents();
                    }
                    break;
                case 2:
                    for (x = 0; x < this.Width; x += 5)
                    {
                        g.DrawImage( img1, x, 0, this.Width - x, this.Height );
                        g.DrawImage( img2, 0, 0, x, this.Height );
                        pictureBox1.Image = bmp1;
                        pictureBox1.Refresh();
                        Application.DoEvents();
                    }
                    break;
                case 3:
                    yr = this.Height / 2;
                    for (x = this.Width / 2; x >= 0; x -= 5)
                    {
                        r = this.Width - 2 * x;
                        yr -= ratio;
                        y = Convert.ToInt32( yr );
                        g.DrawImage( img2, x, y, r + 1, ( this.Height - 2 * y ) + 1 );
                        pictureBox1.Image = bmp1;
                        pictureBox1.Refresh();
                        Application.DoEvents();
                    }
                    break;
                case 4:
                    for (x = this.Width / 2; x >= 0; x -= 5)
                    {
                        r = this.Width - 2 * x;
                        g.DrawImage( img2, x, 0, r + 1, this.Height );
                        pictureBox1.Image = bmp1;
                        pictureBox1.Refresh();
                        Application.DoEvents();
                    }
                    break;
                case 5:
                    for (y = this.Height / 2; y >= 0; y -= 5)
                    {
                        r = this.Height - 2 * y;
                        g.DrawImage( img2, 0, y, this.Width, r + 1 );
                        pictureBox1.Image = bmp1;
                        pictureBox1.Refresh();
                        Application.DoEvents();
                    }
                    break;
                case 6:
                    for (x = 1; x < this.Width; x += 5)
                    {
                        g.DrawImage( img2, 0, 0, x, this.Height );
                        pictureBox1.Image = bmp1;
                        pictureBox1.Refresh();
                        Application.DoEvents();
                    }
                    break;
                case 7:
                    for (y = 1; y < this.Height; y += 5)
                    {
                        g.DrawImage( img2, 0, 0, this.Width, y );
                        pictureBox1.Image = bmp1;
                        pictureBox1.Refresh();
                        Application.DoEvents();
                    }
                    break;
                case 8:
                    for (x = this.Width - 1; x >= 0; x -= 5)
                    {
                        g.DrawImage( img2, x, 0, this.Width - x, this.Height );
                        pictureBox1.Image = bmp1;
                        pictureBox1.Refresh();
                        Application.DoEvents();
                    }
                    break;
                case 9:
                    for (y = this.Height - 1; y >= 0; y -= 5)
                    {
                        g.DrawImage( img2, 0, y, this.Width, this.Height - y );
                        pictureBox1.Image = bmp1;
                        pictureBox1.Refresh();
                        Application.DoEvents();
                    }
                    break;
                case 10:
                    for (y = this.Height; y >= 0; y -= 5)
                    {
                        g.DrawImage( img1, 0, 0, this.Width, y );
                        g.DrawImage( img2, 0, y, this.Width, this.Height - y );
                        pictureBox1.Image = bmp1;
                        pictureBox1.Refresh();
                        Application.DoEvents();
                    }
                    break;
                case 11:
                    for (y = 0; y < this.Height; y += 5)
                    {
                        g.DrawImage( img1, 0, y, this.Width, this.Height - y );
                        g.DrawImage( img2, 0, 0, this.Width, y );
                        pictureBox1.Image = bmp1;
                        pictureBox1.Refresh();
                        Application.DoEvents();
                    }
                    break;

            }
            g.Dispose();
        }

        private void entrelacoV()
        {
            int[] l = new int[this.Width];
            int[] m = new int[this.Width];
            double[] n = new double[this.Width];
            double a, r;
            int x, y, z, s, i, j;
            int tw = this.Width - 1;
            int th = this.Height - 1;

            Color tcol2;
            //Graphics g;
            //Random random = new();
            //g = Graphics.FromImage(bmp1);
            for (i = 0; i <= tw; i++)
            {
                n[i] = -1;
            }
            a = 1;
            i = 0;
            while (a < tw)
            {
                for (r = 1; r <= a; r++)
                {
                    s = Convert.ToInt32( r * Convert.ToDouble( tw ) / a );
                    if (m[s] == 0)
                    {
                        m[s] = 1;
                        n[i] = Convert.ToDouble( tw ) / a;
                        l[i] = s;
                        i++;
                    }
                }
                a *= 2;
            }
            for (s = i; s <= tw; s++)
            {
                for (j = 0; j <= tw; j++)
                {
                    if (m[j] == 0)
                    {
                        m[j] = 1;
                        n[s] = 1;
                        l[s] = j;
                    }
                }
            }
            for (x = 0; x <= tw; x++)
            {
                for (y = 0; y <= th; y++)
                {
                    tcol2 = bmp2.GetPixel( l[x], y );
                    for (z = l[x]; z >= ( l[x] - ( n[x] ) ); z--)
                    {
                        bmp1.SetPixel( z, y, tcol2 );
                    }

                }
                pictureBox1.Image = bmp1;
                pictureBox1.Refresh();
                Application.DoEvents();
            }
        }

        private void entrelacoH()
        {
            int[] l = new int[this.Height];
            int[] m = new int[this.Height];
            double[] n = new double[this.Height];
            double a, r;
            int x, y, z, s, i, j;
            int tw = this.Width - 1;
            int th = this.Height - 1;

            Color tcol2;
            //Graphics g;
            //Random random = new();
            //g = Graphics.FromImage(bmp1);
            for (i = 0; i <= th; i++)
            {
                n[i] = -1;
            }
            a = 1;
            i = 0;
            while (a < th)
            {
                for (r = 1; r <= a; r++)
                {
                    s = Convert.ToInt32( r * Convert.ToDouble( th ) / a );
                    if (m[s] == 0)
                    {
                        m[s] = 1;
                        n[i] = Convert.ToDouble( th ) / a;
                        l[i] = s;
                        i++;
                    }
                }
                a *= 2;
            }
            for (s = i; s <= th; s++)
            {
                for (j = 0; j <= th; j++)
                {
                    if (m[j] == 0)
                    {
                        m[j] = 1;
                        n[s] = 1;
                        l[s] = j;
                    }
                }
            }
            for (y = 0; y <= th; y++)
            {
                for (x = 0; x <= tw; x++)
                {
                    tcol2 = bmp2.GetPixel( x, l[y] );
                    for (z = l[y]; z >= ( l[y] - ( n[y] ) ); z--)
                    {
                        bmp1.SetPixel( x, z, tcol2 );
                    }

                }
                pictureBox1.Image = bmp1;
                pictureBox1.Refresh();
                Application.DoEvents();
            }
        }

        private void quadDraw(int x1, int x2, int y1, int y2, Graphics g)
        {
            Color tcol2;

            int dx, dy;
            int mx, my;
            mx = ( x1 + x2 ) / 2;
            my = ( y1 + y2 ) / 2;
            dx = x2 - x1;
            dy = y2 - y1;

            //area = dx * dy;
            if (( dx < 8 ) || ( dy < 8 ))
            {
                for (mx = x1; mx <= x2; mx++)
                {
                    for (my = y1; my <= y2; my++)
                    {
                        tcol2 = bmp2.GetPixel( mx, my );
                        
                        bmp1.SetPixel( mx, my, tcol2 );
                    }
                    pictureBox1.Image = bmp1;
                }
                return;
            }
                

            tcol2 = bmp2.GetPixel( mx, my );
            SolidBrush brush = new( tcol2 );
            g.FillRectangle( brush, x1, y1, dx, dy );
        }
        private void quad(int x1, int x2, int y1, int y2, Graphics g)
        {
            //int area;

            //int x, y;
            int dx, dy;
            int mx, my;
            mx = ( x1 + x2 ) / 2;
            my = ( y1 + y2 ) / 2;
            dx = x2 - x1;
            dy = y2 - y1;

            if (( dx >= 8 ) || ( dy >= 8 ))
            {
                quadDraw( x1, mx, y1, my, g );
                quadDraw( mx, x2, y1, my, g );
                quadDraw( x1, mx, my, y2, g );
                quadDraw( mx, x2, my, y2, g );
            }
            else
            {
                return;
            }

            pictureBox1.Image = bmp1;
            //pictureBox1.Refresh();
            Application.DoEvents();
            //if (area > 4)
            //{
            quad( x1, mx, y1, my, g );
            quad( mx, x2, y1, my, g );
            quad( x1, mx, my, y2, g );
            quad( mx, x2, my, y2, g );
            //}

        }

        private void quadrant()
        {
            Graphics g;
            g = Graphics.FromImage( bmp1 );
            quad( 0, this.Width - 1, 0, this.Height - 1, g );
            g.Dispose();
            fader();
        }

        private void redimensiona()
        {
            int i, j, t, x, y, c, d, e, f, escala;
            int tw, th;
            Color tcol2;

            Graphics g;
            g = Graphics.FromImage( bmp1 );

            th = this.Height - 1;
            tw = this.Width - 1;
            t = tw * th;
            escala = 1;
            while (t > 2)
            {
                x = ( tw + 1 ) / escala;
                y = ( th + 1 ) / escala;
                escala *= 2;
                t = x * y;
                //a = tw / x;
                //b = th / y;
                c = 0;
                //d = 0;

                for (i = 0; i < tw - 1; i += x)
                {
                    c++;
                    d = 0;
                    for (j = 0; j < th - 1; j += y)
                    {
                        d++;
                        e = i + ( ( ( x * c ) - i ) / 2 );
                        f = j + ( ( ( y * d ) - j ) / 2 );

                        //Debug.WriteLine("getpixel(" + e + ", " + f + ")");

                        if (e < tw && f < th)
                        {
                            tcol2 = bmp2.GetPixel( e, f );
                            //Debug.WriteLine("fillrectangle(" + i + ", " + j + "," + ((x * c) - i) + "," + ((y * d) - j) + ")");
                            SolidBrush brush = new( tcol2 );
                            g.FillRectangle( brush, i, j, ( x * c ) - i, ( y * d ) - j );

                        }
                    }
                    pictureBox1.Image = bmp1;
                    pictureBox1.Refresh();
                    Application.DoEvents();
                }
            }
            g.Dispose();
        }

        private void fader()
        {
            ImageTransparency it;
            it = new ImageTransparency();
            float i;
            Bitmap b;
            Graphics g;
            for (i = 0; i <= 300; i++)
            {
                b = it.ChangeOpacity( bmp2, i / 300 );
                g = Graphics.FromImage( bmp1 );
                g.DrawImage( b, 0, 0, this.Width, this.Height );
                g.Dispose();
                pictureBox1.Image = bmp1;
                pictureBox1.Refresh();
                Application.DoEvents();
            }
        }

        private void DrawShape()
        {
            Random random = new();


            //algo = 21;
            //try
            //{
            tmrMain.Enabled = false;

            switch (algo)
            {
                case 0:
                    randPixels();
                    break;
                case 1:
                    randV();
                    break;
                case 2:
                    randH();
                    break;
                case 3:
                    wipeTB();
                    break;
                case 4:
                    wipeBT();
                    break;
                case 5:
                    wipeL();
                    break;
                case 6:
                    wipeR();
                    break;
                case 7:
                    wipeLR();
                    break;
                case 8:
                    wipeRL();
                    break;
                case 9:
                    diagTL();
                    break;
                case 10:
                    diagTR();
                    break;
                case 11:
                    diagBR();
                    break;
                case 12:
                    diagBL();
                    break;
                case 13:
                    slide();
                    break;
                case 14:
                    Espiral();
                    break;
                case 15:
                    splitEvenH();
                    break;
                case 16:
                    splitEvenV();
                    break;
                case 17:
                    circular();
                    break;
                case 18:
                    opCode();
                    break;
                case 19:
                    entrelacoV();
                    break;
                case 20:
                    entrelacoH();
                    break;
                case 21:
                    quadrant();
                    break;
                case 22:
                    redimensiona();
                    break;
                case 23:
                    fader();
                    break;
            }

            if (firstrun)
            {
                algo++;
                if (algo > 23)
                {
                    firstrun = false;
                    algo = random.Next( 23 );
                }
            }
            else
            {
                algo = random.Next( 23 );
            }


            pictureBox1.Image = bmp2;
            pictureBox1.Refresh();
            Application.DoEvents();

            antcnt = cnt;

            img1 = LerImagem( folderFile[antcnt] );

            random = new();

            //cnt++;
            cnt = random.Next( folderFile.Count );

            img2 = LerImagem( folderFile[cnt] );

            //img2 = Image.FromFile(folderFile[cnt]);
            Size s = new( this.Width, this.Height );
            bmp2 = new Bitmap( img2, s );

            if (cnt == folderFile.Count)
                cnt = 0;
            tmrMain.Enabled = true;

            //}
            //catch
            //{ }
        }



        private void tmrMain_Tick(object sender, EventArgs e)
        {
            this.DrawShape();
        }

        private void pictureBox1_Click(object sender, EventArgs e)
        {

        }

        private void pictureBox1_MouseMove(object sender, MouseEventArgs e)
        {
            ScreenSaverForm_MouseMove( sender, e );
        }

        private void pictureBox1_MouseDown(object sender, MouseEventArgs e)
        {
            ScreenSaverForm_MouseDown( sender, e );
        }
    }
}
