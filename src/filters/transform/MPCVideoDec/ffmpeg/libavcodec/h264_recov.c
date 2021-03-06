/**
 * ffdshow custom stuff (based on decode_nal_units)
 *
 * @param[out] recovery_frame_cnt. Valid only if GDR.
 * @return   0: no recovery point, 1:I-frame 2:Recovery Point SEI (GDR), 3:IDR, -1:error
 */
int avcodec_h264_search_recovery_point(AVCodecContext *avctx,
                         const uint8_t *buf, int buf_size, int *recovery_frame_cnt)
{
    H264Context *h = avctx->priv_data;
    MpegEncContext *s = &h->s;
    int buf_index = 0;
    int found = 0; // 0: no recovery point, 1:Recovery Point SEI (GDR), 2:IDR
    H264Context *hx;
    int Islice_detected = 0;

    h->nal_length_size = avctx->nal_length_size ? avctx->nal_length_size : 4;

    for(;;){
        int consumed;
        int dst_length;
        int bit_length;
        const uint8_t *ptr;
        int i, nalsize = 0;
        int err;

        if(h->is_avc) {
            if(buf_index >= buf_size) break;
            nalsize = 0;
            for(i = 0; i < h->nal_length_size; i++)
                nalsize = (nalsize << 8) | buf[buf_index++];
            if(nalsize <= 1 || (nalsize+buf_index > buf_size)){
                if(nalsize == 1){
                    buf_index++;
                    continue;
                }else{
                    av_log(h->s.avctx, AV_LOG_ERROR, "AVC: nal size %d\n", nalsize);
                    break;
                }
            }
        } else {
            // start code prefix search
            for(; buf_index + 3 < buf_size; buf_index++){
                // This should always succeed in the first iteration.
                if(buf[buf_index] == 0 && buf[buf_index+1] == 0 && buf[buf_index+2] == 1)
                    break;
            }

            if(buf_index+3 >= buf_size) break;

            buf_index+=3;
        }

        hx = h->thread_context[0];

        // h->nal_ref_idc = buf[buf_index] >> 5;
        // h->nal_unit_type = src[buf_index] & 0x1F;

        ptr= ff_h264_decode_nal(hx, buf + buf_index, &dst_length, &consumed, h->is_avc ? nalsize : buf_size - buf_index);
        if (ptr==NULL || dst_length < 0){
            return -1;
        }
        while(ptr[dst_length - 1] == 0 && dst_length > 0)
            dst_length--;
        bit_length= !dst_length ? 0 : (8*dst_length - ff_h264_decode_rbsp_trailing(h, ptr + dst_length - 1));

        if (h->is_avc && (nalsize != consumed)){
            av_log(h->s.avctx, AV_LOG_ERROR, "AVC: Consumed only %d bytes instead of %d\n", consumed, nalsize);
            consumed= nalsize;
        }

        buf_index += consumed;


        switch(hx->nal_unit_type){
        case NAL_IDR_SLICE:
            return 3;
        case NAL_SEI:
            if (ptr[0] == 6/* Recovery Point SEI */){
                init_get_bits(&s->gb, ptr, bit_length);
                ff_h264_decode_sei(h);
                if (found < 2)
                    found = 2;
                break;
            }
            break;
        case NAL_SPS:
            init_get_bits(&s->gb, ptr, bit_length);
            ff_h264_decode_seq_parameter_set(h);

            if(s->flags& CODEC_FLAG_LOW_DELAY)
                s->low_delay=1;

            if(avctx->has_b_frames < 2)
                avctx->has_b_frames= !s->low_delay;
            break;
        case NAL_PPS:
            init_get_bits(&s->gb, ptr, bit_length);

            ff_h264_decode_picture_parameter_set(h, bit_length);

            break;
        case NAL_AUD:
        {
            int primary_pic_type;
            init_get_bits(&s->gb, ptr, bit_length);
            primary_pic_type = get_bits(&s->gb, 3);
            if (found == 0 && (primary_pic_type == 0 || primary_pic_type == 3)) // I-frame (all I/SI slices)
                found = 1;
            break;
        }
        case NAL_SLICE:
        case NAL_DPA:
            // decode part of slice header and find I frame
            if (found == 0){
                unsigned int slice_type;

                init_get_bits(&s->gb, ptr, bit_length);
                get_ue_golomb(&s->gb); // first_mb_in_slice
                slice_type= get_ue_golomb(&s->gb);
                if (slice_type == 2 || slice_type == 4 || slice_type == 7 || slice_type == 9) // I/SI slice
                 Islice_detected = 1;
                else
                 return 0;
                break;
            }
        case NAL_DPB:
        case NAL_DPC:
        case NAL_END_SEQUENCE:
        case NAL_END_STREAM:
        case NAL_FILLER_DATA:
        case NAL_SPS_EXT:
        case NAL_AUXILIARY_SLICE:
            break;
        default:
            av_log(avctx, AV_LOG_DEBUG, "Unknown NAL code: %d (%d bits)\n", h->nal_unit_type, bit_length);
        }

    }
    if (found == 0 && Islice_detected)
     found = 1;
    *recovery_frame_cnt = h->sei_recovery_frame_cnt;
    return found;
}
