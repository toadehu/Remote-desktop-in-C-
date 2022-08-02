using System;
using System.Drawing;
using System.IO;
using System.Windows.Forms;
using Microsoft.Win32;

namespace ServerApp
{
    public partial class Form2 : Form
    {
        public static Aes aes = Aes.Create();
        public Form2()
        {
            InitializeComponent();
            //createCursor();
        }

        void createCursor()
        {
            PictureBox pictureBox = new PictureBox();
            Image cursor = Image.FromFile("cursor.png");

            Size bmpSize = new Size(cursor.Width, cursor.Height);
            this.Size = bmpSize;
            pictureBox.ClientSize = bmpSize;

            pictureBox.Image = cursor;
            pictureBox.Dock = DockStyle.Fill;
            this.Controls.Add(pictureBox);
            this.FormBorderStyle = FormBorderStyle.None;
        }

        private void buttonUpdPass_Click(object sender, EventArgs e)
        {
            string passOld = textBoxConf.Text.ToString();
            byte[] encArr = encryptPass(passOld);
            Jpeg.jpegCompression.writeByteArr("passOld.txt", encArr);

            RegistryKey passAuth = Registry.CurrentUser.OpenSubKey("CocoReg");

            if (passAuth == null)
            {
                MessageBox.Show("Something went horribly wrong.\nPlease restart the program.\nThe password will be reset.");
                return;
            }

            object oldPass = passAuth.GetValue("Password");
            if (oldPass.GetType() != typeof(string))
            {
                Registry.CurrentUser.DeleteSubKey("CocoReg");
                MessageBox.Show("Something went horribly wrong.\nPlease restart the program.\nThe password will be reset.");
                return;

            }

            byte[] passInReg = System.Text.Encoding.Default.GetBytes(oldPass.ToString());

            if (encArr.Length != passInReg.Length)
            {
                textBoxConf.Clear();
                textBoxNewPass.Clear();
                textBoxPassConf.Clear();
                MessageBox.Show("Old Password is wrong.\nTry again 1 " + encArr.Length.ToString() + " " + passInReg.Length.ToString());
                return;
            }

            bool ok = true;

            for (int i = 0; i < passInReg.Length; ++i)
            {
                if (encArr[i] != passInReg[i])
                {
                    MessageBox.Show(i.ToString() + " " + passInReg.Length.ToString());
                    ok = false;
                    break;
                }
            }

            if (ok == false)
            {
                textBoxConf.Clear();
                textBoxNewPass.Clear();
                textBoxPassConf.Clear();
                MessageBox.Show("Old Password is wrong.\nTry again 2");
                return;
            }

            string passNew = textBoxNewPass.Text.ToString();
            string passNewConf = textBoxPassConf.Text.ToString();

            //check if passwords are the same
            StringComparer comparer = StringComparer.InvariantCulture;
            if (comparer.Compare(passNew, passNewConf) != 0)
            {
                textBoxConf.Clear();
                textBoxNewPass.Clear();
                textBoxPassConf.Clear();
                MessageBox.Show("Passwords do not match.\nTry again");
                return;
            }

            //all should be ok now

            byte[] newAuth = encryptPass(passNew);

            passAuth.SetValue("Password", newAuth);
        }

        /// <summary>
        /// encryps a plaintext password using an algorithm
        /// </summary>
        /// <param name="plainText">The plain text password</param>
        /// <returns>A byte array with the encrypted password</returns>
        /// <exception cref="ArgumentNullException"></exception>
        public static byte[] encryptPass(string plainText)
        {
            // Check arguments.
            if (plainText == null || plainText.Length <= 0)
            {
                throw new ArgumentNullException("plainText");
            }
            byte[] encrypted = new byte[plainText.Length * 4 - 3];


            for (int i = 0; i <  encrypted.Length; i+=4)
            {
                byte v1 = (byte)plainText[i / 4], v2 = (byte)plainText[i / 4 + 1];
                encrypted[i] = v1;
                encrypted[i + 2] = (byte)(v1 ^ v2);
                encrypted[i + 1] = (byte)(encrypted[i] | encrypted[i+2]); 
                encrypted[i + 3] = (byte)(encrypted[i+4] | encrypted[i+2]); 
            }

            int p1 = 1234567891, p2 = 666013, p3 = 69494977, p4 = 99044167;

            //int 

            // Create an Aes object
            // with the specified key and IV.


            // Return the encrypted bytes from the memory stream.
            return encrypted;
        }

        private void Form2_Load(object sender, EventArgs e)
        {

        }
    }
}
