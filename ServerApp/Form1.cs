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
        private static System.Timers.Timer ScreenCapT;
        private static UdpClient clientSocket = new UdpClient(5901);
        private static string clientIp = "192.168.1.0";
        //private static IPEndPoint localhost = new IPEndPoint(IPAddress.Parse("127.0.0.1"), 5903);//when testing this should be 5902
        //private static UdpClient localclient = new UdpClient(localhost);
        public Form1()
        {
            InitializeComponent();
            this.FormClosed += new FormClosedEventHandler(Form1_FormClosed);
            inpThr.IsBackground = true;
            inpThr.Start();
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
                    try
                    {
                        IPAddress clientIp = IPAddress.Parse(ip);
                        clientEndPoint = new IPEndPoint(clientIp, 5902);
                    }
                    catch (Exception ex)
                    {
                        //anyone can mess with the ip.txt file
                        clientEndPoint = new IPEndPoint(IPAddress.Parse("192.168.1.0"), 5902);
                    }
                }
            }
            else
            {
                File.Create("ip.txt");
            }
            clientSocket.Connect(new IPEndPoint(IPAddress.Parse(clientIp), 5901) );
            //localTest(); //This works fine
            StartScreenCapture();
            //simulateKeybd.sendKey(Keys.RWin);
            //simulateKeybd.sendKey(Keys.D);
        }
        void Form1_FormClosed(object sender, FormClosedEventArgs e)
        {
            ScreenCapT.Stop();
            //inpThr.Suspend();

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
            IPEndPoint endPoint = new IPEndPoint(IPAddress.Any, listenPort);
            UdpClient receiveClient = new UdpClient(endPoint);
            //receiveClient.Connect(endPoint);
            //localclient.Connect(localhost);
            try
            {
                while (true)
                {
                    byte[] recData = receiveClient.Receive(ref endPoint);
                    //byte[] recData = localclient.Receive(ref endPoint);
                    for (int i = 0; i < recData.Length; ++i)
                    {

                    }
                }
            }
            catch (SocketException ex)
            {
                //i just recall the function
                //I will do something smarter soon
                restartThread(1);
            }
        }

        private void evalInput(byte[] data)
        {
            switch (data[0])
            {
                case RTP.RTP.keyboardInputDown:
                    simulateKeybd.sendKeyDown(data[1]);
                    break;
                case RTP.RTP.keyboardInputUp:
                    simulateKeybd.sendKeyUp(data[1]);
                    break;
                default:
                    if (RTP.RTP.mouseMove < data[0] && data[0] <= RTP.RTP.mouseMiddleClickUp)
                    {
                        simulateMouse.MouseEventNoMove(data[0]);
                    }
                    else
                    {
                        byte[] nr = new byte[2];
                        switch (data[0])
                        {
                            case RTP.RTP.mouseMove:
                                ushort posX, posY;
                                nr[0] = data[1];
                                nr[1] = data[2];
                                posX = BitConverter.ToUInt16(nr);
                                nr[0] = data[3];
                                nr[1] = data[4];
                                posY = BitConverter.ToUInt16(nr);
                                simulateMouse.MouseEventMove((int)posX, (int)posY);
                                break;
                            case RTP.RTP.mouseMWheel:
                                short dwData, dwFlags;
                                nr[0] = data[1];
                                nr[1] = data[2];
                                dwFlags = BitConverter.ToInt16(nr);
                                nr[0] = data[3];
                                nr[1] = data[4];
                                dwData = BitConverter.ToInt16(nr);
                                simulateMouse.MouseEventMWheel(dwFlags, dwData);
                                break;
                        }
                    }
                    break;
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
        private void localTest()
        {
            //all is fine
            /*Thread.Sleep(2000);
            byte[] packetClick = RTP.RTP.createMouseInputNoMove(RTP.RTP.mouseLeftClickDown | RTP.RTP.mouseLeftClickUp);
            localclient.Send(packetClick);
            Thread.Sleep(1500);
            byte[] packetRClick = RTP.RTP.createMouseInputNoMove(RTP.RTP.mouseRightClickDown | RTP.RTP.mouseRightClickUp);
            localclient.Send(packetRClick);
            Thread.Sleep(1500);
            byte[] packetMWheel = RTP.RTP.createMouseInputMWheel(RTP.RTP.mouseMWheelMove, 10);
            localclient.Send(packetMWheel);
            Thread.Sleep(1500);
            byte[] packetMove = RTP.RTP.createMouseInputMove(100, 555);
            localclient.Send(packetMove);*/
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
                ScreenCapT = new System.Timers.Timer(3333);
                ScreenCapT.Elapsed += SendScreen;
                ScreenCapT.AutoReset = true;
                ScreenCapT.Enabled = true;
                ScreenCaptureOn = true;
            }
        }
        private void SendScreen(Object source, System.Timers.ElapsedEventArgs e)
        {
            //simulateKeybd.sendKey(Keys.W);
            //simulateMouse.SimulateLeftClick();
            //System.Diagnostics.Stopwatch watch = System.Diagnostics.Stopwatch.StartNew();wwwwwwwwwwwwwwwwww
            Bitmap screenBmp = screenCapture.captureScreenWithCursor();
            //watch.Stop();
            //MessageBox.Show(watch.ElapsedMilliseconds.ToString());
            //screenBmp.Save("a.bmp");
            byte[] data = jpegCompression.convertBmpToByteArr(screenBmp);
            byte[] img = jpegCompression.notJpegEncoding(data);
            screenBmp.Dispose();
            GC.Collect();
            img = null;
            if (img != null)
            {
                byte[][] imgData = RTP.RTP.convertLongDataToRTPPackets(img, RTP.RTP.imageByte);
                int lg = imgData.Length;
                for (int i = 0; i < lg; ++i)
                {
                    bool a = sendPacket(imgData[i]);
                    if (a == false)
                    {
                        MessageBox.Show("ceva nu e ok");
                    }
                }
            }
        }
        private bool sendPacket(byte[] data)
        {
            try
            {
                clientSocket.Send(data, data.Length);
                return true;
            }
            catch (SocketException ex)
            {
                MessageBox.Show(ex.Message);
                //I will handle the exception in a different way, obviously
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
                captureGraphics.Dispose();
                //GC.Collect();
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

        public static Bitmap captureScreenWithCursorFast(int width = 1920, int height = 1080)
        {
            Bitmap captureBitmap = new Bitmap(width, height, PixelFormat.Format24bppRgb);
            Rectangle captureRectangle = new Rectangle(0, 0, width, height);
            Graphics captureGraphics = Graphics.FromImage(captureBitmap);
            //copying image from the screen
            captureGraphics.CopyFromScreen(captureRectangle.Left, captureRectangle.Top, 0, 0, captureRectangle.Size);
            captureGraphics.Dispose();
            int x, y;
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
                    Rectangle cursorRectangle = new Rectangle(x, y,
                       cursorBmp.Width, cursorBmp.Height);
                    Graphics finalGraphics = Graphics.FromImage(captureBitmap);
                    finalGraphics.DrawImage(cursorBmp, cursorRectangle);
                    finalGraphics.Flush();
                    Bitmap desktopBmpNew = new Bitmap(1920, 1080);
                    Graphics.FromImage(captureBitmap).DrawImage(desktopBmpNew, 0, 0, 1920, 1080);
                }
            }
            return captureBitmap;
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
        /// It uses C++ User32 functions to achieve such great feats of technology
        /// </summary>
        [Flags]
        public enum MouseEventFlags
        {
            LeftDown = 0x00000002,
            LeftUp = 0x00000004,
            MiddleDown = 0x00000020,
            MiddleUp = 0x00000040,
            Move = 0x00000001,
            Absolute = 0x00008000, //Because of the way i planned the client I don't think there is a need for absolute
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
        public static void MouseEventMWheel(int dwFlags, int dwData)
        {
            MousePoint position = GetCursorPosition();
            mouse_event(dwFlags, position.X, position.Y, dwData, 0);
        }
        public static void MouseEventMove(int posX, int posY)
        {
            mouse_event((int)MouseEventFlags.Move, posX, posY, 0, 0);
        }
        public static void MouseEventNoMove(byte value)
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
    internal class simulateKeybd
    {
        [Flags]
        public enum KeyEventFlags
        {
            KeyDown = 0x0000,
            ExtendedKey = 0x0001,
            KeyUp = 0x0002,
            Unicode = 0x0004,
        }

        [DllImport("User32.dll")]
        private static extern void keybd_event(byte bVk, byte bScan, int dwFlags, int dwExtraInfo);

        public static void sendKey(Keys key)
        {
            keybd_event((byte)key, 0, (int)KeyEventFlags.KeyDown, 0);//to be changed to sth correct
            keybd_event((byte)key, 0, (int)KeyEventFlags.KeyUp, 0);//to be changed to sth correct
        }

        public static void sendKeyDown(byte key)
        {
            keybd_event((byte)key, 0, (int)KeyEventFlags.KeyDown, 0);
        }
        
        public static void sendKeyUp(byte key)
        {
            keybd_event(key, 0, (int)KeyEventFlags.KeyUp, 0);
        }
    }
    internal class SendInputClass
    {
        [Flags]
        public enum KeyEventFlags
        {
            KeyDown = 0x0000,
            ExtendedKey = 0x0001,
            KeyUp = 0x0002,
            Unicode = 0x0004,
            Scancode = 0x0008
        }

        [Flags]
        private enum inputType
        {
            INPUT_MOUSE = 0,
            INPUT_KEYBOARD = 1,
            INPUT_HARDWARE = 2
        }

        [Flags]
        private enum MouseEventFlags
        {
            MOUSEEVENTF_MOVE = 0x0001,
            MOUSEEVENTF_LEFTDOWN = 0x0002,
            MOUSEEVENTF_LEFTUP = 0x0004,
            MOUSEEVENTF_RIGHTDOWN = 0x0008,
            MOUSEEVENTF_RIGHTUP = 0x0010,
            MOUSEEVENTF_MIDDLEDOWN = 0x0020,
            MOUSEEVENTF_MIDDLEUP = 0x0040,
            MOUSEEVENTF_XDOWN = 0x0080,
            MOUSEEVENTF_XUP = 0x0100,
            MOUSEEVENTF_WHEEL = 0x0800,
            MOUSEEVENTF_VIRTUALDESK = 0x4000,
            MOUSEEVENTF_ABSOLUTE = 0x8000
        }

        [StructLayout(LayoutKind.Sequential)]
        public struct MOUSEINPUT
        {
            public int dx;
            public int dy;
            public uint mouseData;
            public uint dwFlags;
            public uint time;
            public IntPtr dwExtraInfo;
        }

        [StructLayout(LayoutKind.Sequential)]
        public struct KEYBDINPUT
        {
            public ushort wVk;
            public ushort wScan;
            public uint dwFlags;
            public uint time;
            public IntPtr dwExtraInfo;
        }

        public struct HARDWAREINPUT
        {
            private int uMsg;
            private byte wParamL;
            private byte wParamH;
        }

        [StructLayout(LayoutKind.Explicit)]
        public struct InputUnion
        {
            [FieldOffset(0)] public MOUSEINPUT mi;
            [FieldOffset(0)] public KEYBDINPUT ki;
            [FieldOffset(0)] public HARDWAREINPUT hi;
        }

        [StructLayout(LayoutKind.Sequential)]
        public struct INPUT
        {
            public int type;
            public InputUnion u;
        }

        [DllImport("User32.dll", SetLastError = true)]
        private static extern uint SendInput(uint nInputs, ref INPUT[] pInputs, int cbSize);
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

        [DllImport("user32.dll")]
        private static extern IntPtr GetMessageExtraInfo();

        public static void sendClick()
        {
            MousePoint mp = new MousePoint();
            GetCursorPos(out mp);
            INPUT[] mouseInput = new INPUT[1];
            mouseInput[0].type = (int)inputType.INPUT_MOUSE;
            MOUSEINPUT click = new MOUSEINPUT
            {
                //dx = 100,
                //dy = 100,
                dwFlags = (uint)MouseEventFlags.MOUSEEVENTF_LEFTUP
            };
            mouseInput[0].u.mi = click;
            //mouseInput[0].mi.dx = mp.X;
            //mouseInput[0].mi.dy = mp.Y;
            var a = SendInput((uint)mouseInput.Length, ref mouseInput, Marshal.SizeOf(typeof(INPUT)));
        }
        public static void sendKbIn(Keys keyCode, bool type)
        {
            INPUT[] kbIn = new INPUT[2];
            kbIn[0].type = (int)inputType.INPUT_KEYBOARD;
            kbIn[1].type = (int)inputType.INPUT_KEYBOARD;
            kbIn[0].u = new InputUnion();
            kbIn[1].u = new InputUnion();
            kbIn[0].u.ki.wVk = (ushort)Keys.W;
            kbIn[1].u.ki.wVk = (ushort)Keys.W;
            kbIn[1].u.ki.dwFlags = (uint)KeyEventFlags.KeyUp;
            kbIn[0].u.ki.time = 0;
            kbIn[1].u.ki.time = 0;
            kbIn[0].u.ki.dwExtraInfo = IntPtr.Zero;
            kbIn[1].u.ki.dwExtraInfo = IntPtr.Zero;
            uint a = SendInput(2, ref kbIn, Marshal.SizeOf(typeof(INPUT)) );
            MessageBox.Show(a.ToString());
        }
    }
}
