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
      this.labelLeftScore = new System.Windows.Forms.Label();
      this.labelRightScore = new System.Windows.Forms.Label();
      this.PlayerRight = new System.Windows.Forms.PictureBox();
      this.PlayerLeft = new System.Windows.Forms.PictureBox();
      this.Ball = new System.Windows.Forms.PictureBox();
      this.pictureBox1 = new System.Windows.Forms.PictureBox();
      this.pictureBoxGame = new System.Windows.Forms.PictureBox();
      this.pictureBoxCam = new System.Windows.Forms.PictureBox();
      this.timerRightPaddle = new System.Windows.Forms.Timer(this.components);
      this.labelHelp = new System.Windows.Forms.Label();
      this.chkComputer = new System.Windows.Forms.CheckBox();
      this.lblError = new System.Windows.Forms.Label();
      this.chkLive = new System.Windows.Forms.CheckBox();
      ((System.ComponentModel.ISupportInitialize)(this.PlayerRight)).BeginInit();
      ((System.ComponentModel.ISupportInitialize)(this.PlayerLeft)).BeginInit();
      ((System.ComponentModel.ISupportInitialize)(this.Ball)).BeginInit();
      ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).BeginInit();
      ((System.ComponentModel.ISupportInitialize)(this.pictureBoxGame)).BeginInit();
      ((System.ComponentModel.ISupportInitialize)(this.pictureBoxCam)).BeginInit();
      this.SuspendLayout();
      // 
      // btnStart
      // 
      this.btnStart.Location = new System.Drawing.Point(9, 378);
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
      // labelLeftScore
      // 
      this.labelLeftScore.AutoSize = true;
      this.labelLeftScore.Font = new System.Drawing.Font("Courier New", 18F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.labelLeftScore.ForeColor = System.Drawing.Color.White;
      this.labelLeftScore.Location = new System.Drawing.Point(248, 18);
      this.labelLeftScore.Name = "labelLeftScore";
      this.labelLeftScore.Size = new System.Drawing.Size(26, 27);
      this.labelLeftScore.TabIndex = 17;
      this.labelLeftScore.Text = "0";
      // 
      // labelRightScore
      // 
      this.labelRightScore.AutoSize = true;
      this.labelRightScore.Font = new System.Drawing.Font("Courier New", 18F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.labelRightScore.ForeColor = System.Drawing.Color.White;
      this.labelRightScore.Location = new System.Drawing.Point(372, 18);
      this.labelRightScore.Name = "labelRightScore";
      this.labelRightScore.Size = new System.Drawing.Size(26, 27);
      this.labelRightScore.TabIndex = 19;
      this.labelRightScore.Text = "0";
      // 
      // PlayerRight
      // 
      this.PlayerRight.BackColor = System.Drawing.Color.White;
      this.PlayerRight.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
      this.PlayerRight.Location = new System.Drawing.Point(600, 149);
      this.PlayerRight.Name = "PlayerRight";
      this.PlayerRight.Size = new System.Drawing.Size(30, 90);
      this.PlayerRight.TabIndex = 21;
      this.PlayerRight.TabStop = false;
      // 
      // PlayerLeft
      // 
      this.PlayerLeft.BackColor = System.Drawing.Color.White;
      this.PlayerLeft.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
      this.PlayerLeft.Location = new System.Drawing.Point(20, 149);
      this.PlayerLeft.Name = "PlayerLeft";
      this.PlayerLeft.Size = new System.Drawing.Size(30, 90);
      this.PlayerLeft.TabIndex = 20;
      this.PlayerLeft.TabStop = false;
      // 
      // Ball
      // 
      this.Ball.BackColor = System.Drawing.Color.White;
      this.Ball.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Stretch;
      this.Ball.Image = global::GameServer.Properties.Resources.img_120503;
      this.Ball.Location = new System.Drawing.Point(282, 149);
      this.Ball.Name = "Ball";
      this.Ball.Size = new System.Drawing.Size(80, 80);
      this.Ball.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
      this.Ball.TabIndex = 16;
      this.Ball.TabStop = false;
      // 
      // pictureBox1
      // 
      this.pictureBox1.Image = global::GameServer.Properties.Resources.RoombaPong_logo;
      this.pictureBox1.Location = new System.Drawing.Point(1026, 382);
      this.pictureBox1.Name = "pictureBox1";
      this.pictureBox1.Size = new System.Drawing.Size(257, 61);
      this.pictureBox1.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
      this.pictureBox1.TabIndex = 5;
      this.pictureBox1.TabStop = false;
      // 
      // pictureBoxGame
      // 
      this.pictureBoxGame.BackColor = System.Drawing.Color.Black;
      this.pictureBoxGame.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
      this.pictureBoxGame.Location = new System.Drawing.Point(4, 5);
      this.pictureBoxGame.Name = "pictureBoxGame";
      this.pictureBoxGame.Size = new System.Drawing.Size(640, 360);
      this.pictureBoxGame.TabIndex = 1;
      this.pictureBoxGame.TabStop = false;
      this.pictureBoxGame.Paint += new System.Windows.Forms.PaintEventHandler(this.pictureBoxGame_Paint);
      // 
      // pictureBoxCam
      // 
      this.pictureBoxCam.Location = new System.Drawing.Point(650, 5);
      this.pictureBoxCam.Name = "pictureBoxCam";
      this.pictureBoxCam.Size = new System.Drawing.Size(640, 360);
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
      this.labelHelp.Location = new System.Drawing.Point(146, 377);
      this.labelHelp.Name = "labelHelp";
      this.labelHelp.Size = new System.Drawing.Size(47, 13);
      this.labelHelp.TabIndex = 22;
      this.labelHelp.Text = "help text";
      // 
      // chkComputer
      // 
      this.chkComputer.AutoSize = true;
      this.chkComputer.ForeColor = System.Drawing.Color.White;
      this.chkComputer.Location = new System.Drawing.Point(7, 410);
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
      this.lblError.Location = new System.Drawing.Point(485, 377);
      this.lblError.Name = "lblError";
      this.lblError.Size = new System.Drawing.Size(48, 13);
      this.lblError.TabIndex = 24;
      this.lblError.Text = "error text";
      // 
      // chkLive
      // 
      this.chkLive.AutoSize = true;
      this.chkLive.ForeColor = System.Drawing.Color.White;
      this.chkLive.Location = new System.Drawing.Point(6, 426);
      this.chkLive.Name = "chkLive";
      this.chkLive.Size = new System.Drawing.Size(46, 17);
      this.chkLive.TabIndex = 25;
      this.chkLive.Text = "Live";
      this.chkLive.UseVisualStyleBackColor = true;
      this.chkLive.CheckedChanged += new System.EventHandler(this.chkLive_CheckedChanged);
      // 
      // Form1
      // 
      this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
      this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
      this.BackColor = System.Drawing.Color.Black;
      this.ClientSize = new System.Drawing.Size(1295, 450);
      this.Controls.Add(this.chkLive);
      this.Controls.Add(this.lblError);
      this.Controls.Add(this.chkComputer);
      this.Controls.Add(this.labelHelp);
      this.Controls.Add(this.labelRightScore);
      this.Controls.Add(this.labelLeftScore);
      this.Controls.Add(this.PlayerRight);
      this.Controls.Add(this.PlayerLeft);
      this.Controls.Add(this.Ball);
      this.Controls.Add(this.btnStart);
      this.Controls.Add(this.pictureBox1);
      this.Controls.Add(this.pictureBoxGame);
      this.Controls.Add(this.pictureBoxCam);
      this.Name = "Form1";
      this.Text = "BONG - RoombaPong";
      this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.Form1_FormClosing);
      this.Load += new System.EventHandler(this.Form1_Load);
      this.KeyDown += new System.Windows.Forms.KeyEventHandler(this.Pressed);
      this.KeyUp += new System.Windows.Forms.KeyEventHandler(this.Released);
      ((System.ComponentModel.ISupportInitialize)(this.PlayerRight)).EndInit();
      ((System.ComponentModel.ISupportInitialize)(this.PlayerLeft)).EndInit();
      ((System.ComponentModel.ISupportInitialize)(this.Ball)).EndInit();
      ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).EndInit();
      ((System.ComponentModel.ISupportInitialize)(this.pictureBoxGame)).EndInit();
      ((System.ComponentModel.ISupportInitialize)(this.pictureBoxCam)).EndInit();
      this.ResumeLayout(false);
      this.PerformLayout();

    }

    #endregion

    private System.Windows.Forms.PictureBox pictureBoxCam;
    private System.Windows.Forms.PictureBox pictureBoxGame;
    private System.Windows.Forms.PictureBox pictureBox1;
    private System.Windows.Forms.Button btnStart;
    private System.Windows.Forms.Timer timerLeftPaddle;
    private System.Windows.Forms.Timer timerBall;
    private System.Windows.Forms.Timer timerComputer;
    private System.Windows.Forms.PictureBox Ball;
    private System.Windows.Forms.PictureBox PlayerLeft;
    private System.Windows.Forms.PictureBox PlayerRight;
    private System.Windows.Forms.Label labelLeftScore;
    private System.Windows.Forms.Label labelRightScore;
    private System.Windows.Forms.Timer timerRightPaddle;
    private System.Windows.Forms.Label labelHelp;
    private System.Windows.Forms.CheckBox chkComputer;
    private System.Windows.Forms.Label lblError;
    private System.Windows.Forms.CheckBox chkLive;
  }
}

