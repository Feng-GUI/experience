using System;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Linq;
using System.Net.Sockets;
using System.Threading;
using System.Diagnostics;
using System.Windows.Forms;
using System.IO;
using System.IO.Ports;
using System.Collections.Generic;

using DirectShowLib;

using OpenCvSharp;
using OpenCvSharp.Aruco;
using OpenCvSharp.Extensions;

namespace GameServer
{
	public partial class Form1 : Form
	{
		const int SERIALPORT_BAUD_RATE = 9600;
		const string SERIALPORT_PORT_NAME = "COM6";

		VideoCapture captureMap;
		Mat frame;
		Bitmap image;
		Thread captureMapThread;
		SerialPort serialPort = null;

		// Store past ball positions for drawing trajectory
		static List<OpenCvSharp.Point> trajectoryPoints = new List<OpenCvSharp.Point>();

		// Kalman filter object
		static KalmanFilter kalman;
		bool kalmanInitialized = false;

		// Prediction color (blue)
		static Scalar predictedColor = new Scalar(255, 0, 0);

		// Observation color (green)
		static Scalar observedColor = new Scalar(0, 255, 0);

		// prevent from flooding the serial. minimum time to wait between each serial command
		Stopwatch stopwatch = new Stopwatch();
		TimeSpan minimumInterval = TimeSpan.FromMilliseconds(500);

		public Form1()
		{
			InitializeComponent();
			this.vlcControl1.VlcLibDirectoryNeeded += vlcControl1_VlcLibDirectoryNeeded;

			this.KeyPreview = true;
		}

		private void Form1_Load(object sender, EventArgs e)
		{
			try
			{
				// Initialize Kalman filter (stateSize = 4, measurementSize = 2)
				kalman = new KalmanFilter(4, 2);
				InitKalman();

				string videoPath = @"C:\projects\PlasticDome\Sonar-1.mp4";
				Uri videoUri = new Uri(videoPath);

				// Create a new VlcMedia object with the '--loop' option
				var mediaOptions = new string[] { "input-repeat=1000000", "loop" };

				vlcControl1.SetMedia(new Uri(videoPath), mediaOptions);
				vlcControl1.Play();

				stopwatch.Start();
				CaptureCamera();
			}
			catch (Exception ex)
			{
				Console.WriteLine("Exception: {0}", ex);
				lblError.Text = ex.Message;
			}
		}

		/// <summary>
		/// Finds the camera's index by name using DirectShowLib.
		/// </summary>
		/// <param name="desiredCameraName">The full or partial name of the camera to find.</param>
		/// <returns>The index of the camera if found; otherwise, -1.</returns>
		public static int GetCameraIndexByName(string desiredCameraName)
		{
			// The DsDevice class is part of DirectShowLib and is used to enumerate video devices.
			DsDevice[] videoDevices = DsDevice.GetDevicesOfCat(FilterCategory.VideoInputDevice);

			// Check if any video devices were found.
			if (videoDevices.Length == 0)
			{
				Console.WriteLine("No video input devices found.");
				return -1;
			}

			// Loop through all found devices to find a name match.
			for (int i = 0; i < videoDevices.Length; i++)
			{
				// We use .Contains() for a flexible search (e.g., "Webcam" will match "HP HD Webcam").
				if (videoDevices[i].Name.Contains(desiredCameraName))
				{
					Console.WriteLine($"Found camera '{videoDevices[i].Name}' at index {i}.");
					return i; // Return the found index.
				}
			}

			// If the loop finishes and no camera was found, return -1.
			Console.WriteLine($"Camera with name '{desiredCameraName}' not found.");
			return -1;
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
			string myCameraName = "Lenovo FHD Webcam";
			int cameraIndex = GetCameraIndexByName(myCameraName);
			// --- Set the desired resolution BEFORE opening the camera ---
			captureMap.Set(VideoCaptureProperties.FrameWidth, 1920); // Set to FHD width
			captureMap.Set(VideoCaptureProperties.FrameHeight, 1080); // Set to FHD height
			captureMap.Open(cameraIndex);

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

					// --- IMPORTANT: The UI update must be on the main thread ---
					// We use Invoke to make sure the code block inside the lambda
					// is executed safely on the main UI thread.
					if (pictureBoxMap.InvokeRequired)
					{
						pictureBoxMap.Invoke((MethodInvoker)delegate
						{
							// Dispose of the old image to free up memory
							if (pictureBoxMap.Image != null)
							{
								pictureBoxMap.Image.Dispose();
							}

							// Convert the Mat to a Bitmap and set it as the PictureBox's image
							image = BitmapConverter.ToBitmap(frame);
							pictureBoxMap.Image = image;
						});
					}
					else
					{
						// If we're already on the main thread (unlikely in this scenario),
						// we can update directly.
						if (pictureBoxMap.Image != null)
						{
							pictureBoxMap.Image.Dispose();
						}
						image = BitmapConverter.ToBitmap(frame);
						pictureBoxMap.Image = image;
					}
				


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

		// dt = 1 for regular tracking per frame.
		// dt = 10 frames ahead into the future
		void FutureTransitionMatrix(float dt)
		{
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

			Mat hsv = new Mat();
			Cv2.CvtColor(src, hsv, ColorConversionCodes.BGR2HSV);

/*
						// find the Red object in the HSV color space
						// define the lower and upper boundaries
						// red in HSV is around 0-10 and 170-179 so 2 masks are needed

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
			*/
			// Define the HSV range for yellow
			// You only need one range for yellow, as its hue does not wrap around the 180-degree scale.
			Scalar yellowLower = new Scalar(20, 100, 100);
			Scalar yellowUpper = new Scalar(40, 255, 255);
			Mat objectMask = new Mat();
			Cv2.InRange(hsv, yellowLower, yellowUpper, objectMask);


			OpenCvSharp.Point[][] contours;
			HierarchyIndex[] hierarchyIndexes; //vector<Vec4i> hierarchy;
			Cv2.FindContours(objectMask, out contours, out hierarchyIndexes, RetrievalModes.Tree, ContourApproximationModes.ApproxSimple);

			// Find the largest contour (assumed to be the object)
			// set minimum size to reduce noise
			double maxArea = 50;
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

			if (ballDetected)
			{
				OpenCvSharp.Point measured = detectedCenter;
				//DEBUG trajectoryPoints.Add(measured);

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

				// look few frames into the future
				float dt = 10;
				FutureTransitionMatrix(dt);
				// Predict next state
				Mat prediction = kalman.Predict();
				OpenCvSharp.Point predictedPoint = new OpenCvSharp.Point(prediction.At<float>(0), prediction.At<float>(1));
				// Draw prediction point (blue)
				Cv2.Circle(frame, predictedPoint, 10, predictedColor, 2);

				// protect from flooding the serial
				if ((stopwatch.Elapsed >= minimumInterval))
				{
					stopwatch.Restart();

					SendMessageToSerial(predictedPoint.X.ToString() + "," + predictedPoint.Y.ToString() + "\r");
				}

			}

			//// Draw trajectory line from observed positions
			//for (int i = 1; i < trajectoryPoints.Count; i++)
			//{
			//	Cv2.Line(frame, trajectoryPoints[i - 1], trajectoryPoints[i], new Scalar(0, 255, 255), 2);
			//}

			hsv.Release();
			//objectMask1.Release();
			//objectMask1.Release();
			objectMask.Release();

			return src;

		}


		private void button2_Click(object sender, EventArgs e)
		{
			try
			{
				CaptureCamera();
			}
			catch (Exception ex)
			{
				Console.WriteLine("Exception: {0}", ex);
				lblError.Text = ex.Message;
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
					serialPort.Write(message);
					//serialPort.Close();
				}
			}
			catch (Exception e)
			{
				Console.WriteLine("Exception: {0}", e);
				lblError.Text = e.Message;
			}

		}

		private void button2_Click_2(object sender, EventArgs e)
		{
			trajectoryPoints.Clear();
		}

    private void vlcControl1_VlcLibDirectoryNeeded(object sender, Vlc.DotNet.Forms.VlcLibDirectoryNeededEventArgs e)
    {
			var vlcLibDirectory = new DirectoryInfo(@"C:\Program Files\VideoLAN\VLC");
			e.VlcLibDirectory = vlcLibDirectory;
		}

    private void vlcControl1_Log(object sender, Vlc.DotNet.Core.VlcMediaPlayerLogEventArgs e)
    {
			// You can now access the log message here
			// e.Level gives you the log level (Error, Warning, Info, etc.)
			// e.Message is the actual log string
			// e.Module is the component that logged the message

			//Console.WriteLine($"VLC Log [{e.Level}]: {e.Message} (Module: {e.Module})");

		}

    private void button1_Click(object sender, EventArgs e)
    {
			SendMessageToSerial("0,1920\r");
			Thread.Sleep(1000);
			SendMessageToSerial("1080,0\r");

		}
	}
}
