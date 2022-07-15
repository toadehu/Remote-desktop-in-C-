using Jpeg;
using System;
using System.Collections.Generic;
using System.Drawing;
using System.Drawing.Imaging;
using System.IO;
using System.Net;
using System.Net.Sockets;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;
using System.Windows.Forms;
#nullable enable

namespace ServerApp
{
    public partial class Form1 : Form
    {
        private static int listenPort = 5902;
        private string ip = "192.168.1.0";
        private bool ScreenCaptureOn = false;
        private Bitmap? prevBmp = null;
        private KeyBoardHook keyboardhook;
        private static Thread inpThr = new Thread(new ThreadStart(receiveInputs));
        private static IPEndPoint clientEndPoint = new IPEndPoint(IPAddress.Any, 1);
        private static UdpClient clientSocket = new UdpClient(5902);
        public Form1()
        {
            InitializeComponent();
            //this.keyboardhook = new KeyBoardHook(true);
            //keyboardhook.HookAllKeys = true;
            //this.keyboardhook.KeyDown += new KeyEventHandler(keyboardhook_KeyDown);
            //this.keyboardhook.KeyUp += new KeyEventHandler(keyboardhook_KeyUp);
            //this.keyboardhook.Hook();
            if (File.Exists("ip.txt"))
            {
                string ipPath = Path.GetFullPath("ip.txt");
                using (StreamReader sr = new StreamReader(ipPath))
                {
                    ip = sr.ReadToEnd();
                    sr.Close();
                    textBoxIp.Text = ip;
                }
            }
            StartScreenCapture();
            //clientEndPoint = new IPEndPoint(IPAddress.Parse(ip), 5902);
        }
        private static void restartThread(int tId)
        {
            switch (tId)
            {
                case 1:
                    inpThr.Interrupt();
                    inpThr = new Thread(new ThreadStart(receiveInputs));
                    break;
            }
        }
        private static void receiveInputs()
        {
            IPEndPoint endPoint = new IPEndPoint(IPAddress.Any, 5902);
            UdpClient receiveClient = new UdpClient(5902);
            try
            {
                while (true)
                {
                    Byte[] recData = receiveClient.Receive(ref endPoint);
                    if (recData[0] == RTP.RTP.keyboardInputDown)
                    {
                        SendInputClass.sendKbIn((Keys)recData[1], true);
                        SendInputClass.sendKbIn((Keys)recData[1], false);
                    }
                    if (recData[0] == RTP.RTP.keyboardInputUp)
                    {
                        SendInputClass.sendKbIn((Keys)recData[1], false);
                    }
                }
            }
            catch (SocketException ex)
            {
                //i just recall the function
                restartThread(1);
            }
        }
        private void ServerLoad(object sender, EventArgs e)
        {
            try
            {
                // Initialise the socket
                Socket serverSocket = new Socket(AddressFamily.InterNetwork, SocketType.Dgram, ProtocolType.Udp);

                // Initialise the IPEndPoint for the server and listen on port 5902
                IPEndPoint server = new IPEndPoint(IPAddress.Any, 5902);

                // Associate the socket with this IP address and port
                serverSocket.Bind(server);

                // Initialise the IPEndPoint for the clients
                IPEndPoint clients = new IPEndPoint(IPAddress.Any, 0);

                // Initialise the EndPoint for the clients
                EndPoint epSender = (EndPoint)clients;

                // Start listening for incoming data
                //serverSocket.BeginReceiveFrom(this.dataStream, 0, this.dataStream.Length, SocketFlags.None, ref epSender, new AsyncCallback(ReceiveData), epSender);
            }
            catch (Exception ex)
            {
                MessageBox.Show("Load Error: " + ex.Message, "UDP Server", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }
        private void InitializeTimer()
        {
            // Call this procedure when the application starts.  
            // Set to 1 second.  
            Timer1.Interval = 3400;
            Timer1.Tick += new EventHandler(Timer1_Tick);

            // Enable timer.  
            Timer1.Enabled = true;
        }
        private void Timer1_Tick(object Sender, EventArgs e)
        {
            //SendScreen();
            //testRTP();
        }
        private void StartScreenCapture()
        {
            if (ScreenCaptureOn == false)
            {
                System.Timers.Timer ScreenCapT = new System.Timers.Timer(3333);
                ScreenCapT.Elapsed += SendScreen;
                ScreenCapT.AutoReset = true;
                ScreenCapT.Enabled = true;
                ScreenCaptureOn = true;
            }
        }
        private void SendScreen(Object source, System.Timers.ElapsedEventArgs e)
        {
            Bitmap screenBmp = screenCapture.captureScreenWithCursor();
            System.Diagnostics.Stopwatch watch = System.Diagnostics.Stopwatch.StartNew();
            byte[] test = jpegCompression.convertImage(jpegCompression.convertBmpToByteArr(screenBmp));
            //byte[] test2 = jpegCompression.convertImage(jpegCompression.convertBmpToByteArr(screenBmp));
            watch.Stop();
            //appendToFile(test);
            MessageBox.Show(watch.ElapsedMilliseconds.ToString());
            //MessageBox.Show(test.Length.ToString());
            screenBmp.Dispose();
            GC.Collect();
            //if (JPEGImg != null)
            //{
                //MessageBox.Show(JPEGImg.Length.ToString());
            //}
            //byte[][] imgData = RTP.RTP.convertLongDataToRTPPackets(JPEGImg, 0x02);
            /*for (int i = 0; i < imgData.Length; ++i)
            {
                //sendPacket(imgData[i]);
            }*/
        }
        private bool sendPacket(byte[] data)
        {
            try
            {
                clientSocket.Send(data, data.Length, clientEndPoint);
                return true;
            }
            catch (SocketException ex)
            {
                return false;
            }
        }
        private void testRTP()
        {
            /*string test;
            using (StreamReader sr = new StreamReader(@"C: \Users\razva\Desktop\ServerApp\lorem.txt"))
            {
                test = sr.ReadToEnd();
            }
            byte[][] packetsToSend = RTP.RTP.convertStringDataToRTPPackets(test);

            UdpClient sender = new UdpClient();

            IPEndPoint testEndPoint = new IPEndPoint(IPAddress.Parse("127.0.0.1"), 8000);

            for (int i = 0; i < packetsToSend.Length; ++i)
            {
                sender.Send(packetsToSend[i], packetsToSend[i].Length, testEndPoint);
                //appendToFile(bytes);
                //MessageBox.Show(Encoding.Default.GetString(bytes));
            }*/
        }
        private async void appendToFile(byte[] bytes)
        {
            FileInfo fi = new FileInfo(@"testCon.txt");
            if (!fi.Exists)
            {
                using (StreamWriter sw = fi.CreateText())
                {
                    sw.WriteLine("Beginning of file");
                    sw.Close();
                }
            }
            using (StreamWriter sw2 = fi.AppendText())
            {
                sw2.Write(bytes.ToString());
                //sw2.Write($" {Encoding.ASCII.GetString(bytes, 0, bytes.Length)}");
                sw2.Close();
            }//*/
        }
        private void Start_Click(object sender, EventArgs e)
        {
            ///
            ///Establish the connection. Start sending and recieving information
            ///
            StartScreenCapture();

            UdpClient listener = new UdpClient(listenPort);

            IPEndPoint groupEP = new IPEndPoint(IPAddress.Any, 8000);

            IPEndPoint client = new IPEndPoint(IPAddress.Parse("127.0.0.1"), 8000);

            UdpClient test_sender = new UdpClient();


            try
            {
                /*byte[][] message = new byte[1024][];
                try
                {
                    using (StreamReader sr = new StreamReader(@"C:\Users\razva\Desktop\ServerApp\lorem.txt"))
                    {
                        string lorem_text = sr.ReadToEnd();
                        message = RTP.RTP.convertStringDataToRTPPackets(lorem_text);
                        for (int i = 0; i < 1024; ++i)
                        {
                            test_sender.Send(message[i], message[i].Length, client);
                        }
                    }
                }
                catch (Exception ex)
                {
                    MessageBox.Show(ex.ToString());
                }*/
            }
            catch (SocketException ex)
            {
                MessageBox.Show(ex.ToString());
            }
            finally
            {
                listener.Close();
            }
        }
        private void textBoxIp_TextChanged(object sender, EventArgs e)
        {
            ip = (string)textBoxIp.Text;
            string ipPath = Path.GetFullPath("ip.txt");
            using (StreamWriter sw = new StreamWriter(ipPath, false, Encoding.UTF8, 131072))
            {
                sw.Write(ip);
                sw.Close();
            }
        }
        private void keyboardhook_KeyDown(object sender, KeyEventArgs e)
        {
            SendInputClass.sendKbIn(e.KeyCode, true);
            e.Handled = true;
        }
        private void keyboardhook_KeyUp(object sender, KeyEventArgs e)
        {
            SendInputClass.sendKbIn(e.KeyCode, false);
            e.Handled = true;
        }
    }
    internal class screenCapture
    {
        private const Int32 CURSOR_SHOWING = 0x00000001;
        private const Int32 CURSOR_SUPPRESSED = 0x00000002;
        private struct POINT
        {
            public Int32 x;
            public Int32 y;
        }
        private struct CURSORINFO
        {
            public int cbSize;
            public int flags;
            public IntPtr hCursor;
            public POINT ptScreenPos;
        }
        private struct ICONINFO
        {
            public bool fIcon;
            public Int32 xHotspot;
            public Int32 yHotspot;
            public Bitmap hbmMask;
            public Bitmap hbmColor;
        }

        [DllImport("User32.dll")]
        [return: MarshalAs(UnmanagedType.Bool)]
        private static extern bool GetCursorInfo(ref CURSORINFO pci);

        [DllImport("User32.dll")]
        private static extern IntPtr CopyIcon(IntPtr hIcon);

        /*[DllImport("User32.dll")]
        [return: MarshalAs(UnmanagedType.Bool)]
        private static extern bool GetIconInfo(IntPtr hIcon, out ICONINFO piconinfo);*/

        private static Bitmap captureCursor(ref int x, ref int y)
        {
            Bitmap cursorBmp;
            IntPtr hicon;
            CURSORINFO ci = new CURSORINFO();
            ICONINFO icInfo;
            ci.cbSize = Marshal.SizeOf(ci);
            if (GetCursorInfo(ref ci))
            {
                if (ci.flags == CURSOR_SHOWING)
                {
                    hicon = CopyIcon(ci.hCursor);
                    x = ci.ptScreenPos.x;
                    y = ci.ptScreenPos.y;
                    Icon ic = Icon.FromHandle(hicon);
                    cursorBmp = ic.ToBitmap();
                    return cursorBmp;
                }
            }
            return null;
        }
        private static ImageCodecInfo GetEncoder(ImageFormat format)
        {
            ImageCodecInfo[] codecs = ImageCodecInfo.GetImageEncoders();
            foreach (ImageCodecInfo codec in codecs)
            {
                if (codec.FormatID == format.Guid)
                {
                    return codec;
                }
            }
            return null;
        }
        private static void writeToTxt(Bitmap bmp)
        {
            //await File.WriteAllTextAsync(@"C:\Users\razva\Desktop\ServerApp\test.txt", null);
            Char[] bmpArray = new Char[2073600 * 3];
            try
            {
                for (int i = 0; i < 1920; ++i)
                {
                    for (int j = 0; j < 1080; ++j)
                    {
                        var a = bmp.GetPixel(i, j);
                        Byte r, g, b;
                        r = a.R;
                        g = a.G;
                        b = a.B;
                        bmpArray[i * 1080 + j] = ((char)r);
                        bmpArray[i * 1080 + j + 2073600] = ((char)g);
                        bmpArray[i * 1080 + j + 2073600 * 2] = ((char)b);
                    }
                }
                ImageCodecInfo jpgEncoder = GetEncoder(ImageFormat.Jpeg);
                System.Drawing.Imaging.Encoder myEncoder =
                    System.Drawing.Imaging.Encoder.Quality;
                EncoderParameters myEncoderParameters = new EncoderParameters(1);
                EncoderParameter myEncoderParameter = new EncoderParameter(myEncoder, 50L);
                myEncoderParameters.Param[0] = myEncoderParameter;
                bmp.Save(@"C:\ScreenCapture\test0.jpg", jpgEncoder, myEncoderParameters);
                bmp.Save(@"C:\ScreenCapture\test.jpg", System.Drawing.Imaging.ImageFormat.Jpeg);
                bmp.Save(@"C:\ScreenCapture\test.tif", System.Drawing.Imaging.ImageFormat.Tiff);
                string outp = new string(bmpArray);
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }

            //fout.Start();
            //str = null;
            //MessageBox.Show(rArray[1].ToString());
        }


        /// <summary>
        /// The screen is saved using 24bppRgb. This is VERY important to the functionality of the program
        /// </summary>
        /// <returns>The screen</returns>
        public static Bitmap captureMyScreen()
        {
            try
            {
                Bitmap captureBitmap = new Bitmap(1920, 1080, PixelFormat.Format24bppRgb);
                //creating a rectangle object which will capture our current screen
                Rectangle captureRectangle = Screen.AllScreens[0].Bounds;
                //creating a new graphics object
                Graphics captureGraphics = Graphics.FromImage(captureBitmap);
                //copying image from the screen
                captureGraphics.CopyFromScreen(captureRectangle.Left, captureRectangle.Top, 0, 0, captureRectangle.Size);
                writeToTxt(captureBitmap);
                //MessageBox.Show(cnt.ToString());
                captureGraphics.Dispose();
                //File.Delete(@"C:\temp.jpg");
                //captureBitmap.Save(@"C:\ScreenCapture\temp.jpg", System.Drawing.Imaging.ImageFormat.Jpeg);
                GC.Collect();
                return captureBitmap;
            }

            catch (Exception ex)
            {
                return null;
            }
        }
        public static Bitmap captureScreenWithCursor()
        {
            int cursorX = 0;
            int cursorY = 0;
            Bitmap desktopBmp = captureMyScreen();
            Bitmap cursorBmp = captureCursor(ref cursorX, ref cursorY);
            Rectangle cursorRectangle;
            Graphics finalGraphics;
            if (desktopBmp != null)
            {
                if (cursorBmp != null)
                {
                    cursorRectangle = new Rectangle(cursorX, cursorY,
                        cursorBmp.Width, cursorBmp.Height);
                    finalGraphics = Graphics.FromImage(desktopBmp);
                    finalGraphics.DrawImage(cursorBmp, cursorRectangle);
                    finalGraphics.Flush();
                    //desktopBmp.Save(@"C:\ScreenCapture\tempWithCursor.jpg", System.Drawing.Imaging.ImageFormat.Jpeg);
                    if (desktopBmp.Width != 1920)
                    {
                        Bitmap desktopBmpNew = new Bitmap(1920, 1080);
                        Graphics.FromImage(desktopBmp).DrawImage(desktopBmpNew, 0, 0, 1920, 1080);
                        return desktopBmpNew;
                    }
                    return desktopBmp;
                }
                if (desktopBmp.Width != 1920)
                {
                    Bitmap desktopBmpNew = new Bitmap(1920, 1080);
                    Graphics.FromImage(desktopBmp).DrawImage(desktopBmpNew, 0, 0, 1920, 1080);
                    return desktopBmpNew;
                }
                return desktopBmp;
            }
            return null;

        }
    }
    internal class KeyBoardHook : IDisposable
    {
        #region Api

        private struct keyboardHookStruct
        {
            public int vkCode;
            public int scanCode;
            public int flags;
            public int time;
            public int dwExtraInfo;
        }

        private const int WH_KEYBOARD_LL = 13;
        private const int WM_KEYDOWN = 0x100;
        private const int WM_KEYUP = 0x101;
        private const int WM_SYSKEYDOWN = 0x104;
        private const int WM_SYSKEYUP = 0x105;

        /// <summary>
        /// Sets the windows hook, do the desired event, one of hInstance or threadId must be non-null
        /// </summary>
        /// <param name="idHook">The id of the event you want to hook</param>
        /// <param name="callback">The callback.</param>
        /// <param name="hInstance">The handle you want to attach the event to, can be null</param>
        /// <param name="threadId">The thread you want to attach the event to, can be null</param>
        /// <returns>a handle to the desired hook</returns>
        [DllImport("user32.dll")]
        private static extern IntPtr SetWindowsHookEx(int idHook, keyboardHookCallback callback, IntPtr hInstance, uint threadId);

        /// <summary>
        /// Unhooks the windows hook.
        /// </summary>
        /// <param name="hInstance">The hook handle that was returned from SetWindowsHookEx</param>
        /// <returns>True if successful, false otherwise</returns>
        [DllImport("user32.dll")]
        private static extern bool UnhookWindowsHookEx(IntPtr hInstance);

        /// <summary>
        /// Calls the next hook.
        /// </summary>
        /// <param name="idHook">The hook id</param>
        /// <param name="nCode">The hook code</param>
        /// <param name="wParam">The wparam.</param>
        /// <param name="lParam">The lparam.</param>
        /// <returns></returns>
        [DllImport("user32.dll", SetLastError = true)]
        private static extern int CallNextHookEx(IntPtr idHook, int nCode, int wParam, ref keyboardHookStruct lParam);

        /// <summary>
        /// Loads the library.
        /// </summary>
        /// <param name="lpFileName">Name of the library</param>
        /// <returns>A handle to the library</returns>
        [DllImport("kernel32.dll")]
        private static extern IntPtr LoadLibrary(string lpFileName);

        #endregion

        #region Fields & Properties

        /// <summary>
        /// defines the callback type for the hook
        /// </summary>
        private delegate int keyboardHookCallback(int code, int wParam, ref keyboardHookStruct lParam);

        /// <summary>
        /// The collections of keys to watch for
        /// </summary>
        private List<Keys> hookedKeys = new List<Keys>();

        /// <summary>
        /// Handle to the hook, need this to unhook and call the next hook
        /// </summary>
        private IntPtr hHook = IntPtr.Zero;

        /// <summary>
        /// The callback method reference
        /// </summary>
        private keyboardHookCallback khp;

        /// <summary>
        /// Key press supression
        /// </summary>
        private bool supressKeyPress = false;

        /// <summary>
        /// Gets or sets whether or not to hook all keys.
        /// </summary>
        public bool HookAllKeys { get; set; }

        /// <summary>
        /// is disposed or not
        /// </summary>
        private bool disposed;

        #endregion

        #region Events

        /// <summary>
        /// Occurs when one of the hooked keys is pressed
        /// </summary>
        public event KeyEventHandler KeyDown;
        /// <summary>
        /// Occurs when one of the hooked keys is released
        /// </summary>
        public event KeyEventHandler KeyUp;

        #endregion

        #region Constructor

        public KeyBoardHook(bool supressKeyPress)
        {
            this.disposed = false;
            this.supressKeyPress = supressKeyPress;
            this.HookAllKeys = false;
            khp = new keyboardHookCallback(OnHookCallback);
        }

        ~KeyBoardHook()
        {
            Dispose();
        }

        #endregion

        #region Public Methods

        /// <summary>
        /// Add hooked key
        /// </summary>
        public void AddHookedKey(Keys key)
        {
            ThrowIfDisposed();

            if (!hookedKeys.Contains(key))
            {
                hookedKeys.Add(key);
            }
        }

        /// <summary>
        /// Remove hooked key
        /// </summary>
        public void RemoveHookedKey(Keys key)
        {
            ThrowIfDisposed();

            if (hookedKeys.Contains(key))
            {
                hookedKeys.Remove(key);
            }
        }

        /// <summary>
        /// Hook
        /// </summary>
        public void Hook()
        {
            ThrowIfDisposed();

            IntPtr hInstance = LoadLibrary("User32");
            hHook = SetWindowsHookEx(WH_KEYBOARD_LL, khp, /*IntPtr.Zero*/ hInstance, 0);
        }

        /// <summary>
        /// Unhook
        /// </summary>
        public void Unhook()
        {
            ThrowIfDisposed();

            UnhookWindowsHookEx(hHook);
        }

        /// <summary>
        /// The callback for the keyboard hook
        /// </summary>
        private int OnHookCallback(int code, int wParam, ref keyboardHookStruct lParam)
        {
            if (code >= 0)
            {
                Keys key = (Keys)lParam.vkCode;

                if (HookAllKeys || hookedKeys.Contains(key))
                {
                    KeyEventArgs kea = new KeyEventArgs(key);

                    if ((wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN) && (KeyDown != null))
                    {
                        OnKeyDown(kea);
                    }
                    else if ((wParam == WM_KEYUP || wParam == WM_SYSKEYUP) && (KeyUp != null))
                    {
                        OnKeyUp(kea);
                    }

                    if (kea.Handled && supressKeyPress)
                    {
                        return 1;
                    }
                }
            }

            int ret = CallNextHookEx(hHook, code, wParam, ref lParam);

            int error = Marshal.GetLastWin32Error();

            return ret;
        }

        /// <summary>
        /// Raises the KeyDown event.
        /// </summary>
        private void OnKeyDown(KeyEventArgs e)
        {
            if (KeyDown != null)
            {
                KeyDown(this, e);
            }
        }

        /// <summary>
        /// Raises the KeyUp event.
        /// </summary>
        private void OnKeyUp(KeyEventArgs e)
        {
            if (KeyUp != null)
            {
                KeyUp(this, e);
            }
        }

        #endregion

        #region Disposable

        public void Dispose()
        {
            if (!this.disposed)
            {
                this.Unhook();
                this.khp = null;
                this.hHook = IntPtr.Zero;
                this.hookedKeys.Clear();
                this.hookedKeys = null;
                this.disposed = true;
            }
        }

        private void ThrowIfDisposed()
        {
            if (disposed)
            {
                throw new ObjectDisposedException(GetType().FullName);
            }
        }

        #endregion
    }
    internal class simulateMouse
    {
        /// <summary>
        /// Simulates the functionalities of a mouse
        /// It uses C++ User32 functions to achieve such greats feats of technology
        /// </summary>
        [Flags]
        public enum MouseEventFlags
        {
            LeftDown = 0x00000002,
            LeftUp = 0x00000004,
            MiddleDown = 0x00000020,
            MiddleUp = 0x00000040,
            Move = 0x00000001,
            Absolute = 0x00008000,
            RightDown = 0x00000008,
            RightUp = 0x00000010
        }

        [DllImport("User32.dll")]
        [return: MarshalAs(UnmanagedType.Bool)]
        private static extern bool SetCursorPos(int x, int y);

        [DllImport("user32.dll")]
        [return: MarshalAs(UnmanagedType.Bool)]
        private static extern bool GetCursorPos(out MousePoint lpMousePoint);

        [DllImport("user32.dll")]
        private static extern void mouse_event(int dwFlags, int dx, int dy, int dwData, int dwExtraInfo);

        [StructLayout(LayoutKind.Sequential)]
        public struct MousePoint
        {
            public int X;
            public int Y;

            public MousePoint(int x, int y)
            {
                X = x;
                Y = y;
            }
        }
        public static void SetCursorPosition(MousePoint point)
        {
            SetCursorPos(point.X, point.Y);
        }
        public static MousePoint GetCursorPosition()
        {
            MousePoint currentMousePoint;
            var gotPoint = GetCursorPos(out currentMousePoint);
            if (!gotPoint) { currentMousePoint = new MousePoint(0, 0); }
            return currentMousePoint;
        }
        public static void receiveMouseInput(byte[] mouseInput)
        {
            if (mouseInput.Length == 1)
            {
                MousePoint curMousePos = GetCursorPosition();
                mouse_event((int)mouseInput[0], curMousePos.X, curMousePos.Y, 0, 0);
                return;
            }
            byte[] xPos = new byte[2];
            xPos[0] = mouseInput[1];
            byte[] yPos = new byte[2];
            SetCursorPos(BitConverter.ToUInt16(xPos), BitConverter.ToUInt16(yPos));
        }
        public static void MouseEvent(MouseEventFlags value)
        {
            MousePoint position = GetCursorPosition();

            mouse_event
                ((int)value,
                 position.X,
                 position.Y,
                 0,
                 0);
        }
        public static void SimulateLeftClick()
        {
            ///
            ///MERGE
            ///
            MousePoint CurPos = GetCursorPosition();
            mouse_event((int)MouseEventFlags.LeftDown | (int)MouseEventFlags.LeftUp, CurPos.X, CurPos.Y, 0, 0);
        }
        public static void SimulateLeftClickDown()
        {
            ///
            ///MERGE
            ///
            MousePoint CurPos = GetCursorPosition();
            mouse_event((int)MouseEventFlags.LeftDown, CurPos.X, CurPos.Y, 0, 0);
        }
        public static void SimulateLeftClickUp()
        {
            ///
            ///MERGE
            ///
            MousePoint CurPos = GetCursorPosition();
            mouse_event((int)MouseEventFlags.LeftUp, CurPos.X, CurPos.Y, 0, 0);
        }
        public static void SimulateRightClickDown()
        {
            ///
            ///MERGE
            ///
            MousePoint CurPos = GetCursorPosition();
            mouse_event((int)MouseEventFlags.RightDown, CurPos.X, CurPos.Y, 0, 0);
        }
        public static void SimulateRightClickUp()
        {
            ///
            ///MERGE
            ///
            MousePoint CurPos = GetCursorPosition();
            mouse_event((int)MouseEventFlags.RightUp, CurPos.X, CurPos.Y, 0, 0);
        }
        public static void SimulateMiddleClickDown()
        {
            ///
            ///MERGE
            ///
            MousePoint CurPos = GetCursorPosition();
            mouse_event((int)MouseEventFlags.MiddleDown, CurPos.X, CurPos.Y, 0, 0);
        }
        public static void SimulateMiddleClickUp()
        {
            ///
            ///MERGE
            ///
            MousePoint CurPos = GetCursorPosition();
            mouse_event((int)MouseEventFlags.MiddleUp, CurPos.X, CurPos.Y, 0, 0);
        }

    }
    internal class SendInputClass
    {
        [Flags]
        public enum MouseEventFlags
        {
            LeftDown = 0x00000002,
            LeftUp = 0x00000004,
            MiddleDown = 0x00000020,
            MiddleUp = 0x00000040,
            Move = 0x00000001,
            Absolute = 0x00008000,
            RightDown = 0x00000008,
            RightUp = 0x00000010
        }

        private struct MOUSEINPUT
        {
            public int dx;
            public int dy;
            public int mouseData;
            public int dwFlags;
            public int time;
            public ulong dwExtraInfo;
        }

        [StructLayout(LayoutKind.Sequential)]
        private struct KEYBDINPUT
        {
            public short wVk;
            public short wScan;
            public int dwFlags;
            public int time;
            public ulong dwExtraInfo;
        }

        private struct HARDWAREINPUT
        {
            private int uMsg;
            private byte wParamL;
            private byte wParamH;
        }
        [StructLayout(LayoutKind.Explicit)]
        private struct INPUT
        {
            [FieldOffset(0)]
            public int type;
            [FieldOffset(4)]
            public MOUSEINPUT mi;
            [FieldOffset(4)]
            public KEYBDINPUT ki;
            [FieldOffset(4)]
            public HARDWAREINPUT hi;
        }

        [DllImport("User32.dll")]
        private static extern uint SendInput(uint cInputs, INPUT[] pInputs, int cbSize);
        private struct MousePoint
        {
            public int X;
            public int Y;

            public MousePoint(int x, int y)
            {
                X = x;
                Y = y;
            }
        }
        [DllImport("user32.dll")]
        [return: MarshalAs(UnmanagedType.Bool)]
        private static extern bool GetCursorPos(out MousePoint lpMousePoint);
        public static void sendClick()
        {
            MousePoint CurPos;
            GetCursorPos(out CurPos);
            MOUSEINPUT click = new MOUSEINPUT();
            click.mouseData = 0;
            click.dx = CurPos.X;
            click.dy = CurPos.Y;
            click.dwFlags = (int)MouseEventFlags.LeftDown | (int)MouseEventFlags.LeftUp;
            click.dwExtraInfo = 0;
            INPUT[] pInput = new INPUT[1];
            pInput[0].type = 0;
            pInput[0].mi = click;
            SendInput((uint)pInput.Length, pInput, 6 * sizeof(int) + sizeof(uint));
        }
        public static void sendKbIn(Keys keyCode, bool type)
        {
            short codeVal = ((short)keyCode);
            //MessageBox.Show( codeVal.ToString() );
            KEYBDINPUT KbIn = new KEYBDINPUT();
            KbIn.wVk = 0;
            KbIn.wScan = codeVal;
            KbIn.dwFlags = 0;// x0008;
            if (type == false)//keyUp
            {
                //KbIn.dwFlags = 0x0002;
            }
            KbIn.dwExtraInfo = 0;
            INPUT[] pInput = new INPUT[1];
            pInput[0].type = 1;
            pInput[0].ki = KbIn;
            uint succ = SendInput((uint)pInput.Length, pInput, 2 * sizeof(int) +
                2 * sizeof(short) + sizeof(byte) + sizeof(ulong));
            MessageBox.Show(succ.ToString() + type.ToString());
        }
    }
}
