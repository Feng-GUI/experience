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
<<<<<<< HEAD
      this.pictureBoxCam = new System.Windows.Forms.PictureBox();
      this.pictureBoxGame = new System.Windows.Forms.PictureBox();
      this.button1 = new System.Windows.Forms.Button();
      this.labelScoreLeft = new System.Windows.Forms.Label();
      this.labelScoreRight = new System.Windows.Forms.Label();
      ((System.ComponentModel.ISupportInitialize)(this.pictureBoxCam)).BeginInit();
      ((System.ComponentModel.ISupportInitialize)(this.pictureBoxGame)).BeginInit();
      this.SuspendLayout();
      // 
      // pictureBoxCam
      // 
      this.pictureBoxCam.Location = new System.Drawing.Point(67, 66);
      this.pictureBoxCam.Name = "pictureBoxCam";
      this.pictureBoxCam.Size = new System.Drawing.Size(460, 415);
      this.pictureBoxCam.TabIndex = 0;
      this.pictureBoxCam.TabStop = false;
      this.pictureBoxCam.Click += new System.EventHandler(this.pictureBoxCam_Click);
      // 
      // pictureBoxGame
      // 
      this.pictureBoxGame.BackColor = System.Drawing.Color.Black;
      this.pictureBoxGame.Location = new System.Drawing.Point(700, 66);
      this.pictureBoxGame.Name = "pictureBoxGame";
      this.pictureBoxGame.Size = new System.Drawing.Size(460, 415);
      this.pictureBoxGame.TabIndex = 1;
      this.pictureBoxGame.TabStop = false;
      // 
      // button1
      // 
      this.button1.Location = new System.Drawing.Point(67, 502);
      this.button1.Name = "button1";
      this.button1.Size = new System.Drawing.Size(75, 23);
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
      this.labelScoreLeft.Location = new System.Drawing.Point(816, 85);
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
      this.labelScoreRight.Location = new System.Drawing.Point(1036, 85);
      this.labelScoreRight.Name = "labelScoreRight";
      this.labelScoreRight.Size = new System.Drawing.Size(53, 36);
      this.labelScoreRight.TabIndex = 4;
      this.labelScoreRight.Text = "00";
      // 
      // Form1
      // 
      this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
      this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
      this.ClientSize = new System.Drawing.Size(1215, 539);
      this.Controls.Add(this.labelScoreRight);
      this.Controls.Add(this.labelScoreLeft);
      this.Controls.Add(this.button1);
      this.Controls.Add(this.pictureBoxGame);
      this.Controls.Add(this.pictureBoxCam);
      this.Name = "Form1";
      this.Text = "Form1";
      this.Load += new System.EventHandler(this.Form1_Load);
      ((System.ComponentModel.ISupportInitialize)(this.pictureBoxCam)).EndInit();
      ((System.ComponentModel.ISupportInitialize)(this.pictureBoxGame)).EndInit();
      this.ResumeLayout(false);
      this.PerformLayout();

    }

    #endregion

    private System.Windows.Forms.PictureBox pictureBoxCam;
    private System.Windows.Forms.PictureBox pictureBoxGame;
    private System.Windows.Forms.Button button1;
    private System.Windows.Forms.Label labelScoreLeft;
    private System.Windows.Forms.Label labelScoreRight;
=======
      this.components = new System.ComponentModel.Container();
      this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
      this.ClientSize = new System.Drawing.Size(800, 450);
      this.Text = "Form1";
    }

    #endregion
>>>>>>> 64c199fd5170305554d1f5683298cd630737e448
  }
}

