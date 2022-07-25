using OpenCvSharp;
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
        VideoCapture capture;
        Mat frameOrig;
        Mat frame;
        Bitmap image;
        Thread cameraThread;

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
            cameraThread = new Thread(new ThreadStart(CaptureCameraCallback));
            cameraThread.Start();
        }

        private void ReleaseCamera()
        {
            if (cameraThread != null) cameraThread.Abort();
            if (capture != null) capture.Release();
        }

        // camera calibration
        private void CalibrateCamera()
        {
            cameraMatrix = Mat.Eye(3, 3, MatType.CV_64FC1);
            distCoeffs = Mat.Zeros(4, 1, MatType.CV_64FC1);
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
                while (chkLive.Checked)
                {

                    // read frame from camera
                    capture.Read(frameOrig);

                    // scale down frame
                    Cv2.Resize(frameOrig, frame, new OpenCvSharp.Size(pictureBoxGame.Width, pictureBoxGame.Height)); // 640, 360

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
                    if (pictureBoxCam.Image != null)
                    {
                        pictureBoxCam.Image.Dispose();
                    }
                    pictureBoxCam.Image = image;

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
                e.Graphics.DrawLine(pen, new PointF(pictureBoxGame.Width / 2, 0), new PointF(pictureBoxGame.Width / 2, pictureBoxGame.Height));
            }

            // draw rectangle
            Rectangle rect = new Rectangle(0, 0, pictureBoxGame.Width - penWidth, pictureBoxGame.Height - penWidth);
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
                CalibrateCamera();
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
          new Point2f(0, pictureBoxGame.Height),
          new Point2f(pictureBoxGame.Width, pictureBoxGame.Height),
          new Point2f(pictureBoxGame.Width - 160, 0)
      };

            Point2f[] b =
            {
          new Point2f(0, 0),
          new Point2f(0, pictureBoxGame.Height),
          new Point2f(pictureBoxGame.Width, pictureBoxGame.Height),
          new Point2f(pictureBoxGame.Width, 0)
      };

            Mat dest = new Mat(new OpenCvSharp.Size(pictureBoxGame.Width, pictureBoxGame.Height), MatType.CV_8UC3);
            Mat map_matrix = Cv2.GetPerspectiveTransform(a, b);
            Cv2.WarpPerspective(src, dest, map_matrix, dest.Size(), InterpolationFlags.Linear | InterpolationFlags.WarpFillOutliers, BorderTypes.Default, Scalar.All(255)); //AccessViolation

            map_matrix.Dispose();
            return dest;
        }


        private void SendToRoombaBT(String message)
        {
            try
            {

                if (serialPort == null)
                {
                    serialPort = new SerialPort();
                    serialPort.BaudRate = 9600;
                    serialPort.PortName = "COM6"; // Set in Windows
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
            SendToRoombaBT("go");

        }

        private void button2_Click_1(object sender, EventArgs e)
        {
            SendToRoombaBT("stop");
        }

        private void button3_Click(object sender, EventArgs e)
        {
            SendToRoombaBT("turn90");
        }

        private void button4_Click(object sender, EventArgs e)
        {
            //CalibrateCamera();
            //CaptureCamera();
            timerBounce.Start();
        }

        private int BallX, BallY;   // Position.
        private int BallVx = 4;
        private int BallVy = 4; // Velocity.

        private void SendToRoombaWifi(String message)
        {
            try
            {

                // Create a TcpClient
                TcpClient client = new TcpClient("192.168.4.1", 80);
                //PERF: optimize with client.Connected

                // Translate the passed message into ASCII and store it as a Byte array.
                Byte[] data = System.Text.Encoding.ASCII.GetBytes(message);

                // Get a client stream for reading and writing.
                NetworkStream stream = client.GetStream();

                // Send the message to the connected TcpServer.
                stream.Write(data, 0, data.Length);
                Console.WriteLine("Sent: {0}", message);

                // Receive the TcpServer.response.

                // Buffer to store the response bytes.
                //data = new Byte[256];
                // String to store the response ASCII representation.
                //String responseData = String.Empty;

                // Read the first batch of the TcpServer response bytes.
                ////StreamReader sr = new StreamReader(stream);
                ////responseData = sr.ReadLine();
                //Int32 bytes = stream.Read(data, 0, data.Length);
                //responseData = System.Text.Encoding.ASCII.GetString(data, 0, bytes);
                //Console.WriteLine("Received: {0}", responseData);

                stream.Close();
                client.Close();
            }
            catch (Exception e)
            {
                Console.WriteLine("Exception: {0}", e);
                lblError.Text = e.Message;
            }

        }


    }
}
