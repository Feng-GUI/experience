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
      this.timerLeftPaddle = new System.Windows.Forms.Timer(this.components);
      this.timerBall = new System.Windows.Forms.Timer(this.components);
      this.timerComputer = new System.Windows.Forms.Timer(this.components);
      this.pictureBoxGame = new System.Windows.Forms.PictureBox();
      this.pictureBoxCam = new System.Windows.Forms.PictureBox();
      this.timerRightPaddle = new System.Windows.Forms.Timer(this.components);
      this.labelHelp = new System.Windows.Forms.Label();
      this.chkComputer = new System.Windows.Forms.CheckBox();
      this.lblError = new System.Windows.Forms.Label();
      this.chkLive = new System.Windows.Forms.CheckBox();
      this.button1 = new System.Windows.Forms.Button();
      this.button2 = new System.Windows.Forms.Button();
      this.button3 = new System.Windows.Forms.Button();
      this.lblDebug = new System.Windows.Forms.Label();
      this.button4 = new System.Windows.Forms.Button();
      this.timerBounce = new System.Windows.Forms.Timer(this.components);
      this.pictureBox1 = new System.Windows.Forms.PictureBox();
      ((System.ComponentModel.ISupportInitialize)(this.pictureBoxGame)).BeginInit();
      ((System.ComponentModel.ISupportInitialize)(this.pictureBoxCam)).BeginInit();
      ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).BeginInit();
      this.SuspendLayout();
      // 
      // btnStart
      // 
      this.btnStart.Location = new System.Drawing.Point(10, 630);
      this.btnStart.Name = "btnStart";
      this.btnStart.Size = new System.Drawing.Size(80, 26);
      this.btnStart.TabIndex = 13;
      this.btnStart.Text = "Start Game";
      this.btnStart.UseVisualStyleBackColor = true;
      this.btnStart.Click += new System.EventHandler(this.button2_Click);
      // 
      // timerLeftPaddle
      // 
      this.timerLeftPaddle.Interval = 5;
      this.timerLeftPaddle.Tick += new System.EventHandler(this.timerLeftPaddle_Tick);
      // 
      // timerBall
      // 
      this.timerBall.Tick += new System.EventHandler(this.MoveBall);
      // 
      // timerComputer
      // 
      this.timerComputer.Interval = 5;
      this.timerComputer.Tick += new System.EventHandler(this.Computer);
      // 
      // pictureBoxGame
      // 
      this.pictureBoxGame.BackColor = System.Drawing.Color.Black;
      this.pictureBoxGame.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
      this.pictureBoxGame.Location = new System.Drawing.Point(897, 442);
      this.pictureBoxGame.Name = "pictureBoxGame";
      this.pictureBoxGame.Size = new System.Drawing.Size(363, 224);
      this.pictureBoxGame.TabIndex = 1;
      this.pictureBoxGame.TabStop = false;
      this.pictureBoxGame.Paint += new System.Windows.Forms.PaintEventHandler(this.pictureBoxGame_Paint);
      // 
      // pictureBoxCam
      // 
      this.pictureBoxCam.Location = new System.Drawing.Point(24, 395);
      this.pictureBoxCam.Name = "pictureBoxCam";
      this.pictureBoxCam.Size = new System.Drawing.Size(222, 166);
      this.pictureBoxCam.TabIndex = 0;
      this.pictureBoxCam.TabStop = false;
      // 
      // timerRightPaddle
      // 
      this.timerRightPaddle.Interval = 5;
      this.timerRightPaddle.Tick += new System.EventHandler(this.timerRightPaddle_Tick);
      // 
      // labelHelp
      // 
      this.labelHelp.AutoSize = true;
      this.labelHelp.ForeColor = System.Drawing.Color.White;
      this.labelHelp.Location = new System.Drawing.Point(147, 629);
      this.labelHelp.Name = "labelHelp";
      this.labelHelp.Size = new System.Drawing.Size(47, 13);
      this.labelHelp.TabIndex = 22;
      this.labelHelp.Text = "help text";
      // 
      // chkComputer
      // 
      this.chkComputer.AutoSize = true;
      this.chkComputer.ForeColor = System.Drawing.Color.White;
      this.chkComputer.Location = new System.Drawing.Point(8, 662);
      this.chkComputer.Name = "chkComputer";
      this.chkComputer.Size = new System.Drawing.Size(131, 17);
      this.chkComputer.TabIndex = 23;
      this.chkComputer.Text = "Right Player Computer";
      this.chkComputer.UseVisualStyleBackColor = true;
      // 
      // lblError
      // 
      this.lblError.AutoSize = true;
      this.lblError.ForeColor = System.Drawing.Color.White;
      this.lblError.Location = new System.Drawing.Point(486, 629);
      this.lblError.Name = "lblError";
      this.lblError.Size = new System.Drawing.Size(48, 13);
      this.lblError.TabIndex = 24;
      this.lblError.Text = "error text";
      // 
      // chkLive
      // 
      this.chkLive.AutoSize = true;
      this.chkLive.ForeColor = System.Drawing.Color.White;
      this.chkLive.Location = new System.Drawing.Point(7, 678);
      this.chkLive.Name = "chkLive";
      this.chkLive.Size = new System.Drawing.Size(46, 17);
      this.chkLive.TabIndex = 25;
      this.chkLive.Text = "Live";
      this.chkLive.UseVisualStyleBackColor = true;
      this.chkLive.CheckedChanged += new System.EventHandler(this.chkLive_CheckedChanged);
      // 
      // button1
      // 
      this.button1.Location = new System.Drawing.Point(564, 630);
      this.button1.Name = "button1";
      this.button1.Size = new System.Drawing.Size(80, 26);
      this.button1.TabIndex = 26;
      this.button1.Text = "go";
      this.button1.UseVisualStyleBackColor = true;
      this.button1.Click += new System.EventHandler(this.button1_Click);
      // 
      // button2
      // 
      this.button2.Location = new System.Drawing.Point(564, 657);
      this.button2.Name = "button2";
      this.button2.Size = new System.Drawing.Size(80, 26);
      this.button2.TabIndex = 27;
      this.button2.Text = "stop";
      this.button2.UseVisualStyleBackColor = true;
      this.button2.Click += new System.EventHandler(this.button2_Click_1);
      // 
      // button3
      // 
      this.button3.Location = new System.Drawing.Point(651, 630);
      this.button3.Name = "button3";
      this.button3.Size = new System.Drawing.Size(80, 26);
      this.button3.TabIndex = 28;
      this.button3.Text = "turn";
      this.button3.UseVisualStyleBackColor = true;
      this.button3.Click += new System.EventHandler(this.button3_Click);
      // 
      // lblDebug
      // 
      this.lblDebug.AutoSize = true;
      this.lblDebug.ForeColor = System.Drawing.Color.White;
      this.lblDebug.Location = new System.Drawing.Point(651, 669);
      this.lblDebug.Name = "lblDebug";
      this.lblDebug.Size = new System.Drawing.Size(37, 13);
      this.lblDebug.TabIndex = 29;
      this.lblDebug.Text = "debug";
      // 
      // button4
      // 
      this.button4.Location = new System.Drawing.Point(752, 630);
      this.button4.Name = "button4";
      this.button4.Size = new System.Drawing.Size(80, 26);
      this.button4.TabIndex = 30;
      this.button4.Text = "bounce";
      this.button4.UseVisualStyleBackColor = true;
      this.button4.Click += new System.EventHandler(this.button4_Click);
      // 
      // timerBounce
      // 
      this.timerBounce.Interval = 250;
      this.timerBounce.Tick += new System.EventHandler(this.timerBounce_Tick);
      // 
      // pictureBox1
      // 
      this.pictureBox1.Location = new System.Drawing.Point(0, 0);
      this.pictureBox1.Name = "pictureBox1";
      this.pictureBox1.Size = new System.Drawing.Size(891, 588);
      this.pictureBox1.TabIndex = 31;
      this.pictureBox1.TabStop = false;
      // 
      // Form1
      // 
      this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
      this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
      this.BackColor = System.Drawing.Color.Black;
      this.ClientSize = new System.Drawing.Size(1295, 710);
      this.Controls.Add(this.pictureBoxCam);
      this.Controls.Add(this.pictureBox1);
      this.Controls.Add(this.button4);
      this.Controls.Add(this.lblDebug);
      this.Controls.Add(this.button3);
      this.Controls.Add(this.button2);
      this.Controls.Add(this.button1);
      this.Controls.Add(this.chkLive);
      this.Controls.Add(this.lblError);
      this.Controls.Add(this.chkComputer);
      this.Controls.Add(this.labelHelp);
      this.Controls.Add(this.btnStart);
      this.Controls.Add(this.pictureBoxGame);
      this.Name = "Form1";
      this.Text = "GTA.I.";
      this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.Form1_FormClosing);
      this.Load += new System.EventHandler(this.Form1_Load);
      this.KeyDown += new System.Windows.Forms.KeyEventHandler(this.Pressed);
      this.KeyUp += new System.Windows.Forms.KeyEventHandler(this.Released);
      ((System.ComponentModel.ISupportInitialize)(this.pictureBoxGame)).EndInit();
      ((System.ComponentModel.ISupportInitialize)(this.pictureBoxCam)).EndInit();
      ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).EndInit();
      this.ResumeLayout(false);
      this.PerformLayout();

    }

    #endregion

    private System.Windows.Forms.PictureBox pictureBoxCam;
    private System.Windows.Forms.PictureBox pictureBoxGame;
    private System.Windows.Forms.Button btnStart;
    private System.Windows.Forms.Timer timerLeftPaddle;
    private System.Windows.Forms.Timer timerBall;
    private System.Windows.Forms.Timer timerComputer;
    private System.Windows.Forms.Timer timerRightPaddle;
    private System.Windows.Forms.Label labelHelp;
    private System.Windows.Forms.CheckBox chkComputer;
    private System.Windows.Forms.Label lblError;
    private System.Windows.Forms.CheckBox chkLive;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.Button button2;
        private System.Windows.Forms.Button button3;
        private System.Windows.Forms.Label lblDebug;
        private System.Windows.Forms.Button button4;
        private System.Windows.Forms.Timer timerBounce;
    private System.Windows.Forms.PictureBox pictureBox1;
  }
}

