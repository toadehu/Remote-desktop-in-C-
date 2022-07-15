using System;
using System.Text;
using System.Windows.Forms;

namespace RTP
{
    internal class RTP
    {
        public const byte imageByte = 0x00;
        public const byte mouseNewPosition = 0x01;
        public const byte mouseNotNewPosition = 0x03;
        public const byte mouseLeftClickDown = 0x02;
        public const byte mouseLeftClickUp = 0x04;
        public const byte mouseRightClickDown = 0x08;
        public const byte mouseRightClickUp = 0x10;
        public const byte mouseMiddleClickDown = 0x20;
        public const byte mouseMiddleClickUp = 0x40;
        public const byte keyboardInputDown = 0xFF;
        public const byte keyboardInputUp = 0xFE;
        public const byte clientIpHeader = 0x11;
        internal static string getIp(byte[] packet)
        {
            StringBuilder sb = new StringBuilder();
            for (int i = 1; i < packet.Length; ++i)
            {
                sb.Append(packet[i]);
                if (i < packet.Length-1)
                {
                    sb.Append('.');
                }
            }
            return sb.ToString();
        }
        internal static byte[] createMouseInput(bool type, byte mouseInputByte,
            int X = -1, int Y = -1,
            int serverWidth = 2560, int serverHeight = 1440,
            int clientWidth = 1920, int clientHeight = 1080)
        ///
        /// if type = false it is for position, in this case both X and Y are required
        /// if type = true it is for everything else, X and Y are to be ignored
        /// serverWidth, serverHeight, clientWidht, clientHeight are the resolutions of the screen of the server and client respectively
        ///
        {
            if (type == true)
            {
                byte[] returnPacket = new byte[1];
                returnPacket[0] = mouseInputByte;
                return returnPacket;
            }
            X = (int)(X * clientWidth / serverWidth);
            Y = (int)(Y * clientHeight / serverHeight);
            byte[] inputPacket = new byte[5];
            inputPacket[0] = imageByte;
            byte[] numberByte = new byte[2];
            numberByte = BitConverter.GetBytes(X);
            inputPacket[1] = numberByte[0];
            inputPacket[2] = numberByte[1];
            numberByte = BitConverter.GetBytes(Y);
            inputPacket[3] = numberByte[0];
            inputPacket[4] = numberByte[1];
            return inputPacket;
        }
        internal static byte[] sendKbIn(System.Windows.Forms.Keys keyCode)
        {
            byte[] retBytes = new byte[2];
            retBytes[0] = keyboardInputDown;
            retBytes[1] = (byte)keyCode;
            return retBytes;
        }
        internal static byte[] receiveMouseInput(byte[] mouseInput)
        {
            switch (mouseInput[0])
            {
                case mouseNewPosition:
                    byte[] returnBytes = new byte[4];
                    returnBytes[0] = mouseInput[0];
                    returnBytes[1] = mouseInput[1];
                    returnBytes[2] = mouseInput[2];
                    returnBytes[3] = mouseInput[3];
                    return returnBytes;
                default:
                    byte[] returnByte = new byte[1];
                    returnByte[0] = mouseInput[0];
                    return returnByte;
            }
        }
        internal static byte[][] convertLongDataToRTPPackets(byte[] data, byte type)//the function return
        {
            //this is the size of a packet, considering that the code will always try to send 1024 packets
            //this should be sufficient, considering the high compression ratio of jpeg
            int sizeOfPacket = (data.Length) / 1024;
            int remainingBytes = data.Length - 1023 * sizeOfPacket;
            //initialization of array of packets
            byte[][] packets = new byte[1024][];
            //*NOT USED NOW* first 2 bytes are the size of the padding at the end of the byte array
            // the next 2 are the order in the sequence, the first 6 bits are set to a fixed random variable,
            // the next 10 are the actual order, this should add some minimal level of security while testing unecrypted data
            for (int i = 0; i < 1023; ++i)
            {
                packets[i] = new byte[sizeOfPacket + 3];
            }
            packets[1023] = new byte[remainingBytes + 3];
            Random seed = new Random();
            int mask = seed.Next(1, 31);
            for (int i = 0; i < 1023; ++i)
            {
                //byte[] firstTwo = new byte[2];
                byte[] firstTwo = BitConverter.GetBytes(mask + (i << 5));
                packets[i][0] = imageByte;
                packets[i][1] = firstTwo[0];
                packets[i][2] = firstTwo[1];
                //StringBuilder sb = new StringBuilder();
                for (int j = 0; j < sizeOfPacket; ++j)
                {
                    packets[i][j + 2] = (byte)data[i * sizeOfPacket + j];
                }
            }

            byte[] lastFirstTwo = new byte[2];
            lastFirstTwo = BitConverter.GetBytes(mask + (1023 << 5));
            packets[1023][0] = imageByte;
            packets[1023][1] = lastFirstTwo[0];
            packets[1023][2] = lastFirstTwo[1];
            for (int j = 1023 * sizeOfPacket; j < data.Length; ++j)
            {
                packets[1023][j - 1023 * sizeOfPacket + 3] = (byte)data[j];
            }
            //Tuple<int, byte[][]> ret = new Tuple<int, byte[][]>(sizeOfPacket, packets);
            return packets;
        }
        public string recieveStringData(byte[][] packets)
        {
            int noPackets = packets.Length;
            Tuple<int, int>[] order = new Tuple<int, int>[noPackets];
            for (int i = 0; i < noPackets; ++i)
            {
                byte[] temp = new byte[2];
                temp[1] = packets[i][0];
                temp[2] = packets[i][1];
                order[i] = new Tuple<int, int>(i, BitConverter.ToInt16(temp));
            }
            while (true)
            {
                bool ok = false;
                for (int i = 0; i < noPackets - 1; ++i)
                {
                    if (order[i].Item2 > order[i + 1].Item2)
                    {
                        ok = true;
                        Tuple<int, int> temp = order[i];
                        order[i] = order[i + 1];
                        order[i + 1] = temp;
                    }
                }
                if (ok == false)
                {
                    break;
                }
            }
            StringBuilder retData = new StringBuilder();
            for (int i = 0; i < noPackets; ++i)
            {
                for (int j = 0; j < packets[i].Length - 3; ++j)
                {
                    retData.Append((char)packets[i][order[i].Item1 + 3]);
                }
            }
            return retData.ToString();
        }
    }
}