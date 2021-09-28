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
      this.button1 = new System.Windows.Forms.Button();
      this.labelScoreLeft = new System.Windows.Forms.Label();
      this.labelScoreRight = new System.Windows.Forms.Label();
      this.pictureBox1 = new System.Windows.Forms.PictureBox();
      this.pictureBoxGame = new System.Windows.Forms.PictureBox();
      this.pictureBoxCam = new System.Windows.Forms.PictureBox();
      this.button2 = new System.Windows.Forms.Button();
      this.timer1 = new System.Windows.Forms.Timer(this.components);
      this.timer2 = new System.Windows.Forms.Timer(this.components);
      this.timer3 = new System.Windows.Forms.Timer(this.components);
      this.Ball = new System.Windows.Forms.PictureBox();
      this.Player = new System.Windows.Forms.PictureBox();
      this.PC = new System.Windows.Forms.PictureBox();
      this.label1 = new System.Windows.Forms.Label();
      this.label2 = new System.Windows.Forms.Label();
      this.label3 = new System.Windows.Forms.Label();
      ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).BeginInit();
      ((System.ComponentModel.ISupportInitialize)(this.pictureBoxGame)).BeginInit();
      ((System.ComponentModel.ISupportInitialize)(this.pictureBoxCam)).BeginInit();
      ((System.ComponentModel.ISupportInitialize)(this.Ball)).BeginInit();
      ((System.ComponentModel.ISupportInitialize)(this.Player)).BeginInit();
      ((System.ComponentModel.ISupportInitialize)(this.PC)).BeginInit();
      this.SuspendLayout();
      // 
      // button1
      // 
      this.button1.Location = new System.Drawing.Point(12, 382);
      this.button1.Name = "button1";
      this.button1.Size = new System.Drawing.Size(80, 40);
      this.button1.TabIndex = 2;
      this.button1.Text = "Start";
      this.button1.UseVisualStyleBackColor = true;
      this.button1.Click += new System.EventHandler(this.button1_Click);
      // 
      // labelScoreLeft
      // 
      this.labelScoreLeft.AutoSize = true;
      this.labelScoreLeft.BackColor = System.Drawing.Color.Black;
      this.labelScoreLeft.Font = new System.Drawing.Font("Courier New", 24F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.labelScoreLeft.ForeColor = System.Drawing.Color.White;
      this.labelScoreLeft.Location = new System.Drawing.Point(54, 37);
      this.labelScoreLeft.Name = "labelScoreLeft";
      this.labelScoreLeft.Size = new System.Drawing.Size(53, 36);
      this.labelScoreLeft.TabIndex = 3;
      this.labelScoreLeft.Text = "00";
      // 
      // labelScoreRight
      // 
      this.labelScoreRight.AutoSize = true;
      this.labelScoreRight.BackColor = System.Drawing.Color.Black;
      this.labelScoreRight.Font = new System.Drawing.Font("Courier New", 24F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.labelScoreRight.ForeColor = System.Drawing.Color.White;
      this.labelScoreRight.Location = new System.Drawing.Point(537, 37);
      this.labelScoreRight.Name = "labelScoreRight";
      this.labelScoreRight.Size = new System.Drawing.Size(53, 36);
      this.labelScoreRight.TabIndex = 4;
      this.labelScoreRight.Text = "00";
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
      // 
      // pictureBoxCam
      // 
      this.pictureBoxCam.Location = new System.Drawing.Point(650, 5);
      this.pictureBoxCam.Name = "pictureBoxCam";
      this.pictureBoxCam.Size = new System.Drawing.Size(640, 360);
      this.pictureBoxCam.TabIndex = 0;
      this.pictureBoxCam.TabStop = false;
      // 
      // button2
      // 
      this.button2.Location = new System.Drawing.Point(150, 382);
      this.button2.Name = "button2";
      this.button2.Size = new System.Drawing.Size(80, 40);
      this.button2.TabIndex = 13;
      this.button2.Text = "Start Game";
      this.button2.UseVisualStyleBackColor = true;
      this.button2.Click += new System.EventHandler(this.button2_Click);
      // 
      // timer1
      // 
      this.timer1.Interval = 5;
      this.timer1.Tick += new System.EventHandler(this.MovePaddle);
      // 
      // timer2
      // 
      this.timer2.Interval = 25;
      this.timer2.Tick += new System.EventHandler(this.MoveBall);
      // 
      // timer3
      // 
      this.timer3.Interval = 5;
      this.timer3.Tick += new System.EventHandler(this.Computer);
      // 
      // Ball
      // 
      this.Ball.BackColor = System.Drawing.Color.White;
      this.Ball.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Stretch;
      this.Ball.Location = new System.Drawing.Point(271, 149);
      this.Ball.Name = "Ball";
      this.Ball.Size = new System.Drawing.Size(80, 80);
      this.Ball.TabIndex = 16;
      this.Ball.TabStop = false;
      // 
      // Player
      // 
      this.Player.BackColor = System.Drawing.Color.White;
      this.Player.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
      this.Player.Location = new System.Drawing.Point(20, 149);
      this.Player.Name = "Player";
      this.Player.Size = new System.Drawing.Size(30, 90);
      this.Player.TabIndex = 20;
      this.Player.TabStop = false;
      // 
      // PC
      // 
      this.PC.BackColor = System.Drawing.Color.White;
      this.PC.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
      this.PC.Location = new System.Drawing.Point(600, 149);
      this.PC.Name = "PC";
      this.PC.Size = new System.Drawing.Size(30, 90);
      this.PC.TabIndex = 21;
      this.PC.TabStop = false;
      // 
      // label1
      // 
      this.label1.AutoSize = true;
      this.label1.Font = new System.Drawing.Font("MS Reference Sans Serif", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(238)));
      this.label1.ForeColor = System.Drawing.Color.White;
      this.label1.Location = new System.Drawing.Point(237, 83);
      this.label1.Name = "label1";
      this.label1.Size = new System.Drawing.Size(20, 20);
      this.label1.TabIndex = 17;
      this.label1.Text = "0";
      // 
      // label2
      // 
      this.label2.AutoSize = true;
      this.label2.Font = new System.Drawing.Font("MS Reference Sans Serif", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(238)));
      this.label2.ForeColor = System.Drawing.Color.White;
      this.label2.Location = new System.Drawing.Point(300, 83);
      this.label2.Name = "label2";
      this.label2.Size = new System.Drawing.Size(17, 20);
      this.label2.TabIndex = 18;
      this.label2.Text = ":";
      // 
      // label3
      // 
      this.label3.AutoSize = true;
      this.label3.Font = new System.Drawing.Font("MS Reference Sans Serif", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(238)));
      this.label3.ForeColor = System.Drawing.Color.White;
      this.label3.Location = new System.Drawing.Point(361, 83);
      this.label3.Name = "label3";
      this.label3.Size = new System.Drawing.Size(20, 20);
      this.label3.TabIndex = 19;
      this.label3.Text = "0";
      // 
      // Form1
      // 
      this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
      this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
      this.BackColor = System.Drawing.Color.Black;
      this.ClientSize = new System.Drawing.Size(1295, 450);
      this.Controls.Add(this.label3);
      this.Controls.Add(this.label2);
      this.Controls.Add(this.label1);
      this.Controls.Add(this.PC);
      this.Controls.Add(this.Player);
      this.Controls.Add(this.Ball);
      this.Controls.Add(this.button2);
      this.Controls.Add(this.pictureBox1);
      this.Controls.Add(this.labelScoreRight);
      this.Controls.Add(this.labelScoreLeft);
      this.Controls.Add(this.button1);
      this.Controls.Add(this.pictureBoxGame);
      this.Controls.Add(this.pictureBoxCam);
      this.Name = "Form1";
      this.Text = "BONG - RoombaPong";
      this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.Form1_FormClosing);
      this.Load += new System.EventHandler(this.Form1_Load);
      this.KeyDown += new System.Windows.Forms.KeyEventHandler(this.Pressed);
      this.KeyUp += new System.Windows.Forms.KeyEventHandler(this.Released);
      ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).EndInit();
      ((System.ComponentModel.ISupportInitialize)(this.pictureBoxGame)).EndInit();
      ((System.ComponentModel.ISupportInitialize)(this.pictureBoxCam)).EndInit();
      ((System.ComponentModel.ISupportInitialize)(this.Ball)).EndInit();
      ((System.ComponentModel.ISupportInitialize)(this.Player)).EndInit();
      ((System.ComponentModel.ISupportInitialize)(this.PC)).EndInit();
      this.ResumeLayout(false);
      this.PerformLayout();

    }

    #endregion

    private System.Windows.Forms.PictureBox pictureBoxCam;
    private System.Windows.Forms.PictureBox pictureBoxGame;
    private System.Windows.Forms.Button button1;
    private System.Windows.Forms.Label labelScoreLeft;
    private System.Windows.Forms.Label labelScoreRight;
    private System.Windows.Forms.PictureBox pictureBox1;
    private System.Windows.Forms.Button button2;
    private System.Windows.Forms.Timer timer1;
    private System.Windows.Forms.Timer timer2;
    private System.Windows.Forms.Timer timer3;
    private System.Windows.Forms.PictureBox Ball;
    private System.Windows.Forms.PictureBox Player;
    private System.Windows.Forms.PictureBox PC;
    private System.Windows.Forms.Label label1;
    private System.Windows.Forms.Label label2;
    private System.Windows.Forms.Label label3;
  }
}

