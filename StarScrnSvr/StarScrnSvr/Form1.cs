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

namespace StarScrnSvr
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

        private static int SCALE = 10;
        private Graphics graph;
        private Bitmap bmp;
        private g G;


        #endregion

        public ScreenSaverForm()
        {
            InitializeComponent();
        }

        public ScreenSaverForm(IntPtr previewHandle)
        {
            InitializeComponent();

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

        private void ScreenSaverForm_Load(object sender, EventArgs e)
        {
            this.settings = new Settings();
            this.settings.LoadSettings();
            this.active = false;

            bmp = new Bitmap( this.Width, this.Height );

            pictureBox1.SizeMode = PictureBoxSizeMode.StretchImage;
            pictureBox1.Top = 0;
            pictureBox1.Left = 0;
            pictureBox1.Width = this.Width;
            pictureBox1.Height = this.Height;
            pictureBox1.Image = bmp;

            graph = Graphics.FromImage( bmp );

            G = new g();

            G.Screen.x = this.Width * SCALE;
            G.Screen.y = this.Height * SCALE;
            G.Speed = 20;

            Cursor.Hide();

            tmrMain.Enabled = true;
        }

        void DrawStar(STAR Star, Color Clr)
        {
            try
            {
                int x, y;
                int r1, g1, b1;
                x = (int)Star.Loc.x / SCALE;
                y = (int)Star.Loc.y / SCALE;
                r1 = Clr.R;
                g1 = Clr.G;
                b1 = Clr.B;
                Clr = Color.FromArgb( r1 * (int)Star.Dist / 3000, g1 * (int)Star.Dist / 3000, b1 * (int)Star.Dist / 3000 );
                bmp.SetPixel( x, y, Clr );

                if (Star.Size == 2)
                {
                    bmp.SetPixel( x + 1, y, Clr );
                    bmp.SetPixel( x, y + 1, Clr );
                    bmp.SetPixel( x + 1, y + 1, Clr );
                }
                pictureBox1.Image = bmp;
            }
            catch
            {

            }

        }

        void NewStar( int Staridx)
        {
            Random random = new Random();
            G.Stars[Staridx] = new STAR();
            G.Stars[Staridx].Loc.x = G.Screen.x / 2 - SCALE + random.Next( SCALE * 2 );
            G.Stars[Staridx].Loc.y = G.Screen.y / 2 - SCALE + random.Next( SCALE * 2 );
            G.Stars[Staridx].Dist = 0;
            G.Stars[Staridx].Size = random.Next( 10 ) == 9 ? 2 : 1;

            do
            {
                G.Stars[Staridx].Dir.x = SCALE * 10 - random.Next( SCALE * 20 );
                G.Stars[Staridx].Dir.y = SCALE * 10 - random.Next( SCALE * 20 );
            }
            while (G.Stars[Staridx].Dir.x == 0 && G.Stars[Staridx].Dir.y == 0);

            switch (random.Next( 40 ))
            {
                case 39:
                    G.Stars[Staridx].Clr = Color.Red;// converttorgb( LIGHTRED );
                    break;

                case 38:
                    G.Stars[Staridx].Clr = Color.LightGray; // converttorgb( LIGHTGRAY );
                    break;

                case 37:
                    G.Stars[Staridx].Clr = Color.LightBlue; //converttorgb( LIGHTBLUE );
                    break;

                case 36:
                    G.Stars[Staridx].Clr = Color.Yellow; //converttorgb( YELLOW );
                    break;

                case 35:
                    G.Stars[Staridx].Clr = Color.LightPink; //converttorgb( LIGHTMAGENTA );
                    break;

                case 34:
                    G.Stars[Staridx].Clr = Color.DarkGray; //converttorgb( DARKGRAY );
                    break;

                default:
                    G.Stars[Staridx].Clr = Color.White; //converttorgb( WHITE );
                    break;
            }
        }

        void MoveStars()
        {
            int i;
            STAR Star;
            long origX;
            long origY;

            //i = G.NumStars - 1;
            for (i = G.NumStars - 1; i >= 0; i--)
            {
                Star = G.Stars[i];
                DrawStar( Star, Color.Black );
                origX = Star.Loc.x;
                origY = Star.Loc.y;
                Star.Loc.x += Star.Dir.x * G.Speed / 30;
                Star.Loc.y += Star.Dir.y * G.Speed / 30;
                origX = Star.Loc.x - origX;
                origY = Star.Loc.y - origY;

                if (Star.Dist < 3000)
                {
                    Star.Dist += Math.Sqrt( origX * origX + origY * origY );
                }

                if (Star.Dist > 3000)
                {
                    Star.Dist = 3000;
                }

                if (Star.Loc.x < 1 || Star.Loc.x >= G.Screen.x || Star.Loc.y < 1 || Star.Loc.y >= G.Screen.y)
                {
                    NewStar( i );
                }
                else
                {
                    DrawStar( Star, Star.Clr );
                }
            }
        }

        private void tmrMain_Tick(object sender, EventArgs e)
        {
            //Random random = new Random();
            if (G.NumStars < settings.NumStar)
            {
                NewStar( G.NumStars );
                G.NumStars++;
            }

            MoveStars();
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

        private void ScreenSaverForm_KeyDown(object sender, KeyEventArgs e)
        {
            if (!this.previewMode)
            {
                // Exit the screensaver if not in preview mode
                Application.Exit();
            }
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
