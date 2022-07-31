using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace ScreenSaver
{
    public partial class SettingsForm : Form
    {
        public SettingsForm()
        {
            InitializeComponent();
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
                Settings settings = new();

                // Load the settings
                settings.LoadSettings();



                txtFolder.Text = settings.ImagesFolder;
                folderBrowserDialog1.SelectedPath = settings.ImagesFolder;
            }
            catch (Exception ex)
            {
                MessageBox.Show(string.Format("Error loading settings! {0}", ex.Message), "Dave on C# Screen Saver", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        private void SaveSettings()
        {
            try
            {
                // Create an instance of the Settings class
                Settings settings = new();


                settings.ImagesFolder = txtFolder.Text;

                // Save the settings
                settings.SaveSettings();
            }
            catch (Exception ex)
            {
                MessageBox.Show(string.Format("Erro ao gravar as configurações! {0}", ex.Message), "Dave on C# Screen Saver", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
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

        private void btnEscolhe_Click(object sender, EventArgs e)
        {
            if (folderBrowserDialog1.ShowDialog() == DialogResult.OK) txtFolder.Text = folderBrowserDialog1.SelectedPath;
        }

        private void folderBrowserDialog1_HelpRequest(object sender, EventArgs e)
        {

        }
    }
}
