using System;
using System.Drawing;
using System.Threading;
#nullable enable

namespace Jpeg
{
    internal class jpegCompression
    {
        private static System.Drawing.Imaging.BitmapData? bmpData = null;
        private static readonly double[,] C = new double[8, 8] { {0.3535533905932738,
  0.3535533905932738,
  0.3535533905932738,
  0.3535533905932738,
  0.3535533905932738,
  0.3535533905932738,
  0.3535533905932738,
  0.3535533905932738 },
 { 0.4903926402016152,
  0.4157348061512726,
  0.27778511650980114,
  0.09754516100806417,
  -0.09754516100806417,
  -0.27778511650980114,
  -0.4157348061512726,
  -0.4903926402016152 },
 { 0.46193976625564337,
  0.19134171618254492,
  -0.19134171618254492,
  -0.46193976625564337,
  -0.46193976625564337,
  -0.19134171618254492,
  0.19134171618254492,
  0.46193976625564337 },
 { 0.4157348061512726,
  -0.09754516100806417,
  -0.4903926402016152,
  -0.27778511650980114,
  0.27778511650980114,
  0.4903926402016152,
  0.09754516100806417,
  -0.4157348061512726 },
 { 0.3535533905932738,
  -0.3535533905932738,
  -0.3535533905932738,
  0.3535533905932738,
  0.3535533905932738,
  -0.3535533905932738,
  -0.3535533905932738,
  0.3535533905932738 },
 { 0.27778511650980114,
  -0.4903926402016152,
  0.09754516100806417,
  0.4157348061512726,
  -0.4157348061512726,
  -0.09754516100806417,
  0.4903926402016152,
  -0.27778511650980114 },
 { 0.19134171618254492,
  -0.46193976625564337,
  0.46193976625564337,
  -0.19134171618254492,
  -0.19134171618254492,
  0.46193976625564337,
  -0.46193976625564337,
  0.19134171618254492 },
 { 0.09754516100806417,
  -0.27778511650980114,
  0.4157348061512726,
  -0.4903926402016152,
  0.4903926402016152,
  -0.4157348061512726,
  0.27778511650980114,
  -0.09754516100806417 } };
        private static readonly double[] alpha = new double[8] { 0.5, 0.4903926402016152, 0.46193976625564337, 0.4157348061512726, 0.3535533905932738, 0.27778511650980114, 0.19134171618254492, 0.09754516100806417 };
        private static readonly int[,] LQM = new int[8, 8] {
  { 16, 11, 10, 16, 24, 40, 51, 61 },
  { 12, 12, 14, 19, 26, 58, 60, 55 },
  { 14, 13, 16, 24, 40, 57, 69, 56 },
  { 14, 17, 22, 29, 51, 87, 80, 62 },
  { 18, 22, 37, 56, 68, 109, 103, 77 },
  { 24, 35, 55, 64, 81, 104, 113, 92 },
  { 49, 64, 78, 87, 103, 121, 120, 101 },
  { 72, 92, 95, 98, 112, 100, 103, 99 },
};
        private static readonly int[,] CQM = new int[8, 8] {
  { 17, 18, 24, 47, 99, 99, 99, 99 },
  { 18, 21, 26, 66, 99, 99, 99, 99 },
  { 24, 26, 56, 99, 99, 99, 99, 99 },
  { 47, 66, 99, 99, 99, 99, 99, 99 },
  { 99, 99, 99, 99, 99, 99, 99, 99 },
  { 99, 99, 99, 99, 99, 99, 99, 99 },
  { 99, 99, 99, 99, 99, 99, 99, 99 },
  { 99, 99, 99, 99, 99, 99, 99, 99 },
};
        private static readonly byte[] _posX = new byte[64] { 0, 0, 1, 2, 1, 0, 0, 1, 2, 3, 4, 3, 2, 1, 0, 0, 1, 2, 3, 4, 5, 6, 5, 4, 3, 2, 1, 0, 0, 1, 2, 3, 4, 5, 6, 7, 7, 6, 5, 4, 3, 2, 1, 2, 3, 4, 5, 6, 7, 7, 6, 5, 4, 3, 4, 5, 6, 7, 7, 6, 5, 6, 7, 7 };
        private static readonly byte[] _posY = new byte[64] { 0, 1, 0, 0, 1, 2, 3, 2, 1, 0, 0, 1, 2, 3, 4, 5, 4, 3, 2, 1, 0, 0, 1, 2, 3, 4, 5, 6, 7, 6, 5, 4, 3, 2, 1, 0, 1, 2, 3, 4, 5, 6, 7, 7, 6, 5, 4, 3, 2, 3, 4, 5, 6, 7, 7, 6, 5, 4, 5, 6, 7, 7, 6, 7 };

        /// <summary>
        /// Struct used to represent an 8x8 block
        /// </summary>
        private struct Block
        {
            public double[][] A;

            //public System.Collections.BitArray sgn = new System.Collections.BitArray(64);
            public void DCTandQMethod(bool type)
            {
                for (int i = 0; i < 8; ++i)
                {
                    DoublesDCT1D(ref this.A[i]);
                }
                transpose(ref this.A);
                for (int i = 0; i < 8; ++i)
                {
                    DoublesDCT1D(ref this.A[i]);
                }
                transpose(ref this.A);
                switch (type)
                {
                    case true:
                        Quantize(ref this, LQM, 80);
                        break;
                    case false:
                        Quantize(ref this, CQM, 80);
                        break;
                }

            }

            private void QuantizeM(ref double[][] bl, int[,] Q, int phi)
            {
                int S = 200 - 2 * phi;
                if (phi < 50)
                {
                    S = 5000 / phi;
                }
                for (int i = 0; i < 8; ++i)
                {
                    for (int j = 0; j < 8; ++j)
                    {
                        bl[i][j] /= Math.Ceiling(((Q[i, j] * S + 50) / 100.0));
                    }
                }
            }

            public void IQuantizationAndIDCTMethod(bool type)
            {
                for (int i = 0; i < 8; ++i)
                {
                    for (int j = 0; j < 8; ++j)
                    {
                        if (this.A[i][j] > 127)
                        {
                            this.A[i][j] = 256 - this.A[i][j];
                        }
                    }
                }
                switch (type)
                {
                    case true:
                        QuantizeI(ref this, LQM, 80);
                        break;
                    case false:
                        QuantizeI(ref this, CQM, 80);
                        break;
                }
                transpose(ref this.A);
                IDCT(ref this.A[0]);
                IDCT(ref this.A[1]);
                IDCT(ref this.A[2]);
                IDCT(ref this.A[3]);
                IDCT(ref this.A[4]);
                IDCT(ref this.A[5]);
                IDCT(ref this.A[6]);
                IDCT(ref this.A[7]);
                transpose(ref this.A);
                IDCT(ref this.A[0]);
                IDCT(ref this.A[1]);
                IDCT(ref this.A[2]);
                IDCT(ref this.A[3]);
                IDCT(ref this.A[4]);
                IDCT(ref this.A[5]);
                IDCT(ref this.A[6]);
                IDCT(ref this.A[7]);
            }

            /*private void QuantizeIM(ref double[][] bl, int[,] Q, int phi)
            {
                int S = 200 - 2 * phi;
                if (phi < 50)
                {
                    S = 5000 / phi;
                }
                for (int i = 0; i < 8; ++i)
                {
                    for (int j = 0; j < 8; ++j)
                    {
                        int val = this.A[i][j];
                        if (val > 127 && i + j != 0)
                        {
                            val = val - 256;
                        }
                        bl[i][j] = val * Math.Ceiling(((Q[i, j] * S + 50) / 100.0));
                        //if (bl[i][j] < 0)
                        //{
                        //bl[i][j] = 0;
                        //}
                    }
                }
            }*/
            public Block(int sz)
            {
                A = new double[sz][];
                for (int i = 0; i < sz; ++i)
                {
                    A[i] = new double[sz];
                }
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="bl"></param>
        /// <param name="Q"></param>
        /// <param name="phi"></param>
        private static void Quantize(ref Block bl, int[,] Q, int phi)
        {
            int S = 200 - 2 * phi;
            if (phi < 50)
            {
                S = 5000 / phi;
            }
            for (int i = 0; i < 8; ++i)
            {
                for (int j = 0; j < 8; ++j)
                {
                    bl.A[i][j] /= Math.Ceiling((Q[i, j] * S + 50) / 100.0);
                    //bl[i][j] = Math.Floor(bl[i][j]);
                }
            }
        }

        private static void QuantizeI(ref Block orig, int[,] Q, int phi)
        {
            int S = 200 - 2 * phi;
            if (phi < 50 && phi > 0)
            {
                S = 5000 / phi;
            }
            for (int i = 0; i < 8; ++i)
            {
                for (int j = 0; j < 8; ++j)
                {
                    orig.A[i][j] *= Math.Ceiling(((Q[i, j] * S + 50) / 100.0));
                    //if (bl[i][j] < 0)
                    //{
                    //bl[i][j] = 0;
                    //}
                }
            }
        }


        /// <summary>
        /// Converts RGB values to YCbCr
        /// </summary>
        /// <param name="RValue">Value of the Red channel</param>
        /// <param name="GValue">Value of the Blue channel</param>
        /// <param name="BValue">Value of the Green channel</param>
        /// <returns>A triple of the YCbCr values</returns>
        private static byte[] RGBToYCbCr(byte RValue, byte GValue, byte BValue)
        {
            int R = (int)RValue;
            int G = (int)GValue;
            int B = (int)BValue;
            double Y, Cb, Cr;
            Y = (0.299 * R) + (0.587 * G) + (0.114 * B);
            Cb = 128 - (0.168736 * R) - (0.331264 * G) - (0.5 * B);
            Cr = 128 + (0.5 * R) - (0.418688 * G) - (0.081312 * B);
            int intY = (int)Y;
            int intCb = (int)Cb;
            int intCr = (int)Cr;
            byte[] retBytes = new byte[3];
            retBytes[0] = (byte)(char)intY;
            retBytes[1] = (byte)(char)intCb;
            retBytes[2] = (byte)(char)intCr;
            return retBytes;
        }


        /// <summary>
        /// Normalizes the value in the given interval
        /// </summary>
        /// <param name="value">The value to normalize</param>
        /// <param name="lowerBound">The lower bound on the interval of allowed values</param>
        /// <param name="upperBound">The upper bound of the interval of allowed values</param>
        private static void normalize(ref int value, int lowerBound = 0, int upperBound = 255)
        {
            if (value < lowerBound)
            {
                value = lowerBound;
                return;
            }
            if (value > upperBound)
            {
                value = upperBound;
            }
        }


        /// <summary>
        /// Converts YCbCr values to RGB
        /// </summary>
        /// <param name="YValue">Value of the Y channel</param>
        /// <param name="CbValue">Value of the Cb channel</param>
        /// <param name="CrValue">Value of the Cr channel</param>
        /// <returns>A triple of the RGB values</returns>
        private static byte[] YCbCrToRGB(byte YValue, byte CbValue, byte CrValue)
        {
            int Y = (int)YValue;
            int Cb = (int)CbValue;
            int Cr = (int)CrValue;
            int R = (int)(Y + 1.402 * (Cr - 128));
            int G = (int)(Y - 0.344136 * (Cb - 128) - 0.714136 * (Cr - 128));
            int B = (int)(Y + 1.772 * (Cb - 128));
            normalize(ref R);
            normalize(ref G);
            normalize(ref B);
            byte[] retBytes = new byte[3];
            retBytes[0] = Convert.ToByte((char)R);
            retBytes[1] = Convert.ToByte((char)G);
            retBytes[2] = Convert.ToByte((char)B);
            return retBytes;
        }


        /// <summary>
        /// Subsampling of the pixel data of a channel
        /// If the ratio is changed from 4, it should be updated in the extrapolate function's width and height parameters
        /// </summary>
        /// <param name="pixelData">The data of the channel</param>
        /// <param name="ratio">Subsampling ratio, ideally it is left as 4 or is 2
        /// if it not a divisor of the gcm of the height and width this function will not work</param>
        /// <param name="width">Width of the image, defaults to 1920x1080</param>
        /// <param name="height">Height of the image, defaults to 1920x1080</param>
        /// <returns>Returns the subsamples values, the size is (1920/ratio) * (1080/ratio)</returns>
        private static byte[] subsampling(byte[] pixelData, int ratio = 4, int width = 1920, int height = 1080)
        {
            if (ratio < 0)
            {
                ratio = 1;
            }
            int sz = (width / ratio) * (height / ratio);
            byte[] retByteArr = new byte[sz];
            int cPos = 0;
            for (int i = 0; i < width; i += ratio)
            {
                for (int j = 0; j < height; j += ratio)
                {
                    int nrValues = 0;
                    int sumOfValues = 0;
                    for (int ci = i; ci < i + ratio; ++ci)
                    {
                        for (int cj = j; cj < j + ratio; ++cj)
                        {
                            sumOfValues += pixelData[ci * height + cj];
                            nrValues++;
                        }
                    }
                    //retByteArr[cPos++] = pixelData[i * height + j];
                    switch (nrValues)
                    {
                        case 4:
                            int subsamplingValue = (sumOfValues >> 2);
                            retByteArr[cPos++] = (byte)(char)subsamplingValue;
                            break;
                        case 16:
                            subsamplingValue = (sumOfValues >> 4);
                            retByteArr[cPos++] = (byte)(char)subsamplingValue;
                            break;
                        default:
                            subsamplingValue = (sumOfValues / nrValues);
                            retByteArr[cPos++] = (byte)(char)subsamplingValue;
                            break;
                    }
                }
            }
            return retByteArr;
        }


        /// <summary>
        /// Extrapolating of the pixel data of a channel
        /// If the ratio is changed from 4, it should be updated in the width and height parameters
        /// </summary>
        /// <param name="pixelData">The data of the channel</param>
        /// <param name="ratio">Ideally the ratio is left as 4 or is 2></param>
        /// <param name="width">Width of the image, defaults to 480x270, for a ratio of 4</param>
        /// <param name="height">Height of the image, defaults to 480x270, for a ratio of 4</param>
        /// <returns>Returns the extrapolated values, the size is (width * ratio) * (height * ratio)</returns>
        private static byte[] extrapolate(byte[] pixelData, int ratio = 4, int width = 480, int height = 270)
        {
            int Nwidth = width * ratio, Nheight = height * ratio;
            byte[] retByteArray = new byte[Nwidth * Nheight];
            int cPos = 0;
            for (int i = 0; i < Nwidth; ++i)
            {
                for (int j = 0; j < Nheight; ++j)
                {
                    int pos = (i / ratio) * height + (j / ratio);
                    retByteArray[i * Nheight + j] = pixelData[pos];
                }
            }
            return retByteArray;
        }


        /// <summary>
        /// Splits channel data into 8x8 blocks
        /// </summary>
        /// <param name="channelData">Channel data of the image</param>
        /// <param name="width">Width of the image, defaults to 480x270 for the subsampled values</param>
        /// <param name="height">Width of the image, defaults to 480x270 for the subsampled values</param>
        /// <returns>Returns an array of Block objects</returns>
        private static Block[] splitIntoBlocks(byte[] channelData, ref Block[] blocks, int width = 480, int height = 270)
        {
            //int numberOfBlocks = ((width + 7) / 8) * ((height + 7) / 8);
            //blocks = new Block[numberOfBlocks];
            int curPos = 0;
            /*Block temp;
            temp.A = new byte[8][];
            for (int k = 0; k < 8; ++k)
            {
                temp.A[k] = new byte[8];
            }*/
            for (int i = 0; i < width; i += 8)
            {
                for (int j = 0; j < height; j += 8)
                {
                    for (int ci = i; ci < i + 8; ++ci)
                    {
                        for (int cj = j; cj < j + 8; ++cj)
                        {
                            int cpos = ci * height + cj;
                            double val = 0;
                            if (ci >= width)
                            {
                                if (cj < height)
                                {
                                    val = blocks[curPos].A[ci - i - 1][cj - j];
                                }
                                else
                                {
                                    val = blocks[curPos].A[ci - i][cj - j - 1];
                                }
                            }
                            else
                            {
                                if (cj >= height)
                                {
                                    val = blocks[curPos].A[ci - i][cj - j - 1];
                                }
                                else
                                {
                                    val = channelData[cpos];
                                }
                            }
                            blocks[curPos].A[ci - i][cj - j] = val;
                        }
                    }
                    curPos++;
                }
            }
            return blocks;
        }


        /// <summary>
        /// 
        /// </summary>
        /// <param name="v"></param>
        /// <param name="ret"></param>
        private static void DCT1D(in byte[] v, ref double[] ret)
        {
            double val0 = (v[0] + v[1] + v[2] + v[3] + v[4] + v[5] + v[6] + v[7]) * C[0, 0];
            double val1 = (v[0] - v[7]) * C[1, 0] + (v[1] - v[6]) * C[1, 1] +
                (v[2] - v[5]) * C[1, 2] + (v[3] - v[4]) * C[1, 3];
            double val2 = (v[0] + v[7] - v[3] - v[4]) * C[2, 0] +
                (v[1] + v[6] - v[2] - v[5]) * C[2, 1]; ;
            double val3 = (v[0] - v[7]) * C[3, 0] + (v[1] - v[6]) * C[3, 1] +
                (v[2] - v[5]) * C[3, 2] + (v[3] - v[4]) * C[3, 3];
            double val4 = (v[0] - v[1] - v[2] + v[3] + v[4] - v[5] - v[6] + v[7]) *
                C[4, 0];
            double val5 = (v[0] - v[7]) * C[5, 0] + (v[1] - v[6]) * C[5, 1] +
                (v[2] - v[5]) * C[5, 2] + (v[3] - v[4]) * C[5, 3];
            double val6 = (v[0] + v[7] - v[3] - v[4]) * C[6, 0] +
                (v[1] + v[6] - v[2] - v[5]) * C[6, 1];
            double val7 = (v[0] - v[7]) * C[7, 0] + (v[1] - v[6]) * C[7, 1] +
                (v[2] - v[5]) * C[7, 2] + (v[3] - v[4]) * C[7, 3];
            ret[0] = val0;
            ret[1] = val1;
            ret[2] = val2;
            ret[3] = val3;
            ret[4] = val4;
            ret[5] = val5;
            ret[6] = val6;
            ret[7] = val7;
        }


        /// <summary>
        /// 
        /// </summary>
        /// <param name="v"></param>
        /// <param name="ret"></param>
        private static void DoublesDCT1D(ref double[] v)
        {
            double val0 = (v[0] + v[1] + v[2] + v[3] + v[4] + v[5] + v[6] + v[7]) * C[0, 0];
            double val1 = (v[0] - v[7]) * C[1, 0] + (v[1] - v[6]) * C[1, 1] +
                (v[2] - v[5]) * C[1, 2] + (v[3] - v[4]) * C[1, 3];
            double val2 = (v[0] + v[7] - v[3] - v[4]) * C[2, 0] +
                (v[1] + v[6] - v[2] - v[5]) * C[2, 1]; ;
            double val3 = (v[0] - v[7]) * C[3, 0] + (v[1] - v[6]) * C[3, 1] +
                (v[2] - v[5]) * C[3, 2] + (v[3] - v[4]) * C[3, 3];
            double val4 = (v[0] - v[1] - v[2] + v[3] + v[4] - v[5] - v[6] + v[7]) *
                C[4, 0];
            double val5 = (v[0] - v[7]) * C[5, 0] + (v[1] - v[6]) * C[5, 1] +
                (v[2] - v[5]) * C[5, 2] + (v[3] - v[4]) * C[5, 3];
            double val6 = (v[0] + v[7] - v[3] - v[4]) * C[6, 0] +
                (v[1] + v[6] - v[2] - v[5]) * C[6, 1];
            double val7 = (v[0] - v[7]) * C[7, 0] + (v[1] - v[6]) * C[7, 1] +
                (v[2] - v[5]) * C[7, 2] + (v[3] - v[4]) * C[7, 3];
            v[0] = val0;
            v[1] = val1;
            v[2] = val2;
            v[3] = val3;
            v[4] = val4;
            v[5] = val5;
            v[6] = val6;
            v[7] = val7;
            /*double[] temp = new double[8];
            for (int i = 0; i < 8; ++i)
            {
                double val = 0;
                switch (i)
                {
                    case 0:
                        val = (v[0] + v[1] + v[2] + v[3] + v[4] + v[5] + v[6] + v[7]) * C[0, 0];
                        break;
                    case 2:
                        val = (v[0] + v[7] - v[3] - v[4]) * C[i, 0];
                        val += (v[1] + v[6] - v[2] - v[5]) * C[i, 1];
                        break;
                    case 4:
                        val = (v[0] - v[1] - v[2] + v[3] + v[4] - v[5] - v[6] + v[7]) * C[i, 0];
                        break;
                    case 6:
                        val = (v[0] + v[7] - v[3] - v[4]) * C[i, 0];
                        val += (v[1] + v[6] - v[2] - v[5]) * C[i, 1];
                        break;
                    default:
                        val = (v[0] - v[7]) * C[i, 0] + (v[1] - v[6]) * C[i, 1] + (v[2] - v[5]) * C[i, 2] + (v[3] - v[4]) * C[i, 3];
                        break;
                }
                ret[i] = val;
            }
            */
        }


        /// <summary>
        /// 
        /// </summary>
        /// <param name="v"></param>
        private static void IDCT(ref double[] v)
        {
            double val0f = (v[0] + v[4]) * alpha[4] + v[2] * alpha[2] + v[6] * alpha[6];
            double val0s = v[1] * alpha[1] + v[3] * alpha[3] + v[5] * alpha[5] + v[7] * alpha[7];
            double val1f = (v[0] - v[4]) * alpha[4] + v[2] * alpha[6] + v[6] * (-alpha[2]);
            double val1s = v[1] * alpha[3] + v[3] * (-alpha[7]) + v[5] * (-alpha[1]) + v[7] * (-alpha[5]);
            double val2f = (v[0] - v[4]) * alpha[4] + v[2] * (-alpha[6]) + v[6] * (alpha[2]);
            double val2s = v[1] * alpha[5] + v[3] * (-alpha[1]) + v[5] * alpha[7] + v[7] * alpha[3];
            double val3f = (v[0] + v[4]) * alpha[4] + v[2] * (-alpha[2]) + v[6] * (-alpha[6]);
            double val3s = v[1] * alpha[7] + v[3] * (-alpha[5]) + v[5] * alpha[3] + v[7] * (-alpha[1]);
            v[0] = val0f + val0s;
            v[1] = val1f + val1s;
            v[2] = val2f + val2s;
            v[3] = val3f + val3s;
            v[4] = val3f - val3s;
            v[5] = val2f - val2s;
            v[6] = val1f - val1s;
            v[7] = val0f - val0s;
        }


        /// <summary>
        /// 
        /// </summary>
        /// <param name="m"></param>
        private static void transpose(ref double[][] m)
        {
            for (int i = 0; i < 8; ++i)
            {
                for (int j = 0; j < i; ++j)
                {
                    double temp = m[i][j];
                    m[i][j] = m[j][i];
                    m[j][i] = temp;
                }
            }
        }

        /// <summary>
        /// Does the DCT and quantization on a Block
        /// </summary>
        /// <param name="bl">a Block</param>
        /// <param name="type">true for Y channel false for Cb and Cr</param>
        private static void DCTandQuantization(ref Block bl, bool type)
        {
            double[][] tempBlock = new double[8][];
            for (int i = 0; i < 8; ++i)
            {
                tempBlock[i] = new double[8];
                DoublesDCT1D(ref bl.A[i]);
            }
            transpose(ref bl.A);
            for (int i = 0; i < 8; ++i)
            {
                DoublesDCT1D(ref bl.A[i]);
            }
            transpose(ref bl.A);
            switch (type)
            {
                case true:
                    Quantize(ref bl, LQM, 90);
                    break;
                case false:
                    Quantize(ref bl, CQM, 90);
                    break;
            }
            //write8x8Darr(tempBlock, "DCTResult2.txt");
            for (int i = 0; i < 8; ++i)
            {
                for (int j = 0; j < 8; ++j)
                {
                    bl.A[i][j] = (byte)(char)(int)tempBlock[i][j];
                    //if (bl.A[i][j] > 250 && i + j > 0)
                    //{
                    //bl.A[i][j] = 0;
                    //System.Windows.Forms.MessageBox.Show(tempBlock[i][j].ToString());
                    //}
                    /*if (bl.A[i][j] >= 250)
                    {
                        bl.A[i][j] = 0;
                    }*/
                }
            }

        }

        private static void IQuantizationAndIDCT(ref Block bl, bool type)
        {
            double[][] iBl = new double[8][];
            iBl[0] = new double[8];
            iBl[1] = new double[8];
            iBl[2] = new double[8];
            iBl[3] = new double[8];
            iBl[4] = new double[8];
            iBl[5] = new double[8];
            iBl[6] = new double[8];
            iBl[7] = new double[8];
            switch (type)
            {
                case true:
                    QuantizeI(ref bl, LQM, 90);
                    break;
                case false:
                    QuantizeI(ref bl, CQM, 90);
                    break;
            }
            IDCT(ref iBl[0]);
            IDCT(ref iBl[1]);
            IDCT(ref iBl[2]);
            IDCT(ref iBl[3]);
            IDCT(ref iBl[4]);
            IDCT(ref iBl[5]);
            IDCT(ref iBl[6]);
            IDCT(ref iBl[7]);
            transpose(ref iBl);
            IDCT(ref iBl[0]);
            IDCT(ref iBl[1]);
            IDCT(ref iBl[2]);
            IDCT(ref iBl[3]);
            IDCT(ref iBl[4]);
            IDCT(ref iBl[5]);
            IDCT(ref iBl[6]);
            IDCT(ref iBl[7]);
            transpose(ref iBl);
            for (int i = 0; i < 8; ++i)
            {
                for (int j = 0; j < 8; ++j)
                {
                    if (iBl[i][j] < 0)
                    {
                        iBl[i][j] = -iBl[i][j];
                    }
                    bl.A[i][j] = (byte)(char)(int)iBl[i][j];
                }
            }
        }


        /// <summary>
        /// 
        /// </summary>
        /// <param name="block"></param>
        /// <returns></returns>
        private static byte[] finalEncoding(Block block)
        {
            byte[] retValue = new byte[128];
            byte nrApp = 0;
            int sizeOf = 0;
            for (int i = 0; i < 64; ++i)
            {
                if (block.A[_posX[i]][_posY[i]] < -0.1 || block.A[_posX[i]][_posY[i]] > 0.1)
                {
                    retValue[sizeOf++] = nrApp;
                    nrApp = 0;
                    retValue[sizeOf++] = (byte)(char)(int)block.A[_posX[i]][_posY[i]];
                    continue;
                }
                nrApp++;
            }
            if (nrApp != 0)
            {
                --nrApp;
                retValue[sizeOf++] = nrApp;
                retValue[sizeOf++] = 0;
            }
            byte[] ret = new byte[sizeOf];
            for (int i = 0; i < sizeOf; ++i)
            {
                ret[i] = retValue[i];
            }
            return ret;

        }


        /// <summary>
        /// 
        /// </summary>
        /// <param name="encodedArr"></param>
        /// <returns></returns>
        private static Block Decode(byte[] encodedArr)
        {
            Block decodedBlock = new Block(8);
            int cPos = 0;
            for (int i = 0; i < encodedArr.Length; i += 2)
            {
                for (int j = 0; j < encodedArr[i]; ++j)
                {
                    decodedBlock.A[_posX[cPos]][_posY[cPos]] = 0;
                    cPos++;
                }
                decodedBlock.A[_posX[cPos]][_posY[cPos]] = encodedArr[i + 1];
                cPos++;
            }
            return decodedBlock;
        }


        /// <summary>
        /// 
        /// </summary>
        /// <param name="blockData"></param>
        /// <param name="width"></param>
        /// <param name="height"></param>
        /// <returns></returns>
        private static byte[] convertBlocksToImageData(Block[] blockData, int width = 480, int height = 270)
        {
            byte[] imageData = new byte[width * height];
            int blocksPerLine = (width + 7) / 8;
            for (int i = 0; i < width; ++i)
            {
                for (int j = i; j < height; ++j)
                {
                    int curBlock = blocksPerLine * (int)(i / 8) + (int)j / 8;
                    int xPos = i % 8, yPos = j % 8;
                    imageData[i * height + j] = (byte)(char)(int)blockData[curBlock].A[xPos][yPos];
                }
            }
            return imageData;
        }


        /// <summary>
        /// 
        /// </summary>
        /// <param name="imageData"></param>
        /// <param name="width"></param>
        /// <param name="height"></param>
        /// <returns></returns>
        private static byte[] convertToYCbCr(byte[] imageData, int width = 1920, int height = 1080)
        {
            int length = imageData.Length;
            int lengthPerChannel = imageData.Length / 3;
            byte[] convertedData = new byte[imageData.Length];
            for (int i = 0; i < length; i += 3)
            {
                int R = (int)imageData[i + 2];
                int G = (int)imageData[i + 1];
                int B = (int)imageData[i];
                double Y, Cb, Cr;
                Y = 16 + (65.738 * R + 129.057 * G + 25.068 * B) / 256;
                Cb = 128 - (37.945 * R + 74.494 * G - 112.439 * B) / 256;
                Cr = 128 + (112.439 * R - 94.154 * G - 18.285 * B) / 256;
                /*byte[] conversion = RGBToYCbCr(imageData[i],
                    imageData[i + lengthPerChannel], imageData[i + 2 * lengthPerChannel]);*/
                int pos = i / 3;
                convertedData[pos] = (byte)(char)(int)Y;
                convertedData[pos + lengthPerChannel] = (byte)(char)(int)Cb;
                convertedData[pos + lengthPerChannel * 2] = (byte)(char)(int)Cr;
            }
            return convertedData;
        }


        /// <summary>
        /// 
        /// </summary>
        /// <param name="imageData"></param>
        /// <param name="width"></param>
        /// <param name="height"></param>
        /// <returns></returns>
        private static byte[] convertToRGB(byte[] imageData, int width = 1920, int height = 1080)
        {
            int lengthPerChannel = imageData.Length / 3;
            byte[] convertedData = new byte[imageData.Length];
            byte[] convertedDataY = new byte[1];
            byte[] convertedDataCb = new byte[1];
            byte[] convertedDataCr = new byte[1];
            for (int i = 0; i < lengthPerChannel; ++i)
            {
                byte[] conversion = YCbCrToRGB(imageData[i],
                    imageData[i + lengthPerChannel], imageData[i + 2 * lengthPerChannel]);
                convertedData[i] = conversion[0];
                convertedData[i + lengthPerChannel] = conversion[1];
                convertedData[i + lengthPerChannel * 2] = conversion[2];
            }
            return convertedData;
        }


        /// <summary>
        /// This is way faster than the garbage above
        /// </summary>
        /// <param name="imageData"></param>
        /// <param name="width"></param>
        /// <param name="height"></param>
        /// <returns></returns>
        private static void convertToRGBInPlace(ref byte[] imageData, int width = 1920, int height = 1080)
        {
            int length = imageData.Length;
            //byte[] convertedData = new byte[imageData.Length];
            for (int i = 0; i < length; i += 3)
            {
                int R = (int)((imageData[i] * 298.082 + imageData[i + 2] * 408.583) / 256 - 222.921);
                int G = (int)((imageData[i] * 298.082 - 100.291 * imageData[i + 1] - 208.120 * imageData[i + 2]) / 256 + 135.576);
                int B = (int)((imageData[i] * 298.082 + 516.412 * imageData[i + 1]) / 256 - 276.836);
                normalize(ref R);
                normalize(ref G);
                normalize(ref B);
                imageData[i] = (byte)(char)B;
                imageData[i + 1] = (byte)(char)G;
                imageData[i + 2] = (byte)(char)R;
            }
            //return convertedData;
        }


        private static void applyDCT(ref Block[] blocks, bool type)
        {
            for (int k = 0; k < blocks.Length; ++k)
            {
                DCTandQuantization(ref blocks[k], type);
                /*double[][] tempBlock = new double[8][];
                for (int i = 0; i < 8; ++i)
                {
                    tempBlock[i] = new double[8];
                    DCT1D(blocks[k].A[i], ref tempBlock[i]);
                }
                transpose(ref tempBlock);
                for (int i = 0; i < 8; ++i)
                {
                    DoublesDCT1D(tempBlock[i], ref tempBlock[i]);
                }
                transpose(ref tempBlock);
                switch (type)
                {
                    case true:
                        Quantize(ref tempBlock, LQM, 75);
                        break;
                    case false:
                        Quantize(ref tempBlock, CQM, 75);
                        break;
                }
                for (int i = 0; i < 8; ++i)
                {
                    for (int j = 0; j < 8; ++j)
                    {
                        blocks[k].A[i][j] = (byte)((char)((int)tempBlock[i][j]));
                    }
                }*/
            }
        }

        private static void apply1DCT(ref Block b, bool type)
        {
            b.DCTandQMethod(type);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="imageData"></param>
        /// <param name="width"></param>
        /// <param name="height"></param>
        /// <returns></returns>
        internal static byte[] convertImage(byte[] imageData, int width = 1920, int height = 1080)
        {
            testDCTandIDCT();
            //return null;
            byte[] YData = new byte[width * height];
            byte[] CbData = new byte[width * height];
            byte[] CrData = new byte[width * height];
            int offset = height * width;
            byte[] YCbCrData = convertToYCbCr(imageData);
            // 12ms
            Buffer.BlockCopy(YCbCrData, 0, YData, 0, YData.Length);
            Buffer.BlockCopy(YCbCrData, YData.Length, CbData, 0, CbData.Length);
            Buffer.BlockCopy(YCbCrData, YData.Length + CbData.Length, CrData, 0, CrData.Length);
            //11ms in release
            //return null;
            byte[] NewCrData = new byte[width / 4 * height / 4];
            Thread NewCr = new Thread(
                () => NewCrData = subsampling(CrData));
            NewCr.Start();
            byte[] NewCbData = subsampling(CbData);
            NewCr.Join();
            //writeByteArr("origCrdata.txt", CrData);
            //writeByteArr("CbSubsamplingData.txt", NewCbData);
            //writeByteArr("CrSubsamplingData.txt", NewCrData);
            //writeByteArr("Ydata.txt", YData);
            int sz = (width / 8) * (height / 8);
            Block[] YBlocks = new Block[sz];
            Thread YBlThread = new Thread(
                () =>
                {
                    for (int i = 0; i < sz; ++i)
                    {
                        YBlocks[i] = new Block(8);
                    }
                    splitIntoBlocks(YData, ref YBlocks, 1920, 1080);
                });
            YBlThread.Start();
            Block[] CbBlocks = new Block[60 * 34];
            for (int i = 0; i < CbBlocks.Length; ++i)
            {
                CbBlocks[i] = new Block(8);
            }
            splitIntoBlocks(NewCbData, ref CbBlocks);
            Block[] CrBlocks = new Block[60 * 34];
            for (int i = 0; i < CrBlocks.Length; ++i)
            {
                CrBlocks[i] = new Block(8);
            }
            splitIntoBlocks(NewCrData, ref CrBlocks);
            YBlThread.Join();
            writeBlock(CbBlocks[0], "cbBlOrig.txt");
            //writeBlock(YBlocks[10], "cbBlOrig1.txt");
            //return null;
            //in debug splitting takes ~30ms
            int lg = YBlocks.Length / 2, Lg = YBlocks.Length;
            Thread Ythread1 = new Thread(
                () =>
                {
                    for (int j = 0; j < lg; ++j)
                    {
                            //Block temp = YBlocks[j];
                            //temp.DCTandQMethod(true);
                        YBlocks[j].DCTandQMethod(true);
                    }
                });
            Ythread1.Start();
            Thread Ythread2 = new Thread(
                () =>
                {
                    for (int k = lg; k < Lg; ++k)
                    {
                            //DCTandQuantization(ref YBlocks[i], true);
                        YBlocks[k].DCTandQMethod(true);
                    }
                });
            Ythread2.Start();
            int Clen = CbBlocks.Length;
            for (int i = 0; i < Clen; ++i)
            {
                CbBlocks[i].DCTandQMethod(false);
                CrBlocks[i].DCTandQMethod(false);
            }
            //Ythread2.Join();
            Ythread1.Join();
            //writeBlock(YBlocks[0], "cbBl.txt");
            writeBlock(CbBlocks[0], "cbBl1.txt");
            //DCT Chen on the Y channel takes ~175ms in debug, however i have the resources to compute it in 2 threads at the same time, hopefully loeffler can save the day
            //return null;
            byte[][] compressedBlocks = new byte[YBlocks.Length + 2 * CbBlocks.Length][];
            byte[]? retByteArr = null;
            int finalLength = 0;
            int Yl = YBlocks.Length, Cl = CbBlocks.Length;
            int L = Yl / 2;
            Thread Ythr1 = new Thread(
                () =>
                {
                    for (int i = 0; i < L; ++i)
                    {
                        compressedBlocks[i] = finalEncoding(YBlocks[i]);
                    }
                });
            Ythr1.Start();
            Thread Ythr2 = new Thread(
                () =>
                {
                    for (int j = L; j < Yl; ++j)
                    {
                        compressedBlocks[j] = finalEncoding(YBlocks[j]);
                    }
                });
            Ythr2.Start();
            //writeBlock(CbBlocks[1], "cbBl.txt");
            for (int i = 0; i < CbBlocks.Length; ++i)
            {
                compressedBlocks[i + Yl] = finalEncoding(CbBlocks[i]);
                //finalLength += compressedBlocks[i + Yl].Length;
                compressedBlocks[i + Yl + Cl] = finalEncoding(CrBlocks[i]);
                //finalLength += compressedBlocks[i + Yl + Cl].Length;
            }
            Ythr2.Join();
            Ythr1.Join();
            for (int i = 0; i < compressedBlocks.Length; ++i)
            {
                finalLength += compressedBlocks[i].Length;
            }
            retByteArr = new byte[finalLength];
            int retArrPos = 0;
            //Buffer.BlockCopy(compressedBlocks, 0, retByteArr, 0, finalLength);
            //System.Windows.Forms.MessageBox.Show(Yl.ToString());
            for (int i = 0; i < compressedBlocks.Length; ++i)
            {
                for (int j = 0; j < compressedBlocks[i].Length; ++j)
                {
                    try
                    {
                        retByteArr[retArrPos++] =
                            compressedBlocks[i][j];
                    }
                    catch (Exception ex)
                    {
                        System.Windows.Forms.MessageBox.Show(finalLength.ToString() + " " + retArrPos.ToString() + " " + i.ToString() + "\n" + ex.ToString());
                    }
                    //System.Windows.Forms.MessageBox.Show(retArrPos.ToString() + " " + retByteArr.Length.ToString() + "\n" +
                    //i.ToString() + " " + j.ToString() + " " + (YBlocks.Length + 2 * CbBlocks.Length).ToString());
                }
            }
            writeByteArr("CompData.txt", retByteArr);
            //MessageBox.Show((YBlocks.Length + 2 * CbBlocks.Length).ToString() + " " + retByteArr.Length.ToString());
            return retByteArr;
        }


        /// <summary>
        /// 
        /// </summary>
        /// <param name="data"></param>
        /// <param name="noBl"></param>
        /// <param name="width"></param>
        /// <param name="height"></param>
        /// <returns></returns>
        private static Block[] getBlocks(byte[] data, int noBl, int width = 1920, int height = 1080)
        {
            int prevPos = 0, nrVals = 0, idBl = 0;
            Block[] rets = new Block[noBl];
            for (int i = 0; i < noBl; ++i)
            {
                rets[i] = new Block(8);
            }
            int nr = 0;
            for (int i = 0; i < data.Length; i += 2)
            {
                while (data[i] > 0)
                {
                    rets[idBl].A[_posX[nr]][_posY[nr]] = 0;
                    nr++;
                    data[i]--;
                }
                rets[idBl].A[_posX[nr]][_posY[nr]] = data[i + 1];
                nr++;
                if (nr == 64)
                {
                    idBl++;
                    nr = 0;
                }
                /*nrVals += data[i] + 1;
                if (nrVals == 64)
                {
                    int cpos = 0;
                    for (int j = prevPos; j < i; j += 2)
                    {
                        int nr0 = data[j];
                        while (nr0 != 0 && cpos < 64)
                        {
                            rets[idBl].A[_posX[cpos]][_posY[cpos]] = 0;
                            nr0--;
                            cpos++;
                        }
                        if (cpos < 64)
                        {
                            rets[idBl].A[_posX[cpos]][_posY[cpos]] = data[j + 1];
                            cpos++;
                        }
                    }
                    nrVals = 0;
                    prevPos = i;
                    idBl++;
                }*/
            }
            return rets;
        }


        /// <summary>
        /// Recovers the image from the compressed data
        /// </summary>
        /// <param name="compData">Compressed image data</param>
        /// <param name="width">Width of the image in pixels, default value is 1920</param>
        /// <param name="height">Height of the image in pixels, default value is 1080</param>
        /// <returns>Complete RGB data for the image, the default size if 1920x1080
        /// The compression ratio is assumed to be 4 for the Cb and Cr channels</returns>
        internal static byte[] recoverImage(byte[] compData, int width = 1920, int height = 1080)
        {
            //System.IO.File.WriteAllBytes("encodedData.txt", compData);
            //writeByteArr("origdata.txt", compData);
            byte[] imageData = new byte[width * height * 3];
            int tempSum = 0, noBl = 0;
            for (int i = 0; i < compData.Length; i += 2)
            {
                tempSum += (compData[i] + 1);
                if (tempSum == 64)
                {
                    noBl++;
                    tempSum = 0;
                }
            }
            Block[] encodedBlocks = getBlocks(compData, noBl);
            //writeBlock(encodedBlocks[0], "f15.txt");
            int nrYbl = ((width + 7) / 8) * ((height + 7) / 8);
            int cBl = (encodedBlocks.Length - nrYbl) / 2;
            //writeBlock(encodedBlocks[0], "e0.txt");
            writeBlock(encodedBlocks[nrYbl], "e1.txt");
            Thread Ythr1 = new Thread(
                () =>
                {
                    for (int i = 0; i < nrYbl / 2; ++i)
                    {
                            //IQuantizationAndIDCT(ref encodedBlocks[i], true);
                        encodedBlocks[i].IQuantizationAndIDCTMethod(true);
                    }
                });
            Thread Ythr2 = new Thread(
                () =>
                {
                    for (int j = nrYbl / 2; j < nrYbl; ++j)
                    {
                            //IQuantizationAndIDCT(ref encodedBlocks[i], true);
                        encodedBlocks[j].IQuantizationAndIDCTMethod(true);
                    }
                });
            Ythr1.Start();
            Ythr2.Start();
            for (int k = nrYbl; k < encodedBlocks.Length; ++k)
            {
                //IQuantizationAndIDCT(ref encodedBlocks[i], false);
                encodedBlocks[k].IQuantizationAndIDCTMethod(false);
            }
            Ythr2.Join();
            writeBlock(encodedBlocks[nrYbl], "first.txt");
            //writeBlock(encodedBlocks[nrYbl], "first.txt");
            int e1 = encodedBlocks.Length / 3, e2 = e1 * 2, horizontaOffset = width;
            byte[] Ydata = new byte[width * height];
            byte[] Ydata2 = new byte[(width * height) / 2];
            int offset1 = 0;
            //writeBlock(encodedBlocks[0], "f35.txt");
            Thread fill1 = new Thread(
                () =>
                {
                    byte[] Ydata1 = new byte[(width * height) / 2];
                    int _cpos0 = 0, _cpos1 = width, _cpos2 = width * 2, _cpos3 = width * 3, _cpos4 = width * 4, _cpos5 = width * 5, _cpos6 = width * 6, _cpos7 = width * 7;
                    bool _suntProst = true;
                    for (int i = 0; i < nrYbl / 2; ++i)
                    {
                        for (int j = 0; j < 8; ++j)
                        {
                            if (_cpos0 % width == 0 && _suntProst == false)
                                //this means that I completed the whole width of the image, the next blocks go 8 pixels down 
                                //it shouldn't happen on the Y channel but I'd like to err on the side of caution
                            {
                                break;
                            }
                            Ydata1[_cpos0] = (byte)(char)(int)encodedBlocks[i].A[0][j];
                            ++_cpos0;
                            if (_cpos1 < Ydata2.Length)
                            {
                                Ydata2[_cpos1] = (byte)(char)(int)encodedBlocks[i].A[1][j];
                            }
                            ++_cpos1;
                            if (_cpos2 < Ydata2.Length)
                            {
                                Ydata2[_cpos2] = (byte)(char)(int)encodedBlocks[i].A[2][j];
                            }
                            ++_cpos2;
                            if (_cpos3 < Ydata2.Length)
                            {
                                Ydata2[_cpos3] = (byte)(char)(int)encodedBlocks[i].A[3][j];
                            }
                            ++_cpos3;
                            if (_cpos4 < Ydata2.Length)
                            {
                                Ydata2[_cpos4] = (byte)(char)(int)encodedBlocks[i].A[4][j];
                            }
                            ++_cpos4;
                            if (_cpos5 < Ydata2.Length)
                            {
                                Ydata2[_cpos5] = (byte)(char)(int)encodedBlocks[i].A[5][j];
                            }
                            ++_cpos5;
                            if (_cpos6 < Ydata2.Length)
                            {
                                Ydata2[_cpos6] = (byte)(char)(int)encodedBlocks[i].A[6][j];
                            }
                            ++_cpos6;
                            if (_cpos7 < Ydata2.Length)
                            {
                                Ydata2[_cpos7] = (byte)(char)(int)encodedBlocks[i].A[7][j];
                            }
                            ++_cpos7;
                        }
                        if (_cpos0 % width == 0)
                        {
                            if (_suntProst == false)
                            {
                                _cpos0 += 7 * width;
                                _cpos1 += 7 * width;
                                _cpos2 += 7 * width;
                                _cpos3 += 7 * width;
                                _cpos4 += 7 * width;
                                _cpos5 += 7 * width;
                                _cpos6 += 7 * width;
                                _cpos7 += 7 * width;
                            }
                            _suntProst = !_suntProst;
                        }
                    }
                    offset1 = 0;//ideally it should stay 0, unless something goes horribly wrong
                    for (int i = Ydata1.Length - 1; i > 0; --i)
                    {
                        if (Ydata1[i] != null)
                        {
                            break;
                        }
                        ++offset1;
                    }
                    Buffer.BlockCopy(Ydata1, 0, Ydata, 0, Ydata1.Length - offset1);
                    offset1 = Ydata1.Length - offset1;
                });
            Thread fill2 = new Thread(
                () =>
                {
                    int _cpos0 = 0, _cpos1 = width, _cpos2 = width * 2, _cpos3 = width * 3, _cpos4 = width * 4, _cpos5 = width * 5, _cpos6 = width * 6, _cpos7 = width * 7;
                    bool _suntProst = true;
                    for (int i = nrYbl / 2; i < nrYbl; ++i)
                    {
                        for (int j = 0; j < 8; ++j)
                        {
                            if (_cpos0 % width == 0 && _suntProst == false)
                                //this means that I completed the whole width of the image, the next blocks go 8 pixels down 
                                //it shouldn't happen on the Y channel but I'd like to err on the side of caution
                            {
                                break;
                            }
                            Ydata2[_cpos0] = (byte)(char)(int)encodedBlocks[i].A[0][j];
                            ++_cpos0;
                            if (_cpos1 < Ydata2.Length)
                            {
                                Ydata2[_cpos1] = (byte)(char)(int)encodedBlocks[i].A[1][j];
                            }
                            ++_cpos1;
                            if (_cpos2 < Ydata2.Length)
                            {
                                Ydata2[_cpos2] = (byte)(char)(int)encodedBlocks[i].A[2][j];
                            }
                            ++_cpos2;
                            if (_cpos3 < Ydata2.Length)
                            {
                                Ydata2[_cpos3] = (byte)(char)(int)encodedBlocks[i].A[3][j];
                            }
                            ++_cpos3;
                            if (_cpos4 < Ydata2.Length)
                            {
                                Ydata2[_cpos4] = (byte)(char)(int)encodedBlocks[i].A[4][j];
                            }
                            ++_cpos4;
                            if (_cpos5 < Ydata2.Length)
                            {
                                Ydata2[_cpos5] = (byte)(char)(int)encodedBlocks[i].A[5][j];
                            }
                            ++_cpos5;
                            if (_cpos6 < Ydata2.Length)
                            {
                                Ydata2[_cpos6] = (byte)(char)(int)encodedBlocks[i].A[6][j];
                            }
                            ++_cpos6;
                            if (_cpos7 < Ydata2.Length)
                            {
                                Ydata2[_cpos7] = (byte)(char)(int)encodedBlocks[i].A[7][j];
                            }
                            ++_cpos7;
                        }
                        if (_cpos0 % width == 0)
                        {
                            if (_suntProst == false)
                            {
                                _cpos0 += 7 * width;
                                _cpos1 += 7 * width;
                                _cpos2 += 7 * width;
                                _cpos3 += 7 * width;
                                _cpos4 += 7 * width;
                                _cpos5 += 7 * width;
                                _cpos6 += 7 * width;
                                _cpos7 += 7 * width;
                            }
                            _suntProst = !_suntProst;
                        }
                    }
                });
            fill1.Start();
            fill2.Start();
            int Nwidth = width / 4;
            int cpos0 = 0, cpos1 = Nwidth, cpos2 = Nwidth * 2, cpos3 = Nwidth * 3, cpos4 = Nwidth * 4, cpos5 = Nwidth * 5, cpos6 = Nwidth * 6, cpos7 = Nwidth * 7;
            byte[] Crdata = new byte[(width / 4) * (height / 4)];//this assumes compression ratio is 4, if the data was compressed using something else this will crash, which is not ideal
            byte[] Cbdata = new byte[(width / 4) * (height / 4)];//this assumes compression ratio is 4
            int diff = (encodedBlocks.Length - nrYbl) / 2;//the number of blocks and the number of blocks for the Y channel should have the same parity
            bool suntProst = true;
            for (int i = 0; i < diff; ++i)
            {
                for (int j = 0; j < 8; ++j)
                {
                    if (cpos0 % Nwidth == 0 && suntProst == false)
                    {
                        break;
                    }
                    Crdata[cpos0] = (byte)(char)(int)encodedBlocks[i + nrYbl].A[0][j];
                    Cbdata[cpos0] = (byte)(char)(int)encodedBlocks[i + nrYbl + diff].A[0][j];
                    ++cpos0;
                    if (cpos1 < Crdata.Length)
                    {
                        Crdata[cpos1] = (byte)(char)(int)encodedBlocks[i + nrYbl].A[1][j];
                        Cbdata[cpos1] = (byte)(char)(int)encodedBlocks[i + nrYbl + diff].A[1][j];
                    }
                    ++cpos1;
                    if (cpos2 < Crdata.Length)
                    {
                        Crdata[cpos2] = (byte)(char)(int)encodedBlocks[i + nrYbl].A[2][j];
                        Cbdata[cpos2] = (byte)(char)(int)encodedBlocks[i + nrYbl + diff].A[2][j];
                    }
                    ++cpos2;
                    if (cpos3 < Crdata.Length)
                    {
                        Crdata[cpos3] = (byte)(char)(int)encodedBlocks[i + nrYbl].A[3][j];
                        Cbdata[cpos3] = (byte)(char)(int)encodedBlocks[i + nrYbl + diff].A[3][j];
                    }
                    ++cpos3;
                    if (cpos4 < Crdata.Length)
                    {
                        Crdata[cpos4] = (byte)(char)(int)encodedBlocks[i + nrYbl].A[4][j];
                        Cbdata[cpos4] = (byte)(char)(int)encodedBlocks[i + nrYbl + diff].A[4][j];
                    }
                    ++cpos4;
                    if (cpos5 < Crdata.Length)
                    {
                        Crdata[cpos5] = (byte)(char)(int)encodedBlocks[i + nrYbl].A[5][j];
                        Cbdata[cpos5] = (byte)(char)(int)encodedBlocks[i + nrYbl + diff].A[5][j];
                    }
                    ++cpos5;
                    if (cpos6 < Crdata.Length)
                    {
                        Crdata[cpos6] = (byte)(char)(int)encodedBlocks[i + nrYbl].A[6][j];
                        Cbdata[cpos6] = (byte)(char)(int)encodedBlocks[i + nrYbl + diff].A[6][j];
                    }
                    ++cpos6;
                    if (cpos7 < Crdata.Length)
                    {
                        Crdata[cpos7] = (byte)(char)(int)encodedBlocks[i + nrYbl].A[7][j];
                        Cbdata[cpos7] = (byte)(char)(int)encodedBlocks[i + nrYbl + diff].A[7][j];
                    }
                    ++cpos7;
                }
                if (cpos0 % Nwidth == 0)
                {
                    if (suntProst == false)
                    {
                        cpos0 += 7 * Nwidth;
                        cpos1 += 7 * Nwidth;
                        cpos2 += 7 * Nwidth;
                        cpos3 += 7 * Nwidth;
                        cpos4 += 7 * Nwidth;
                        cpos5 += 7 * Nwidth;
                        cpos6 += 7 * Nwidth;
                        cpos7 += 7 * Nwidth;
                    }
                    suntProst = !suntProst;
                }
            }
            fill2.Join();
            //writeByteArr(Ydata2, "Ydata2.txt");
            int offset2 = 0;
            for (int i = Ydata2.Length - 1; i >= 0; --i)
            {
                if (Ydata2[i] != null)
                {
                    break;
                }
                ++offset2;
            }
            Buffer.BlockCopy(Ydata2, 0, Ydata, offset1, Ydata2.Length);//All of the Y channel data is where it needs to be, finnaly
            byte[] CbBigData = new byte[Cbdata.Length * 16];
            byte[] CrBigData = new byte[Crdata.Length * 16];
            Thread extraCr = new Thread(
                () =>
                {
                    CrBigData = extrapolate(Crdata);
                        //convertToRGBInPlace(ref CrBigData);
                });
            extraCr.Start();
            CbBigData = extrapolate(Cbdata);
            //convertToRGBInPlace(ref CbBigData);
            extraCr.Join();
            //Buffer.BlockCopy(Ydata, 0, imageData, 0, Ydata.Length);
            //Buffer.BlockCopy(CbBigData, 0, imageData, Ydata.Length, CbBigData.Length);
            //Buffer.BlockCopy(CrBigData, 0, imageData, Ydata.Length + CbBigData.Length, CrBigData.Length);
            int len = 3 * width * height;
            for (int i = 0; i < len; i += 3)
            {
                int pos = i / 3;
                imageData[i] = Ydata[pos];
                imageData[i + 1] = CbBigData[pos];
                imageData[i + 2] = CrBigData[pos];
            }
            convertToRGBInPlace(ref imageData);
            //writeByteArr("Result.txt", imageData);
            return imageData;
        }

        internal static Bitmap createBmp(byte[] data)
        {
            Bitmap bmp = new Bitmap(1920, 1080, System.Drawing.Imaging.PixelFormat.Format24bppRgb);
            System.Drawing.Imaging.BitmapData dataToCopy = bmp.LockBits(new Rectangle(0, 0, bmp.Width, bmp.Height), System.Drawing.Imaging.ImageLockMode.WriteOnly, bmp.PixelFormat);
            IntPtr Iptr = IntPtr.Zero;
            Iptr = dataToCopy.Scan0;
            System.Runtime.InteropServices.Marshal.Copy(data, 0, Iptr, data.Length);
            bmp.UnlockBits(dataToCopy);
            return bmp;
        }


        /// <summary>
        /// Converts a Bitmap to a bytearray of pixel informations
        /// The channels are in order
        /// </summary>
        /// <param name="bmp">The Bitmap to get the data from</param>
        /// <returns></returns>
        internal static byte[] convertBmpToByteArr(Bitmap bmp)
        {
            int size = bmp.Height * bmp.Width;
            byte[] imageData = new byte[size * 3];
            Rectangle rect = new Rectangle(0, 0, bmp.Width, bmp.Height);
            bmpData = bmp.LockBits(rect, System.Drawing.Imaging.ImageLockMode.ReadOnly, bmp.PixelFormat);
            IntPtr Iptr = IntPtr.Zero;
            Iptr = bmpData.Scan0;
            System.Runtime.InteropServices.Marshal.Copy(Iptr, imageData, 0, imageData.Length);
            //order is B G R (blue green red)
            bmp.UnlockBits(bmpData);
            bmp.Dispose();
            return imageData;
        }

        private static void testOk(byte[] arr, Bitmap bmp)
        {
            try
            {
                int size = bmp.Height * bmp.Width;
                byte[] compData = new byte[size * 3];
                for (int i = 0; i < bmp.Width; ++i)
                {
                    for (int j = 0; j < bmp.Height; ++j)
                    {
                        Color col = bmp.GetPixel(i, j);
                        compData[(i * bmp.Height + j) * 3] = col.R;
                        compData[(i * bmp.Height + j) * 3 + 1] = col.G;
                        compData[(i * bmp.Height + j) * 3 + 2] = col.B;
                    }
                }
                for (int i = 0; i < size * 3; ++i)
                {
                    if (compData[i] != arr[i])
                    {
                        System.Windows.Forms.MessageBox.Show(i.ToString() + "aici e buba");
                        break;
                    }
                }
                System.Windows.Forms.MessageBox.Show("E ok");
                return;
            }
            catch (Exception ex)
            {
                System.Windows.Forms.MessageBox.Show(ex.Message);
            }
        }

        private static void writeByteArr(string filename, byte[] arr)
        {
            using (System.IO.StreamWriter sw = new System.IO.StreamWriter(filename))
            {
                for (int i = 0; i < arr.Length; ++i)
                {
                    sw.Write(arr[i]);
                    sw.Write(" ");
                }
            }
        }

        private static void writeBlock(Block bl, string filename)
        {
            using (System.IO.StreamWriter sw = new System.IO.StreamWriter(filename))
            {
                for (int i = 0; i < 8; ++i)
                {
                    for (int j = 0; j < 8; ++j)
                    {
                        sw.Write(bl.A[i][j]);
                        sw.Write(" ");
                    }
                    sw.Write("\n");
                }
                sw.Close();
            }
        }

        private static void write8x8Darr(double[][] arr, string filename)
        {
            using (System.IO.StreamWriter sw = new System.IO.StreamWriter(filename))
            {
                for (int i = 0; i < 8; ++i)
                {
                    for (int j = 0; j < 8; ++j)
                    {
                        sw.Write(Math.Round(arr[i][j], 2));
                        sw.Write(" ");
                    }
                    sw.Write("\n");
                }
                sw.Close();
            }
        }

        private static void testDCTandIDCT()
        {
            Random rand = new Random();
            Block[] testB = new Block[1];
            testB[0] = new Block(8);
            for (int i = 0; i < 8; ++i)
            {
                for (int j = 0; j < 8; ++j)
                {
                    if (rand.Next(0, 10) == 8 && i == 3)
                    {
                        testB[0].A[i][j] = (byte)(char)rand.Next(0, 255);
                        continue;
                    }
                    testB[0].A[i][j] = (byte)(char)(rand.Next(7, 10) * 10);
                }
            }
            writeBlock(testB[0], "f1.txt");
            testB[0].DCTandQMethod(false);
            //DCTandQuantization(ref testB, true);
            writeBlock(testB[0], "f2.txt");
            for (int i = 0; i < 8; ++i)
            {
                for (int j = 0; j < 8; ++j)
                {
                    testB[0].A[i][j] = (byte)(char)(int)Math.Round(testB[0].A[i][j]);
                }
            }
            writeBlock(testB[0], "f25.txt");
            //IQuantizationAndIDCT(ref testB, true);
            testB[0].IQuantizationAndIDCTMethod(false);
            writeBlock(testB[0], "f3.txt");
        }

        /// <summary>
        /// not jpeg for testyng everything else
        /// </summary>
        /// <returns></returns>
        internal static byte[] notJpegEncoding(byte[] imageData, int width = 1920, int height = 1080)
        {
            byte[] YData = new byte[width * height];
            byte[] CbData = new byte[width * height];
            byte[] CrData = new byte[width * height];
            int offset = height * width;
            byte[] YCbCrData = convertToYCbCr(imageData);
            // 12ms
            Buffer.BlockCopy(YCbCrData, 0, YData, 0, YData.Length);
            Buffer.BlockCopy(YCbCrData, YData.Length, CbData, 0, CbData.Length);
            Buffer.BlockCopy(YCbCrData, YData.Length + CbData.Length, CrData, 0, CrData.Length);
            //11ms in release
            byte[] NewCrData = new byte[width / 4 * height / 4];
            Thread NewCr = new Thread(
                () => NewCrData = subsampling(CrData));
            NewCr.Start();
            byte[] NewCbData = subsampling(CbData);
            NewCr.Join();
            byte[] ret = new byte[YData.Length + NewCbData.Length + NewCrData.Length];
            Buffer.BlockCopy(YData, 0, ret, 0, YData.Length);
            Buffer.BlockCopy(NewCbData, 0, ret, YData.Length, NewCbData.Length);
            Buffer.BlockCopy(NewCrData, 0, ret, YData.Length + NewCbData.Length, NewCrData.Length);
            return ret;
        }

        internal static byte[] notJpegDecoding(byte[] imageData, int width = 1920, int height = 1080)
        {
            byte[] Ydata = new byte[width * height];
            byte[] CbData = new byte[(width / 4) * (height / 4)];
            byte[] CrData = new byte[(width / 4) * (height / 4)];
            Buffer.BlockCopy(imageData, 0, Ydata, 0, Ydata.Length);
            Buffer.BlockCopy(imageData, Ydata.Length, CbData, 0, CbData.Length);
            Buffer.BlockCopy(imageData, Ydata.Length + CbData.Length, CrData, 0, CrData.Length);
            byte[] newCb = extrapolate(CbData);
            byte[] newCr = extrapolate(CrData);
            byte[] ret = new byte[3 * width * height];
            for (int i = 0; i < ret.Length; i+=3)
            {
                int pos = i / 3;
                ret[i] = Ydata[pos];
                ret[i+1] = newCb[pos];
                ret[i+2] = newCr[pos];
            }
            convertToRGBInPlace(ref ret);
            return ret;
        }
    }
}
