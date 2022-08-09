﻿using OpenCvSharp;
using OpenCvSharp.Aruco;
using OpenCvSharp.Extensions;
using System;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Linq;
using System.Net.Sockets;
using System.Threading;
using System.Windows.Forms;
using System.IO.Ports;

namespace GameServer
{
    public partial class Form1 : Form
    {
        VideoCapture captureMap;
        Mat frameOrig;
        Mat frame;
        Bitmap image;
        Thread captureMapThread;

        VideoCapture captureCar;
        Thread captureCarThread;

        SerialPort serialPort = null;

        Dictionary dictionary = CvAruco.GetPredefinedDictionary(PredefinedDictionaryName.Dict4X4_250);
        DetectorParameters parameters = DetectorParameters.Create();

        int prevX = -1;
        int prevY = -1;

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

        // camera calibration
        private void CalibrateCamera()
        {
            cameraMatrix = Mat.Eye(3, 3, MatType.CV_64FC1);
            distCoeffs = Mat.Zeros(4, 1, MatType.CV_64FC1);
        }

        private void CaptureCameraMapCallback()
        {
            captureMap = new VideoCapture();
            captureMap.Open(1);
            captureMap.FrameWidth = 1920;
            captureMap.FrameHeight = 1080;

            frameOrig = new Mat();
            frame = new Mat();

            if (captureMap.IsOpened())
            {
                while (chkLive.Checked)
                {

                    // read frame from camera
                    captureMap.Read(frameOrig);

                    // scale down frame
                    Cv2.Resize(frameOrig, frame, new OpenCvSharp.Size(pictureBoxMap.Width, pictureBoxMap.Height)); // 640, 360

                    // calibrate camera - de-skew frame
                    frame = OpenWarpPerspective(frame);

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
            captureCar.Open(2);
            captureCar.FrameWidth = 1920;
            captureCar.FrameHeight = 1080;

            Mat frame = new Mat();

            if (captureCar.IsOpened())
            {
                while (chkLive.Checked)
                {
                    // read frame from camera
                    captureCar.Read(frame);

                    // draw frame on picture
                    image = BitmapConverter.ToBitmap(frame);
                    if (pictureBoxCar.Image != null)
                    {
                        pictureBoxCar.Image.Dispose();
                    }
                    pictureBoxCar.Image = image;
                }
            }
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
            labelHelp.Text = "left up Q down A";
            labelHelp.Text += "\nright up P down L";

            CalibrateCamera();
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

        int player_right_won = 0;
        int player_won = 0;

        int stepTop;
        int stepLeft;

        int colisionDegree = -1;
        Boolean isCW = false;

        bool leftUp = false;
        bool leftDown = false;
        bool rightUp = false;
        bool rightDown = false;

        Random r = new Random();

        // key pressed event
        private void Pressed(object sender, KeyEventArgs e)
        {

            // left paddle
            if (e.KeyCode == Keys.Q)
            {
                leftUp = true;
            }
            else if (e.KeyCode == Keys.A)
            {
                leftDown = true;
            }

            // right paddle
            if (e.KeyCode == Keys.Up || e.KeyCode == Keys.P)
            {
                rightUp = true;
            }
            else if (e.KeyCode == Keys.Down || e.KeyCode == Keys.L)
            {
                rightDown = true;
            }

            timerLeftPaddle.Start();
            timerRightPaddle.Start();
        }

        // key released event
        private void Released(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Q)
            {
                leftUp = false;
            }
            else if (e.KeyCode == Keys.A)
            {
                leftDown = false;
            }

            if (e.KeyCode == Keys.Up || e.KeyCode == Keys.P)
            {
                rightUp = false;
            }
            else if (e.KeyCode == Keys.Up || e.KeyCode == Keys.L)
            {
                rightDown = false;
            }

            timerLeftPaddle.Stop();
            timerRightPaddle.Stop();
        }



        private void button2_Click(object sender, EventArgs e)
        {
            timerLeftPaddle.Start();
            timerRightPaddle.Start();
            timerBall.Start();
            timerComputer.Start();
        }

        private void pictureBoxGame_Paint(object sender, PaintEventArgs e)
        {
            int penWidth = 4;
            // draw middle dashed line
            using (Pen pen = new Pen(Color.White, penWidth))
            {
                pen.DashStyle = System.Drawing.Drawing2D.DashStyle.Dash;
                e.Graphics.DrawLine(pen, new PointF(pictureBoxCar.Width / 2, 0), new PointF(pictureBoxCar.Width / 2, pictureBoxCar.Height));
            }

            // draw rectangle
            Rectangle rect = new Rectangle(0, 0, pictureBoxCar.Width - penWidth, pictureBoxCar.Height - penWidth);
            using (Pen pen = new Pen(Color.White, penWidth))
            {
                e.Graphics.DrawRectangle(pen, rect);
            }

        }

        private void EndGame()
        {
            player_won = 0;
            player_right_won = 0;
            timerLeftPaddle.Stop();
            timerRightPaddle.Stop();
            timerBall.Stop();
            timerComputer.Stop();
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
                    serialPort.BaudRate = 9600; //FIXME
                    serialPort.PortName = "COM6"; //FIXME Set in Windows
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

        private void button3_Click(object sender, EventArgs e)
        {
            SendMessageToSerial("turn90");
        }





    }
}
