/*
 * Misc image conversion routines
 * Copyright (c) 2001, 2002, 2003 Fabrice Bellard
 *
 * This file is part of FFmpeg.
 *
 * FFmpeg is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * FFmpeg is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with FFmpeg; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

/**
 * @file
 * misc image conversion routines
 */

/* TODO:
 * - write 'ffimg' program to test all the image related stuff
 * - move all api to slice based system
 * - integrate deinterlacing, postprocessing and scaling in the conversion process
 */

#include "avcodec.h"
#include "dsputil.h"
#include "internal.h"
#include "imgconvert.h"
#include "libavutil/pixdesc.h"
#include "libavcore/imgutils.h"
#include "libavcore/internal.h"

#if HAVE_MMX && HAVE_YASM
#include "x86/dsputil_mmx.h"
#endif

#define xglue(x, y) x ## y
#define glue(x, y) xglue(x, y)

#define FF_COLOR_RGB      0 /**< RGB color space */
#define FF_COLOR_GRAY     1 /**< gray color space */
#define FF_COLOR_YUV      2 /**< YUV color space. 16 <= Y <= 235, 16 <= U, V <= 240 */
#define FF_COLOR_YUV_JPEG 3 /**< YUV color space. 0 <= Y <= 255, 0 <= U, V <= 255 */

#define FF_PIXEL_PLANAR   0 /**< each channel has one component in AVPicture */
#define FF_PIXEL_PACKED   1 /**< only one components containing all the channels */
#define FF_PIXEL_PALETTE  2  /**< one components containing indexes for a palette */

typedef struct PixFmtInfo {
    uint8_t nb_channels;     /**< number of channels (including alpha) */
    uint8_t color_type;      /**< color type (see FF_COLOR_xxx constants) */
    uint8_t pixel_type;      /**< pixel storage type (see FF_PIXEL_xxx constants) */
    uint8_t is_alpha : 1;    /**< true if alpha can be specified */
    uint8_t depth;           /**< bit depth of the color components */
} PixFmtInfo;

/* this table gives more information about formats */
#if __STDC_VERSION__ >= 199901L
static const PixFmtInfo pix_fmt_info[PIX_FMT_NB] = {
    /* YUV formats */
    [PIX_FMT_YUV420P] = {
        .nb_channels = 3,
        .color_type = FF_COLOR_YUV,
        .pixel_type = FF_PIXEL_PLANAR,
        .depth = 8,
    },
    [PIX_FMT_YUV422P] = {
        .nb_channels = 3,
        .color_type = FF_COLOR_YUV,
        .pixel_type = FF_PIXEL_PLANAR,
        .depth = 8,
    },
    [PIX_FMT_YUV444P] = {
        .nb_channels = 3,
        .color_type = FF_COLOR_YUV,
        .pixel_type = FF_PIXEL_PLANAR,
        .depth = 8,
    },
    [PIX_FMT_YUYV422] = {
        .nb_channels = 1,
        .color_type = FF_COLOR_YUV,
        .pixel_type = FF_PIXEL_PACKED,
        .depth = 8,
    },
    [PIX_FMT_UYVY422] = {
        .nb_channels = 1,
        .color_type = FF_COLOR_YUV,
        .pixel_type = FF_PIXEL_PACKED,
        .depth = 8,
    },
    [PIX_FMT_YUV410P] = {
        .nb_channels = 3,
        .color_type = FF_COLOR_YUV,
        .pixel_type = FF_PIXEL_PLANAR,
        .depth = 8,
    },
    [PIX_FMT_YUV411P] = {
        .nb_channels = 3,
        .color_type = FF_COLOR_YUV,
        .pixel_type = FF_PIXEL_PLANAR,
        .depth = 8,
    },
    [PIX_FMT_YUV440P] = {
        .nb_channels = 3,
        .color_type = FF_COLOR_YUV,
        .pixel_type = FF_PIXEL_PLANAR,
        .depth = 8,
    },
    [PIX_FMT_YUV420P16LE] = {
        .nb_channels = 3,
        .color_type = FF_COLOR_YUV,
        .pixel_type = FF_PIXEL_PLANAR,
        .depth = 16,
    },
    [PIX_FMT_YUV422P16LE] = {
        .nb_channels = 3,
        .color_type = FF_COLOR_YUV,
        .pixel_type = FF_PIXEL_PLANAR,
        .depth = 16,
    },
    [PIX_FMT_YUV444P16LE] = {
        .nb_channels = 3,
        .color_type = FF_COLOR_YUV,
        .pixel_type = FF_PIXEL_PLANAR,
        .depth = 16,
    },
    [PIX_FMT_YUV420P16BE] = {
        .nb_channels = 3,
        .color_type = FF_COLOR_YUV,
        .pixel_type = FF_PIXEL_PLANAR,
        .depth = 16,
    },
    [PIX_FMT_YUV422P16BE] = {
        .nb_channels = 3,
        .color_type = FF_COLOR_YUV,
        .pixel_type = FF_PIXEL_PLANAR,
        .depth = 16,
    },
    [PIX_FMT_YUV444P16BE] = {
        .nb_channels = 3,
        .color_type = FF_COLOR_YUV,
        .pixel_type = FF_PIXEL_PLANAR,
        .depth = 16,
    },


    /* YUV formats with alpha plane */
    [PIX_FMT_YUVA420P] = {
        .nb_channels = 4,
        .color_type = FF_COLOR_YUV,
        .pixel_type = FF_PIXEL_PLANAR,
        .depth = 8,
    },

    /* JPEG YUV */
    [PIX_FMT_YUVJ420P] = {
        .nb_channels = 3,
        .color_type = FF_COLOR_YUV_JPEG,
        .pixel_type = FF_PIXEL_PLANAR,
        .depth = 8,
    },
    [PIX_FMT_YUVJ422P] = {
        .nb_channels = 3,
        .color_type = FF_COLOR_YUV_JPEG,
        .pixel_type = FF_PIXEL_PLANAR,
        .depth = 8,
    },
    [PIX_FMT_YUVJ444P] = {
        .nb_channels = 3,
        .color_type = FF_COLOR_YUV_JPEG,
        .pixel_type = FF_PIXEL_PLANAR,
        .depth = 8,
    },
    [PIX_FMT_YUVJ440P] = {
        .nb_channels = 3,
        .color_type = FF_COLOR_YUV_JPEG,
        .pixel_type = FF_PIXEL_PLANAR,
        .depth = 8,
    },

    /* RGB formats */
    [PIX_FMT_RGB24] = {
        .nb_channels = 3,
        .color_type = FF_COLOR_RGB,
        .pixel_type = FF_PIXEL_PACKED,
        .depth = 8,
    },
    [PIX_FMT_BGR24] = {
        .nb_channels = 3,
        .color_type = FF_COLOR_RGB,
        .pixel_type = FF_PIXEL_PACKED,
        .depth = 8,
    },
    [PIX_FMT_ARGB] = {
        .nb_channels = 4, .is_alpha = 1,
        .color_type = FF_COLOR_RGB,
        .pixel_type = FF_PIXEL_PACKED,
        .depth = 8,
    },
    [PIX_FMT_RGB48BE] = {
        .nb_channels = 3,
        .color_type = FF_COLOR_RGB,
        .pixel_type = FF_PIXEL_PACKED,
        .depth = 16,
    },
    [PIX_FMT_RGB48LE] = {
        .nb_channels = 3,
        .color_type = FF_COLOR_RGB,
        .pixel_type = FF_PIXEL_PACKED,
        .depth = 16,
    },
    [PIX_FMT_RGB565BE] = {
        .nb_channels = 3,
        .color_type = FF_COLOR_RGB,
        .pixel_type = FF_PIXEL_PACKED,
        .depth = 5,
    },
    [PIX_FMT_RGB565LE] = {
        .nb_channels = 3,
        .color_type = FF_COLOR_RGB,
        .pixel_type = FF_PIXEL_PACKED,
        .depth = 5,
    },
    [PIX_FMT_RGB555BE] = {
        .nb_channels = 3,
        .color_type = FF_COLOR_RGB,
        .pixel_type = FF_PIXEL_PACKED,
        .depth = 5,
    },
    [PIX_FMT_RGB555LE] = {
        .nb_channels = 3,
        .color_type = FF_COLOR_RGB,
        .pixel_type = FF_PIXEL_PACKED,
        .depth = 5,
    },
    [PIX_FMT_RGB444BE] = {
        .nb_channels = 3,
        .color_type = FF_COLOR_RGB,
        .pixel_type = FF_PIXEL_PACKED,
        .depth = 4,
    },
    [PIX_FMT_RGB444LE] = {
        .nb_channels = 3,
        .color_type = FF_COLOR_RGB,
        .pixel_type = FF_PIXEL_PACKED,
        .depth = 4,
    },

    /* gray / mono formats */
    [PIX_FMT_GRAY16BE] = {
        .nb_channels = 1,
        .color_type = FF_COLOR_GRAY,
        .pixel_type = FF_PIXEL_PLANAR,
        .depth = 16,
    },
    [PIX_FMT_GRAY16LE] = {
        .nb_channels = 1,
        .color_type = FF_COLOR_GRAY,
        .pixel_type = FF_PIXEL_PLANAR,
        .depth = 16,
    },
    [PIX_FMT_GRAY8] = {
        .nb_channels = 1,
        .color_type = FF_COLOR_GRAY,
        .pixel_type = FF_PIXEL_PLANAR,
        .depth = 8,
    },
    [PIX_FMT_MONOWHITE] = {
        .nb_channels = 1,
        .color_type = FF_COLOR_GRAY,
        .pixel_type = FF_PIXEL_PLANAR,
        .depth = 1,
    },
    [PIX_FMT_MONOBLACK] = {
        .nb_channels = 1,
        .color_type = FF_COLOR_GRAY,
        .pixel_type = FF_PIXEL_PLANAR,
        .depth = 1,
    },

    /* paletted formats */
    [PIX_FMT_PAL8] = {
        .nb_channels = 4, .is_alpha = 1,
        .color_type = FF_COLOR_RGB,
        .pixel_type = FF_PIXEL_PALETTE,
        .depth = 8,
    },
    [PIX_FMT_UYYVYY411] = {
        .nb_channels = 1,
        .color_type = FF_COLOR_YUV,
        .pixel_type = FF_PIXEL_PACKED,
        .depth = 8,
    },
    [PIX_FMT_ABGR] = {
        .nb_channels = 4, .is_alpha = 1,
        .color_type = FF_COLOR_RGB,
        .pixel_type = FF_PIXEL_PACKED,
        .depth = 8,
    },
    [PIX_FMT_BGR565BE] = {
        .nb_channels = 3,
        .color_type = FF_COLOR_RGB,
        .pixel_type = FF_PIXEL_PACKED,
        .depth = 5,
    },
    [PIX_FMT_BGR565LE] = {
        .nb_channels = 3,
        .color_type = FF_COLOR_RGB,
        .pixel_type = FF_PIXEL_PACKED,
        .depth = 5,
    },
    [PIX_FMT_BGR555BE] = {
        .nb_channels = 3,
        .color_type = FF_COLOR_RGB,
        .pixel_type = FF_PIXEL_PACKED,
        .depth = 5,
    },
    [PIX_FMT_BGR555LE] = {
        .nb_channels = 3,
        .color_type = FF_COLOR_RGB,
        .pixel_type = FF_PIXEL_PACKED,
        .depth = 5,
    },
    [PIX_FMT_BGR444BE] = {
        .nb_channels = 3,
        .color_type = FF_COLOR_RGB,
        .pixel_type = FF_PIXEL_PACKED,
        .depth = 4,
    },
    [PIX_FMT_BGR444LE] = {
        .nb_channels = 3,
        .color_type = FF_COLOR_RGB,
        .pixel_type = FF_PIXEL_PACKED,
        .depth = 4,
    },
    [PIX_FMT_RGB8] = {
        .nb_channels = 1,
        .color_type = FF_COLOR_RGB,
        .pixel_type = FF_PIXEL_PACKED,
        .depth = 8,
    },
    [PIX_FMT_RGB4] = {
        .nb_channels = 1,
        .color_type = FF_COLOR_RGB,
        .pixel_type = FF_PIXEL_PACKED,
        .depth = 4,
    },
    [PIX_FMT_RGB4_BYTE] = {
        .nb_channels = 1,
        .color_type = FF_COLOR_RGB,
        .pixel_type = FF_PIXEL_PACKED,
        .depth = 8,
    },
    [PIX_FMT_BGR8] = {
        .nb_channels = 1,
        .color_type = FF_COLOR_RGB,
        .pixel_type = FF_PIXEL_PACKED,
        .depth = 8,
    },
    [PIX_FMT_BGR4] = {
        .nb_channels = 1,
        .color_type = FF_COLOR_RGB,
        .pixel_type = FF_PIXEL_PACKED,
        .depth = 4,
    },
    [PIX_FMT_BGR4_BYTE] = {
        .nb_channels = 1,
        .color_type = FF_COLOR_RGB,
        .pixel_type = FF_PIXEL_PACKED,
        .depth = 8,
    },
    [PIX_FMT_NV12] = {
        .nb_channels = 2,
        .color_type = FF_COLOR_YUV,
        .pixel_type = FF_PIXEL_PLANAR,
        .depth = 8,
    },
    [PIX_FMT_NV21] = {
        .nb_channels = 2,
        .color_type = FF_COLOR_YUV,
        .pixel_type = FF_PIXEL_PLANAR,
        .depth = 8,
    },

    [PIX_FMT_BGRA] = {
        .nb_channels = 4, .is_alpha = 1,
        .color_type = FF_COLOR_RGB,
        .pixel_type = FF_PIXEL_PACKED,
        .depth = 8,
    },
    [PIX_FMT_RGBA] = {
        .nb_channels = 4, .is_alpha = 1,
        .color_type = FF_COLOR_RGB,
        .pixel_type = FF_PIXEL_PACKED,
        .depth = 8,
    },
};

#else
static PixFmtInfo pix_fmt_info[PIX_FMT_NB];

void avpicture_init_pixfmtinfo(void)
{
 pix_fmt_info[PIX_FMT_YUV420P].nb_channels = 3;
 pix_fmt_info[PIX_FMT_YUV420P].color_type = FF_COLOR_YUV;
 pix_fmt_info[PIX_FMT_YUV420P].pixel_type = FF_PIXEL_PLANAR;
 pix_fmt_info[PIX_FMT_YUV420P].depth = 8;

 pix_fmt_info[PIX_FMT_YUV422P].nb_channels = 3;
 pix_fmt_info[PIX_FMT_YUV422P].color_type = FF_COLOR_YUV;
 pix_fmt_info[PIX_FMT_YUV422P].pixel_type = FF_PIXEL_PLANAR;
 pix_fmt_info[PIX_FMT_YUV422P].depth = 8,

 pix_fmt_info[PIX_FMT_YUV444P].nb_channels = 3;
 pix_fmt_info[PIX_FMT_YUV444P].color_type = FF_COLOR_YUV;
 pix_fmt_info[PIX_FMT_YUV444P].pixel_type = FF_PIXEL_PLANAR;
 pix_fmt_info[PIX_FMT_YUV444P].depth = 8;

 pix_fmt_info[PIX_FMT_YUYV422].nb_channels = 1;
 pix_fmt_info[PIX_FMT_YUYV422].color_type = FF_COLOR_YUV;
 pix_fmt_info[PIX_FMT_YUYV422].pixel_type = FF_PIXEL_PACKED;
 pix_fmt_info[PIX_FMT_YUYV422].depth = 8;

 pix_fmt_info[PIX_FMT_UYVY422].nb_channels = 1;
 pix_fmt_info[PIX_FMT_UYVY422].color_type = FF_COLOR_YUV;
 pix_fmt_info[PIX_FMT_UYVY422].pixel_type = FF_PIXEL_PACKED;
 pix_fmt_info[PIX_FMT_UYVY422].depth = 8;

 pix_fmt_info[PIX_FMT_YUV410P].nb_channels = 3;
 pix_fmt_info[PIX_FMT_YUV410P].color_type = FF_COLOR_YUV;
 pix_fmt_info[PIX_FMT_YUV410P].pixel_type = FF_PIXEL_PLANAR;
 pix_fmt_info[PIX_FMT_YUV410P].depth = 8;

 pix_fmt_info[PIX_FMT_YUV411P].nb_channels = 3;
 pix_fmt_info[PIX_FMT_YUV411P].color_type = FF_COLOR_YUV;
 pix_fmt_info[PIX_FMT_YUV411P].pixel_type = FF_PIXEL_PLANAR;
 pix_fmt_info[PIX_FMT_YUV411P].depth = 8;
 
 pix_fmt_info[PIX_FMT_YUV440P].nb_channels = 3;
 pix_fmt_info[PIX_FMT_YUV440P].color_type = FF_COLOR_YUV;
 pix_fmt_info[PIX_FMT_YUV440P].pixel_type = FF_PIXEL_PLANAR;
 pix_fmt_info[PIX_FMT_YUV440P].depth = 8;
 
 pix_fmt_info[PIX_FMT_YUV420P16LE].nb_channels = 3;
 pix_fmt_info[PIX_FMT_YUV420P16LE].color_type = FF_COLOR_YUV;
 pix_fmt_info[PIX_FMT_YUV420P16LE].pixel_type = FF_PIXEL_PLANAR;
 pix_fmt_info[PIX_FMT_YUV420P16LE].depth = 16;
 
 pix_fmt_info[PIX_FMT_YUV422P16LE].nb_channels = 3;
 pix_fmt_info[PIX_FMT_YUV422P16LE].color_type = FF_COLOR_YUV;
 pix_fmt_info[PIX_FMT_YUV422P16LE].pixel_type = FF_PIXEL_PLANAR;
 pix_fmt_info[PIX_FMT_YUV422P16LE].depth = 16;
 
 pix_fmt_info[PIX_FMT_YUV444P16LE].nb_channels = 3;
 pix_fmt_info[PIX_FMT_YUV444P16LE].color_type = FF_COLOR_YUV;
 pix_fmt_info[PIX_FMT_YUV444P16LE].pixel_type = FF_PIXEL_PLANAR;
 pix_fmt_info[PIX_FMT_YUV444P16LE].depth = 16;
 
 pix_fmt_info[PIX_FMT_YUV420P16BE].nb_channels = 3;
 pix_fmt_info[PIX_FMT_YUV420P16BE].color_type = FF_COLOR_YUV;
 pix_fmt_info[PIX_FMT_YUV420P16BE].pixel_type = FF_PIXEL_PLANAR;
 pix_fmt_info[PIX_FMT_YUV420P16BE].depth = 16;
 
 pix_fmt_info[PIX_FMT_YUV422P16BE].nb_channels = 3;
 pix_fmt_info[PIX_FMT_YUV422P16BE].color_type = FF_COLOR_YUV;
 pix_fmt_info[PIX_FMT_YUV422P16BE].pixel_type = FF_PIXEL_PLANAR;
 pix_fmt_info[PIX_FMT_YUV422P16BE].depth = 16;
 
 pix_fmt_info[PIX_FMT_YUV444P16BE].nb_channels = 3;
 pix_fmt_info[PIX_FMT_YUV444P16BE].color_type = FF_COLOR_YUV;
 pix_fmt_info[PIX_FMT_YUV444P16BE].pixel_type = FF_PIXEL_PLANAR;
 pix_fmt_info[PIX_FMT_YUV444P16BE].depth = 16;
 
 /* YUV formats with alpha plane */
 pix_fmt_info[PIX_FMT_YUVA420P].nb_channels = 4;
 pix_fmt_info[PIX_FMT_YUVA420P].color_type = FF_COLOR_YUV;
 pix_fmt_info[PIX_FMT_YUVA420P].pixel_type = FF_PIXEL_PLANAR;
 pix_fmt_info[PIX_FMT_YUVA420P].depth = 8;

 /* JPEG YUV */
 pix_fmt_info[PIX_FMT_YUVJ420P].nb_channels = 3;
 pix_fmt_info[PIX_FMT_YUVJ420P].color_type = FF_COLOR_YUV_JPEG;
 pix_fmt_info[PIX_FMT_YUVJ420P].pixel_type = FF_PIXEL_PLANAR;
 pix_fmt_info[PIX_FMT_YUVJ420P].depth = 8;

 pix_fmt_info[PIX_FMT_YUVJ422P].nb_channels = 3;
 pix_fmt_info[PIX_FMT_YUVJ422P].color_type = FF_COLOR_YUV_JPEG;
 pix_fmt_info[PIX_FMT_YUVJ422P].pixel_type = FF_PIXEL_PLANAR;
 pix_fmt_info[PIX_FMT_YUVJ422P].depth = 8;

 pix_fmt_info[PIX_FMT_YUVJ444P].nb_channels = 3;
 pix_fmt_info[PIX_FMT_YUVJ444P].color_type = FF_COLOR_YUV_JPEG;
 pix_fmt_info[PIX_FMT_YUVJ444P].pixel_type = FF_PIXEL_PLANAR;
 pix_fmt_info[PIX_FMT_YUVJ444P].depth = 8;
 
 pix_fmt_info[PIX_FMT_YUVJ440P].nb_channels = 3;
 pix_fmt_info[PIX_FMT_YUVJ440P].color_type = FF_COLOR_YUV_JPEG;
 pix_fmt_info[PIX_FMT_YUVJ440P].pixel_type = FF_PIXEL_PLANAR;
 pix_fmt_info[PIX_FMT_YUVJ440P].depth = 8;

 /* RGB formats */
 pix_fmt_info[PIX_FMT_RGB24].nb_channels = 3;
 pix_fmt_info[PIX_FMT_RGB24].color_type = FF_COLOR_RGB;
 pix_fmt_info[PIX_FMT_RGB24].pixel_type = FF_PIXEL_PACKED;
 pix_fmt_info[PIX_FMT_RGB24].depth = 8;

 pix_fmt_info[PIX_FMT_BGR24].nb_channels = 3;
 pix_fmt_info[PIX_FMT_BGR24].color_type = FF_COLOR_RGB;
 pix_fmt_info[PIX_FMT_BGR24].pixel_type = FF_PIXEL_PACKED;
 pix_fmt_info[PIX_FMT_BGR24].depth = 8;

 pix_fmt_info[PIX_FMT_ARGB].nb_channels = 4;
 pix_fmt_info[PIX_FMT_ARGB].is_alpha = 1;
 pix_fmt_info[PIX_FMT_ARGB].color_type = FF_COLOR_RGB;
 pix_fmt_info[PIX_FMT_ARGB].pixel_type = FF_PIXEL_PACKED;
 pix_fmt_info[PIX_FMT_ARGB].depth = 8;
 
 pix_fmt_info[PIX_FMT_RGB48BE].nb_channels = 3;
 pix_fmt_info[PIX_FMT_RGB48BE].color_type = FF_COLOR_RGB;
 pix_fmt_info[PIX_FMT_RGB48BE].pixel_type = FF_PIXEL_PACKED;
 pix_fmt_info[PIX_FMT_RGB48BE].depth = 16;

 pix_fmt_info[PIX_FMT_RGB48LE].nb_channels = 3;
 pix_fmt_info[PIX_FMT_RGB48LE].color_type = FF_COLOR_RGB;
 pix_fmt_info[PIX_FMT_RGB48LE].pixel_type = FF_PIXEL_PACKED;
 pix_fmt_info[PIX_FMT_RGB48LE].depth = 16;
 
 pix_fmt_info[PIX_FMT_RGB565BE].nb_channels = 3;
 pix_fmt_info[PIX_FMT_RGB565BE].color_type = FF_COLOR_RGB;
 pix_fmt_info[PIX_FMT_RGB565BE].pixel_type = FF_PIXEL_PACKED;
 pix_fmt_info[PIX_FMT_RGB565BE].depth = 5;
 
 pix_fmt_info[PIX_FMT_RGB565LE].nb_channels = 3;
 pix_fmt_info[PIX_FMT_RGB565LE].color_type = FF_COLOR_RGB;
 pix_fmt_info[PIX_FMT_RGB565LE].pixel_type = FF_PIXEL_PACKED;
 pix_fmt_info[PIX_FMT_RGB565LE].depth = 5;

 pix_fmt_info[PIX_FMT_RGB555BE].nb_channels = 3;
 pix_fmt_info[PIX_FMT_RGB555BE].color_type = FF_COLOR_RGB;
 pix_fmt_info[PIX_FMT_RGB555BE].pixel_type = FF_PIXEL_PACKED;
 pix_fmt_info[PIX_FMT_RGB555BE].depth = 5;
 
 pix_fmt_info[PIX_FMT_RGB555LE].nb_channels = 3;
 pix_fmt_info[PIX_FMT_RGB555LE].color_type = FF_COLOR_RGB;
 pix_fmt_info[PIX_FMT_RGB555LE].pixel_type = FF_PIXEL_PACKED;
 pix_fmt_info[PIX_FMT_RGB555LE].depth = 5;
 
 pix_fmt_info[PIX_FMT_RGB444BE].nb_channels = 3;
 pix_fmt_info[PIX_FMT_RGB444BE].color_type = FF_COLOR_RGB;
 pix_fmt_info[PIX_FMT_RGB444BE].pixel_type = FF_PIXEL_PACKED;
 pix_fmt_info[PIX_FMT_RGB444BE].depth = 4;
 
 pix_fmt_info[PIX_FMT_RGB444LE].nb_channels = 3;
 pix_fmt_info[PIX_FMT_RGB444LE].color_type = FF_COLOR_RGB;
 pix_fmt_info[PIX_FMT_RGB444LE].pixel_type = FF_PIXEL_PACKED;
 pix_fmt_info[PIX_FMT_RGB444LE].depth = 4;

 /* gray / mono formats */
 pix_fmt_info[PIX_FMT_GRAY16BE].nb_channels = 1;
 pix_fmt_info[PIX_FMT_GRAY16BE].color_type = FF_COLOR_GRAY;
 pix_fmt_info[PIX_FMT_GRAY16BE].pixel_type = FF_PIXEL_PLANAR;
 pix_fmt_info[PIX_FMT_GRAY16BE].depth = 16;

 pix_fmt_info[PIX_FMT_GRAY16LE].nb_channels = 1;
 pix_fmt_info[PIX_FMT_GRAY16LE].color_type = FF_COLOR_GRAY;
 pix_fmt_info[PIX_FMT_GRAY16LE].pixel_type = FF_PIXEL_PLANAR;
 pix_fmt_info[PIX_FMT_GRAY16LE].depth = 16;

 pix_fmt_info[PIX_FMT_GRAY8].nb_channels = 1;
 pix_fmt_info[PIX_FMT_GRAY8].color_type = FF_COLOR_GRAY;
 pix_fmt_info[PIX_FMT_GRAY8].pixel_type = FF_PIXEL_PLANAR;
 pix_fmt_info[PIX_FMT_GRAY8].depth = 8;

 pix_fmt_info[PIX_FMT_MONOWHITE].nb_channels = 1;
 pix_fmt_info[PIX_FMT_MONOWHITE].color_type = FF_COLOR_GRAY;
 pix_fmt_info[PIX_FMT_MONOWHITE].pixel_type = FF_PIXEL_PLANAR;
 pix_fmt_info[PIX_FMT_MONOWHITE].depth = 1;

 pix_fmt_info[PIX_FMT_MONOBLACK].nb_channels = 1;
 pix_fmt_info[PIX_FMT_MONOBLACK].color_type = FF_COLOR_GRAY;
 pix_fmt_info[PIX_FMT_MONOBLACK].pixel_type = FF_PIXEL_PLANAR;
 pix_fmt_info[PIX_FMT_MONOBLACK].depth = 1;

 /* paletted formats */
 pix_fmt_info[PIX_FMT_PAL8].nb_channels = 4;
 pix_fmt_info[PIX_FMT_PAL8].is_alpha = 1;
 pix_fmt_info[PIX_FMT_PAL8].color_type = FF_COLOR_RGB;
 pix_fmt_info[PIX_FMT_PAL8].pixel_type = FF_PIXEL_PALETTE;
 pix_fmt_info[PIX_FMT_PAL8].depth = 8;

 pix_fmt_info[PIX_FMT_UYYVYY411].nb_channels = 1;
 pix_fmt_info[PIX_FMT_UYYVYY411].color_type = FF_COLOR_YUV;
 pix_fmt_info[PIX_FMT_UYYVYY411].pixel_type = FF_PIXEL_PACKED;
 pix_fmt_info[PIX_FMT_UYYVYY411].depth = 8;

 pix_fmt_info[PIX_FMT_ABGR].nb_channels = 4;
 pix_fmt_info[PIX_FMT_ABGR].is_alpha = 1;
 pix_fmt_info[PIX_FMT_ABGR].color_type = FF_COLOR_RGB;
 pix_fmt_info[PIX_FMT_ABGR].pixel_type = FF_PIXEL_PACKED;
 pix_fmt_info[PIX_FMT_ABGR].depth = 8;

 pix_fmt_info[PIX_FMT_BGR565BE].nb_channels = 3;
 pix_fmt_info[PIX_FMT_BGR565BE].color_type = FF_COLOR_RGB;
 pix_fmt_info[PIX_FMT_BGR565BE].pixel_type = FF_PIXEL_PACKED;
 pix_fmt_info[PIX_FMT_BGR565BE].depth = 5;
 
 pix_fmt_info[PIX_FMT_BGR565LE].nb_channels = 3;
 pix_fmt_info[PIX_FMT_BGR565LE].color_type = FF_COLOR_RGB;
 pix_fmt_info[PIX_FMT_BGR565LE].pixel_type = FF_PIXEL_PACKED;
 pix_fmt_info[PIX_FMT_BGR565LE].depth = 5;

 pix_fmt_info[PIX_FMT_BGR555BE].nb_channels = 3;
 pix_fmt_info[PIX_FMT_BGR555BE].color_type = FF_COLOR_RGB;
 pix_fmt_info[PIX_FMT_BGR555BE].pixel_type = FF_PIXEL_PACKED;
 pix_fmt_info[PIX_FMT_BGR555BE].depth = 5;
 
 pix_fmt_info[PIX_FMT_BGR555LE].nb_channels = 3;
 pix_fmt_info[PIX_FMT_BGR555LE].color_type = FF_COLOR_RGB;
 pix_fmt_info[PIX_FMT_BGR555LE].pixel_type = FF_PIXEL_PACKED;
 pix_fmt_info[PIX_FMT_BGR555LE].depth = 5;
 
 pix_fmt_info[PIX_FMT_BGR444BE].nb_channels = 3;
 pix_fmt_info[PIX_FMT_BGR444BE].color_type = FF_COLOR_RGB;
 pix_fmt_info[PIX_FMT_BGR444BE].pixel_type = FF_PIXEL_PACKED;
 pix_fmt_info[PIX_FMT_BGR444BE].depth = 4;
 
 pix_fmt_info[PIX_FMT_BGR444LE].nb_channels = 3;
 pix_fmt_info[PIX_FMT_BGR444LE].color_type = FF_COLOR_RGB;
 pix_fmt_info[PIX_FMT_BGR444LE].pixel_type = FF_PIXEL_PACKED;
 pix_fmt_info[PIX_FMT_BGR444LE].depth = 4;

 pix_fmt_info[PIX_FMT_RGB8].nb_channels = 1;
 pix_fmt_info[PIX_FMT_RGB8].color_type = FF_COLOR_RGB;
 pix_fmt_info[PIX_FMT_RGB8].pixel_type = FF_PIXEL_PACKED;
 pix_fmt_info[PIX_FMT_RGB8].depth = 8;

 pix_fmt_info[PIX_FMT_RGB4].nb_channels = 1;
 pix_fmt_info[PIX_FMT_RGB4].color_type = FF_COLOR_RGB;
 pix_fmt_info[PIX_FMT_RGB4].pixel_type = FF_PIXEL_PACKED;
 pix_fmt_info[PIX_FMT_RGB4].depth = 4;

 pix_fmt_info[PIX_FMT_RGB4_BYTE].nb_channels = 1;
 pix_fmt_info[PIX_FMT_RGB4_BYTE].color_type = FF_COLOR_RGB;
 pix_fmt_info[PIX_FMT_RGB4_BYTE].pixel_type = FF_PIXEL_PACKED;
 pix_fmt_info[PIX_FMT_RGB4_BYTE].depth = 8;

 pix_fmt_info[PIX_FMT_BGR8].nb_channels = 1;
 pix_fmt_info[PIX_FMT_BGR8].color_type = FF_COLOR_RGB;
 pix_fmt_info[PIX_FMT_BGR8].pixel_type = FF_PIXEL_PACKED;
 pix_fmt_info[PIX_FMT_BGR8].depth = 8;

 pix_fmt_info[PIX_FMT_BGR4].nb_channels = 1;
 pix_fmt_info[PIX_FMT_BGR4].color_type = FF_COLOR_RGB;
 pix_fmt_info[PIX_FMT_BGR4].pixel_type = FF_PIXEL_PACKED;
 pix_fmt_info[PIX_FMT_BGR4].depth = 4;

 pix_fmt_info[PIX_FMT_BGR4_BYTE].nb_channels = 1;
 pix_fmt_info[PIX_FMT_BGR4_BYTE].color_type = FF_COLOR_RGB;
 pix_fmt_info[PIX_FMT_BGR4_BYTE].pixel_type = FF_PIXEL_PACKED;
 pix_fmt_info[PIX_FMT_BGR4_BYTE].depth = 8;

 pix_fmt_info[PIX_FMT_NV12].nb_channels = 2;
 pix_fmt_info[PIX_FMT_NV12].color_type = FF_COLOR_YUV;
 pix_fmt_info[PIX_FMT_NV12].pixel_type = FF_PIXEL_PLANAR;
 pix_fmt_info[PIX_FMT_NV12].depth = 8;

 pix_fmt_info[PIX_FMT_NV21].nb_channels = 2;
 pix_fmt_info[PIX_FMT_NV21].color_type = FF_COLOR_YUV;
 pix_fmt_info[PIX_FMT_NV21].pixel_type = FF_PIXEL_PLANAR;
 pix_fmt_info[PIX_FMT_NV21].depth = 8;

 pix_fmt_info[PIX_FMT_BGRA].nb_channels = 4;
 pix_fmt_info[PIX_FMT_BGRA].is_alpha = 1;
 pix_fmt_info[PIX_FMT_BGRA].color_type = FF_COLOR_RGB;
 pix_fmt_info[PIX_FMT_BGRA].pixel_type = FF_PIXEL_PACKED;
 pix_fmt_info[PIX_FMT_BGRA].depth = 8;

 pix_fmt_info[PIX_FMT_RGBA].nb_channels = 4;
 pix_fmt_info[PIX_FMT_RGBA].is_alpha = 1;
 pix_fmt_info[PIX_FMT_RGBA].color_type = FF_COLOR_RGB;
 pix_fmt_info[PIX_FMT_RGBA].pixel_type = FF_PIXEL_PACKED;
 pix_fmt_info[PIX_FMT_RGBA].depth = 8;
}
#endif

void avcodec_get_chroma_sub_sample(enum PixelFormat pix_fmt, int *h_shift, int *v_shift)
{
    *h_shift = av_pix_fmt_descriptors[pix_fmt].log2_chroma_w;
    *v_shift = av_pix_fmt_descriptors[pix_fmt].log2_chroma_h;
}

const char *avcodec_get_pix_fmt_name(enum PixelFormat pix_fmt)
{
    if ((unsigned)pix_fmt >= PIX_FMT_NB)
        return NULL;
    else
        return av_pix_fmt_descriptors[pix_fmt].name;
}

int ff_is_hwaccel_pix_fmt(enum PixelFormat pix_fmt)
{
    return av_pix_fmt_descriptors[pix_fmt].flags & PIX_FMT_HWACCEL;
}

#if LIBAVCODEC_VERSION_MAJOR < 53
int ff_set_systematic_pal(uint32_t pal[256], enum PixelFormat pix_fmt){
    return ff_set_systematic_pal2(pal, pix_fmt);
}

int ff_fill_linesize(AVPicture *picture, enum PixelFormat pix_fmt, int width)
{
    return av_image_fill_linesizes(picture->linesize, pix_fmt, width);
}

int ff_fill_pointer(AVPicture *picture, uint8_t *ptr, enum PixelFormat pix_fmt,
                    int height)
{
    return av_image_fill_pointers(picture->data, pix_fmt, height, ptr, picture->linesize);
}
#endif

static int avg_bits_per_pixel(enum PixelFormat pix_fmt)
{
    int bits;
    const PixFmtInfo *pf;
    const AVPixFmtDescriptor *desc = &av_pix_fmt_descriptors[pix_fmt];

    pf = &pix_fmt_info[pix_fmt];
    switch(pf->pixel_type) {
    case FF_PIXEL_PACKED:
        switch(pix_fmt) {
        case PIX_FMT_YUYV422:
        case PIX_FMT_UYVY422:
        case PIX_FMT_RGB565BE:
        case PIX_FMT_RGB565LE:
        case PIX_FMT_RGB555BE:
        case PIX_FMT_RGB555LE:
        case PIX_FMT_RGB444BE:
        case PIX_FMT_RGB444LE:
        case PIX_FMT_BGR565BE:
        case PIX_FMT_BGR565LE:
        case PIX_FMT_BGR555BE:
        case PIX_FMT_BGR555LE:
        case PIX_FMT_BGR444BE:
        case PIX_FMT_BGR444LE:
            bits = 16;
            break;
        case PIX_FMT_UYYVYY411:
            bits = 12;
            break;
        default:
            bits = pf->depth * pf->nb_channels;
            break;
        }
        break;
    case FF_PIXEL_PLANAR:
        if (desc->log2_chroma_w == 0 && desc->log2_chroma_h == 0) {
            bits = pf->depth * pf->nb_channels;
        } else {
            bits = pf->depth + ((2 * pf->depth) >>
                                (desc->log2_chroma_w + desc->log2_chroma_h));
        }
        break;
    case FF_PIXEL_PALETTE:
        bits = 8;
        break;
    default:
        bits = -1;
        break;
    }
    return bits;
}

#if LIBAVCODEC_VERSION_MAJOR < 53
void ff_img_copy_plane(uint8_t *dst, int dst_wrap,
                           const uint8_t *src, int src_wrap,
                           int width, int height)
{
    av_image_copy_plane(dst, dst_wrap, src, src_wrap, width, height);
}

int ff_get_plane_bytewidth(enum PixelFormat pix_fmt, int width, int plane)
{
    return av_image_get_linesize(pix_fmt, width, plane);
}

void av_picture_data_copy(uint8_t *dst_data[4], int dst_linesize[4],
                          uint8_t *src_data[4], int src_linesize[4],
                          enum PixelFormat pix_fmt, int width, int height)
{
    av_image_copy(dst_data, dst_linesize, src_data, src_linesize,
                  pix_fmt, width, height);
}
#endif

void av_picture_copy(AVPicture *dst, const AVPicture *src,
                     enum PixelFormat pix_fmt, int width, int height)
{
    av_image_copy(dst->data, dst->linesize, src->data,
                  src->linesize, pix_fmt, width, height);
}

/* 2x2 -> 1x1 */
void ff_shrink22(uint8_t *dst, int dst_wrap,
                     const uint8_t *src, int src_wrap,
                     int width, int height)
{
    int w;
    const uint8_t *s1, *s2;
    uint8_t *d;

    for(;height > 0; height--) {
        s1 = src;
        s2 = s1 + src_wrap;
        d = dst;
        for(w = width;w >= 4; w-=4) {
            d[0] = (s1[0] + s1[1] + s2[0] + s2[1] + 2) >> 2;
            d[1] = (s1[2] + s1[3] + s2[2] + s2[3] + 2) >> 2;
            d[2] = (s1[4] + s1[5] + s2[4] + s2[5] + 2) >> 2;
            d[3] = (s1[6] + s1[7] + s2[6] + s2[7] + 2) >> 2;
            s1 += 8;
            s2 += 8;
            d += 4;
        }
        for(;w > 0; w--) {
            d[0] = (s1[0] + s1[1] + s2[0] + s2[1] + 2) >> 2;
            s1 += 2;
            s2 += 2;
            d++;
        }
        src += 2 * src_wrap;
        dst += dst_wrap;
    }
}

/* 4x4 -> 1x1 */
void ff_shrink44(uint8_t *dst, int dst_wrap,
                     const uint8_t *src, int src_wrap,
                     int width, int height)
{
    int w;
    const uint8_t *s1, *s2, *s3, *s4;
    uint8_t *d;

    for(;height > 0; height--) {
        s1 = src;
        s2 = s1 + src_wrap;
        s3 = s2 + src_wrap;
        s4 = s3 + src_wrap;
        d = dst;
        for(w = width;w > 0; w--) {
            d[0] = (s1[0] + s1[1] + s1[2] + s1[3] +
                    s2[0] + s2[1] + s2[2] + s2[3] +
                    s3[0] + s3[1] + s3[2] + s3[3] +
                    s4[0] + s4[1] + s4[2] + s4[3] + 8) >> 4;
            s1 += 4;
            s2 += 4;
            s3 += 4;
            s4 += 4;
            d++;
        }
        src += 4 * src_wrap;
        dst += dst_wrap;
    }
}

/* 8x8 -> 1x1 */
void ff_shrink88(uint8_t *dst, int dst_wrap,
                     const uint8_t *src, int src_wrap,
                     int width, int height)
{
    int w, i;

    for(;height > 0; height--) {
        for(w = width;w > 0; w--) {
            int tmp=0;
            for(i=0; i<8; i++){
                tmp += src[0] + src[1] + src[2] + src[3] + src[4] + src[5] + src[6] + src[7];
                src += src_wrap;
            }
            *(dst++) = (tmp + 32)>>6;
            src += 8 - 8*src_wrap;
        }
        src += 8*src_wrap - 8*width;
        dst += dst_wrap - width;
    }
}
