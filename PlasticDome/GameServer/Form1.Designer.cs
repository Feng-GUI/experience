namespace GameServer
{
  partial class Form1
  {
    /// <summary>
    /// Required designer variable.
    /// </summary>
    private System.ComponentModel.IContainer components = null;

    /// <summary>
    /// Clean up any resources being used.
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
    /// Required method for Designer support - do not modify
    /// the contents of this method with the code editor.
    /// </summary>
    private void InitializeComponent()
    {
      System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Form1));
      this.btnStart = new System.Windows.Forms.Button();
      this.labelHelp = new System.Windows.Forms.Label();
      this.lblError = new System.Windows.Forms.Label();
      this.button1 = new System.Windows.Forms.Button();
      this.lblDebug = new System.Windows.Forms.Label();
      this.pictureBox2 = new System.Windows.Forms.PictureBox();
      this.pictureBoxMap = new System.Windows.Forms.PictureBox();
      this.pictureBoxCar = new System.Windows.Forms.PictureBox();
      this.pictureBox3 = new System.Windows.Forms.PictureBox();
      this.button2 = new System.Windows.Forms.Button();
      this.vlcControl1 = new Vlc.DotNet.Forms.VlcControl();
      this.label1 = new System.Windows.Forms.Label();
      this.pictureBox1 = new System.Windows.Forms.PictureBox();
      ((System.ComponentModel.ISupportInitialize)(this.pictureBox2)).BeginInit();
      ((System.ComponentModel.ISupportInitialize)(this.pictureBoxMap)).BeginInit();
      ((System.ComponentModel.ISupportInitialize)(this.pictureBoxCar)).BeginInit();
      ((System.ComponentModel.ISupportInitialize)(this.pictureBox3)).BeginInit();
      ((System.ComponentModel.ISupportInitialize)(this.vlcControl1)).BeginInit();
      ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).BeginInit();
      this.SuspendLayout();
      // 
      // btnStart
      // 
      this.btnStart.Location = new System.Drawing.Point(1516, 461);
      this.btnStart.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
      this.btnStart.Name = "btnStart";
      this.btnStart.Size = new System.Drawing.Size(120, 40);
      this.btnStart.TabIndex = 13;
      this.btnStart.Text = "Start";
      this.btnStart.UseVisualStyleBackColor = true;
      this.btnStart.Click += new System.EventHandler(this.button2_Click);
      // 
      // labelHelp
      // 
      this.labelHelp.AutoSize = true;
      this.labelHelp.ForeColor = System.Drawing.Color.White;
      this.labelHelp.Location = new System.Drawing.Point(48, 968);
      this.labelHelp.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
      this.labelHelp.Name = "labelHelp";
      this.labelHelp.Size = new System.Drawing.Size(69, 20);
      this.labelHelp.TabIndex = 22;
      this.labelHelp.Text = "help text";
      // 
      // lblError
      // 
      this.lblError.AutoSize = true;
      this.lblError.ForeColor = System.Drawing.Color.White;
      this.lblError.Location = new System.Drawing.Point(369, 968);
      this.lblError.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
      this.lblError.Name = "lblError";
      this.lblError.Size = new System.Drawing.Size(72, 20);
      this.lblError.TabIndex = 24;
      this.lblError.Text = "error text";
      // 
      // button1
      // 
      this.button1.Location = new System.Drawing.Point(1516, 511);
      this.button1.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
      this.button1.Name = "button1";
      this.button1.Size = new System.Drawing.Size(120, 40);
      this.button1.TabIndex = 26;
      this.button1.Text = "go";
      this.button1.UseVisualStyleBackColor = true;
      this.button1.Click += new System.EventHandler(this.button1_Click);
      // 
      // lblDebug
      // 
      this.lblDebug.AutoSize = true;
      this.lblDebug.ForeColor = System.Drawing.Color.White;
      this.lblDebug.Location = new System.Drawing.Point(48, 1026);
      this.lblDebug.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
      this.lblDebug.Name = "lblDebug";
      this.lblDebug.Size = new System.Drawing.Size(54, 20);
      this.lblDebug.TabIndex = 29;
      this.lblDebug.Text = "debug";
      // 
      // pictureBox2
      // 
      this.pictureBox2.BackColor = System.Drawing.Color.Transparent;
      this.pictureBox2.Location = new System.Drawing.Point(1536, 872);
      this.pictureBox2.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
      this.pictureBox2.Name = "pictureBox2";
      this.pictureBox2.Size = new System.Drawing.Size(182, 226);
      this.pictureBox2.SizeMode = System.Windows.Forms.PictureBoxSizeMode.Zoom;
      this.pictureBox2.TabIndex = 33;
      this.pictureBox2.TabStop = false;
      // 
      // pictureBoxMap
      // 
      this.pictureBoxMap.BackColor = System.Drawing.Color.Gray;
      this.pictureBoxMap.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
      this.pictureBoxMap.Location = new System.Drawing.Point(926, 22);
      this.pictureBoxMap.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
      this.pictureBoxMap.Name = "pictureBoxMap";
      this.pictureBoxMap.Size = new System.Drawing.Size(561, 311);
      this.pictureBoxMap.SizeMode = System.Windows.Forms.PictureBoxSizeMode.AutoSize;
      this.pictureBoxMap.TabIndex = 0;
      this.pictureBoxMap.TabStop = false;
      // 
      // pictureBoxCar
      // 
      this.pictureBoxCar.Location = new System.Drawing.Point(0, 0);
      this.pictureBoxCar.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
      this.pictureBoxCar.Name = "pictureBoxCar";
      this.pictureBoxCar.Size = new System.Drawing.Size(1497, 917);
      this.pictureBoxCar.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
      this.pictureBoxCar.TabIndex = 31;
      this.pictureBoxCar.TabStop = false;
      // 
      // pictureBox3
      // 
      this.pictureBox3.BackColor = System.Drawing.Color.Transparent;
      this.pictureBox3.Location = new System.Drawing.Point(1156, 926);
      this.pictureBox3.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
      this.pictureBox3.Name = "pictureBox3";
      this.pictureBox3.Size = new System.Drawing.Size(318, 148);
      this.pictureBox3.SizeMode = System.Windows.Forms.PictureBoxSizeMode.Zoom;
      this.pictureBox3.TabIndex = 34;
      this.pictureBox3.TabStop = false;
      // 
      // button2
      // 
      this.button2.Location = new System.Drawing.Point(1516, 574);
      this.button2.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
      this.button2.Name = "button2";
      this.button2.Size = new System.Drawing.Size(120, 40);
      this.button2.TabIndex = 35;
      this.button2.Text = "clean";
      this.button2.UseVisualStyleBackColor = true;
      this.button2.Click += new System.EventHandler(this.button2_Click_2);
      // 
      // vlcControl1
      // 
      this.vlcControl1.BackColor = System.Drawing.Color.Black;
      this.vlcControl1.Location = new System.Drawing.Point(23, 154);
      this.vlcControl1.Name = "vlcControl1";
      this.vlcControl1.Size = new System.Drawing.Size(883, 637);
      this.vlcControl1.Spu = -1;
      this.vlcControl1.TabIndex = 36;
      this.vlcControl1.Text = "vlcControl1";
      this.vlcControl1.VlcLibDirectory = null;
      this.vlcControl1.VlcMediaplayerOptions = null;
      this.vlcControl1.VlcLibDirectoryNeeded += new System.EventHandler<Vlc.DotNet.Forms.VlcLibDirectoryNeededEventArgs>(this.vlcControl1_VlcLibDirectoryNeeded);
      this.vlcControl1.EncounteredError += new System.EventHandler<Vlc.DotNet.Core.VlcMediaPlayerEncounteredErrorEventArgs>(this.vlcControl1_EncounteredError);
      this.vlcControl1.Log += new System.EventHandler<Vlc.DotNet.Core.VlcMediaPlayerLogEventArgs>(this.vlcControl1_Log);
      this.vlcControl1.Click += new System.EventHandler(this.vlcControl1_Click_1);
      // 
      // label1
      // 
      this.label1.AutoSize = true;
      this.label1.Font = new System.Drawing.Font("Microsoft Sans Serif", 28F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.label1.ForeColor = System.Drawing.Color.White;
      this.label1.Location = new System.Drawing.Point(28, 45);
      this.label1.Name = "label1";
      this.label1.Size = new System.Drawing.Size(358, 64);
      this.label1.TabIndex = 37;
      this.label1.Text = "Plastic Dome";
      // 
      // pictureBox1
      // 
      this.pictureBox1.Image = ((System.Drawing.Image)(resources.GetObject("pictureBox1.Image")));
      this.pictureBox1.Location = new System.Drawing.Point(1504, 22);
      this.pictureBox1.Name = "pictureBox1";
      this.pictureBox1.Size = new System.Drawing.Size(268, 258);
      this.pictureBox1.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
      this.pictureBox1.TabIndex = 38;
      this.pictureBox1.TabStop = false;
      this.pictureBox1.Click += new System.EventHandler(this.pictureBox1_Click);
      // 
      // Form1
      // 
      this.AutoScaleDimensions = new System.Drawing.SizeF(9F, 20F);
      this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
      this.BackColor = System.Drawing.Color.Black;
      this.ClientSize = new System.Drawing.Size(1784, 1049);
      this.Controls.Add(this.pictureBox1);
      this.Controls.Add(this.label1);
      this.Controls.Add(this.vlcControl1);
      this.Controls.Add(this.button2);
      this.Controls.Add(this.pictureBox3);
      this.Controls.Add(this.pictureBox2);
      this.Controls.Add(this.pictureBoxMap);
      this.Controls.Add(this.pictureBoxCar);
      this.Controls.Add(this.lblDebug);
      this.Controls.Add(this.button1);
      this.Controls.Add(this.lblError);
      this.Controls.Add(this.labelHelp);
      this.Controls.Add(this.btnStart);
      this.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
      this.Name = "Form1";
      this.Text = "PlasticDome";
      this.WindowState = System.Windows.Forms.FormWindowState.Maximized;
      this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.Form1_FormClosing);
      this.Load += new System.EventHandler(this.Form1_Load);
      ((System.ComponentModel.ISupportInitialize)(this.pictureBox2)).EndInit();
      ((System.ComponentModel.ISupportInitialize)(this.pictureBoxMap)).EndInit();
      ((System.ComponentModel.ISupportInitialize)(this.pictureBoxCar)).EndInit();
      ((System.ComponentModel.ISupportInitialize)(this.pictureBox3)).EndInit();
      ((System.ComponentModel.ISupportInitialize)(this.vlcControl1)).EndInit();
      ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).EndInit();
      this.ResumeLayout(false);
      this.PerformLayout();

    }

    #endregion

    private System.Windows.Forms.PictureBox pictureBoxMap;
    private System.Windows.Forms.Button btnStart;
    private System.Windows.Forms.Label labelHelp;
    private System.Windows.Forms.Label lblError;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.Label lblDebug;
    private System.Windows.Forms.PictureBox pictureBoxCar;
        private System.Windows.Forms.PictureBox pictureBox2;
    private System.Windows.Forms.PictureBox pictureBox3;
		private System.Windows.Forms.Button button2;
    private Vlc.DotNet.Forms.VlcControl vlcControl1;
    private System.Windows.Forms.Label label1;
    private System.Windows.Forms.PictureBox pictureBox1;
  }
}

