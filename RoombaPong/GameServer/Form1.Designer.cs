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
            this.button1 = new System.Windows.Forms.Button();
            this.button2 = new System.Windows.Forms.Button();
            this.button3 = new System.Windows.Forms.Button();
            this.lblDebug = new System.Windows.Forms.Label();
            this.button4 = new System.Windows.Forms.Button();
            this.timerBounce = new System.Windows.Forms.Timer(this.components);
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
            this.btnStart.Location = new System.Drawing.Point(12, 465);
            this.btnStart.Margin = new System.Windows.Forms.Padding(4);
            this.btnStart.Name = "btnStart";
            this.btnStart.Size = new System.Drawing.Size(107, 32);
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
            this.labelLeftScore.Location = new System.Drawing.Point(331, 22);
            this.labelLeftScore.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.labelLeftScore.Name = "labelLeftScore";
            this.labelLeftScore.Size = new System.Drawing.Size(33, 34);
            this.labelLeftScore.TabIndex = 17;
            this.labelLeftScore.Text = "0";
            // 
            // labelRightScore
            // 
            this.labelRightScore.AutoSize = true;
            this.labelRightScore.Font = new System.Drawing.Font("Courier New", 18F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.labelRightScore.ForeColor = System.Drawing.Color.White;
            this.labelRightScore.Location = new System.Drawing.Point(496, 22);
            this.labelRightScore.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.labelRightScore.Name = "labelRightScore";
            this.labelRightScore.Size = new System.Drawing.Size(33, 34);
            this.labelRightScore.TabIndex = 19;
            this.labelRightScore.Text = "0";
            // 
            // PlayerRight
            // 
            this.PlayerRight.BackColor = System.Drawing.Color.White;
            this.PlayerRight.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.PlayerRight.Location = new System.Drawing.Point(800, 183);
            this.PlayerRight.Margin = new System.Windows.Forms.Padding(4);
            this.PlayerRight.Name = "PlayerRight";
            this.PlayerRight.Size = new System.Drawing.Size(39, 110);
            this.PlayerRight.TabIndex = 21;
            this.PlayerRight.TabStop = false;
            // 
            // PlayerLeft
            // 
            this.PlayerLeft.BackColor = System.Drawing.Color.White;
            this.PlayerLeft.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.PlayerLeft.Location = new System.Drawing.Point(27, 183);
            this.PlayerLeft.Margin = new System.Windows.Forms.Padding(4);
            this.PlayerLeft.Name = "PlayerLeft";
            this.PlayerLeft.Size = new System.Drawing.Size(39, 110);
            this.PlayerLeft.TabIndex = 20;
            this.PlayerLeft.TabStop = false;
            // 
            // Ball
            // 
            this.Ball.BackColor = System.Drawing.Color.White;
            this.Ball.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Stretch;
            this.Ball.Image = global::GameServer.Properties.Resources.img_120503;
            this.Ball.Location = new System.Drawing.Point(381, 204);
            this.Ball.Margin = new System.Windows.Forms.Padding(4);
            this.Ball.Name = "Ball";
            this.Ball.Size = new System.Drawing.Size(107, 98);
            this.Ball.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            this.Ball.TabIndex = 16;
            this.Ball.TabStop = false;
            // 
            // pictureBox1
            // 
            this.pictureBox1.Image = global::GameServer.Properties.Resources.RoombaPong_logo;
            this.pictureBox1.Location = new System.Drawing.Point(1368, 470);
            this.pictureBox1.Margin = new System.Windows.Forms.Padding(4);
            this.pictureBox1.Name = "pictureBox1";
            this.pictureBox1.Size = new System.Drawing.Size(343, 75);
            this.pictureBox1.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            this.pictureBox1.TabIndex = 5;
            this.pictureBox1.TabStop = false;
            // 
            // pictureBoxGame
            // 
            this.pictureBoxGame.BackColor = System.Drawing.Color.Black;
            this.pictureBoxGame.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.pictureBoxGame.Location = new System.Drawing.Point(5, 6);
            this.pictureBoxGame.Margin = new System.Windows.Forms.Padding(4);
            this.pictureBoxGame.Name = "pictureBoxGame";
            this.pictureBoxGame.Size = new System.Drawing.Size(853, 443);
            this.pictureBoxGame.TabIndex = 1;
            this.pictureBoxGame.TabStop = false;
            this.pictureBoxGame.Paint += new System.Windows.Forms.PaintEventHandler(this.pictureBoxGame_Paint);
            // 
            // pictureBoxCam
            // 
            this.pictureBoxCam.Location = new System.Drawing.Point(867, 6);
            this.pictureBoxCam.Margin = new System.Windows.Forms.Padding(4);
            this.pictureBoxCam.Name = "pictureBoxCam";
            this.pictureBoxCam.Size = new System.Drawing.Size(853, 443);
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
            this.labelHelp.Location = new System.Drawing.Point(195, 464);
            this.labelHelp.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.labelHelp.Name = "labelHelp";
            this.labelHelp.Size = new System.Drawing.Size(61, 17);
            this.labelHelp.TabIndex = 22;
            this.labelHelp.Text = "help text";
            // 
            // chkComputer
            // 
            this.chkComputer.AutoSize = true;
            this.chkComputer.ForeColor = System.Drawing.Color.White;
            this.chkComputer.Location = new System.Drawing.Point(9, 505);
            this.chkComputer.Margin = new System.Windows.Forms.Padding(4);
            this.chkComputer.Name = "chkComputer";
            this.chkComputer.Size = new System.Drawing.Size(172, 21);
            this.chkComputer.TabIndex = 23;
            this.chkComputer.Text = "Right Player Computer";
            this.chkComputer.UseVisualStyleBackColor = true;
            // 
            // lblError
            // 
            this.lblError.AutoSize = true;
            this.lblError.ForeColor = System.Drawing.Color.White;
            this.lblError.Location = new System.Drawing.Point(647, 464);
            this.lblError.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.lblError.Name = "lblError";
            this.lblError.Size = new System.Drawing.Size(65, 17);
            this.lblError.TabIndex = 24;
            this.lblError.Text = "error text";
            // 
            // chkLive
            // 
            this.chkLive.AutoSize = true;
            this.chkLive.ForeColor = System.Drawing.Color.White;
            this.chkLive.Location = new System.Drawing.Point(8, 524);
            this.chkLive.Margin = new System.Windows.Forms.Padding(4);
            this.chkLive.Name = "chkLive";
            this.chkLive.Size = new System.Drawing.Size(56, 21);
            this.chkLive.TabIndex = 25;
            this.chkLive.Text = "Live";
            this.chkLive.UseVisualStyleBackColor = true;
            this.chkLive.CheckedChanged += new System.EventHandler(this.chkLive_CheckedChanged);
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(751, 465);
            this.button1.Margin = new System.Windows.Forms.Padding(4);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(107, 32);
            this.button1.TabIndex = 26;
            this.button1.Text = "go";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // button2
            // 
            this.button2.Location = new System.Drawing.Point(751, 498);
            this.button2.Margin = new System.Windows.Forms.Padding(4);
            this.button2.Name = "button2";
            this.button2.Size = new System.Drawing.Size(107, 32);
            this.button2.TabIndex = 27;
            this.button2.Text = "stop";
            this.button2.UseVisualStyleBackColor = true;
            this.button2.Click += new System.EventHandler(this.button2_Click_1);
            // 
            // button3
            // 
            this.button3.Location = new System.Drawing.Point(867, 465);
            this.button3.Margin = new System.Windows.Forms.Padding(4);
            this.button3.Name = "button3";
            this.button3.Size = new System.Drawing.Size(107, 32);
            this.button3.TabIndex = 28;
            this.button3.Text = "turn";
            this.button3.UseVisualStyleBackColor = true;
            this.button3.Click += new System.EventHandler(this.button3_Click);
            // 
            // lblDebug
            // 
            this.lblDebug.AutoSize = true;
            this.lblDebug.ForeColor = System.Drawing.Color.White;
            this.lblDebug.Location = new System.Drawing.Point(866, 513);
            this.lblDebug.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.lblDebug.Name = "lblDebug";
            this.lblDebug.Size = new System.Drawing.Size(48, 17);
            this.lblDebug.TabIndex = 29;
            this.lblDebug.Text = "debug";
            // 
            // button4
            // 
            this.button4.Location = new System.Drawing.Point(1001, 465);
            this.button4.Margin = new System.Windows.Forms.Padding(4);
            this.button4.Name = "button4";
            this.button4.Size = new System.Drawing.Size(107, 32);
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
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.Color.Black;
            this.ClientSize = new System.Drawing.Size(1727, 554);
            this.Controls.Add(this.button4);
            this.Controls.Add(this.lblDebug);
            this.Controls.Add(this.button3);
            this.Controls.Add(this.button2);
            this.Controls.Add(this.button1);
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
            this.Margin = new System.Windows.Forms.Padding(4);
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
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.Button button2;
        private System.Windows.Forms.Button button3;
        private System.Windows.Forms.Label lblDebug;
        private System.Windows.Forms.Button button4;
        private System.Windows.Forms.Timer timerBounce;
    }
}

