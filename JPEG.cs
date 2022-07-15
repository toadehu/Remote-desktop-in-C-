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
        private static int finalLength = 0;
        private static int finalLength1 = 0;
        private static int finalLength2 = 0;

        /// <summary>
        /// Struct used to represent an 8x8 block
        /// </summary>
        private struct Block
        {
            public byte[][] A;
            public Block(int sz)
            {
                A = new byte[sz][];
                for (int i = 0; i < sz; ++i)
                {
                    A[i] = new byte[8];
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
            byte[] retByteArray = new byte[width * ratio * height * ratio];
            int cPos = 0;
            for (int i = 0; i < width; ++i)
            {
                byte[] tempByteArr = new byte[height];
                for (int j = 0; j < height; ++j)
                {
                    tempByteArr[ratio * j] = pixelData[i * height + j];
                }
                for (int r = 0; r < ratio; ++r)
                {
                    for (int j = 0; j < height; ++j)
                    {
                        for (int r2 = 0; r2 < ratio; ++r2)
                        {
                            retByteArray[cPos++] = tempByteArr[j];
                        }
                    }
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
            int numberOfBlocks = ((width + 7) / 8) * ((height + 7) / 8);
            blocks = new Block[numberOfBlocks];
            int curPos = 0;
            Block temp;
            temp.A = new byte[8][];
            for (int k = 0; k < 8; ++k)
            {
                temp.A[k] = new byte[8];
            }
            for (int i = 0; i < width; i += 8)
            {
                for (int j = 0; j < height; j += 8)
                {
                    for (int ci = i; ci < i + 8; ++ci)
                    {
                        for (int cj = j; cj < j + 8; ++cj)
                        {
                            int cpos = ci * height + cj;
                            byte val = 0;
                            if (cj > height)
                            {
                                if (cj == height)
                                {
                                    val = temp.A[ci - i - 1][0];
                                }
                                else
                                {
                                    val = temp.A[ci - i][cj - j - 1];
                                }
                            }
                            else
                            {
                                if (ci > width)
                                {
                                    val = temp.A[ci - i - 1][cj - j];
                                }
                                else
                                {
                                    if (ci < width && cj < height)
                                    {
                                        val = channelData[cpos];
                                    }
                                }
                            }
                            temp.A[ci - i][cj - j] = val;
                        }
                    }
                    blocks[curPos++] = temp;
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
            double val1 = (v[0] - v[7]) * C[1, 0] + (v[1] - v[6]) * C[1, 1] + (v[2] - v[5]) * C[1, 2] + (v[3] - v[4]) * C[1, 3];
            double val2 = (v[0] + v[7] - v[3] - v[4]) * C[2, 0] + (v[1] + v[6] - v[2] - v[5]) * C[2, 1]; ;
            double val3 = (v[0] - v[7]) * C[3, 0] + (v[1] - v[6]) * C[3, 1] + (v[2] - v[5]) * C[3, 2] + (v[3] - v[4]) * C[3, 3];
            double val4 = (v[0] - v[1] - v[2] + v[3] + v[4] - v[5] - v[6] + v[7]) * C[4, 0];
            double val5 = (v[0] - v[7]) * C[5, 0] + (v[1] - v[6]) * C[5, 1] + (v[2] - v[5]) * C[5, 2] + (v[3] - v[4]) * C[5, 3];
            double val6 = (v[0] + v[7] - v[3] - v[4]) * C[6, 0] + (v[1] + v[6] - v[2] - v[5]) * C[6, 1];
            double val7 = (v[0] - v[7]) * C[7, 0] + (v[1] - v[6]) * C[7, 1] + (v[2] - v[5]) * C[7, 2] + (v[3] - v[4]) * C[7, 3];
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
        private static void DoublesDCT1D(in double[] v, ref double[] ret)
        {
            double val0 = (v[0] + v[1] + v[2] + v[3] + v[4] + v[5] + v[6] + v[7]) * C[0, 0];
            double val1 = (v[0] - v[7]) * C[1, 0] + (v[1] - v[6]) * C[1, 1] + (v[2] - v[5]) * C[1, 2] + (v[3] - v[4]) * C[1, 3];
            double val2 = (v[0] + v[7] - v[3] - v[4]) * C[2, 0] + (v[1] + v[6] - v[2] - v[5]) * C[2, 1]; ;
            double val3 = (v[0] - v[7]) * C[3, 0] + (v[1] - v[6]) * C[3, 1] + (v[2] - v[5]) * C[3, 2] + (v[3] - v[4]) * C[3, 3];
            double val4 = (v[0] - v[1] - v[2] + v[3] + v[4] - v[5] - v[6] + v[7]) * C[4, 0];
            double val5 = (v[0] - v[7]) * C[5, 0] + (v[1] - v[6]) * C[5, 1] + (v[2] - v[5]) * C[5, 2] + (v[3] - v[4]) * C[5, 3];
            double val6 = (v[0] + v[7] - v[3] - v[4]) * C[6, 0] + (v[1] + v[6] - v[2] - v[5]) * C[6, 1];
            double val7 = (v[0] - v[7]) * C[7, 0] + (v[1] - v[6]) * C[7, 1] + (v[2] - v[5]) * C[7, 2] + (v[3] - v[4]) * C[7, 3];
            ret[0] = val0;
            ret[1] = val1;
            ret[2] = val2;
            ret[3] = val3;
            ret[4] = val4;
            ret[5] = val5;
            ret[6] = val6;
            ret[7] = val7;
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
            v[4] = val0f - val0s;
            v[5] = val1f - val1s;
            v[6] = val2f - val2s;
            v[7] = val3f - val3s;
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
        /// 
        /// </summary>
        /// <param name="bl"></param>
        /// <param name="Q"></param>
        /// <param name="phi"></param>
        private static void Quantize(ref double[][] bl, int[,] Q, int phi)
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
                    bl[i][j] /= Math.Ceiling((double)((Q[i, j] * S + 50) / 100));
                }
            }
        }

        private static void QuantizeI(ref Block orig, double[][] bl, int[,] Q, int phi)
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
                    bl[i][j] = orig.A[i][j] * Math.Ceiling((double)((Q[i, j] * S + 50) / 100));
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
                DCT1D(in bl.A[i], ref tempBlock[i]);
            }
            transpose(ref tempBlock);
            for (int i = 0; i < 8; ++i)
            {
                DoublesDCT1D(in tempBlock[i], ref tempBlock[i]);
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
                    bl.A[i][j] = (byte)(char)(int)tempBlock[i][j];
                }
            }

        }

        private static void IQuantizationAndIDCT(ref Block bl, bool type)
        {
            double[][]? iBl = null;
            switch (type)
            {
                case true:
                    QuantizeI(ref bl, iBl, LQM, 75);
                    break;
                case false:
                    QuantizeI(ref bl, iBl, CQM, 75);
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
                if (block.A[_posX[i]][_posY[i]] != 0)
                {
                    retValue[sizeOf++] = nrApp;
                    nrApp = 0;
                    retValue[sizeOf++] = block.A[_posX[i]][_posY[i]];
                    continue;
                }
                nrApp++;
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
                    imageData[i * height + j] = blockData[curBlock].A[xPos][yPos];
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
            int lengthPerChannel = imageData.Length / 3;
            byte[] convertedData = new byte[imageData.Length];
            for (int i = 0; i < lengthPerChannel; i += 3)
            {
                int R = (int)imageData[i + 2];
                int G = (int)imageData[i + 1];
                int B = (int)imageData[i];
                double Y, Cb, Cr;
                Y = (0.299 * R) + (0.587 * G) + (0.114 * B);
                Cb = 128 - (0.168736 * R) - (0.331264 * G) - (0.5 * B);
                Cr = 128 + (0.5 * R) - (0.418688 * G) - (0.081312 * B);
                /*byte[] conversion = RGBToYCbCr(imageData[i],
                    imageData[i + lengthPerChannel], imageData[i + 2 * lengthPerChannel]);*/
                convertedData[i] = (byte)(char)(int)Y;
                convertedData[i + lengthPerChannel] = (byte)(char)(int)Cb;
                convertedData[i + lengthPerChannel * 2] = (byte)(char)(int)Cr;
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

        /// <summary>
        /// 
        /// </summary>
        /// <param name="imageData"></param>
        /// <param name="width"></param>
        /// <param name="height"></param>
        /// <returns></returns>
        internal static byte[] convertImage(byte[] imageData, int width = 1920, int height = 1080)
        {
            byte[] YData = new byte[width * height];
            byte[] CbData = new byte[width * height];
            byte[] CrData = new byte[width * height];
            int offset = height * width;
            byte[] YCbCrData = convertToYCbCr(imageData);
            // 12ms
            Buffer.BlockCopy(YCbCrData, 0, YData, 0, YData.Length);
            Buffer.BlockCopy(YCbCrData, YData.Length, CbData, 0, CbData.Length);
            Buffer.BlockCopy(YCbCrData, YData.Length + CbData.Length, CbData, 0, CrData.Length);
            //11ms in release
            //return null;
            // 14ms
            /*for (int i = 0; i < width; ++i)
            {
                for (int j = 0; j < height; ++j)
                {
                    int cPos = i * height + j;
                    /*if (cPos > offset)
                    {
                        MessageBox.Show(i.ToString() + " " + j.ToString() + " <> " + cPos.ToString() + " " + (i*height).ToString());
                    }
                    YData[cPos] = YCbCrData[cPos];
                    CbData[cPos] = YCbCrData[cPos + offset];
                    CrData[cPos] = YCbCrData[cPos + (offset<<1)];
                }
            }*/
            byte[]? NewCrData = null;
            Thread NewCr = new Thread(
                () => NewCrData = subsampling(CrData));
            NewCr.Start();
            byte[] NewCbData = subsampling(CbData);
            NewCr.Join();
            Block[] YBlocks = new Block[1];
            Thread YBlThread = new Thread(
                () => splitIntoBlocks(YData, ref YBlocks, 1920, 1080));
            YBlThread.Start();
            Block[]? CbBlocks = null;
            splitIntoBlocks(NewCbData, ref CbBlocks);
            Block[]? CrBlocks = null;
            splitIntoBlocks(NewCrData, ref CrBlocks);
            YBlThread.Join();
            return null;
            //in debug splitting takes ~30ms
            int lg = YBlocks.Length / 2;
            Thread Ythread1 = new Thread(
                () =>
                {
                    for (int i = 0; i < lg; ++i)
                    {
                        DCTandQuantization(ref YBlocks[i], true);
                    }
                });
            Ythread1.Start();
            Thread Ythread2 = new Thread(
                () =>
                {
                    for (int i = lg; i < YBlocks.Length; ++i)
                    {
                        DCTandQuantization(ref YBlocks[i], true);
                    }
                });
            Ythread2.Start();
            /*Thread Cbthread = new Thread(
                () => applyDCT(ref CbBlocks, false));
            Cbthread.Start();*/
            applyDCT(ref CrBlocks, false);
            applyDCT(ref CbBlocks, false);
            Ythread2.Join();
            //DCT Chen on the Y channel takes ~175ms in debug, however i have the resources to compute it in 2 threads at the same time, hopefully loeffler can save the day
            //return null;
            byte[][] compressedBlocks = new byte[YBlocks.Length + 2 * CbBlocks.Length][];
            byte[]? retByteArr = null;
            finalLength = 0;
            finalLength1 = 0;
            finalLength2 = 0;
            int L = YBlocks.Length / 2;
            Thread Ythr1 = new Thread(
                () =>
                {
                    for (int i = 0; i < L; ++i)
                    {
                        compressedBlocks[i] = finalEncoding(YBlocks[i]);
                        finalLength1 += compressedBlocks[i].Length;
                    }
                });
            Ythr1.Start();
            Thread Ythr2 = new Thread(
                () =>
                {
                    for (int i = L; i < YBlocks.Length; ++i)
                    {
                        compressedBlocks[i] = finalEncoding(YBlocks[i]);
                        finalLength2 += compressedBlocks[i].Length;
                    }
                });
            Ythr2.Start();
            for (int i = 0; i < CbBlocks.Length; ++i)
            {
                compressedBlocks[i + YBlocks.Length] = finalEncoding(CbBlocks[i]);
                finalLength += compressedBlocks[i + YBlocks.Length].Length;
                compressedBlocks[i + YBlocks.Length + CbBlocks.Length] = finalEncoding(CrBlocks[i]);
                finalLength += compressedBlocks[i + YBlocks.Length + CbBlocks.Length].Length;
            }
            Ythr2.Join();
            finalLength += (finalLength1 + finalLength2);
            retByteArr = new byte[finalLength];
            int retArrPos = 0;
            //Buffer.BlockCopy(compressedBlocks, 0, retByteArr, 0, finalLength);
            //System.Windows.Forms.MessageBox.Show(finalLength.ToString());
            for (int i = 0; i < YBlocks.Length + 2 * CbBlocks.Length; ++i)
            {
                for (int j = 0; j < compressedBlocks[i].Length; ++j)
                {
                    retByteArr[retArrPos++] = compressedBlocks[i][j];
                    //System.Windows.Forms.MessageBox.Show(retArrPos.ToString() + " " + retByteArr.Length.ToString() + "\n" +
                    //i.ToString() + " " + j.ToString() + " " + (YBlocks.Length + 2 * CbBlocks.Length).ToString());
                }
            }
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
            for (int i = 0; i < data.Length; i += 2)
            {
                nrVals += data[i] + 1;
                if (nrVals == 64)
                {
                    int cpos = 0;
                    for (int j = prevPos; j < i; j += 2)
                    {
                        int nr0 = data[i];
                        while (nr0 != 0)
                        {
                            rets[idBl].A[_posX[cpos]][_posY[cpos]] = 0;
                            nr0--;
                            cpos++;
                        }
                        rets[idBl].A[_posX[cpos]][_posY[cpos]] = data[i + 1];
                        cpos++;
                    }
                    idBl++;
                }
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
        /// The compression ratio is assumed to be 4</returns>
        internal static byte[] recoverImage(byte[] compData, int width = 1920, int height = 1080)
        {
            byte[] imageData = new byte[width * height * 3];
            int tempSum = 0, noBl = 0;
            for (int i = 0; i < compData.Length; i+=2)
            {
                tempSum += compData[i];
                tempSum += compData[i + 1];
                if (tempSum == 64)
                {
                    noBl++;
                    tempSum = 0;
                }
            }
            Block[] encodedBlocks = getBlocks(compData, noBl);
            int nrYbl = (width + 7) / 8 * (height + 7) / 8;
            int cBl = (encodedBlocks.Length - nrYbl)/ 2;
            Thread Ythr1 = new Thread(
                () =>
                {
                    for (int i = 0; i < nrYbl/2; ++i)
                    {
                        IQuantizationAndIDCT(ref encodedBlocks[i], true);
                    }
                });
            Thread Ythr2 = new Thread(
                () =>
                {
                    for (int i = nrYbl/2; i < nrYbl; ++i)
                    {
                        IQuantizationAndIDCT(ref encodedBlocks[i], true);
                    }
                });
            Ythr1.Start();
            Ythr2.Start();
            for (int i = nrYbl; i < encodedBlocks.Length; ++i)
            {
                IQuantizationAndIDCT(ref encodedBlocks[i], false);
            }
            Ythr2.Join();
            return imageData;
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
            bmpData = bmp.LockBits(rect, System.Drawing.Imaging.ImageLockMode.WriteOnly, bmp.PixelFormat);
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
    }
}
