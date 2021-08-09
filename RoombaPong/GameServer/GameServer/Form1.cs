﻿using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Drawing.Imaging;
using System.Threading;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;


using OpenCvSharp;
using OpenCvSharp.Extensions;

namespace GameServer
{
  public partial class Form1 : Form
  {
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


    public Form1()
    {
      InitializeComponent();
    }

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
  }
}
