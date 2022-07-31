using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using System.IO;
using System.Xml.Serialization;
using System.Xml;
using System.Windows.Forms;
using Microsoft.Win32;
using System.Security.Cryptography;
using System.Runtime.InteropServices;


namespace ScreenSaver
{
    public class Settings
    {




        #region Private Members

        private string imagesFolder;

        private readonly string settingsPath = Application.StartupPath + "\\screensaver.xml";


        #endregion

        #region Public Properties

        public string ImagesFolder
        {
            get { return this.imagesFolder; }
            set { this.imagesFolder = value; }
        }


        #endregion

        #region Constructor

        public Settings()
        {

            this.imagesFolder = imgFolders();
        }

        #endregion

        #region Methods


        private static string imgFolders()
        {
            string folder;
            //string folder1;


            folder = "";
            //Software\Microsoft\Windows\CurrentVersion\Explorer\User Shell Folders
            string softwareRegLoc = @"Software\Microsoft\Windows\CurrentVersion\Explorer\User Shell Folders";
            RegistryKey regKey = Registry.CurrentUser.OpenSubKey(softwareRegLoc, false);
            //RegistryKey subKey = regKey.OpenSubKey(subKeyName);
            folder = (string)regKey.GetValue("My Pictures");


            if (string.IsNullOrEmpty(folder))
            {
                folder = "";
            }

            return folder;
        }

        public void LoadSettings()
        {
            try
            {
                // Create an instance of the Settings class
                Settings settings = new();

                if (File.Exists(this.settingsPath))
                {
                    // Create an instance of System.Xml.Serialization.XmlSerializer
                    XmlSerializer serializer = new(typeof(Settings));

                    // Create an instance of System.IO.StreamReader 
                    // to point to the settings file on disk
                    StreamReader textReader = new(this.settingsPath);

                    // Create an instance of System.Xml.XmlTextReader
                    // to read from the StreamReader
                    XmlTextReader xmlReader = new(textReader);

                    if (serializer.CanDeserialize(xmlReader))
                    {
                        // Assign the deserialized object to the new settings object
#pragma warning disable CS8600 // Conversão de literal nula ou possível valor nulo em tipo não anulável.
                        settings = (Settings)serializer.Deserialize(xmlReader);
#pragma warning restore CS8600 // Conversão de literal nula ou possível valor nulo em tipo não anulável.


                        this.imagesFolder = settings.ImagesFolder;
                    }
                    else
                    {
                        // Save a new settings file
                        this.SaveSettings();
                    }

                    // Close the XmlTextReader
                    xmlReader.Close();
                    // Close the XmlTextReader
                    textReader.Close();
                }
                else
                {
                    // Save a new settings file
                    this.imagesFolder = imgFolders();
                    this.SaveSettings();
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show(string.Format("Error retrieving deserialized settings! {0}", ex.Message), "Dave on C# Screen Saver", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        public void SaveSettings()
        {
            try
            {
                // Create an instance of the Settings class
                Settings settings = new();

                settings.imagesFolder = this.imagesFolder;


                // Create an instance of System.Xml.Serialization.XmlSerializer
                XmlSerializer serializer = new(settings.GetType());

                // Create an instance of System.IO.TextWriter
                // to save the serialized object to disk
                TextWriter textWriter = new StreamWriter(this.settingsPath);

                // Serialize the settings object
                serializer.Serialize(textWriter, settings);

                // Close the TextWriter
                textWriter.Close();
            }
            catch (Exception ex)
            {
                MessageBox.Show(string.Format("Erro ao gravar as configurações serializadas! {0}", ex.Message), "C# Screen Saver", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        #endregion
    }
}
