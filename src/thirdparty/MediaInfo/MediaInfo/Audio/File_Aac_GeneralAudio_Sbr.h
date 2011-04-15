// File_Aac - Info for AAC files
// Copyright (C) 2008-2011 MediaArea.net SARL, Info@MediaArea.net
//
// This library is free software: you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this library. If not, see <http://www.gnu.org/licenses/>.
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

namespace MediaInfoLib
{

const int8s t_huffman_env_1_5dB[120][2]=
{
    {   1,    2 },
    { -64,  -65 },
    {   3,    4 },
    { -63,  -66 },
    {   5,    6 },
    { -62,  -67 },
    {   7,    8 },
    { -61,  -68 },
    {   9,   10 },
    { -60,  -69 },
    {  11,   12 },
    { -59,  -70 },
    {  13,   14 },
    { -58,  -71 },
    {  15,   16 },
    { -57,  -72 },
    {  17,   18 },
    { -73,  -56 },
    {  19,   21 },
    { -74,   20 },
    { -55,  -75 },
    {  22,   26 },
    {  23,   24 },
    { -54,  -76 },
    { -77,   25 },
    { -53,  -78 },
    {  27,   34 },
    {  28,   29 },
    { -52,  -79 },
    {  30,   31 },
    { -80,  -51 },
    {  32,   33 },
    { -83,  -82 },
    { -81,  -50 },
    {  35,   57 },
    {  36,   40 },
    {  37,   38 },
    { -88,  -84 },
    { -48,   39 },
    { -90,  -85 },
    {  41,   46 },
    {  42,   43 },
    { -49,  -87 },
    {  44,   45 },
    { -89,  -86 },
    {-124, -123 },
    {  47,   50 },
    {  48,   49 },
    {-122, -121 },
    {-120, -119 },
    {  51,   54 },
    {  52,   53 },
    {-118, -117 },
    {-116, -115 },
    {  55,   56 },
    {-114, -113 },
    {-112, -111 },
    {  58,   89 },
    {  59,   74 },
    {  60,   67 },
    {  61,   64 },
    {  62,   63 },
    {-110, -109 },
    {-108, -107 },
    {  65,   66 },
    {-106, -105 },
    {-104, -103 },
    {  68,   71 },
    {  69,   70 },
    {-102, -101 },
    {-100,  -99 },
    {  72,   73 },
    { -98,  -97 },
    { -96,  -95 },
    {  75,   82 },
    {  76,   79 },
    {  77,   78 },
    { -94,  -93 },
    { -92,  -91 },
    {  80,   81 },
    { -47,  -46 },
    { -45,  -44 },
    {  83,   86 },
    {  84,   85 },
    { -43,  -42 },
    { -41,  -40 },
    {  87,   88 },
    { -39,  -38 },
    { -37,  -36 },
    {  90,  105 },
    {  91,   98 },
    {  92,   95 },
    {  93,   94 },
    { -35,  -34 },
    { -33,  -32 },
    {  96,   97 },
    { -31,  -30 },
    { -29,  -28 },
    {  99,  102 },
    { 100,  101 },
    { -27,  -26 },
    { -25,  -24 },
    { 103,  104 },
    { -23,  -22 },
    { -21,  -20 },
    { 106,  113 },
    { 107,  110 },
    { 108,  109 },
    { -19,  -18 },
    { -17,  -16 },
    { 111,  112 },
    { -15,  -14 },
    { -13,  -12 },
    { 114,  117 },
    { 115,  116 },
    { -11,  -10 },
    {  -9,   -8 },
    { 118,  119 },
    {  -7,   -6 },
    {  -5,   -4 }
};

const int8s f_huffman_env_1_5dB[120][2]=
{
    {   1,    2 },
    { -64,  -65 },
    {   3,    4 },
    { -63,  -66 },
    {   5,    6 },
    { -67,  -62 },
    {   7,    8 },
    { -68,  -61 },
    {   9,   10 },
    { -69,  -60 },
    {  11,   13 },
    { -70,   12 },
    { -59,  -71 },
    {  14,   16 },
    { -58,   15 },
    { -72,  -57 },
    {  17,   19 },
    { -73,   18 },
    { -56,  -74 },
    {  20,   23 },
    {  21,   22 },
    { -55,  -75 },
    { -54,  -53 },
    {  24,   27 },
    {  25,   26 },
    { -76,  -52 },
    { -77,  -51 },
    {  28,   31 },
    {  29,   30 },
    { -50,  -78 },
    { -79,  -49 },
    {  32,   36 },
    {  33,   34 },
    { -48,  -47 },
    { -80,   35 },
    { -81,  -82 },
    {  37,   47 },
    {  38,   41 },
    {  39,   40 },
    { -83,  -46 },
    { -45,  -84 },
    {  42,   44 },
    { -85,   43 },
    { -44,  -43 },
    {  45,   46 },
    { -88,  -87 },
    { -86,  -90 },
    {  48,   66 },
    {  49,   56 },
    {  50,   53 },
    {  51,   52 },
    { -92,  -42 },
    { -41,  -39 },
    {  54,   55 },
    {-105,  -89 },
    { -38,  -37 },
    {  57,   60 },
    {  58,   59 },
    { -94,  -91 },
    { -40,  -36 },
    {  61,   63 },
    { -20,   62 },
    {-115, -110 },
    {  64,   65 },
    {-108, -107 },
    {-101,  -97 },
    {  67,   89 },
    {  68,   75 },
    {  69,   72 },
    {  70,   71 },
    { -95,  -93 },
    { -34,  -27 },
    {  73,   74 },
    { -22,  -17 },
    { -16, -124 },
    {  76,   82 },
    {  77,   79 },
    {-123,   78 },
    {-122, -121 },
    {  80,   81 },
    {-120, -119 },
    {-118, -117 },
    {  83,   86 },
    {  84,   85 },
    {-116, -114 },
    {-113, -112 },
    {  87,   88 },
    {-111, -109 },
    {-106, -104 },
    {  90,  105 },
    {  91,   98 },
    {  92,   95 },
    {  93,   94 },
    {-103, -102 },
    {-100,  -99 },
    {  96,   97 },
    { -98,  -96 },
    { -35,  -33 },
    {  99,  102 },
    { 100,  101 },
    { -32,  -31 },
    { -30,  -29 },
    { 103,  104 },
    { -28,  -26 },
    { -25,  -24 },
    { 106,  113 },
    { 107,  110 },
    { 108,  109 },
    { -23,  -21 },
    { -19,  -18 },
    { 111,  112 },
    { -15,  -14 },
    { -13,  -12 },
    { 114,  117 },
    { 115,  116 },
    { -11,  -10 },
    {  -9,   -8 },
    { 118,  119 },
    {  -7,   -6 },
    {  -5,   -4 }
};

const int8s t_huffman_env_bal_1_5dB[48][2]=
{
    { -64,    1 },
    { -63,    2 },
    { -65,    3 },
    { -62,    4 },
    { -66,    5 },
    { -61,    6 },
    { -67,    7 },
    { -60,    8 },
    { -68,    9 },
    {  10,   11 },
    { -69,  -59 },
    {  12,   13 },
    { -70,  -58 },
    {  14,   28 },
    {  15,   21 },
    {  16,   18 },
    { -57,   17 },
    { -71,  -56 },
    {  19,   20 },
    { -88,  -87 },
    { -86,  -85 },
    {  22,   25 },
    {  23,   24 },
    { -84,  -83 },
    { -82,  -81 },
    {  26,   27 },
    { -80,  -79 },
    { -78,  -77 },
    {  29,   36 },
    {  30,   33 },
    {  31,   32 },
    { -76,  -75 },
    { -74,  -73 },
    {  34,   35 },
    { -72,  -55 },
    { -54,  -53 },
    {  37,   41 },
    {  38,   39 },
    { -52,  -51 },
    { -50,   40 },
    { -49,  -48 },
    {  42,   45 },
    {  43,   44 },
    { -47,  -46 },
    { -45,  -44 },
    {  46,   47 },
    { -43,  -42 },
    { -41,  -40 }
};

const int8s f_huffman_env_bal_1_5dB[48][2]=
{
    { -64,    1 },
    { -65,    2 },
    { -63,    3 },
    { -66,    4 },
    { -62,    5 },
    { -61,    6 },
    { -67,    7 },
    { -68,    8 },
    { -60,    9 },
    {  10,   11 },
    { -69,  -59 },
    { -70,   12 },
    { -58,   13 },
    {  14,   17 },
    { -71,   15 },
    { -57,   16 },
    { -56,  -73 },
    {  18,   32 },
    {  19,   25 },
    {  20,   22 },
    { -72,   21 },
    { -88,  -87 },
    {  23,   24 },
    { -86,  -85 },
    { -84,  -83 },
    {  26,   29 },
    {  27,   28 },
    { -82,  -81 },
    { -80,  -79 },
    {  30,   31 },
    { -78,  -77 },
    { -76,  -75 },
    {  33,   40 },
    {  34,   37 },
    {  35,   36 },
    { -74,  -55 },
    { -54,  -53 },
    {  38,   39 },
    { -52,  -51 },
    { -50,  -49 },
    {  41,   44 },
    {  42,   43 },
    { -48,  -47 },
    { -46,  -45 },
    {  45,   46 },
    { -44,  -43 },
    { -42,   47 },
    { -41,  -40 }
};

const int8s t_huffman_env_3_0dB[62][2]=
{
    { -64,    1 },
    { -65,    2 },
    { -63,    3 },
    { -66,    4 },
    { -62,    5 },
    { -67,    6 },
    { -61,    7 },
    { -68,    8 },
    { -60,    9 },
    {  10,   11 },
    { -69,  -59 },
    {  12,   14 },
    { -70,   13 },
    { -71,  -58 },
    {  15,   18 },
    {  16,   17 },
    { -72,  -57 },
    { -73,  -74 },
    {  19,   22 },
    { -56,   20 },
    { -55,   21 },
    { -54,  -77 },
    {  23,   31 },
    {  24,   25 },
    { -75,  -76 },
    {  26,   27 },
    { -78,  -53 },
    {  28,   29 },
    { -52,  -95 },
    { -94,   30 },
    { -93,  -92 },
    {  32,   47 },
    {  33,   40 },
    {  34,   37 },
    {  35,   36 },
    { -91,  -90 },
    { -89,  -88 },
    {  38,   39 },
    { -87,  -86 },
    { -85,  -84 },
    {  41,   44 },
    {  42,   43 },
    { -83,  -82 },
    { -81,  -80 },
    {  45,   46 },
    { -79,  -51 },
    { -50,  -49 },
    {  48,   55 },
    {  49,   52 },
    {  50,   51 },
    { -48,  -47 },
    { -46,  -45 },
    {  53,   54 },
    { -44,  -43 },
    { -42,  -41 },
    {  56,   59 },
    {  57,   58 },
    { -40,  -39 },
    { -38,  -37 },
    {  60,   61 },
    { -36,  -35 },
    { -34,  -33 }
};

const int8s f_huffman_env_3_0dB[62][2]=
{
    { -64,    1 },
    { -65,    2 },
    { -63,    3 },
    { -66,    4 },
    { -62,    5 },
    { -67,    6 },
    {   7,    8 },
    { -61,  -68 },
    {   9,   10 },
    { -60,  -69 },
    {  11,   12 },
    { -59,  -70 },
    {  13,   14 },
    { -58,  -71 },
    {  15,   16 },
    { -57,  -72 },
    {  17,   19 },
    { -56,   18 },
    { -55,  -73 },
    {  20,   24 },
    {  21,   22 },
    { -74,  -54 },
    { -53,   23 },
    { -75,  -76 },
    {  25,   30 },
    {  26,   27 },
    { -52,  -51 },
    {  28,   29 },
    { -77,  -79 },
    { -50,  -49 },
    {  31,   39 },
    {  32,   35 },
    {  33,   34 },
    { -78,  -46 },
    { -82,  -88 },
    {  36,   37 },
    { -83,  -48 },
    { -47,   38 },
    { -86,  -85 },
    {  40,   47 },
    {  41,   44 },
    {  42,   43 },
    { -80,  -44 },
    { -43,  -42 },
    {  45,   46 },
    { -39,  -87 },
    { -84,  -40 },
    {  48,   55 },
    {  49,   52 },
    {  50,   51 },
    { -95,  -94 },
    { -93,  -92 },
    {  53,   54 },
    { -91,  -90 },
    { -89,  -81 },
    {  56,   59 },
    {  57,   58 },
    { -45,  -41 },
    { -38,  -37 },
    {  60,   61 },
    { -36,  -35 },
    { -34,  -33 }
};

const int8s t_huffman_env_bal_3_0dB[24][2]=
{
    { -64,    1 },
    { -63,    2 },
    { -65,    3 },
    { -66,    4 },
    { -62,    5 },
    { -61,    6 },
    { -67,    7 },
    { -68,    8 },
    { -60,    9 },
    {  10,   16 },
    {  11,   13 },
    { -69,   12 },
    { -76,  -75 },
    {  14,   15 },
    { -74,  -73 },
    { -72,  -71 },
    {  17,   20 },
    {  18,   19 },
    { -70,  -59 },
    { -58,  -57 },
    {  21,   22 },
    { -56,  -55 },
    { -54,   23 },
    { -53,  -52 }
};

const int8s f_huffman_env_bal_3_0dB[24][2]=
{
    { -64,    1 },
    { -65,    2 },
    { -63,    3 },
    { -66,    4 },
    { -62,    5 },
    { -61,    6 },
    { -67,    7 },
    { -68,    8 },
    { -60,    9 },
    {  10,   13 },
    { -69,   11 },
    { -59,   12 },
    { -58,  -76 },
    {  14,   17 },
    {  15,   16 },
    { -75,  -74 },
    { -73,  -72 },
    {  18,   21 },
    {  19,   20 },
    { -71,  -70 },
    { -57,  -56 },
    {  22,   23 },
    { -55,  -54 },
    { -53,  -52 }
};

const int8s t_huffman_noise_3_0dB[62][2]=
{
    { -64,    1 },
    { -63,    2 },
    { -65,    3 },
    { -66,    4 },
    { -62,    5 },
    { -67,    6 },
    {   7,    8 },
    { -61,  -68 },
    {   9,   30 },
    {  10,   15 },
    { -60,   11 },
    { -69,   12 },
    {  13,   14 },
    { -59,  -53 },
    { -95,  -94 },
    {  16,   23 },
    {  17,   20 },
    {  18,   19 },
    { -93,  -92 },
    { -91,  -90 },
    {  21,   22 },
    { -89,  -88 },
    { -87,  -86 },
    {  24,   27 },
    {  25,   26 },
    { -85,  -84 },
    { -83,  -82 },
    {  28,   29 },
    { -81,  -80 },
    { -79,  -78 },
    {  31,   46 },
    {  32,   39 },
    {  33,   36 },
    {  34,   35 },
    { -77,  -76 },
    { -75,  -74 },
    {  37,   38 },
    { -73,  -72 },
    { -71,  -70 },
    {  40,   43 },
    {  41,   42 },
    { -58,  -57 },
    { -56,  -55 },
    {  44,   45 },
    { -54,  -52 },
    { -51,  -50 },
    {  47,   54 },
    {  48,   51 },
    {  49,   50 },
    { -49,  -48 },
    { -47,  -46 },
    {  52,   53 },
    { -45,  -44 },
    { -43,  -42 },
    {  55,   58 },
    {  56,   57 },
    { -41,  -40 },
    { -39,  -38 },
    {  59,   60 },
    { -37,  -36 },
    { -35,   61 },
    { -34,  -33 }
};

const int8s t_huffman_noise_bal_3_0dB[24][2]=
{
    { -64,    1 },
    { -65,    2 },
    { -63,    3 },
    {   4,    9 },
    { -66,    5 },
    { -62,    6 },
    {   7,    8 },
    { -76,  -75 },
    { -74,  -73 },
    {  10,   17 },
    {  11,   14 },
    {  12,   13 },
    { -72,  -71 },
    { -70,  -69 },
    {  15,   16 },
    { -68,  -67 },
    { -61,  -60 },
    {  18,   21 },
    {  19,   20 },
    { -59,  -58 },
    { -57,  -56 },
    {  22,   23 },
    { -55,  -54 },
    { -53,  -52 }
};

//---------------------------------------------------------------------------
// Master frequency band table
// k0 = lower frequency boundary
const int8s Aac_k0_offset_16[16]=
{
     -8, -7, -6, -5, -4, -3, -2, -1,  0,  1,  2,  3,  4,  5,  6,  7,
};
const int8s Aac_k0_offset_22[16]=
{
     -5, -4, -3, -2, -1,  0,  1,  2,  3,  4,  5,  6,  7,  9, 11, 13,
};
const int8s Aac_k0_offset_24[16]=
{
     -5, -3, -2, -1,  0,  1,  2,  3,  4,  5,  6,  7,  9, 11, 13, 16,
};
const int8s Aac_k0_offset_32[16]=
{
     -6, -4, -2, -1,  0,  1,  2,  3,  4,  5,  6,  7,  9, 11, 13, 16,
};
const int8s Aac_k0_offset_64[16]=
{
     -4, -2, -1,  0,  1,  2,  3,  4,  5,  6,  7,  9, 11, 13, 16, 20,
};
const int8s Aac_k0_offset_96[16]=
{
     -2, -1,  0,  1,  2,  3,  4,  5,  6,  7,  9, 11, 13, 16, 20, 24,
};
const int8s* Aac_k0_offset[9]=
{
    Aac_k0_offset_96,       //96000
    Aac_k0_offset_96,       //88200
    Aac_k0_offset_64,       //64000
    Aac_k0_offset_64,       //48000
    Aac_k0_offset_64,       //44100
    Aac_k0_offset_32,       //32000
    Aac_k0_offset_24,       //24000
    Aac_k0_offset_22,       //22050
    Aac_k0_offset_16,       //16000
};
const int8u Aac_k0_startMin[9]=
{
     7,  7, 10, 11, 12, 16, 16, 17, 24, //32, 35, 48,  0,  0,  0,  0,
};

//---------------------------------------------------------------------------
// Master frequency band table
// k2 = upper frequency boundary
const int8u Aac_k2_stopMin[9]=
{
    13, 15, 20, 21, 23, 32, 32, 35, 48, //64, 70, 96,  0,  0,  0,  0,
};
const int8s Aac_k2_offset[9][14]=
{
    { 0,  2,  4,  6,  8, 11, 14, 18, 22, 26, 31, 37, 44, 51 },
    { 0,  2,  4,  6,  8, 11, 14, 18, 22, 26, 31, 36, 42, 49 },
    { 0,  2,  4,  6,  8, 11, 14, 17, 21, 25, 29, 34, 39, 44 },
    { 0,  2,  4,  6,  8, 11, 14, 17, 20, 24, 28, 33, 38, 43 },
    { 0,  2,  4,  6,  8, 11, 14, 17, 20, 24, 28, 32, 36, 41 },
    { 0,  2,  4,  6,  8, 10, 12, 14, 17, 20, 23, 26, 29, 32 },
    { 0,  2,  4,  6,  8, 10, 12, 14, 17, 20, 23, 26, 29, 32 },
    { 0,  1,  3,  5,  7,  9, 11, 13, 15, 17, 20, 23, 26, 29 },
    { 0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 12, 14, 16 },
};

} //NameSpace

