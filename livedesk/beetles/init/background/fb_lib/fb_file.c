/*
*********************************************************************************************************
*                                                   eMOD
*                                  the Easy Portable/Player Operation System
*                                              mod_willow sub-system
*
*                               (c) Copyright 2006-2009, ALLWINNERME, China
*                                           All Rights Reserved
*
* File   : fb_file.c
* Version: V1.0
* By     : xiechuanrong
*********************************************************************************************************
*/
#include "mod_init_i.h"
#include "fb_file.h"
#include <string.h>
#include <log.h>
#include <libc/eLIBs_az100.h>
#include <emodules/mod_display.h>
#include <kconfig.h>

/************************************************************************
* Function: backlayer_create_fb_file
* Description: ����fb���ļ�
* Input:
*    FB *fb���������fb
*    uint8_t * filename������fb���ļ���
* Output:
* Return:
*     EPDK_OK: �ɹ�
*     EPDK_FAIL: ʧ��
*************************************************************************/
typedef struct
{
    __disp_fb_t     input_fb;
    __disp_rect_t   source_regn;
    __disp_fb_t     output_fb;
    //__disp_rect_t   out_regn;
} __disp_scaler_para_t;

int32_t create_fb_file(FB *framebuffer, uint8_t *filename)
{
    __hdle fd;
    __fb_file_header_t file_header;
    int32_t write_num;

    fd = fopen((char *)filename, "w+");

    if (fd == NULL)
    {
        __wrn("------open file:%s err!------", filename);
        return EPDK_FAIL;
    }

    memset(&file_header, 0, sizeof(file_header));
    file_header.version = 0x100;
    file_header.height = framebuffer->size.height;
    file_header.width = framebuffer->size.width;
    file_header.cs_mode = framebuffer->fmt.cs_mode;
    file_header.fmt_type = framebuffer->fmt.type;

    if (framebuffer->fmt.type == FB_TYPE_RGB)
    {
        file_header.pix_fmt = framebuffer->fmt.fmt.rgb.pixelfmt;
        file_header.pix_seq = framebuffer->fmt.fmt.rgb.pixseq;
        file_header.mod_or_br_swap = framebuffer->fmt.fmt.rgb.br_swap;
        file_header.data_len[0] = file_header.height * file_header.width * 4;
    }
    else
    {
        if (framebuffer->fmt.fmt.yuv.mod != YUV_MOD_NON_MB_PLANAR)
        {
            __wrn("not support format!");
            fclose(fd);
            return EPDK_FAIL;
        }

        file_header.pix_fmt = framebuffer->fmt.fmt.yuv.pixelfmt;
        file_header.pix_seq = framebuffer->fmt.fmt.yuv.yuvseq;
        file_header.mod_or_br_swap = framebuffer->fmt.fmt.yuv.mod;
        file_header.data_len[0] = file_header.height * file_header.width;

        switch (framebuffer->fmt.fmt.yuv.pixelfmt)
        {
            case PIXEL_YUV444:
            {
                file_header.data_len[1] = file_header.height * file_header.width;
                file_header.data_len[2] = file_header.height * file_header.width;
                break;
            }

            case PIXEL_YUV422:
            {
                file_header.data_len[1] = file_header.height * file_header.width / 2;
                file_header.data_len[2] = file_header.height * file_header.width / 2;
                break;
            }

            case PIXEL_YUV420:
            {
                file_header.data_len[1] = file_header.height * file_header.width / 4;
                file_header.data_len[2] = file_header.height * file_header.width / 4;
                break;
            }

            case PIXEL_YUV411:
            {
                file_header.data_len[1] = file_header.height * file_header.width / 4;
                file_header.data_len[2] = file_header.height * file_header.width / 4;
                break;
            }

            default:
            {
                __wrn("not support format!");
                fclose(fd);
                return EPDK_FAIL;
            }
        }
    }

    file_header.offset_data[0] = sizeof(__fb_file_header_t);
    file_header.offset_data[1] = file_header.offset_data[0] + file_header.data_len[0];
    file_header.offset_data[2] = file_header.offset_data[1] + file_header.data_len[1];
    write_num = fwrite(&file_header, 1, sizeof(file_header), fd);

    if (write_num != sizeof(file_header))
    {
        __log("not enough space...");
        fclose(fd);
        return EPDK_FAIL;
    }

    write_num = fwrite(framebuffer->addr[0], 1, file_header.data_len[0], fd);

    if (write_num != file_header.data_len[0])
    {
        __log("not enough space...");
        fclose(fd);
        return EPDK_FAIL;
    }

    if (file_header.data_len[1] != 0)
    {
        write_num = fwrite(framebuffer->addr[1], 1, file_header.data_len[1], fd);

        if (write_num != file_header.data_len[1])
        {
            __log("not enough space...");
            fclose(fd);
            return EPDK_FAIL;
        }
    }

    if (file_header.data_len[2] != 0)
    {
        write_num = fwrite(framebuffer->addr[2], 1, file_header.data_len[2], fd);

        if (write_num != file_header.data_len[2])
        {
            __log("not enough space...");
            fclose(fd);
            return EPDK_FAIL;
        }
    }

    fclose(fd);
    return EPDK_OK;
}



/************************************************************************
* Function: convert_fb_yuv420
* Description: ��framebufferת��Ϊyuv420��ʽ
* Input:
*    FB *in_frame��Դframe buffer
*    FB *out_frame��Ŀ��frame buffer
*    SIZE *screen_size: ��Ļ�ߴ�
*    uint8_t mod: ģʽ����:
*       BACKLAYER_MOD_RATIO----�ʺ���Ļ�ߴ�(ͼƬ�����������)
*       BACKLAYER_MOD_STRETCH----����ģʽ,���ŵ���Ļ�ߴ�
* Output:
* Return:
*     EPDK_OK: �ɹ�
*     EPDK_FAIL: ʧ��
*************************************************************************/
int32_t convert_fb(FB *in_frame, FB *out_frame, SIZE *screen_size, uint8_t mod)
{
    int32_t 				scaler_hdle;
    ES_FILE 				*de_hdle;
    __disp_scaler_para_t 	param;
    __disp_fb_t         	disp_in_frame;
    __disp_fb_t         	disp_out_frame;
    int32_t					ret_val = EPDK_OK;
    uint32_t 				arg[3];

    //  if (mod == BACKLAYER_MOD_RATIO)
    {
        out_frame->size = in_frame->size ;

        if (out_frame->size.width > screen_size->width)
        {
            out_frame->size.width = screen_size->width;
            out_frame->size.height = out_frame->size.width * in_frame->size.height / in_frame->size.width;
        }

        if (out_frame->size.height > screen_size->height)
        {
            out_frame->size.height = screen_size->height;
            out_frame->size.width = out_frame->size.height * in_frame->size.width / in_frame->size.height;
        }

        if (out_frame->size.width == 0)
        {
            out_frame->size.width = 1;
        }

        if (out_frame->size.height == 0)
        {
            out_frame->size.height = 1;
        }
    }

    //  else
    //  {
    //      out_frame->size = *screen_size;
    //  }
    /*���ڴ������⣬���ߴ�Ϊ1280*720����������ߴ���scaler mode�Ŵ�õ�*/
    if (out_frame->size.width > 1280)
    {
        out_frame->size.width = 1280;
        out_frame->size.height = out_frame->size.width * in_frame->size.height / in_frame->size.width;
    }

    if (out_frame->size.height > 720)
    {
        out_frame->size.height = 720;
        out_frame->size.width = out_frame->size.height * in_frame->size.width / in_frame->size.height;
    }

    if (out_frame->size.width > SCALE_OUT_MAX_WIDTH)
    {
        out_frame->size.width = SCALE_OUT_MAX_WIDTH;
    }

    if (out_frame->size.height > SCALE_OUT_MAX_HEIGHT)
    {
        out_frame->size.height = SCALE_OUT_MAX_HEIGHT;
    }

    if (in_frame->fmt.type == FB_TYPE_RGB)
    {
        __err("not support fb type!");
        return EPDK_FAIL;
    }

    out_frame->size.height = ((out_frame->size.height + 3) >> 2) << 2;
    out_frame->size.width = ((out_frame->size.width + 3) >> 2) << 2;

    if (out_frame->fmt.type != FB_TYPE_YUV && out_frame->fmt.type != FB_TYPE_RGB)
    {
        out_frame->fmt.type = FB_TYPE_YUV;
    }

    out_frame->fmt.cs_mode = in_frame->fmt.cs_mode;

    if (out_frame->fmt.type == FB_TYPE_YUV)
    {
        uint32_t tmplen;
        out_frame->fmt.fmt.yuv.mod = YUV_MOD_NON_MB_PLANAR;
        out_frame->fmt.fmt.yuv.pixelfmt = PIXEL_YUV422;
        out_frame->fmt.fmt.yuv.yuvseq = YUV_SEQ_OTHRS;
        tmplen = out_frame->size.height * out_frame->size.width;
        tmplen = ((tmplen >> 6) + 1) << 6;
        out_frame->addr[1] = (uint8_t *)out_frame->addr[0] + tmplen;
        out_frame->addr[2] = (uint8_t *)out_frame->addr[1] + tmplen / 2;
#if 0
        {
            ES_FILE *pfile ;
            esKRNL_TimeDly(2000);
            pfile = eLIBs_fopen("f:\\bg_src_y", "w+");
            eLIBs_fwrite(in_frame->addr[0], 1, in_frame->size.width * in_frame->size.height, pfile);
            eLIBs_fclose(pfile);
            pfile = eLIBs_fopen("f:\\bg_src_u", "w+");
            eLIBs_fwrite(in_frame->addr[1], 1, in_frame->size.width * in_frame->size.height, pfile);
            eLIBs_fclose(pfile);
            pfile = eLIBs_fopen("f:\\bg_src_v", "w+");
            eLIBs_fwrite(in_frame->addr[2], 1, in_frame->size.width * in_frame->size.height, pfile);
            eLIBs_fclose(pfile);
        }
#endif
        memcpy(out_frame->addr[0], in_frame->addr[0], tmplen);
       // __log("yuv444 to yuv22 start tick:%d", esKRNL_TimeGet());
        {
            // UV444->UV422
            uint32_t   i, j ;
            char *pdst, *psrc ;
            psrc = in_frame->addr[1] ;
            pdst = out_frame->addr[1] ;

            for (i = 0; i < in_frame->size.height ; i++)
            {
                for (j = 0; j < in_frame->size.width ; j += 2)
                {
                    *pdst++ = *psrc++ ;
                    psrc++ ;
                }
            }
        }
        //memcpy(out_frame->addr[1],in_frame->addr[1],tmplen/2);
        {
            uint32_t   i, j ;
            char *pdst, *psrc ;
            psrc = in_frame->addr[2] ;
            pdst = out_frame->addr[2] ;

            for (i = 0; i < in_frame->size.height ; i++)
            {
                for (j = 0; j < in_frame->size.width ; j += 2)
                {
                    *pdst++ = *psrc++ ;
                    psrc++ ;
                }
            }
        }
        //memcpy(out_frame->addr[2],in_frame->addr[2],tmplen/2);
       // __log("yuv444 to yuv22 finish tick:%d", esKRNL_TimeGet());
#if 0
        {
            ES_FILE *pfile ;
            pfile = eLIBs_fopen("f:\\bg_dst_y", "w+");
            eLIBs_fwrite(out_frame->addr[0], 1, in_frame->size.width * in_frame->size.height, pfile);
            eLIBs_fclose(pfile);
            pfile = eLIBs_fopen("f:\\bg_dst_u", "w+");
            eLIBs_fwrite(out_frame->addr[1], 1, in_frame->size.width * in_frame->size.height, pfile);
            eLIBs_fclose(pfile);
            pfile = eLIBs_fopen("f:\\bg_dst_v", "w+");
            eLIBs_fwrite(out_frame->addr[2], 1, in_frame->size.width * in_frame->size.height, pfile);
            eLIBs_fclose(pfile);
        }
#endif
    }
    else
    {
        out_frame->fmt.fmt.rgb.pixelfmt = PIXEL_COLOR_ARGB8888;
        out_frame->fmt.fmt.rgb.br_swap = 0;
        out_frame->fmt.fmt.rgb.pixseq = 0;
    }

    return ret_val;
}


/************************************************************************
* Function: get_fb_from_file
* Description: ���ļ���ȡ��fb
* Input:
*    FB *framebuffer��frame buffer.
*            ���������fb typeΪFB_TYPE_RGB����FB_TYPE_YUV
*    uint8_t *buffer: framebuffer�ռ䡣Ĭ��Ϊ��ȡyuv422��ʽ����СΪ2*height*width
*    uint8_t *screen_size: ��Ļ�ߴ�
*    uint8_t mod: ģʽ����:
*       BACKLAYER_MOD_RATIO----�ʺ���Ļ�ߴ�(ͼƬ�����������)
*       BACKLAYER_MOD_STRETCH----����ģʽ,���ŵ���Ļ�ߴ�
*    uint8_t * filename������fb���ļ���
* Output:
*    FB *framebuffer����ȡ���ĵ�frame buffer
* Return:
*     EPDK_OK: �ɹ�
*     EPDK_FAIL: ʧ��
*************************************************************************/
int32_t get_fb_from_file(FB *framebuffer, uint8_t *buffer, SIZE *screen_size, uint8_t mod, uint8_t *filename)
{
    __hdle              fd;
    uint32_t            tmp_len;
    __fb_file_header_t  file_header;
    int32_t             ret_val = EPDK_FAIL;
    FB                  tmp_frame;
    char                *pbuf;
    uint32_t            file_len;
    uint32_t            read_len;
    char                *uncompress_buf;
    uint32_t            uncompress_len;

    __log("filename=%s", filename);

    fd = eLIBs_fopen((char *)filename, "rb");

    if (fd == NULL)
    {
        __wrn("open file:%s err!", filename);
        return EPDK_FAIL;
    }

    eLIBs_fseek(fd, 0, SEEK_END);
    file_len    = eLIBs_ftell(fd);
    pbuf        = palloc((file_len + 1023) / 1024, 0);

    if (!pbuf)
    {
        eLIBs_fclose(fd);
        __log("palloc fail...");
        return EPDK_FAIL;
    }

    eLIBs_fseek(fd, 0, SEEK_SET);
    read_len    = eLIBs_fread(pbuf, 1, file_len, fd);

    if (read_len != file_len)
    {
        pfree(pbuf, (file_len + 1023) / 1024);
        eLIBs_fclose(fd);
        __log("fread fail...");
        return EPDK_FAIL;
    }

    //δѹ���ı���
    if (EPDK_FALSE == AZ100_IsCompress(pbuf, file_len))
    {
        __log("bg pic is uncompress...");
        uncompress_buf  = pbuf;
        uncompress_len  = file_len;
        __log("uncompress_buf=%x", uncompress_buf);
    }
    else//��ѹ���ı���
    {
        __log("bg pic is compress...");
        uncompress_len  = AZ100_GetUncompressSize(pbuf, file_len);
        uncompress_buf  = palloc((uncompress_len + 1023) / 1024, 0);
        __log("compress_buf=%x", uncompress_buf);

        if (!uncompress_buf)
        {
            pfree(pbuf, (file_len + 1023) / 1024);
            eLIBs_fclose(fd);
            __log("palloc fail...");
            return EPDK_FAIL;
        }

        ret_val = AZ100_Uncompress(pbuf, file_len, uncompress_buf, uncompress_len);

        if (EPDK_FAIL == ret_val)
        {
            pfree(pbuf, (file_len + 1023) / 1024);
            pfree(uncompress_buf, (uncompress_len + 1023) / 1024);
            eLIBs_fclose(fd);
            __log("uncompress fail...");
            return EPDK_FAIL;
        }

        pfree(pbuf, (file_len + 1023) / 1024);
        pbuf = NULL;
    }

    pbuf    = uncompress_buf;
    memcpy((void *)&file_header, pbuf, sizeof(__fb_file_header_t));
    
    pbuf    += sizeof(__fb_file_header_t);
    memset(&tmp_frame, 0x0, sizeof(FB));

    tmp_frame.size.height   = file_header.height;
    tmp_frame.size.width    = file_header.width;
    tmp_frame.fmt.cs_mode   = (__cs_mode_t)file_header.cs_mode;
    tmp_frame.fmt.type      = (__fb_type_t)file_header.fmt_type;

    if (tmp_frame.fmt.type == FB_TYPE_RGB)
    {
    	__log("tmp_frame.fmt.type == FB_TYPE_RGB");
        tmp_frame.fmt.fmt.rgb.pixelfmt  = (__pixel_rgbfmt_t)file_header.pix_fmt;
        tmp_frame.fmt.fmt.rgb.pixseq    = (uint8_t)file_header.pix_seq;
        tmp_frame.fmt.fmt.rgb.br_swap   = (__bool)file_header.mod_or_br_swap;
    }
    else
    {
        if (file_header.mod_or_br_swap != YUV_MOD_NON_MB_PLANAR)
        {
            __wrn("not support format!");
            goto EXIT_GET_FB0;
        }

        tmp_frame.fmt.fmt.yuv.pixelfmt  = (__pixel_yuvfmt_t)file_header.pix_fmt;
        tmp_frame.fmt.fmt.yuv.mod       = (__yuv_mod_t)file_header.mod_or_br_swap;
        tmp_frame.fmt.fmt.yuv.pixelfmt  = (__pixel_yuvfmt_t)file_header.pix_fmt;
        tmp_frame.fmt.fmt.yuv.yuvseq    = (__yuv_seq_t)file_header.pix_seq;
    }

    tmp_frame.addr[0]   = pbuf;
    pbuf    +=  file_header.data_len[0];

    if (file_header.data_len[1] != 0)
    {
        tmp_frame.addr[1]   = pbuf;
        pbuf    +=  file_header.data_len[1];
    }

    if (file_header.data_len[2] != 0)
    {
        tmp_frame.addr[2]   = pbuf;
    }
/*	
if(1)
{
	static int cnt = 0;
	ES_FILE *yfile;
	ES_FILE *ufile;
	ES_FILE *vfile;
	char filenamey[20] ; 
	char filenameu[20] ; 
	char filenamev[20] ; 
		eLIBs_sprintf(filenamey, "E:\\y%d_.bin",cnt);
		eLIBs_sprintf(filenameu, "E:\\u%d_.bin",cnt);
		eLIBs_sprintf(filenamev, "E:\\v%d_.bin",cnt);
		cnt++;
	__err("filenamey = %s\n",filenamey);
	__err("filenameu = %s\n",filenameu);
	__err("filenamev = %s\n",filenamev);
	yfile=eLIBs_fopen(filenamey,"wb");
	ufile=eLIBs_fopen(filenameu,"wb");
	vfile=eLIBs_fopen(filenamev,"wb");
	if(yfile && ufile && vfile)
	{
	  __err("Create y.dat successed\n");      
	  __err("Create u.dat successed\n");      
	  __err("Create v.dat successed\n");      
	  eLIBs_fwrite((void *)tmp_frame.addr[0],1,640 * 480,yfile);
	  eLIBs_fwrite((void *)tmp_frame.addr[1],1,640 * 480/2,ufile);
	  eLIBs_fwrite((void *)tmp_frame.addr[2],1,640 * 480/2,vfile);
	  eLIBs_fclose(yfile);        
	  eLIBs_fclose(ufile);        
	  eLIBs_fclose(vfile);        
	}

}
*/
    framebuffer->addr[0]    = buffer;

    ret_val = convert_fb(&tmp_frame, framebuffer, screen_size, mod);

	//while(1)
		//esKRNL_TimeDly(100);
EXIT_GET_FB0:
    eLIBs_fclose(fd);

    pfree(uncompress_buf, (uncompress_len + 1023) / 1024);

    return ret_val;
}
