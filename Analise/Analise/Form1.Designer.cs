
namespace Analise
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
            this.pictureBox1 = new System.Windows.Forms.PictureBox();
            this.radioButton1 = new System.Windows.Forms.RadioButton();
            this.radioButton2 = new System.Windows.Forms.RadioButton();
            this.radioButton3 = new System.Windows.Forms.RadioButton();
            this.radioButton4 = new System.Windows.Forms.RadioButton();
            this.radioButton5 = new System.Windows.Forms.RadioButton();
            this.radioButton6 = new System.Windows.Forms.RadioButton();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.funcao = new System.Windows.Forms.TextBox();
            this.label3 = new System.Windows.Forms.Label();
            this.numIter = new System.Windows.Forms.NumericUpDown();
            this.label4 = new System.Windows.Forms.Label();
            this.inicioTxt = new System.Windows.Forms.TextBox();
            this.finalTxt = new System.Windows.Forms.TextBox();
            this.label5 = new System.Windows.Forms.Label();
            this.label6 = new System.Windows.Forms.Label();
            this.apinicTxt = new System.Windows.Forms.TextBox();
            this.label7 = new System.Windows.Forms.Label();
            this.precTxt = new System.Windows.Forms.TextBox();
            this.apfimTxt = new System.Windows.Forms.TextBox();
            this.label8 = new System.Windows.Forms.Label();
            this.button1 = new System.Windows.Forms.Button();
            this.foundTxt = new System.Windows.Forms.Label();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.numIter)).BeginInit();
            this.SuspendLayout();
            // 
            // pictureBox1
            // 
            this.pictureBox1.BackColor = System.Drawing.Color.Black;
            this.pictureBox1.Location = new System.Drawing.Point(0, 0);
            this.pictureBox1.Name = "pictureBox1";
            this.pictureBox1.Size = new System.Drawing.Size(640, 400);
            this.pictureBox1.TabIndex = 0;
            this.pictureBox1.TabStop = false;
            this.pictureBox1.Click += new System.EventHandler(this.pictureBox1_Click);
            this.pictureBox1.Paint += new System.Windows.Forms.PaintEventHandler(this.pictureBox1_Paint);
            // 
            // radioButton1
            // 
            this.radioButton1.AutoSize = true;
            this.radioButton1.Location = new System.Drawing.Point(659, 27);
            this.radioButton1.Name = "radioButton1";
            this.radioButton1.Size = new System.Drawing.Size(71, 19);
            this.radioButton1.TabIndex = 1;
            this.radioButton1.TabStop = true;
            this.radioButton1.Text = "Bisecção";
            this.radioButton1.UseVisualStyleBackColor = true;
            this.radioButton1.CheckedChanged += new System.EventHandler(this.radioButton1_CheckedChanged);
            // 
            // radioButton2
            // 
            this.radioButton2.AutoSize = true;
            this.radioButton2.Location = new System.Drawing.Point(659, 53);
            this.radioButton2.Name = "radioButton2";
            this.radioButton2.Size = new System.Drawing.Size(93, 19);
            this.radioButton2.TabIndex = 2;
            this.radioButton2.TabStop = true;
            this.radioButton2.Text = "Posição falsa";
            this.radioButton2.UseVisualStyleBackColor = true;
            this.radioButton2.CheckedChanged += new System.EventHandler(this.radioButton2_CheckedChanged);
            // 
            // radioButton3
            // 
            this.radioButton3.AutoSize = true;
            this.radioButton3.Location = new System.Drawing.Point(659, 78);
            this.radioButton3.Name = "radioButton3";
            this.radioButton3.Size = new System.Drawing.Size(156, 19);
            this.radioButton3.TabIndex = 3;
            this.radioButton3.TabStop = true;
            this.radioButton3.Text = "Posição falsa modificada";
            this.radioButton3.UseVisualStyleBackColor = true;
            this.radioButton3.CheckedChanged += new System.EventHandler(this.radioButton3_CheckedChanged);
            // 
            // radioButton4
            // 
            this.radioButton4.AutoSize = true;
            this.radioButton4.Location = new System.Drawing.Point(659, 104);
            this.radioButton4.Name = "radioButton4";
            this.radioButton4.Size = new System.Drawing.Size(100, 19);
            this.radioButton4.TabIndex = 4;
            this.radioButton4.TabStop = true;
            this.radioButton4.Text = "Iterativo linear";
            this.radioButton4.UseVisualStyleBackColor = true;
            this.radioButton4.CheckedChanged += new System.EventHandler(this.radioButton4_CheckedChanged);
            // 
            // radioButton5
            // 
            this.radioButton5.AutoSize = true;
            this.radioButton5.Location = new System.Drawing.Point(659, 130);
            this.radioButton5.Name = "radioButton5";
            this.radioButton5.Size = new System.Drawing.Size(124, 19);
            this.radioButton5.TabIndex = 5;
            this.radioButton5.TabStop = true;
            this.radioButton5.Text = "Newton - Raphson";
            this.radioButton5.UseVisualStyleBackColor = true;
            this.radioButton5.CheckedChanged += new System.EventHandler(this.radioButton5_CheckedChanged);
            // 
            // radioButton6
            // 
            this.radioButton6.AutoSize = true;
            this.radioButton6.Location = new System.Drawing.Point(659, 155);
            this.radioButton6.Name = "radioButton6";
            this.radioButton6.Size = new System.Drawing.Size(66, 19);
            this.radioButton6.TabIndex = 6;
            this.radioButton6.TabStop = true;
            this.radioButton6.Text = "Secante";
            this.radioButton6.UseVisualStyleBackColor = true;
            this.radioButton6.CheckedChanged += new System.EventHandler(this.radioButton6_CheckedChanged);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(659, 9);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(52, 15);
            this.label1.TabIndex = 7;
            this.label1.Text = "Método:";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(13, 430);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(118, 15);
            this.label2.TabIndex = 8;
            this.label2.Text = "Função de variável X:";
            // 
            // funcao
            // 
            this.funcao.Location = new System.Drawing.Point(13, 449);
            this.funcao.Name = "funcao";
            this.funcao.Size = new System.Drawing.Size(520, 23);
            this.funcao.TabIndex = 9;
            this.funcao.Text = "x^2-3";
            this.funcao.Leave += new System.EventHandler(this.funcao_Leave);
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(13, 479);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(123, 15);
            this.label3.TabIndex = 10;
            this.label3.Text = "Número de Iterações: ";
            // 
            // numIter
            // 
            this.numIter.Location = new System.Drawing.Point(143, 479);
            this.numIter.Maximum = new decimal(new int[] {
            65535,
            0,
            0,
            0});
            this.numIter.Name = "numIter";
            this.numIter.Size = new System.Drawing.Size(120, 23);
            this.numIter.TabIndex = 11;
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(13, 511);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(90, 15);
            this.label4.TabIndex = 12;
            this.label4.Text = "Intervalo inicial:";
            this.label4.TextAlign = System.Drawing.ContentAlignment.TopRight;
            // 
            // inicioTxt
            // 
            this.inicioTxt.Location = new System.Drawing.Point(109, 506);
            this.inicioTxt.Name = "inicioTxt";
            this.inicioTxt.Size = new System.Drawing.Size(71, 23);
            this.inicioTxt.TabIndex = 13;
            this.inicioTxt.Text = "-5";
            // 
            // finalTxt
            // 
            this.finalTxt.Location = new System.Drawing.Point(109, 535);
            this.finalTxt.Name = "finalTxt";
            this.finalTxt.Size = new System.Drawing.Size(71, 23);
            this.finalTxt.TabIndex = 15;
            this.finalTxt.Text = "5";
            this.finalTxt.TextChanged += new System.EventHandler(this.textBox1_TextChanged);
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(21, 538);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(82, 15);
            this.label5.TabIndex = 14;
            this.label5.Text = "Intervalo final:";
            this.label5.TextAlign = System.Drawing.ContentAlignment.TopRight;
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(208, 511);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(115, 15);
            this.label6.TabIndex = 16;
            this.label6.Text = "Aproximação inicial:";
            // 
            // apinicTxt
            // 
            this.apinicTxt.Location = new System.Drawing.Point(329, 506);
            this.apinicTxt.Name = "apinicTxt";
            this.apinicTxt.Size = new System.Drawing.Size(100, 23);
            this.apinicTxt.TabIndex = 17;
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(51, 570);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(54, 15);
            this.label7.TabIndex = 18;
            this.label7.Text = "Precisão:";
            // 
            // precTxt
            // 
            this.precTxt.Location = new System.Drawing.Point(109, 564);
            this.precTxt.Name = "precTxt";
            this.precTxt.Size = new System.Drawing.Size(71, 23);
            this.precTxt.TabIndex = 19;
            this.precTxt.Text = "1";
            this.precTxt.TextChanged += new System.EventHandler(this.precTxt_TextChanged);
            this.precTxt.Validating += new System.ComponentModel.CancelEventHandler(this.precTxt_Validating);
            // 
            // apfimTxt
            // 
            this.apfimTxt.Location = new System.Drawing.Point(329, 530);
            this.apfimTxt.Name = "apfimTxt";
            this.apfimTxt.Size = new System.Drawing.Size(100, 23);
            this.apfimTxt.TabIndex = 21;
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Location = new System.Drawing.Point(208, 535);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(107, 15);
            this.label8.TabIndex = 20;
            this.label8.Text = "Aproximação final:";
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(384, 566);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(149, 23);
            this.button1.TabIndex = 22;
            this.button1.Text = "Calcular";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // foundTxt
            // 
            this.foundTxt.AutoSize = true;
            this.foundTxt.Location = new System.Drawing.Point(319, 415);
            this.foundTxt.Name = "foundTxt";
            this.foundTxt.Size = new System.Drawing.Size(78, 15);
            this.foundTxt.TabIndex = 23;
            this.foundTxt.Text = "Procurando...";
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 15F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(830, 599);
            this.Controls.Add(this.foundTxt);
            this.Controls.Add(this.button1);
            this.Controls.Add(this.apfimTxt);
            this.Controls.Add(this.label8);
            this.Controls.Add(this.precTxt);
            this.Controls.Add(this.label7);
            this.Controls.Add(this.apinicTxt);
            this.Controls.Add(this.label6);
            this.Controls.Add(this.finalTxt);
            this.Controls.Add(this.label5);
            this.Controls.Add(this.inicioTxt);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.numIter);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.funcao);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.radioButton6);
            this.Controls.Add(this.radioButton5);
            this.Controls.Add(this.radioButton4);
            this.Controls.Add(this.radioButton3);
            this.Controls.Add(this.radioButton2);
            this.Controls.Add(this.radioButton1);
            this.Controls.Add(this.pictureBox1);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.Fixed3D;
            this.MaximizeBox = false;
            this.Name = "Form1";
            this.Text = "Form1";
            this.Load += new System.EventHandler(this.Form1_Load);
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.numIter)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.PictureBox pictureBox1;
        private System.Windows.Forms.RadioButton radioButton1;
        private System.Windows.Forms.RadioButton radioButton2;
        private System.Windows.Forms.RadioButton radioButton3;
        private System.Windows.Forms.RadioButton radioButton4;
        private System.Windows.Forms.RadioButton radioButton5;
        private System.Windows.Forms.RadioButton radioButton6;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TextBox funcao;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.NumericUpDown numIter;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.TextBox inicioTxt;
        private System.Windows.Forms.TextBox finalTxt;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.TextBox apinicTxt;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.TextBox precTxt;
        private System.Windows.Forms.TextBox apfimTxt;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.Label foundTxt;
    }
}

