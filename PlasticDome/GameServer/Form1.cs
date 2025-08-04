using System;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Linq;
using System.Net.Sockets;
using System.Threading;
using System.Windows.Forms;
using System.IO.Ports;
using System.Collections.Generic;

using OpenCvSharp;
using OpenCvSharp.Aruco;
using OpenCvSharp.Extensions;

namespace GameServer
{
	public partial class Form1 : Form
	{
		const int MAP_CAM_USB_INDEX = 0;
		const int SERIALPORT_BAUD_RATE = 9600;
		const string SERIALPORT_PORT_NAME = "COM6";

		VideoCapture captureMap;
		Mat frame;
		Bitmap image;
		Thread captureMapThread;
		//Rect objectRect;

		SerialPort serialPort = null;

		// Store past ball positions for drawing trajectory
		static List<OpenCvSharp.Point> trajectoryPoints = new List<OpenCvSharp.Point>();

		// Kalman filter object
		static KalmanFilter kalman;
		bool kalmanInitialized = false;
		float deltaTime = 0.1f; // 100 ms = 0.1 second

		// Prediction color (blue)
		static Scalar predictedColor = new Scalar(255, 0, 0);

		// Observation color (green)
		static Scalar observedColor = new Scalar(0, 255, 0);

		//////////////////////////////////////
		//
		// Remote Control 
		//
		const int limit_Pad = 170;
		const int limit_Ball = 245;
		const int x = 227, y = 120;

		// keys controlling the Remote Control
		bool leftKey = false;
		bool rightKey = false;
		bool upKey = false;
		bool downKey = false;
		Random r = new Random();


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

				// Initialize Kalman filter (stateSize = 4, measurementSize = 2)
				kalman = new KalmanFilter(4, 2);
				InitKalman();

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
					if (frame.Empty())
						continue;

					// detect the missile
					frame = ProcessFrame(frame);

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

		static void InitKalman()
		{
			kalman = new KalmanFilter(4, 2); // 4 dynamic parameters (x, y, vx, vy), 2 measurements (x, y)

			// Transition Matrix (constant velocity model)
			kalman.TransitionMatrix = new Mat(4, 4, MatType.CV_32FC1, new float[]
			{
				1, 0, 1, 0,
				0, 1, 0, 1,
				0, 0, 1, 0,
				0, 0, 0, 1
			});

			// Measurement Matrix
			kalman.MeasurementMatrix = new Mat(2, 4, MatType.CV_32FC1, new float[]
			{
				1, 0, 0, 0,
				0, 1, 0, 0
			});

			// Process noise covariance
			kalman.ProcessNoiseCov = Mat.Eye(4, 4, MatType.CV_32FC1) * 1e-2;

			// Measurement noise covariance
			kalman.MeasurementNoiseCov = Mat.Eye(2, 2, MatType.CV_32FC1) * 1e-1;

			// Posteriori error estimate covariance matrix
			kalman.ErrorCovPost = Mat.Eye(4, 4, MatType.CV_32FC1);

			// Initial state
			kalman.StatePost = new Mat(4, 1, MatType.CV_32FC1, new float[] { 0, 0, 0, 0 });
		}


		//ResetTransitionMatrix(); // Reset to dt = 1 for standard use
		void ResetTransitionMatrix()
		{
				kalman.TransitionMatrix.Set<float>(0, 0, 1);
				kalman.TransitionMatrix.Set<float>(0, 1, 0);
				kalman.TransitionMatrix.Set<float>(0, 2, 1);
				kalman.TransitionMatrix.Set<float>(0, 3, 0);

				kalman.TransitionMatrix.Set<float>(1, 0, 0);
				kalman.TransitionMatrix.Set<float>(1, 1, 1);
				kalman.TransitionMatrix.Set<float>(1, 2, 0);
				kalman.TransitionMatrix.Set<float>(1, 3, 1);

				kalman.TransitionMatrix.Set<float>(2, 0, 0);
				kalman.TransitionMatrix.Set<float>(2, 1, 0);
				kalman.TransitionMatrix.Set<float>(2, 2, 1);
				kalman.TransitionMatrix.Set<float>(2, 3, 0);

				kalman.TransitionMatrix.Set<float>(3, 0, 0);
				kalman.TransitionMatrix.Set<float>(3, 1, 0);
				kalman.TransitionMatrix.Set<float>(3, 2, 0);
				kalman.TransitionMatrix.Set<float>(3, 3, 1);
		}

		void FutureTransitionMatrix()
		{
			float dt = deltaTime; // seconds into the future

			kalman.TransitionMatrix.Set<float>(0, 0, 1);
			kalman.TransitionMatrix.Set<float>(0, 1, 0);
			kalman.TransitionMatrix.Set<float>(0, 2, dt);
			kalman.TransitionMatrix.Set<float>(0, 3, 0);

			kalman.TransitionMatrix.Set<float>(1, 0, 0);
			kalman.TransitionMatrix.Set<float>(1, 1, 1);
			kalman.TransitionMatrix.Set<float>(1, 2, 0);
			kalman.TransitionMatrix.Set<float>(1, 3, dt);

			kalman.TransitionMatrix.Set<float>(2, 0, 0);
			kalman.TransitionMatrix.Set<float>(2, 1, 0);
			kalman.TransitionMatrix.Set<float>(2, 2, 1);
			kalman.TransitionMatrix.Set<float>(2, 3, 0);

			kalman.TransitionMatrix.Set<float>(3, 0, 0);
			kalman.TransitionMatrix.Set<float>(3, 1, 0);
			kalman.TransitionMatrix.Set<float>(3, 2, 0);
			kalman.TransitionMatrix.Set<float>(3, 3, 1);

		}



		public Mat ProcessFrame(Mat src)
		{
			// find the Red object in the HSV color space
			// define the lower and upper boundaries
			Mat hsv = new Mat();
			Cv2.CvtColor(src, hsv, ColorConversionCodes.BGR2HSV);

			// lower mask (0-10)
			Scalar redLower1 = new Scalar(0, 150, 150);
			Scalar redUpper1 = new Scalar(10, 255, 255);
			Mat objectMask1 = new Mat();
			Cv2.InRange(hsv, redLower1, redUpper1, objectMask1);

			// lower mask (0-10)
			Scalar redLower2 = new Scalar(170, 150, 150);
			Scalar redUpper2 = new Scalar(180, 255, 255);
			Mat objectMask2 = new Mat();
			Cv2.InRange(hsv, redLower2, redUpper2, objectMask2);

			// join the masks
			Mat objectMask = objectMask1 + objectMask2;

			OpenCvSharp.Point[][] contours;
			HierarchyIndex[] hierarchyIndexes; //vector<Vec4i> hierarchy;
			Cv2.FindContours(objectMask, out contours, out hierarchyIndexes, RetrievalModes.Tree, ContourApproximationModes.ApproxSimple);

			// Find the largest contour (assumed to be the object)
			double maxArea = 0;
			OpenCvSharp.Point[] largestContour = null;
			OpenCvSharp.Point detectedCenter = new OpenCvSharp.Point();
			bool ballDetected = false;

			foreach (var contour in contours)
			{
				double area = Cv2.ContourArea(contour);
				if (area > maxArea)
				{
					maxArea = area;
					largestContour = contour;

					Moments m = Cv2.Moments(contour);
					int cX = (int)(m.M10 / m.M00);
					int cY = (int)(m.M01 / m.M00);
					detectedCenter = new OpenCvSharp.Point(cX, cY);
					ballDetected = true;

				}
			}

			FutureTransitionMatrix();

			// Predict next state
			Mat prediction = kalman.Predict();
			float px = prediction.At<float>(0);
			float py = prediction.At<float>(1);
			Console.WriteLine($"Predicted: ({px}, {py})");

			OpenCvSharp.Point predictedPoint = new OpenCvSharp.Point(prediction.At<float>(0), prediction.At<float>(1));

			// Draw prediction point (blue)
			Cv2.Circle(frame, predictedPoint, 10, predictedColor, 2);
			Cv2.PutText(frame, "Predicted", predictedPoint + new OpenCvSharp.Point(15, -15), HersheyFonts.HersheySimplex, 0.5, predictedColor);

			if (ballDetected)
			{
				OpenCvSharp.Point measured = detectedCenter;
				trajectoryPoints.Add(measured);

				var measurement = new Mat(2, 1, MatType.CV_32FC1);
				measurement.Set<float>(0, 0, measured.X);
				measurement.Set<float>(1, 0, measured.Y);

				// First time: manually set initial state
				if (!kalmanInitialized)
				{
					kalman.StatePost.Set<float>(0, 0, measured.X); // x
					kalman.StatePost.Set<float>(1, 0, measured.Y); // y
					kalman.StatePost.Set<float>(2, 0, 0);          // vx
					kalman.StatePost.Set<float>(3, 0, 0);          // vy
					kalmanInitialized = true;
				}

				// Correct Kalman with actual measured position
				kalman.Correct(measurement);

				// Draw actual position (green)
				Cv2.Circle(frame, measured, 10, observedColor, -1);
			}


			// Draw trajectory line from observed positions
			for (int i = 1; i < trajectoryPoints.Count; i++)
			{
				Cv2.Line(frame, trajectoryPoints[i - 1], trajectoryPoints[i], new Scalar(0, 255, 255), 2);
			}


			hsv.Release();
			objectMask1.Release();
			objectMask1.Release();
			objectMask.Release();

			return src;

		}


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
			//e.Graphics.CompositingMode = CompositingMode.SourceOver;
			//e.Graphics.PixelOffsetMode = PixelOffsetMode.HighQuality;
			//e.Graphics.SmoothingMode = SmoothingMode.AntiAlias;

			//base.OnPaint(e);
			//var rect = new Rectangle(0, 0, pictureBoxMap.Width - 1, pictureBoxMap.Height - 1);

			//using (var gp = new GraphicsPath())
			//{
			//  gp.AddEllipse(rect);
			//  pictureBoxMap.Region = new Region(gp);
			//}

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
