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

namespace StarScrnSvr
{
    public class Settings
    {
        private readonly string settingsPath = Application.StartupPath + "\\starscreensaver.xml";
        private int numStar;
        private int spdStar;

        public int NumStar
        {
            get
            {
                return this.numStar;
            }
            set
            {
                this.numStar = value;
            }
        }
        public int SpdStar
        {
            get
            {
                return this.spdStar;
            }
            set
            {
                this.spdStar = value;
            }
        }

        public void LoadSettings()
        {
            try
            {
                // Create an instance of the Settings class
                Settings settings = new Settings();

                if (File.Exists( this.settingsPath ))
                {
                    // Create an instance of System.Xml.Serialization.XmlSerializer
                    XmlSerializer serializer = new XmlSerializer( typeof( Settings ) );

                    // Create an instance of System.IO.StreamReader 
                    // to point to the settings file on disk
                    StreamReader textReader = new StreamReader( this.settingsPath );

                    // Create an instance of System.Xml.XmlTextReader
                    // to read from the StreamReader
                    XmlTextReader xmlReader = new XmlTextReader( textReader );

                    if (serializer.CanDeserialize( xmlReader ))
                    {
                        // Assign the deserialized object to the new settings object
#pragma warning disable CS8600 // Conversão de literal nula ou possível valor nulo em tipo não anulável.
                        settings = (Settings)serializer.Deserialize( xmlReader );
#pragma warning restore CS8600 // Conversão de literal nula ou possível valor nulo em tipo não anulável.


                        this.numStar = settings.NumStar;
                        this.spdStar = settings.SpdStar;
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
                    //this.imagesFolder = imgFolders();
                    this.numStar = 300;
                    this.spdStar = 20;
                    this.SaveSettings();
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show( string.Format( "Erro ao recuperar as configurações deserializadas! {0}", ex.Message ), "Star C# Screen Saver", MessageBoxButtons.OK, MessageBoxIcon.Error );
            }
        }

        public void SaveSettings()
        {
            try
            {
                // Create an instance of the Settings class
                Settings settings = new Settings();

                settings.NumStar = this.numStar;
                settings.SpdStar = this.spdStar;


                // Create an instance of System.Xml.Serialization.XmlSerializer
                XmlSerializer serializer = new XmlSerializer( settings.GetType() );

                // Create an instance of System.IO.TextWriter
                // to save the serialized object to disk
                TextWriter textWriter = new StreamWriter( this.settingsPath );

                // Serialize the settings object
                serializer.Serialize( textWriter, settings );

                // Close the TextWriter
                textWriter.Close();
            }
            catch (Exception ex)
            {
                MessageBox.Show( string.Format( "Erro ao gravar as configurações serializadas! {0}", ex.Message ), "Star C# Screen Saver", MessageBoxButtons.OK, MessageBoxIcon.Error );
            }
        }
    }
}
