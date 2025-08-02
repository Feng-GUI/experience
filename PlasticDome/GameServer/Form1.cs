using System;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Linq;
using System.Net.Sockets;
using System.Threading;
using System.Windows.Forms;
using System.IO.Ports;


using OpenCvSharp;
using OpenCvSharp.Aruco;
using OpenCvSharp.Extensions;

namespace GameServer
{
  public partial class Form1 : Form
  {
    const int MAP_CAM_USB_INDEX = 1;
    const int SERIALPORT_BAUD_RATE = 9600;
    const string SERIALPORT_PORT_NAME = "COM6";

    VideoCapture captureMap;
    Mat frame;
    Bitmap image;
    Thread captureMapThread;
    Rect carRect;

    SerialPort serialPort = null;

    private void CaptureCamera()
    {
      captureMapThread = new Thread(new ThreadStart(CaptureCameraMapCallback));
      captureMapThread.Start();
    }

    private void ReleaseCamera()
    {
      if (captureMapThread != null) captureMapThread.Abort();
      if (captureMap != null) captureMap.Release();
    }

    private void CaptureCameraMapCallback()
    {
      captureMap = new VideoCapture();
      //captureMap.Set(VideoCaptureProperties.FrameWidth, 640);
      //captureMap.Set(VideoCaptureProperties.FrameHeight, 480);
      captureMap.Open(MAP_CAM_USB_INDEX);
      // reduce frame size to allow 2 camera work on USB bandwith
      captureMap.FrameWidth = 640;//1920;
      captureMap.FrameHeight = 480;//1080;

      frame = new Mat();

      if (captureMap.IsOpened())
      {
        while (true)
        {
          // read frame from camera
          captureMap.Read(frame);

          // detect the car
          frame = detectCar(frame);

          // draw frame on picture
          image = BitmapConverter.ToBitmap(frame);
          if (pictureBoxMap.Image != null)
          {
            pictureBoxMap.Image.Dispose();
          }
          pictureBoxMap.Image = image;

        }
      }
    }


    public Mat detectCar(Mat src)
    {
      // find the Red car in the HSV color space
      // define the lower and upper boundaries
      Mat hsv = new Mat();
      Cv2.CvtColor(src, hsv, ColorConversionCodes.BGR2HSV);

      // lower mask (0-10)
      Scalar redLower1 = new Scalar(0, 150, 150);
      Scalar redUpper1 = new Scalar(10, 255, 255);
      Mat carMask1 = new Mat();
      Cv2.InRange(hsv, redLower1, redUpper1, carMask1);

      // lower mask (0-10)
      Scalar redLower2 = new Scalar(170, 150, 150);
      Scalar redUpper2 = new Scalar(180, 255, 255);
      Mat carMask2 = new Mat();
      Cv2.InRange(hsv, redLower2, redUpper2, carMask2);

      // join the masks
      Mat carMask = carMask1 + carMask2;

      OpenCvSharp.Point[][] contours;
      HierarchyIndex[] hierarchyIndexes; //vector<Vec4i> hierarchy;
      Cv2.FindContours(carMask, out contours, out hierarchyIndexes, RetrievalModes.Tree, ContourApproximationModes.ApproxSimple);

      for (int c=0; c < contours.Length ; c++)
      {
        var contour = contours[c];
        var area = Cv2.ContourArea(contour);
        if (area > 1000)
        {
          carRect = Cv2.BoundingRect(contour);
          Cv2.Rectangle(src,
                      new OpenCvSharp.Point(carRect.X, carRect.Y),
                      new OpenCvSharp.Point(carRect.X + carRect.Width, carRect.Y + carRect.Height),
                      new Scalar(0, 255, 255), 10);

          break;
        }
      }

      hsv.Release();
      carMask1.Release();
      carMask1.Release();
      carMask.Release();

      return src;

    }

    public Form1()
    {
      InitializeComponent();
      this.KeyPreview = true;
    }

    private void Form1_Load(object sender, EventArgs e)
    {
      try
      {
        labelHelp.Text = "Use the arrow keys to drive the turret";
      }
      catch (Exception ex)
      {
        Console.WriteLine("Exception: {0}", ex);
        lblError.Text = ex.Message;
      }
    }

    private void Form1_FormClosing(object sender, FormClosingEventArgs e)
    {
      try
      {
        ReleaseCamera();

        //exit application
        //Environment.Exit(0);

      }
      catch (Exception ex)
      {
        Console.WriteLine("Exception: {0}", ex);
        //lblError.Text = e.Message;
      }

    }

    const int limit_Pad = 170;
    const int limit_Ball = 245;
    const int x = 227, y = 120;

    // keys controlling the car remote
    bool leftKey = false;
    bool rightKey = false;
    bool upKey = false;
    bool downKey = false;

    Random r = new Random();

    // key pressed event
    private void Pressed(object sender, KeyEventArgs e)
    {

      if (e.KeyCode == Keys.Left)
      {
        leftKey = true;
      }
      else if (e.KeyCode == Keys.Right)
      {
        rightKey = true;
      }
      else if (e.KeyCode == Keys.Up)
      {
        upKey = true;
      }
      else if (e.KeyCode == Keys.Down)
      {
        downKey = true;
      }

      timerSelfDrive.Start();
    }

    // key released event
    private void Released(object sender, KeyEventArgs e)
    {
      if (e.KeyCode == Keys.Left)
      {
        leftKey = false;
      }
      else if (e.KeyCode == Keys.Right)
      {
        rightKey = false;
      }

      if (e.KeyCode == Keys.Up)
      {
        upKey = false;
      }
      else if (e.KeyCode == Keys.Down)
      {
        downKey = false;
      }

      timerSelfDrive.Stop();
    }

    private void button2_Click(object sender, EventArgs e)
    {
      try
      {
        timerSelfDrive.Start();
        CaptureCamera();
      }
      catch (Exception ex)
      {
        Console.WriteLine("Exception: {0}", ex);
        lblError.Text = ex.Message;
      }
    }

    private void pictureBoxGame_Paint(object sender, PaintEventArgs e)
    {
      int penWidth = 4;
      //// draw middle dashed line
      //using (Pen pen = new Pen(Color.White, penWidth))
      //{
      //    pen.DashStyle = System.Drawing.Drawing2D.DashStyle.Dash;
      //    e.Graphics.DrawLine(pen, new PointF(pictureBoxCar.Width / 2, 0), new PointF(pictureBoxCar.Width / 2, pictureBoxCar.Height));
      //}

      // draw rectangle
      Rectangle rect = new Rectangle(0, 0, pictureBoxCar.Width - penWidth, pictureBoxCar.Height - penWidth);
      using (Pen pen = new Pen(Color.White, penWidth))
      {
        e.Graphics.DrawRectangle(pen, rect);
      }

    }

    private void SendMessageToSerial(String message)
    {
      try
      {

        if (serialPort == null)
        {
          serialPort = new SerialPort();
          serialPort.BaudRate = SERIALPORT_BAUD_RATE;
          serialPort.PortName = SERIALPORT_PORT_NAME;
          serialPort.StopBits = StopBits.One;
          serialPort.Parity = Parity.None;
          serialPort.Open();
        }
        if (serialPort.IsOpen)
        {
          // WRITE THE INCOMING BUFFER TO CONSOLE
          //while (serialPort.BytesToRead > 0)
          //{
          //    Console.Write(Convert.ToChar(serialPort.ReadChar()));
          //}
          // SEND
          serialPort.Write(message);
          Thread.Sleep(1000);
          //serialPort.Close();
        }
      }
      catch (Exception e)
      {
        Console.WriteLine("Exception: {0}", e);
        lblError.Text = e.Message;
      }

    }

    private void button1_Click(object sender, EventArgs e)
    {
      SendMessageToSerial("go");

    }

    private void button2_Click_1(object sender, EventArgs e)
    {
      SendMessageToSerial("stop");
    }

    private void pictureBoxMap_Paint(object sender, PaintEventArgs e)
    {
      e.Graphics.CompositingMode = CompositingMode.SourceOver;
      e.Graphics.PixelOffsetMode = PixelOffsetMode.HighQuality;
      e.Graphics.SmoothingMode = SmoothingMode.AntiAlias;

      base.OnPaint(e);
      var rect = new Rectangle(0, 0, pictureBoxMap.Width - 1, pictureBoxMap.Height - 1);


      using (var gp = new GraphicsPath())
      {
        gp.AddEllipse(rect);
        pictureBoxMap.Region = new Region(gp);
      }

      //rect.Inflate(-7 / 2 + 1, -7 / 2 + 1);
      //using (var pen = new Pen(Color.Pink, 7))
      //{
      //  e.Graphics.DrawEllipse(pen, rect);
      //}

    }

    private void timerSelfDrive_Tick(object sender, EventArgs e)
    {

    }

    private void button3_Click(object sender, EventArgs e)
    {
      SendMessageToSerial("turn90");
    }


  }
}
