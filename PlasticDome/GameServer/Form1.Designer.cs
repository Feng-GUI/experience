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
			this.components = new System.ComponentModel.Container();
			this.btnStart = new System.Windows.Forms.Button();
			this.timerSelfDrive = new System.Windows.Forms.Timer(this.components);
			this.labelHelp = new System.Windows.Forms.Label();
			this.lblError = new System.Windows.Forms.Label();
			this.button1 = new System.Windows.Forms.Button();
			this.lblDebug = new System.Windows.Forms.Label();
			this.pictureBox2 = new System.Windows.Forms.PictureBox();
			this.pictureBox1 = new System.Windows.Forms.PictureBox();
			this.pictureBoxMap = new System.Windows.Forms.PictureBox();
			this.pictureBoxCar = new System.Windows.Forms.PictureBox();
			this.pictureBox3 = new System.Windows.Forms.PictureBox();
			((System.ComponentModel.ISupportInitialize)(this.pictureBox2)).BeginInit();
			((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).BeginInit();
			((System.ComponentModel.ISupportInitialize)(this.pictureBoxMap)).BeginInit();
			((System.ComponentModel.ISupportInitialize)(this.pictureBoxCar)).BeginInit();
			((System.ComponentModel.ISupportInitialize)(this.pictureBox3)).BeginInit();
			this.SuspendLayout();
			// 
			// btnStart
			// 
			this.btnStart.Location = new System.Drawing.Point(1011, 211);
			this.btnStart.Name = "btnStart";
			this.btnStart.Size = new System.Drawing.Size(80, 26);
			this.btnStart.TabIndex = 13;
			this.btnStart.Text = "Start Game";
			this.btnStart.UseVisualStyleBackColor = true;
			this.btnStart.Click += new System.EventHandler(this.button2_Click);
			// 
			// timerSelfDrive
			// 
			this.timerSelfDrive.Interval = 10000;
			this.timerSelfDrive.Tick += new System.EventHandler(this.timerSelfDrive_Tick);
			// 
			// labelHelp
			// 
			this.labelHelp.AutoSize = true;
			this.labelHelp.ForeColor = System.Drawing.Color.White;
			this.labelHelp.Location = new System.Drawing.Point(32, 629);
			this.labelHelp.Name = "labelHelp";
			this.labelHelp.Size = new System.Drawing.Size(47, 13);
			this.labelHelp.TabIndex = 22;
			this.labelHelp.Text = "help text";
			// 
			// lblError
			// 
			this.lblError.AutoSize = true;
			this.lblError.ForeColor = System.Drawing.Color.White;
			this.lblError.Location = new System.Drawing.Point(246, 629);
			this.lblError.Name = "lblError";
			this.lblError.Size = new System.Drawing.Size(48, 13);
			this.lblError.TabIndex = 24;
			this.lblError.Text = "error text";
			// 
			// button1
			// 
			this.button1.Location = new System.Drawing.Point(1011, 285);
			this.button1.Name = "button1";
			this.button1.Size = new System.Drawing.Size(80, 26);
			this.button1.TabIndex = 26;
			this.button1.Text = "go";
			this.button1.UseVisualStyleBackColor = true;
			this.button1.Click += new System.EventHandler(this.button1_Click);
			// 
			// lblDebug
			// 
			this.lblDebug.AutoSize = true;
			this.lblDebug.ForeColor = System.Drawing.Color.White;
			this.lblDebug.Location = new System.Drawing.Point(32, 667);
			this.lblDebug.Name = "lblDebug";
			this.lblDebug.Size = new System.Drawing.Size(37, 13);
			this.lblDebug.TabIndex = 29;
			this.lblDebug.Text = "debug";
			// 
			// pictureBox2
			// 
			this.pictureBox2.BackColor = System.Drawing.Color.Transparent;
			this.pictureBox2.Location = new System.Drawing.Point(1024, 567);
			this.pictureBox2.Name = "pictureBox2";
			this.pictureBox2.Size = new System.Drawing.Size(121, 147);
			this.pictureBox2.SizeMode = System.Windows.Forms.PictureBoxSizeMode.Zoom;
			this.pictureBox2.TabIndex = 33;
			this.pictureBox2.TabStop = false;
			// 
			// pictureBox1
			// 
			this.pictureBox1.BackColor = System.Drawing.Color.Transparent;
			this.pictureBox1.Location = new System.Drawing.Point(989, 12);
			this.pictureBox1.Name = "pictureBox1";
			this.pictureBox1.Size = new System.Drawing.Size(193, 134);
			this.pictureBox1.SizeMode = System.Windows.Forms.PictureBoxSizeMode.Zoom;
			this.pictureBox1.TabIndex = 32;
			this.pictureBox1.TabStop = false;
			// 
			// pictureBoxMap
			// 
			this.pictureBoxMap.BackColor = System.Drawing.Color.Gray;
			this.pictureBoxMap.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
			this.pictureBoxMap.Location = new System.Drawing.Point(138, 118);
			this.pictureBoxMap.Name = "pictureBoxMap";
			this.pictureBoxMap.Size = new System.Drawing.Size(502, 379);
			this.pictureBoxMap.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
			this.pictureBoxMap.TabIndex = 0;
			this.pictureBoxMap.TabStop = false;
			this.pictureBoxMap.Paint += new System.Windows.Forms.PaintEventHandler(this.pictureBoxMap_Paint);
			// 
			// pictureBoxCar
			// 
			this.pictureBoxCar.Location = new System.Drawing.Point(0, 0);
			this.pictureBoxCar.Name = "pictureBoxCar";
			this.pictureBoxCar.Size = new System.Drawing.Size(983, 596);
			this.pictureBoxCar.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
			this.pictureBoxCar.TabIndex = 31;
			this.pictureBoxCar.TabStop = false;
			this.pictureBoxCar.Paint += new System.Windows.Forms.PaintEventHandler(this.pictureBoxGame_Paint);
			// 
			// pictureBox3
			// 
			this.pictureBox3.BackColor = System.Drawing.Color.Transparent;
			this.pictureBox3.Location = new System.Drawing.Point(771, 602);
			this.pictureBox3.Name = "pictureBox3";
			this.pictureBox3.Size = new System.Drawing.Size(212, 96);
			this.pictureBox3.SizeMode = System.Windows.Forms.PictureBoxSizeMode.Zoom;
			this.pictureBox3.TabIndex = 34;
			this.pictureBox3.TabStop = false;
			// 
			// Form1
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.BackColor = System.Drawing.Color.Black;
			this.ClientSize = new System.Drawing.Size(1189, 682);
			this.Controls.Add(this.pictureBox3);
			this.Controls.Add(this.pictureBox2);
			this.Controls.Add(this.pictureBox1);
			this.Controls.Add(this.pictureBoxMap);
			this.Controls.Add(this.pictureBoxCar);
			this.Controls.Add(this.lblDebug);
			this.Controls.Add(this.button1);
			this.Controls.Add(this.lblError);
			this.Controls.Add(this.labelHelp);
			this.Controls.Add(this.btnStart);
			this.Name = "Form1";
			this.Text = "PlasticDome";
			this.WindowState = System.Windows.Forms.FormWindowState.Maximized;
			this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.Form1_FormClosing);
			this.Load += new System.EventHandler(this.Form1_Load);
			this.KeyDown += new System.Windows.Forms.KeyEventHandler(this.Pressed);
			this.KeyUp += new System.Windows.Forms.KeyEventHandler(this.Released);
			((System.ComponentModel.ISupportInitialize)(this.pictureBox2)).EndInit();
			((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).EndInit();
			((System.ComponentModel.ISupportInitialize)(this.pictureBoxMap)).EndInit();
			((System.ComponentModel.ISupportInitialize)(this.pictureBoxCar)).EndInit();
			((System.ComponentModel.ISupportInitialize)(this.pictureBox3)).EndInit();
			this.ResumeLayout(false);
			this.PerformLayout();

    }

    #endregion

    private System.Windows.Forms.PictureBox pictureBoxMap;
    private System.Windows.Forms.Button btnStart;
    private System.Windows.Forms.Timer timerSelfDrive;
    private System.Windows.Forms.Label labelHelp;
    private System.Windows.Forms.Label lblError;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.Label lblDebug;
    private System.Windows.Forms.PictureBox pictureBoxCar;
        private System.Windows.Forms.PictureBox pictureBox1;
        private System.Windows.Forms.PictureBox pictureBox2;
    private System.Windows.Forms.PictureBox pictureBox3;
  }
}

