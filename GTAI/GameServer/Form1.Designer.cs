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
            this.pictureBoxMap = new System.Windows.Forms.PictureBox();
            this.timerRightPaddle = new System.Windows.Forms.Timer(this.components);
            this.labelHelp = new System.Windows.Forms.Label();
            this.chkComputer = new System.Windows.Forms.CheckBox();
            this.lblError = new System.Windows.Forms.Label();
            this.chkLive = new System.Windows.Forms.CheckBox();
            this.button1 = new System.Windows.Forms.Button();
            this.button2 = new System.Windows.Forms.Button();
            this.button3 = new System.Windows.Forms.Button();
            this.lblDebug = new System.Windows.Forms.Label();
            this.pictureBoxCar = new System.Windows.Forms.PictureBox();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBoxMap)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBoxCar)).BeginInit();
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
            // pictureBoxMap
            // 
            this.pictureBoxMap.Location = new System.Drawing.Point(930, 324);
            this.pictureBoxMap.Name = "pictureBoxMap";
            this.pictureBoxMap.Size = new System.Drawing.Size(222, 166);
            this.pictureBoxMap.TabIndex = 0;
            this.pictureBoxMap.TabStop = false;
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
            this.lblError.Location = new System.Drawing.Point(246, 629);
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
            // pictureBoxCar
            // 
            this.pictureBoxCar.Location = new System.Drawing.Point(0, 0);
            this.pictureBoxCar.Name = "pictureBoxCar";
            this.pictureBoxCar.Size = new System.Drawing.Size(891, 588);
            this.pictureBoxCar.TabIndex = 31;
            this.pictureBoxCar.TabStop = false;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.Color.Black;
            this.ClientSize = new System.Drawing.Size(1295, 710);
            this.Controls.Add(this.pictureBoxMap);
            this.Controls.Add(this.pictureBoxCar);
            this.Controls.Add(this.lblDebug);
            this.Controls.Add(this.button3);
            this.Controls.Add(this.button2);
            this.Controls.Add(this.button1);
            this.Controls.Add(this.chkLive);
            this.Controls.Add(this.lblError);
            this.Controls.Add(this.chkComputer);
            this.Controls.Add(this.labelHelp);
            this.Controls.Add(this.btnStart);
            this.Name = "Form1";
            this.Text = "GTA.I.";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.Form1_FormClosing);
            this.Load += new System.EventHandler(this.Form1_Load);
            this.KeyDown += new System.Windows.Forms.KeyEventHandler(this.Pressed);
            this.KeyUp += new System.Windows.Forms.KeyEventHandler(this.Released);
            ((System.ComponentModel.ISupportInitialize)(this.pictureBoxMap)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBoxCar)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

    }

    #endregion

    private System.Windows.Forms.PictureBox pictureBoxMap;
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
    private System.Windows.Forms.PictureBox pictureBoxCar;
  }
}

