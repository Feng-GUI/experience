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
    // hard coded
    const int MAP_CAM_USB_INDEX = 1;
    const int CAR_CAM_USB_INDEX = 2;
    const int SERIALPORT_BAUD_RATE = 9600;
    const string SERIALPORT_PORT_NAME = "COM6";

    // top camera
    VideoCapture captureMap;
    Mat frameOrig;
    Mat frame;
    Bitmap image;
    Thread captureMapThread;
    Rect carRect;

    // car front camera
    VideoCapture captureCar;
    Thread captureCarThread;
    Mat cartoon;

    SerialPort serialPort = null;

    Dictionary dictionary = CvAruco.GetPredefinedDictionary(PredefinedDictionaryName.Dict4X4_250);
    DetectorParameters parameters = DetectorParameters.Create();

    // camera calibration
    Mat cameraMatrix = new Mat(new OpenCvSharp.Size(3, 3), MatType.CV_32F, 1);
    Mat distCoeffs = new Mat(1, 8, MatType.CV_32F, 1);

    private void CaptureCamera()
    {
      captureMapThread = new Thread(new ThreadStart(CaptureCameraMapCallback));
      captureMapThread.Start();

      captureCarThread = new Thread(new ThreadStart(CaptureCameraCarCallback));
      captureCarThread.Start();
    }

    private void ReleaseCamera()
    {
      if (captureMapThread != null) captureMapThread.Abort();
      if (captureMap != null) captureMap.Release();

      if (captureCarThread != null) captureCarThread.Abort();
      if (captureCar != null) captureCar.Release();
    }

    // camera skew calibration init
    private void CalibrateCameraInit()
    {
      cameraMatrix = Mat.Eye(3, 3, MatType.CV_64FC1);
      distCoeffs = Mat.Zeros(4, 1, MatType.CV_64FC1);
    }

    private void CaptureCameraMapCallback()
    {
      captureMap = new VideoCapture();
      captureMap.Open(MAP_CAM_USB_INDEX);
      captureMap.FrameWidth = 1920;
      captureMap.FrameHeight = 1080;

      frameOrig = new Mat();
      frame = new Mat();

      if (captureMap.IsOpened())
      {
        while (chkLive.Checked)
        {

          // read frame from camera
          captureMap.Read(frame);

          // detect the car
          frame = detectCar(frame);


/*
 * detect object using markers 
 * 
          captureMap.Read(frameOrig);

          // scale down frame
          Cv2.Resize(frameOrig, frame, new OpenCvSharp.Size(pictureBoxMap.Width, pictureBoxMap.Height)); // 640, 360

          // calibrate camera - de-skew frame
          //removed skew as it is not correct frame = OpenWarpPerspective(frame);

          CvAruco.DetectMarkers(frame, dictionary, out Point2f[][] corners, out int[] ids, parameters, out Point2f[][] rejected);
          if (ids.Any())
          {
            // detect markers
            CvAruco.DrawDetectedMarkers(frame, corners, ids);

            // detect markers pose
            using (var rvecs = new Mat())
            using (var tvecs = new Mat())
            {
              CvAruco.EstimatePoseSingleMarkers(corners, 0.065f, cameraMatrix, distCoeffs, rvecs, tvecs);
              for (var i = 0; i < ids.Length; i++)
              {
                var rvec = rvecs.Get<Vec3d>(i);
                var tvec = tvecs.Get<Vec3d>(i);

                // draw markers pose
                CvAruco.DrawAxis(frame, cameraMatrix, distCoeffs, rvec, tvec, 0.05f);

                Mat R = new Mat();
                OpenCvSharp.Cv2.Rodrigues(rvec, R);

                double[,] rvecDouble = new double[3, 3];

                //rvecDouble[0,0]=rvec.Item0;
                //rvecDouble[0,1]=rvec.Item1;
                //rvecDouble[0,2]=rvec.Item2;

                rvecDouble[0, 0] = R.Get<double>(0, 0);
                rvecDouble[0, 1] = R.Get<double>(0, 1);
                rvecDouble[0, 2] = R.Get<double>(0, 2);
                rvecDouble[1, 0] = R.Get<double>(1, 0);
                rvecDouble[1, 1] = R.Get<double>(1, 1);
                rvecDouble[1, 2] = R.Get<double>(1, 2);
                rvecDouble[2, 0] = R.Get<double>(2, 0);
                rvecDouble[2, 1] = R.Get<double>(2, 1);
                rvecDouble[2, 2] = R.Get<double>(2, 2);

                RotationMatrixToEulerZXY(rvecDouble);

                // if this is the roomba Marker id
                if (ids[i] == 0)
                {

                }
                // left player Marker id
                else if (ids[i] == 1)
                {

                }
                // right player Marker id
                else if (ids[i] == 2)
                {

                }

              }
            }


          }

*/
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

    private void CaptureCameraCarCallback()
    {
      captureCar = new VideoCapture();
      captureCar.Open(CAR_CAM_USB_INDEX);
      captureCar.FrameWidth = 1920;
      captureCar.FrameHeight = 1080;

      Mat frame = new Mat();

      if (captureCar.IsOpened())
      {
        while (chkLive.Checked)
        {
          // read frame from camera
          captureCar.Read(frame);

          // cartoonize the frame
          frame = cartoonize(frame);

          // draw frame on picture
          image = BitmapConverter.ToBitmap(frame);
          // dispose previous image
          if (pictureBoxCar.Image != null)
          {
            pictureBoxCar.Image.Dispose();
          }
          // draw image on picture
          pictureBoxCar.Image = image;
        }
      }
    }

    public Mat cartoonize(Mat src)
    {
      //PERF: consider resize the image for fater manipulation

      // Edges
      // grayscale and smooth
      Mat gray = new Mat();
      Cv2.CvtColor(src, gray, ColorConversionCodes.BGR2GRAY);
      Cv2.MedianBlur(gray, gray, 5);
      // thresholding to get the edges for cartoon effect
      Mat edgesC1 = new Mat();
      Cv2.AdaptiveThreshold(gray, edgesC1, 255, AdaptiveThresholdTypes.MeanC, ThresholdTypes.Binary, 9, 9);

      Mat edgesC3 = new Mat(src.Size(), MatType.CV_8UC3);
      Cv2.Merge(new Mat[] { edgesC1, edgesC1, edgesC1 }, edgesC3);

      // Color
      // applying bilateral filter to remove noise 
      // and keep edge sharp as required
      Mat color = new Mat(src.Size(), MatType.CV_8UC3);
      Cv2.BilateralFilter(src, color, 9, 300, 300);

      //
      // Cartoon
      //
      // merge the thick edges on the original image
      //cartoon = new Mat(src.Size(), MatType.CV_8UC3);
      // could not get core_bitwise_and to work. using AddWeighted instead
      //Mat mask = Mat.Zeros(src.Size(), MatType.CV_8UC3);// new Mat(src.Size(), MatType.CV_8UC3);
      //Cv2.bitwise_and(color, color, mask = edges);
      //OpenCvSharp.Internal.NativeMethods.core_bitwise_and(color.CvPtr, edgesC3.CvPtr, cartoon.CvPtr, mask.CvPtr);

      Cv2.AddWeighted(src, 0.7, edgesC3, 0.3, 0, src);

      // release Mats.
      //PERF: consider moving the Mats to global and not releasing them
      gray.Release();
      edgesC1.Release();
      edgesC3.Release();
      color.Release();

      return src;
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

    public void RotationMatrixToEulerZXY(double[,] R)
    {
      var i = 2;
      var j = 0; // EULER_NEXT[2]
      var k = 1; // EULER_NEXT[3]

      var cos_beta = Math.Sqrt(Math.Pow(R[i, i], 2) + Math.Pow(R[j, i], 2));

      double alpha, beta, gamma;
      double EULER_EPSILON = 2.71828;
      if (cos_beta > EULER_EPSILON)
      {
        alpha = Math.Atan2(R[k, j], R[k, k]);
        beta = Math.Atan2(-R[k, i], cos_beta);
        gamma = Math.Atan2(R[j, i], R[i, i]);
      }
      else
      {
        alpha = Math.Atan2(-R[j, k], R[j, j]);
        beta = Math.Atan2(-R[k, i], cos_beta);
        gamma = 0.0;
      }

      // Radian To Degree
      alpha = 180.0 - alpha * (180 / Math.PI);
      beta = beta * (180 / Math.PI);
      gamma = gamma * (180 / Math.PI);

      Console.WriteLine(alpha + "," + beta + "," + gamma);
    }

    public Form1()
    {
      InitializeComponent();
      this.KeyPreview = true;
    }

    private void Form1_Load(object sender, EventArgs e)
    {
      labelHelp.Text = "Use the arrow keys to drive the car";

      CalibrateCameraInit();

      chkLive.Checked = true;
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
      timerSelfDrive.Start();
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

    private void chkLive_CheckedChanged(object sender, EventArgs e)
    {
      if (chkLive.Checked)
      {
        CaptureCamera();
      }
      else
      {
        ReleaseCamera();
      }
    }

    // de-skew camera trapez into rectangle
    Mat OpenWarpPerspective(Mat src)
    {

      Point2f[] a =
      {
                new Point2f(0 + 160, 0),
                new Point2f(0, pictureBoxMap.Height),
                new Point2f(pictureBoxMap.Width, pictureBoxMap.Height),
                new Point2f(pictureBoxMap.Width - 160, 0)
            };

      Point2f[] b =
      {
                new Point2f(0, 0),
                new Point2f(0, pictureBoxMap.Height),
                new Point2f(pictureBoxMap.Width, pictureBoxMap.Height),
                new Point2f(pictureBoxMap.Width, 0)
            };

      Mat dest = new Mat(new OpenCvSharp.Size(pictureBoxMap.Width, pictureBoxMap.Height), MatType.CV_8UC3);
      Mat map_matrix = Cv2.GetPerspectiveTransform(a, b);
      Cv2.WarpPerspective(src, dest, map_matrix, dest.Size(), InterpolationFlags.Linear | InterpolationFlags.WarpFillOutliers, BorderTypes.Default, Scalar.All(255)); //AccessViolation

      map_matrix.Dispose();
      return dest;
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

      rect.Inflate(-7 / 2 + 1, -7 / 2 + 1);
      using (var pen = new Pen(Color.Pink, 7))
      {
        e.Graphics.DrawEllipse(pen, rect);
      }

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
