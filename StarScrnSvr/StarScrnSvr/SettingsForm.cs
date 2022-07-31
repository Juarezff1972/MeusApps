using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace StarScrnSvr
{
    public partial class SettingsForm : Form
    {
        public SettingsForm()
        {
            InitializeComponent();
        }

        private void btnOk_Click(object sender, EventArgs e)
        {
            SaveSettings();
            Application.Exit();
        }

        private void btnCancel_Click(object sender, EventArgs e)
        {
            Application.Exit();
        }

        private void SettingsForm_Load(object sender, EventArgs e)
        {
            LoadSettings();
        }

        private void LoadSettings()
        {
            try
            {
                // Create an instance of the Settings class
                Settings settings = new Settings();

                // Load the settings
                settings.LoadSettings();


                spdStars.Value = settings.SpdStar;
                numStars.Value = settings.NumStar;

            }
            catch (Exception ex)
            {
                MessageBox.Show( string.Format( "Erro ao gravar configurações! {0}", ex.Message ), "Star C# Screen Saver", MessageBoxButtons.OK, MessageBoxIcon.Error );
            }
        }

        private void SaveSettings()
        {
            try
            {
                // Create an instance of the Settings class
                Settings settings = new Settings();


                settings.SpdStar = (int)spdStars.Value;
                settings.NumStar = (int)numStars.Value;

                // Save the settings
                settings.SaveSettings();
            }
            catch (Exception ex)
            {
                MessageBox.Show( string.Format( "Erro ao gravar as configurações! {0}", ex.Message ), "Star C# Screen Saver", MessageBoxButtons.OK, MessageBoxIcon.Error );
            }
        }
    }
}
