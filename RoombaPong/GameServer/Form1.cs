using System;
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

using OpenCvSharp.Aruco;

namespace GameServer
{
  public partial class Form1 : Form
  {

    VideoCapture capture;
    Mat frameOrig;
    Mat frame;
    Bitmap image;
    Thread cameraThread;
    bool isCameraRunning = false;

    Dictionary dictionary = CvAruco.GetPredefinedDictionary(PredefinedDictionaryName.Dict6X6_250);
    DetectorParameters parameters = DetectorParameters.Create();

    // calibration
    Mat cameraMatrix = new Mat(new OpenCvSharp.Size(3, 3), MatType.CV_32F, 1);
    Mat distCoeffs = new Mat(1, 8, MatType.CV_32F, 1);

    private void CaptureCamera()
    {
      cameraThread = new Thread(new ThreadStart(CaptureCameraCallback));
      cameraThread.Start();
    }

    private void CaptureCameraCallback()
    {

      capture = new VideoCapture(0);
      capture.Open(0);
      capture.FrameWidth = 1920;
      capture.FrameHeight = 1080;

      frameOrig = new Mat();
      frame = new Mat();

      if (capture.IsOpened())
      {
        while (isCameraRunning)
        {

          // read frame from camera
          capture.Read(frameOrig);
          // scale down frame
          Cv2.Resize(frameOrig, frame, new OpenCvSharp.Size(640, 360));


          // detect markers
          //std::vector<int> ids;
          //std::vector<std::vector<cv::Point2f> > corners;
          //cv::aruco::detectMarkers(image, dictionary, corners, ids);
          //using (var undistorted = new Mat())
          //Cv2.Undistort(image, undistorted, camera, distortion, newCamera);
          var undistorted = new Mat();
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
                //CvAruco.DrawAxis(frameBong, cameraMatrix, distCoeffs, rvec, tvec, 0.05f);
              }
            }


          }


          // draw frame on picture
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
      this.KeyPreview = true;
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

    private void Form1_FormClosing(object sender, FormClosingEventArgs e)
    {
      //close threads
      //cameraThread.Abort();
      //capture.Release();

      //exit application
      Environment.Exit(0);
    }


    /*
    private async Task DetectMarkers()
    {
        string output = @"C:\projects\src\OfirNoy\Geekcon2021\GameServer\output.png";
        var rms = 0.0;
        var calib = 100;
        var size = new OpenCvSharp.Size(9, 6);
        var frameSize = OpenCvSharp.Size.Zero;
        var distortion = new Mat();
        var imgPoints = new List<MatOfPoint2f>();
        var objPoints = new List<MatOfPoint3f>();
        var criteria = new TermCriteria(CriteriaType.Eps | CriteriaType.MaxIter, 30, 0.001);
        var objp = MatOfPoint3f.FromArray(Create3DChessboardCorners(size, 0.025f));
        using (var capture = new VideoCapture(0))
        using (var paramters = DetectorParameters.Create())
        using (var camera = new MatOfDouble(Mat.Eye(3, 3, MatType.CV_64FC1)))
        using (var dictionary = CvAruco.GetPredefinedDictionary(PredefinedDictionaryName.Dict4X4_50))
        {
            while (capture.Grab() && calib > 0)
            {
                using (var image = capture.RetrieveMat())
                using (var gray = new Mat())
                {
                    frameSize = image.Size();
                    Cv2.CvtColor(image, gray, ColorConversionCodes.BGR2GRAY);
                    if (Cv2.FindChessboardCorners(gray, size, out Point2f[] corners))
                    {
                        objPoints.Add(objp);
                        imgPoints.Add(MatOfPoint2f.FromArray(corners.ToArray()));
                        var corners2 = Cv2.CornerSubPix(gray, corners, new OpenCvSharp.Size(11, 11), new OpenCvSharp.Size(-1, -1), criteria);
                        Cv2.DrawChessboardCorners(image, size, corners2, true);
                        image.SaveImage(output);
                        calib--;
                        await Task.Delay(100);
                    }
                    image.SaveImage(output);
                }
            }
            rms = Cv2.CalibrateCamera(objPoints, imgPoints, frameSize, camera, distortion, out var rvectors, out var tvectors, CalibrationFlags.UseIntrinsicGuess | CalibrationFlags.FixK5);
            using (var newCamera = Cv2.GetOptimalNewCameraMatrix(camera, distortion, frameSize, 1, frameSize, out var roi))
            {
                await Task.Delay(1);
                while (capture.Grab())
                {
                    using (var undistorted = new Mat())
                    using (var image = capture.RetrieveMat())
                    {
                        Cv2.Undistort(image, undistorted, camera, distortion, newCamera);
                        CvAruco.DetectMarkers(undistorted, dictionary, out Point2f[][] corners, out int[] ids, paramters, out Point2f[][] rejected);
                        if (ids.Any())
                        {
                            CvAruco.DrawDetectedMarkers(undistorted, corners, ids);
                            using (var rvecs = new Mat())
                            using (var tvecs = new Mat())
                            {
                                CvAruco.EstimatePoseSingleMarkers(corners, 0.065f, newCamera, distortion, rvecs, tvecs);
                                for (var i = 0; i < ids.Length; i++)
                                {
                                    var rvec = rvecs.Get<Vec3d>(i);
                                    var tvec = tvecs.Get<Vec3d>(i);
                                    DrawAxis(undistorted, newCamera, distortion, rvec, tvec, 0.05f);
                                }
                            }
                        }
                        undistorted.SaveImage(output);
                    }
                }
            }
        }
    }

    private static IEnumerable<Point3f> Create3DChessboardCorners(OpenCvSharp.Size boardSize, float squareSize)
    {
        for (int y = 0; y < boardSize.Height; y++)
        {
            for (int x = 0; x < boardSize.Width; x++)
            {
                yield return new Point3f(x * squareSize, y * squareSize, 0);
            }
        }
    }
*/

    /*
    private static void DrawAxis(Mat image, InputArray camera, InputArray distortion, Vec3d rvec, Vec3d tvec, float length)
    {
        if (image.Total() == 0 || (image.Channels() != 1 && image.Channels() != 3))
        {
            throw new ArgumentException(nameof(image));
        }
        if (length <= 0)
        {
            throw new ArgumentException(nameof(length));
        }
        // project axis points
        var axisPoints = new MatOfPoint3f()
        {
            new Point3f(0, 0, 0),
            new Point3f(length, 0, 0),
            new Point3f(0, length, 0),
            new Point3f(0, 0, length),
        };
        var imagePoints = new MatOfPoint2f();
        Cv2.ProjectPoints(axisPoints, InputArray.Create(new[] { rvec }), InputArray.Create(new[] { tvec }), camera, distortion, imagePoints);
        // draw axis lines
        //Cv2.Line(image, imagePoints.Get<Point2f>(0), imagePoints.Get<Point2f>(1), new Scalar(0, 0, 255), 3);
        //Cv2.Line(image, imagePoints.Get<Point2f>(0), imagePoints.Get<Point2f>(2), new Scalar(0, 255, 0), 3);
        //Cv2.Line(image, imagePoints.Get<Point2f>(0), imagePoints.Get<Point2f>(3), new Scalar(255, 0, 0), 3);
    }
    */

    const int limit_Pad = 170;
    const int limit_Ball = 245;
    const int x = 227, y = 120;

    int computer_won = 0;
    int player_won = 0;

    int speed_Top;
    int speed_Left;

    bool up = false;
    bool down = false;
    bool game = false;

    Random r = new Random();

    private int getLimitTop()
    {
      return pictureBoxGame.Top;
    }
    private int getLimitBottom()
    {
      return pictureBoxGame.Height - pictureBoxGame.Top;
    }
    private int getLimitRight()
    {
      return pictureBoxGame.Left + pictureBoxGame.Width;
    }

    private void Pressed(object sender, KeyEventArgs e)
    {
      if (game)
      {
        if (e.KeyCode == Keys.Up || e.KeyCode == Keys.W)
        {
          up = true;
        }
        else if (e.KeyCode == Keys.Down || e.KeyCode == Keys.S)
        {
          down = true;
        }
        timer1.Start();
      }
    }
    private void MovePaddle(object sender, EventArgs e)
    {
      if (up && Player.Location.Y > getLimitTop())
      {
        Player.Top -= 3;
      }
      else if (down && Player.Location.Y < getLimitBottom() - Player.Height)
      {
        Player.Top += 3;
      }
    }
    private void Released(object sender, KeyEventArgs e)
    {
      if (e.KeyCode == Keys.Up || e.KeyCode == Keys.W)
      {
        up = false;
      }
      else if (e.KeyCode == Keys.Up || e.KeyCode == Keys.S)
      {
        down = false;
      }
      timer1.Stop();
    }
    private void MoveBall(object sender, EventArgs e)
    {
      if (Ball.Bounds.IntersectsWith(Player.Bounds))
      {
        Collision(Player);
      }
      else if (Ball.Bounds.IntersectsWith(PC.Bounds))
      {
        Collision(PC);
      }
      HitBorder();
      BallLeftField();
      BallMoves();
    }
    private void Collision(PictureBox Paddle)
    {
      switch (true)
      {
        case true when Upper(Paddle):
          speed_Top = Negative(4, 6);
          speed_Left = AdjustCoordinates(5, 6);
          break;
        case true when High(Paddle):
          speed_Top = Negative(2, 3);
          speed_Left = AdjustCoordinates(6, 7);
          break;
        case true when Middle(Paddle):
          speed_Top = 0;
          speed_Left = AdjustCoordinates(5, 5);
          break;
        case true when Low(Paddle):
          speed_Top = r.Next(2, 3);
          speed_Left = AdjustCoordinates(6, 7);
          break;
        case true when Bot(Paddle):
          speed_Top = r.Next(4, 6);
          speed_Left = AdjustCoordinates(5, 6);
          break;
      }
      Edge();
    }
    private int AdjustCoordinates(int i, int n)
    {
      int res = 0;

      if (Ball.Location.X < getLimitRight() / 2)
      {
        res = r.Next(i, n);
      }
      else if (Ball.Location.X > getLimitRight() / 2)
      {
        res = Negative(i, n);
      }
      return res;
    }
    private int Negative(int i, int n)
    {
      int myval = r.Next(i, n) * -1;
      return myval;
    }
    private bool Upper(PictureBox Pad)
    {
      return Ball.Location.Y >= Pad.Top - Ball.Height && Ball.Location.Y <= Pad.Top + Ball.Height;
    }
    private bool High(PictureBox Pad)
    {
      return Ball.Location.Y > Pad.Top + Ball.Height && Ball.Location.Y <= Pad.Top + 2 * Ball.Height;
    }
    private bool Middle(PictureBox Pad)
    {
      return Ball.Location.Y > Pad.Top + 2 * Ball.Height && Ball.Location.Y <= Pad.Top + 3 * Ball.Height;
    }
    private bool Low(PictureBox Pad)
    {
      return Ball.Location.Y > Pad.Top + 3 * Ball.Height && Ball.Location.Y <= Pad.Top + 4 * Ball.Height;
    }
    private bool Bot(PictureBox Pad)
    {
      return Ball.Location.Y > Pad.Top + 4 * Ball.Height && Ball.Location.Y <= Pad.Bottom + Ball.Height;
    }
    private void HitBorder()
    {
      if (Ball.Location.Y <= 0 || Ball.Location.Y >= limit_Ball)
      {
        speed_Top *= -1;
      }
    }
    private void BallLeftField()
    {
      if (player_won == 10 || computer_won == 10)
      {
        EndGame();
      }

      if (Ball.Location.X < 0 - Player.Width && Ball.Location.X < getLimitRight() / 2)
      {
        NewPoint(5);
        ComputerWon();
      }
      else if (Ball.Location.X > PC.Location.X + PC.Width && Ball.Location.X > getLimitRight() / 2)
      {
        NewPoint(-5);
        PlayerWon();
      }
    }
    private void Edge()
    {
      if (Ball.Location.X < getLimitRight() / 2)
      {
        if (Ball.Location.X < 0 + Ball.Height / 3)
        {
          speed_Left *= -1;
        }
      }
      else if (Ball.Location.X > getLimitRight() / 2)
      {
        if (Ball.Location.X > PC.Location.X + (Ball.Width / 3))
        {
          speed_Left *= -1;
        }
      }
    }
    private void NewPoint(int n)
    {
      Ball.Location = new System.Drawing.Point(x, y);
      speed_Top = 0;
      speed_Left = n;
    }
    private void StartValues()
    {
      speed_Top = 0;
      speed_Left = -5;
    }
    private void BallMoves()
    {
      Ball.Top += speed_Top;
      Ball.Left += speed_Left;
    }
    private void Computer(object sender, EventArgs e)
    {
      if (PC.Location.Y <= 0)
      {
        PC.Location = new System.Drawing.Point(PC.Location.X, 0);
      }
      else if (PC.Location.Y >= limit_Pad)
      {
        PC.Location = new System.Drawing.Point(PC.Location.X, limit_Pad);
      }
      if (Ball.Location.Y < PC.Top + (PC.Height / 2))
      {
        PC.Top -= 3;
      }
      else if (Ball.Location.Y > PC.Top + (PC.Height / 2))
      {
        PC.Top += 3;
      }
    }
    private void PlayerWon()
    {
      player_won++;
      label1.Text = player_won.ToString();
    }
    private void ComputerWon()
    {
      computer_won++;
      label3.Text = computer_won.ToString();
    }
    private void button2_Click(object sender, EventArgs e)
    {
      StartValues();
      game = true;
      timer1.Start();
      timer2.Start();
      timer3.Start();
    }

    private void EndGame()
    {
      Player.Location = new System.Drawing.Point(0, 75);
      PC.Location = new System.Drawing.Point(454, 75);
      game = false;
      player_won = 0;
      computer_won = 0;
      label1.Text = player_won.ToString();
      label3.Text = computer_won.ToString();
      timer1.Stop();
      timer2.Stop();
      timer3.Stop();
    }

  }
}
