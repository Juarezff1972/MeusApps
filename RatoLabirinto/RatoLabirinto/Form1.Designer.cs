
namespace RatoLabirinto
{
    partial class Form1
    {
        /// <summary>
        ///  Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        ///  Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        ///  Required method for Designer support - do not modify
        ///  the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Form1));
            this.pictureBox1 = new System.Windows.Forms.PictureBox();
            this.procBtn = new System.Windows.Forms.Button();
            this.editBtn = new System.Windows.Forms.Button();
            this.foundTXT = new System.Windows.Forms.Label();
            this.imageList1 = new System.Windows.Forms.ImageList(this.components);
            this.paredeBtn = new System.Windows.Forms.CheckBox();
            this.caminhoBtn = new System.Windows.Forms.CheckBox();
            this.ratoBtn = new System.Windows.Forms.CheckBox();
            this.saidaBtn = new System.Windows.Forms.CheckBox();
            this.gerarBtn = new System.Windows.Forms.Button();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).BeginInit();
            this.SuspendLayout();
            // 
            // pictureBox1
            // 
            this.pictureBox1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.pictureBox1.BackColor = System.Drawing.Color.Black;
            this.pictureBox1.Location = new System.Drawing.Point(21, 21);
            this.pictureBox1.Name = "pictureBox1";
            this.pictureBox1.Size = new System.Drawing.Size(1023, 528);
            this.pictureBox1.TabIndex = 0;
            this.pictureBox1.TabStop = false;
            this.pictureBox1.Paint += new System.Windows.Forms.PaintEventHandler(this.pictureBox1_Paint);
            this.pictureBox1.MouseDown += new System.Windows.Forms.MouseEventHandler(this.pictureBox1_MouseDown);
            this.pictureBox1.MouseMove += new System.Windows.Forms.MouseEventHandler(this.pictureBox1_MouseMove);
            this.pictureBox1.MouseUp += new System.Windows.Forms.MouseEventHandler(this.pictureBox1_MouseUp);
            // 
            // procBtn
            // 
            this.procBtn.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.procBtn.CausesValidation = false;
            this.procBtn.Enabled = false;
            this.procBtn.Location = new System.Drawing.Point(1063, 444);
            this.procBtn.Name = "procBtn";
            this.procBtn.Size = new System.Drawing.Size(91, 40);
            this.procBtn.TabIndex = 2;
            this.procBtn.Text = "Procurar";
            this.procBtn.UseVisualStyleBackColor = true;
            this.procBtn.Click += new System.EventHandler(this.procBtn_Click);
            // 
            // editBtn
            // 
            this.editBtn.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.editBtn.CausesValidation = false;
            this.editBtn.Location = new System.Drawing.Point(1063, 490);
            this.editBtn.Name = "editBtn";
            this.editBtn.Size = new System.Drawing.Size(91, 43);
            this.editBtn.TabIndex = 3;
            this.editBtn.Text = "Editar";
            this.editBtn.UseVisualStyleBackColor = true;
            this.editBtn.Click += new System.EventHandler(this.editBtn_Click);
            // 
            // foundTXT
            // 
            this.foundTXT.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.foundTXT.AutoSize = true;
            this.foundTXT.Location = new System.Drawing.Point(1063, 536);
            this.foundTXT.Name = "foundTXT";
            this.foundTXT.Size = new System.Drawing.Size(87, 15);
            this.foundTXT.TabIndex = 9;
            this.foundTXT.Text = "Não encontrou";
            // 
            // imageList1
            // 
            this.imageList1.ColorDepth = System.Windows.Forms.ColorDepth.Depth8Bit;
            this.imageList1.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("imageList1.ImageStream")));
            this.imageList1.TransparentColor = System.Drawing.Color.Transparent;
            this.imageList1.Images.SetKeyName(0, "leste");
            this.imageList1.Images.SetKeyName(1, "norte");
            this.imageList1.Images.SetKeyName(2, "oeste");
            this.imageList1.Images.SetKeyName(3, "sul");
            // 
            // paredeBtn
            // 
            this.paredeBtn.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.paredeBtn.Appearance = System.Windows.Forms.Appearance.Button;
            this.paredeBtn.Enabled = false;
            this.paredeBtn.Image = global::RatoLabirinto.Properties.Resources.parede;
            this.paredeBtn.Location = new System.Drawing.Point(1059, 21);
            this.paredeBtn.Name = "paredeBtn";
            this.paredeBtn.Size = new System.Drawing.Size(90, 25);
            this.paredeBtn.TabIndex = 10;
            this.paredeBtn.Text = "Parede";
            this.paredeBtn.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            this.paredeBtn.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageBeforeText;
            this.paredeBtn.UseVisualStyleBackColor = true;
            this.paredeBtn.CheckedChanged += new System.EventHandler(this.paredeBtn_CheckedChanged);
            // 
            // caminhoBtn
            // 
            this.caminhoBtn.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.caminhoBtn.Appearance = System.Windows.Forms.Appearance.Button;
            this.caminhoBtn.Enabled = false;
            this.caminhoBtn.Image = global::RatoLabirinto.Properties.Resources.caminho;
            this.caminhoBtn.Location = new System.Drawing.Point(1059, 52);
            this.caminhoBtn.Name = "caminhoBtn";
            this.caminhoBtn.Size = new System.Drawing.Size(90, 25);
            this.caminhoBtn.TabIndex = 11;
            this.caminhoBtn.Text = "Caminho";
            this.caminhoBtn.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageBeforeText;
            this.caminhoBtn.UseVisualStyleBackColor = true;
            this.caminhoBtn.CheckedChanged += new System.EventHandler(this.caminhoBtn_CheckedChanged);
            // 
            // ratoBtn
            // 
            this.ratoBtn.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.ratoBtn.Appearance = System.Windows.Forms.Appearance.Button;
            this.ratoBtn.Enabled = false;
            this.ratoBtn.Image = global::RatoLabirinto.Properties.Resources.rato;
            this.ratoBtn.Location = new System.Drawing.Point(1059, 83);
            this.ratoBtn.Name = "ratoBtn";
            this.ratoBtn.Size = new System.Drawing.Size(90, 25);
            this.ratoBtn.TabIndex = 12;
            this.ratoBtn.Text = "Rato";
            this.ratoBtn.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageBeforeText;
            this.ratoBtn.UseVisualStyleBackColor = true;
            this.ratoBtn.CheckedChanged += new System.EventHandler(this.ratoBtn_CheckedChanged);
            // 
            // saidaBtn
            // 
            this.saidaBtn.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.saidaBtn.Appearance = System.Windows.Forms.Appearance.Button;
            this.saidaBtn.Enabled = false;
            this.saidaBtn.Image = global::RatoLabirinto.Properties.Resources.saida;
            this.saidaBtn.Location = new System.Drawing.Point(1059, 114);
            this.saidaBtn.Name = "saidaBtn";
            this.saidaBtn.Size = new System.Drawing.Size(90, 25);
            this.saidaBtn.TabIndex = 13;
            this.saidaBtn.Text = "Saída";
            this.saidaBtn.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageBeforeText;
            this.saidaBtn.UseVisualStyleBackColor = true;
            this.saidaBtn.CheckedChanged += new System.EventHandler(this.saidaBtn_CheckedChanged);
            // 
            // gerarBtn
            // 
            this.gerarBtn.Enabled = false;
            this.gerarBtn.Location = new System.Drawing.Point(1063, 394);
            this.gerarBtn.Name = "gerarBtn";
            this.gerarBtn.Size = new System.Drawing.Size(91, 44);
            this.gerarBtn.TabIndex = 14;
            this.gerarBtn.Text = "Gerar labirinto";
            this.gerarBtn.UseVisualStyleBackColor = true;
            this.gerarBtn.Click += new System.EventHandler(this.gerarBtn_Click);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 15F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1162, 569);
            this.Controls.Add(this.gerarBtn);
            this.Controls.Add(this.saidaBtn);
            this.Controls.Add(this.ratoBtn);
            this.Controls.Add(this.caminhoBtn);
            this.Controls.Add(this.paredeBtn);
            this.Controls.Add(this.foundTXT);
            this.Controls.Add(this.editBtn);
            this.Controls.Add(this.procBtn);
            this.Controls.Add(this.pictureBox1);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.Fixed3D;
            this.MaximizeBox = false;
            this.Name = "Form1";
            this.SizeGripStyle = System.Windows.Forms.SizeGripStyle.Hide;
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Labirinto";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.Shown += new System.EventHandler(this.Form1_Shown);
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.PictureBox pictureBox1;
        private System.Windows.Forms.Button procBtn;
        private System.Windows.Forms.Button editBtn;
        private System.Windows.Forms.Label foundTXT;
        private System.Windows.Forms.ImageList imageList1;
        private System.Windows.Forms.CheckBox paredeBtn;
        private System.Windows.Forms.CheckBox caminhoBtn;
        private System.Windows.Forms.CheckBox ratoBtn;
        private System.Windows.Forms.CheckBox saidaBtn;
        private System.Windows.Forms.Button gerarBtn;
    }
}

