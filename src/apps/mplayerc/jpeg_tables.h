/*
 * $Id: jpeg_tables.h 2786 2010-12-17 16:42:55Z XhmikosR $
 *
 * (C) 2003-2006 Gabest
 * (C) 2006-2010 see AUTHORS
 *
 * This file is part of mplayerc.
 *
 * Mplayerc is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * Mplayerc is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#pragma once

/* Tables */

static unsigned char quanttbl[2][64] = {
	{
		16,
		11, 12,
		14, 12, 10,
		16, 14, 13, 14,
		18, 17, 16, 19, 24,
		40, 26, 24, 22, 22, 24,
		49, 35, 37, 29, 40, 58, 51,
		61, 30, 57, 51, 56, 55, 64, 72,
		92, 78, 64, 68, 87, 69, 55,
		56, 80, 109, 81, 87, 95,
		98, 103, 104, 103, 62,
		77, 113, 121, 112,
		100, 120, 92,
		101, 103,
		99
	},
	{
		17,
		18, 18,
		24, 21, 24,
		47, 26, 26, 47,
		99, 66, 56, 66, 99,
		99, 99, 99, 99, 99, 99,
		99, 99, 99, 99, 99, 99, 99,
		99, 99, 99, 99, 99, 99, 99, 99,
		99, 99, 99, 99, 99, 99, 99,
		99, 99, 99, 99, 99, 99,
		99, 99, 99, 99, 99,
		99, 99, 99, 99,
		99, 99, 99,
		99, 99,
		99
	}
};

static unsigned char zigzagU[64] = {
	0,
	1, 0,
	0, 1, 2,
	3, 2, 1, 0,
	0, 1, 2, 3, 4,
	5, 4, 3, 2, 1, 0,
	0, 1, 2, 3, 4, 5, 6,
	7, 6, 5, 4, 3, 2, 1, 0,
	1, 2, 3, 4, 5, 6, 7,
	7, 6, 5, 4, 3, 2,
	3, 4, 5, 6, 7,
	7, 6, 5, 4,
	5, 6, 7,
	7, 6,
	7,
};

static unsigned char zigzagV[64] = {
	0,
	0, 1,
	2, 1, 0,
	0, 1, 2, 3,
	4, 3, 2, 1, 0,
	0, 1, 2, 3, 4, 5,
	6, 5, 4, 3, 2, 1, 0,
	0, 1, 2, 3, 4, 5, 6, 7,
	7, 6, 5, 4, 3, 2, 1,
	2, 3, 4, 5, 6, 7,
	7, 6, 5, 4, 3,
	4, 5, 6, 7,
	7, 6, 5,
	6, 7,
	7,
};

static unsigned short DCVLC[2][12] = {
	{0, 2, 3, 4, 5, 6, 14, 30, 62, 126, 254, 510},
	{0, 1, 2, 6, 14, 30, 62, 126, 254, 510, 1022, 2046}
};

static unsigned char DCVLC_Size[2][12] = {
	{2, 3, 3, 3, 3, 3, 4, 5, 6, 7, 8, 9},
	{2, 2, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11}
};

static unsigned char DCVLC_NumByLength[2][16] = {
	{0, 1, 5, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0},
	{0, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0}
};

static unsigned short ACVLC[2][16][11] = {
	{
		{10, 0, 1, 4, 11, 26, 120, 248, 1014, 65410, 65410},
		{0, 12, 27, 121, 502, 2038, 65412, 65413, 65414, 65415, 65416},
		{0, 28, 249, 1015, 4084, 65417, 65418, 65419, 65420, 65421, 65422},
		{0, 58, 503, 8181, 65423, 65424, 65425, 65426, 65427, 65428, 65429},
		{0, 59, 1016, 65430, 65431, 65432, 65433, 65434, 65435, 65436, 65437},
		{0, 122, 2039, 65438, 65439, 65440, 65441, 65442, 65443, 65444, 65445},
		{0, 123, 4086, 65446, 65447, 65448, 65449, 65450, 65451, 65452, 65453},
		{0, 250, 4087, 65454, 65455, 65456, 65457, 65458, 65459, 65460, 65461},
		{0, 504, 32704, 65462, 65463, 65464, 65465, 65466, 65467, 65468, 65469},
		{0, 505, 65470, 65471, 65472, 65473, 65474, 65475, 65476, 65477, 65478},
		{0, 506, 65479, 65480, 65481, 65482, 65483, 65484, 65485, 65486, 65487},
		{0, 1017, 65488, 65489, 65490, 65491, 65492, 65493, 65494, 65495, 65496},
		{0, 1018, 65497, 65498, 65499, 65500, 65501, 65502, 65503, 65504, 65505},
		{0, 2040, 65506, 65507, 65508, 65509, 65510, 65511, 65512, 65513, 65514},
		{0, 65515, 65516, 65517, 65518, 65519, 65520, 65521, 65522, 65523, 65524},
		{2041, 65525, 65526, 65527, 65528, 65529, 65530, 65531, 65532, 65533, 65534}
	},
	{
		{0, 1, 4, 10, 24, 25, 56, 120, 500, 1014, 4084},
		{0, 11, 57, 246, 501, 2038, 4085, 65416, 65417, 65418, 65419},
		{0, 26, 247, 1015, 4086, 32706, 65420, 65421, 65422, 65423, 65424},
		{0, 27, 248, 1016, 4087, 65425, 65426, 65427, 65428, 65429, 65430},
		{0, 58, 502, 65431, 65432, 65433, 65434, 65435, 65436, 65437, 65438},
		{0, 59, 1017, 65439, 65440, 65441, 65442, 65443, 65444, 65445, 65446},
		{0, 121, 2039, 65447, 65448, 65449, 65450, 65451, 65452, 65453, 65454},
		{0, 122, 2040, 65455, 65456, 65457, 65458, 65459, 65460, 65461, 65462},
		{0, 249, 65463, 65464, 65465, 65466, 65467, 65468, 65469, 65470, 65471},
		{0, 503, 65472, 65473, 65474, 65475, 65476, 65477, 65478, 65479, 65480},
		{0, 504, 65481, 65482, 65483, 65484, 65485, 65486, 65487, 65488, 65489},
		{0, 505, 65490, 65491, 65492, 65493, 65494, 65495, 65496, 65497, 65498},
		{0, 506, 65499, 65500, 65501, 65502, 65503, 65504, 65505, 65506, 65507},
		{0, 2041, 65508, 65509, 65510, 65511, 65512, 65513, 65514, 65515, 65516},
		{0, 16352, 65517, 65518, 65519, 65520, 65521, 65522, 65523, 65524, 65525},
		{1018, 32707, 65526, 65527, 65528, 65529, 65530, 65531, 65532, 65533, 65534}
	}
};

static unsigned char ACVLC_Size[2][16][11] = {
	{
		{4, 2, 2, 3, 4, 5, 7, 8, 10, 16, 16},
		{0, 4, 5, 7, 9, 11, 16, 16, 16, 16, 16},
		{0, 5, 8, 10, 12, 16, 16, 16, 16, 16, 16},
		{0, 6, 9, 12, 16, 16, 16, 16, 16, 16, 16},
		{0, 6, 10, 16, 16, 16, 16, 16, 16, 16, 16},
		{0, 7, 11, 16, 16, 16, 16, 16, 16, 16, 16},
		{0, 7, 12, 16, 16, 16, 16, 16, 16, 16, 16},
		{0, 8, 12, 16, 16, 16, 16, 16, 16, 16, 16},
		{0, 9, 15, 16, 16, 16, 16, 16, 16, 16, 16},
		{0, 9, 16, 16, 16, 16, 16, 16, 16, 16, 16},
		{0, 9, 16, 16, 16, 16, 16, 16, 16, 16, 16},
		{0, 10, 16, 16, 16, 16, 16, 16, 16, 16, 16},
		{0, 10, 16, 16, 16, 16, 16, 16, 16, 16, 16},
		{0, 11, 16, 16, 16, 16, 16, 16, 16, 16, 16},
		{0, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16},
		{11, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16}
	},
	{
		{2, 2, 3, 4, 5, 5, 6, 7, 9, 10, 12},
		{0, 4, 6, 8, 9, 11, 12, 16, 16, 16, 16},
		{0, 5, 8, 10, 12, 15, 16, 16, 16, 16, 16},
		{0, 5, 8, 10, 12, 16, 16, 16, 16, 16, 16},
		{0, 6, 9, 16, 16, 16, 16, 16, 16, 16, 16},
		{0, 6, 10, 16, 16, 16, 16, 16, 16, 16, 16},
		{0, 7, 11, 16, 16, 16, 16, 16, 16, 16, 16},
		{0, 7, 11, 16, 16, 16, 16, 16, 16, 16, 16},
		{0, 8, 16, 16, 16, 16, 16, 16, 16, 16, 16},
		{0, 9, 16, 16, 16, 16, 16, 16, 16, 16, 16},
		{0, 9, 16, 16, 16, 16, 16, 16, 16, 16, 16},
		{0, 9, 16, 16, 16, 16, 16, 16, 16, 16, 16},
		{0, 9, 16, 16, 16, 16, 16, 16, 16, 16, 16},
		{0, 11, 16, 16, 16, 16, 16, 16, 16, 16, 16},
		{0, 14, 16, 16, 16, 16, 16, 16, 16, 16, 16},
		{10, 15, 16, 16, 16, 16, 16, 16, 16, 16, 16}
	}
};

static unsigned char ACVLC_NumByLength[2][16] = {
	{0, 2, 1, 3, 3, 2, 4, 3, 5, 5, 4, 4, 0, 0, 1, 125},
	{0, 2, 1, 2, 4, 4, 3, 4, 7, 5, 4, 4, 0, 1, 2, 119}
};

static unsigned char ACVLC_Data[2][162] = {
	{
		0x01, 0x02, 0x03, 0x00, 0x04, 0x11, 0x05, 0x12,
		0x21, 0x31, 0x41, 0x06, 0x13, 0x51, 0x61, 0x07,
		0x22, 0x71, 0x14, 0x32, 0x81, 0x91, 0xA1, 0x08,
		0x23, 0x42, 0xB1, 0xC1, 0x15, 0x52, 0xD1, 0xF0,
		0x24, 0x33, 0x62, 0x72, 0x82, 0x09, 0x0A, 0x16,
		0x17, 0x18, 0x19, 0x1A, 0x25, 0x26, 0x27, 0x28,
		0x29, 0x2A, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39,
		0x3A, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49,
		0x4A, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59,
		0x5A, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69,
		0x6A, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79,
		0x7A, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89,
		0x8A, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98,
		0x99, 0x9A, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7,
		0xA8, 0xA9, 0xAA, 0xB2, 0xB3, 0xB4, 0xB5, 0xB6,
		0xB7, 0xB8, 0xB9, 0xBA, 0xC2, 0xC3, 0xC4, 0xC5,
		0xC6, 0xC7, 0xC8, 0xC9, 0xCA, 0xD2, 0xD3, 0xD4,
		0xD5, 0xD6, 0xD7, 0xD8, 0xD9, 0xDA, 0xE1, 0xE2,
		0xE3, 0xE4, 0xE5, 0xE6, 0xE7, 0xE8, 0xE9, 0xEA,
		0xF1, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6, 0xF7, 0xF8,
		0xF9, 0xFA
	},
	{
		0x00, 0x01, 0x02, 0x03, 0x11, 0x04, 0x05, 0x21,
		0x31, 0x06, 0x12, 0x41, 0x51, 0x07, 0x61, 0x71,
		0x13, 0x22, 0x32, 0x81, 0x08, 0x14, 0x42, 0x91,
		0xA1, 0xB1, 0xC1, 0x09, 0x23, 0x33, 0x52, 0xF0,
		0x15, 0x62, 0x72, 0xD1, 0x0A, 0x16, 0x24, 0x34,
		0xE1, 0x25, 0xF1, 0x17, 0x18, 0x19, 0x1A, 0x26,
		0x27, 0x28, 0x29, 0x2A, 0x35, 0x36, 0x37, 0x38,
		0x39, 0x3A, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48,
		0x49, 0x4A, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58,
		0x59, 0x5A, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68,
		0x69, 0x6A, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78,
		0x79, 0x7A, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87,
		0x88, 0x89, 0x8A, 0x92, 0x93, 0x94, 0x95, 0x96,
		0x97, 0x98, 0x99, 0x9A, 0xA2, 0xA3, 0xA4, 0xA5,
		0xA6, 0xA7, 0xA8, 0xA9, 0xAA, 0xB2, 0xB3, 0xB4,
		0xB5, 0xB6, 0xB7, 0xB8, 0xB9, 0xBA, 0xC2, 0xC3,
		0xC4, 0xC5, 0xC6, 0xC7, 0xC8, 0xC9, 0xCA, 0xD2,
		0xD3, 0xD4, 0xD5, 0xD6, 0xD7, 0xD8, 0xD9, 0xDA,
		0xE2, 0xE3, 0xE4, 0xE5, 0xE6, 0xE7, 0xE8, 0xE9,
		0xEA, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6, 0xF7, 0xF8,
		0xF9, 0xFA
	}
};
