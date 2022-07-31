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

namespace RenomearTTF
{
    public partial class Form1 : Form
    {

        [DllImport("gdi32.dll")]
        static extern bool CreateScalableFontResource(uint fdwHidden, string lpszFontRes, string lpszFontFile, string lpszCurrentPath);

        /*
        Private Declare Function CreateScalableFontResource Lib "gdi32" Alias "CreateScalableFontResourceA" (ByVal fHidden As Long, ByVal lpszResourceFile As String, ByVal lpszFontFile As String, ByVal lpszCurrentPath As String) As Long

Public Const CSIDL_FONTS = &H14
*/

        private List<string> folderFile;

        public Form1()
        {
            InitializeComponent();
            folderFile = new List<string>();
        }

        private String getFontName(String FileNameTTF)
        {
            String result = "";
            string pasta;
            string ext;
            string nome;
            int i;
            int j;
            


            string TempName = Application.StartupPath + "\\TEMP.FOT";
            string FontName = "";
            if (File.Exists(TempName))
            {
                // If file found, delete it    
                File.Delete(TempName);
            }
            if (CreateScalableFontResource(1, TempName, FileNameTTF, String.Empty))
            {
                //Encoding enc = Encoding.GetEncoding(1252);
                byte[] fileBytes = File.ReadAllBytes(TempName);
                string Buffer = "";
                for (i = 0; i < fileBytes.Length; i++)
                {
                    Buffer+=Convert.ToChar(fileBytes[i]);
                }
                /*using (StreamReader sr = new StreamReader(TempName, Encoding.ASCII, false))
                {
                    String line;
                    while ((line = sr.ReadLine()) != null)
                    {
                        Buffer += line;
                    }
                }*/

                int iPos = Buffer.IndexOf("FONTRES:") + 8;
                int iEndPos = Buffer.IndexOf(Char.MinValue, iPos);
                FontName = Buffer[iPos..iEndPos];
                FontName = FontName.Trim();
                result = FontName;
            }
            if (result == "")
            {
                i = FileNameTTF.LastIndexOf("\\");
                pasta = FileNameTTF[..i];
                j = FileNameTTF.LastIndexOf(".");
                ext = FileNameTTF[j..];

                result = pasta + "\\" + FileNameTTF.Substring(i + 1, (j - 1 - i)) + "------------------------[[[DEFEITO]]]" + ext;
                //result = FileNameTTF;
            }

            else
            {
                i = FileNameTTF.LastIndexOf("\\");
                pasta = FileNameTTF[..i];
                j = FileNameTTF.LastIndexOf(".");
                ext = FileNameTTF[j..];



                //result=result.Normalize(NormalizationForm.FormC);
                //Encoding code = Encoding.ASCII;
                Encoding code = Encoding.UTF8;
                // Array to hold encoded bytes.
                byte[] bytes;
                // Array to hold decoded characters.
                char[] chars = new char[255];
                // Create index for current position of character array.
                int index = 0;


                // Encode the string to a byte array.
                bytes = code.GetBytes(result);


                // Decode the bytes to a single character array.
                int count = code.GetCharCount(bytes);
                if (count + index >= chars.Length)
                    Array.Resize(ref chars, chars.Length + 255);

                int written = code.GetChars(bytes, 0, bytes.Length, chars, index);
                index += written;


                // Instantiate a single string containing the characters.
                string decodedString = new(chars, 0, index);

                result = decodedString;

                result = result.Replace("&", "");
                result = result.Replace("\"", "");
                result = result.Replace("!", "");
                result = result.Replace("<", "");
                result = result.Replace(">", "");
                result = result.Replace("[", "");
                result = result.Replace("]", "");
                result = result.Replace("*", "");
                result = result.Replace("/", "");
                result = result.Replace("\\", "");
                result = result.Replace("(", "");
                result = result.Replace(")", "");
                result = result.Replace("{", "");
                result = result.Replace("}", "");
                result = result.Replace(".", "");
                result = result.Replace("-", "");
                result = result.Replace("?", "_");
                result = result.Replace(":", "_");
                result = result.Replace("|", "");
                result = result.Replace("  ", " ");
                result = result.Replace("\u0099", "(TM)");

                if (ext.ToLower() == ".fon")
                {
                    nome = FileNameTTF.Substring(i + 1, (j - 1 - i));
                    nome = nome.Replace(" [" + result + "]", "");
                    result = nome + " [" + result + "]";
                }

                result = pasta + "\\" + result + ext;
            }

            return result;
        }


        private void lerPastas(string pasta)
        {
            try
            {
                string[] part1 = null, part2 = null, part3 = null;
                string[] p;
                int i;
                part1 = Directory.GetFiles(pasta, "*.ttf");
                part2 = Directory.GetFiles(pasta, "*.fon");
                part3 = Directory.GetFiles(pasta, "*.otf");
                folderFile.AddRange(part1);
                folderFile.AddRange(part2);
                folderFile.AddRange(part3);

                p = Directory.GetDirectories(pasta, "*.*");
                if (p.Length > 0)
                {
                    for (i = 0; i < p.Length; i++)
                    {
                        lerPastas(p[i]);
                    }
                }
            }
            catch (Exception)
            {

            }

        }

        private void button1_Click(object sender, EventArgs e)
        {
            int conta;
            int i;
            if (folderBrowserDialog1.ShowDialog() == DialogResult.OK)
            {
                listBox1.Items.Clear();
                listBox2.Items.Clear();
                txtFolder.Text = folderBrowserDialog1.SelectedPath;
                lerPastas(txtFolder.Text);
                conta = folderFile.Count;
                progressBar1.Maximum = conta-1;
                for (i = 0; i < conta; i++)
                {
                    progressBar1.Value = i;
                    listBox1.Items.Add(folderFile[i]);
                    if (i % 10==0) listBox1.Refresh();
                    this.Text = "Fontes: " + i;
                }
            }

        }

        private string Existe(string nome)
        {
            string ret = nome;
            string pasta;
            string ext;
            string orig;
            int i;
            int j;
            int conta = 1;

            i = nome.LastIndexOf("\\");
            pasta = nome[..i];
            j = nome.LastIndexOf(".");
            ext = nome[j..];
            orig = nome.Substring(i + 1, (j - 1 - i));
            while (File.Exists(nome))
            {
                nome = pasta + "\\" + orig + " [" + conta + "]" + ext;
                conta++;
            }
            ret = nome;

            return ret;
        }

        private void button2_Click(object sender, EventArgs e)
        {
            int conta = listBox1.Items.Count-1;
            int i;
            String s;
            string orig;
            string dest;
            progressBar1.Maximum = conta;
            for (i = 0; i < conta+1; i++)
            {
                s = getFontName(listBox1.Items[i].ToString());
                listBox2.Items.Add(s);
                progressBar1.Value = i;
                if (i % 10 == 0)
                    listBox2.Refresh();
            }
            for (i = conta; i >= 0; i--)
            {
                progressBar1.Value = conta - i;
                if (listBox1.Items[i].ToString().ToLower() == listBox2.Items[i].ToString().ToLower())
                {
                    listBox1.SelectedIndex = i;
                    listBox2.SelectedIndex = i;
                    listBox1.Items.RemoveAt(i);
                    listBox2.Items.RemoveAt(i);
                    Form1.ActiveForm.Text = "Fontes: " + listBox1.Items.Count;
                    if (i % 10 == 0)
                    {
                        listBox2.Refresh();
                        listBox1.Refresh();
                    }
                }
            }
            conta = listBox1.Items.Count - 1;
            for (i = 0; i < conta + 1; i++)
            {
                listBox1.SelectedIndex = i;
                listBox2.SelectedIndex = i;
                orig = listBox1.Items[i].ToString();
                dest = Existe(listBox2.Items[i].ToString());
                if (dest.IndexOf("------------------------[[[DEFEITO]]]") > 0)
                {
                    FileOperationAPIWrapper.MoveToRecycleBin(orig);
                }
                else
                {
                    System.IO.File.Move(orig, dest);

                }
                progressBar1.Value = i;
            }
        }
    }
}