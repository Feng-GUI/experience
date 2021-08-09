using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
<<<<<<< HEAD
using System.Drawing.Imaging;
using System.Threading;
=======
>>>>>>> 64c199fd5170305554d1f5683298cd630737e448
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

<<<<<<< HEAD

using OpenCvSharp;
using OpenCvSharp.Extensions;

=======
>>>>>>> 64c199fd5170305554d1f5683298cd630737e448
namespace GameServer
{
  public partial class Form1 : Form
  {
<<<<<<< HEAD
    // Create class-level accesible variables
    VideoCapture capture;
    Mat frame;
    Bitmap image;
    private Thread camera;
    bool isCameraRunning = false;


    // Declare required methods
    private void CaptureCamera()
    {
        camera = new Thread(new ThreadStart(CaptureCameraCallback));
        camera.Start();
    }

    private void CaptureCameraCallback()
    {

        frame = new Mat();
        capture = new VideoCapture(0);
        capture.Open(0);

        if (capture.IsOpened())
        {
            while (isCameraRunning)
            {

                capture.Read(frame);
                image = BitmapConverter.ToBitmap(frame);
                if (pictureBoxCam.Image != null)
                {
                    pictureBoxCam.Image.Dispose();
                }
                pictureBoxCam.Image = image;
            }
        }
    }


=======
>>>>>>> 64c199fd5170305554d1f5683298cd630737e448
    public Form1()
    {
      InitializeComponent();
    }
<<<<<<< HEAD

    private void Form1_Load(object sender, EventArgs e)
    {

    }

    // When the user clicks on the start/stop button, start or release the camera and setup flags
    private void button1_Click(object sender, EventArgs e)
    {
        if (button1.Text.Equals("Start"))
        {
            CaptureCamera();
            button1.Text = "Stop";
            isCameraRunning = true;
        }
        else
        {
            capture.Release();
            button1.Text = "Start";
            isCameraRunning = false;
        }
    }

    private void pictureBoxCam_Click(object sender, EventArgs e)
    {

    }
=======
>>>>>>> 64c199fd5170305554d1f5683298cd630737e448
  }
}
