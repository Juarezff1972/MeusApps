using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace StarScrnSvr
{
    internal static class Program
    {
        /// <summary>
        /// Ponto de entrada principal para o aplicativo.
        /// </summary>
        [STAThread]
        static void Main(string[] args)
        {
            //Application.SetHighDpiMode(HighDpiMode.SystemAware);
            //Application.EnableVisualStyles();
            //Application.SetCompatibleTextRenderingDefault(false);
            //Application.Run(new ScreenSaverForm());
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault( false );

            // Check for any passed arguments
            if (args.Length > 0)
            {
                switch (args[0].ToLower().Trim().Substring( 0, 2 ))
                {
                    // Preview the screen saver
                    case "/p":
                        // args[1] is the handle to the preview window
                        ScreenSaverForm screenSaverForm = new ScreenSaverForm( new IntPtr( long.Parse( args[1] ) ) );
                        screenSaverForm.ShowDialog();
                        break;

                    // Show the screen saver
                    case "/s":
                        RunScreensaver();
                        break;

                    // Configure the screesaver's settings
                    case "/c":
                        // Show the settings form
                        SettingsForm settingsForm = new SettingsForm();
                        settingsForm.ShowDialog();
                        break;

                    // Show the screen saver
                    default:
                        RunScreensaver();
                        break;
                }
            }
            else
            {
                // No arguments were passed so we show the screensaver anyway
                RunScreensaver();
            }
        }

        private static void RunScreensaver()
        {
            ScreenSaverForm screenSaverForm = new ScreenSaverForm();
            screenSaverForm.ShowDialog();
        }
    }
}
