#include "decoder.hpp"

Decoder::Decoder() {

        packet = av_packet_alloc();
        if (packet == nullptr) {
			throw std::runtime_error{ "av_packet_alloc failed" };
		}
        /* set end of buffer to 0 (this ensures that no overreading happens for damaged MPEG streams) */
        memset(inbuf + INBUF_SIZE, 0, AV_INPUT_BUFFER_PADDING_SIZE);

        codec = avcodec_find_decoder(AV_CODEC_ID_MPEG4);
		if (codec == nullptr) {
			throw std::runtime_error{ "avcodec_find_encoder_by_name failed" };
		}
        
        parser = av_parser_init(codec->id);
        if (parser == nullptr) {
			throw std::runtime_error{ "av_parser_init failed" };
		}
        

		context = avcodec_alloc_context3(codec);

		if (context == nullptr) {
			throw std::runtime_error{ "avcodec_alloc_context3 failed" };
		}
      /* For some codecs, such as msmpeg4 and mpeg4, width and height
       MUST be initialized there because this information is not
       available in the bitstream. */

        if (avcodec_open2(context, codec, NULL) < 0) {
			throw std::runtime_error{ "Could not open codec" };
        }
        frame = av_frame_alloc();
        if (frame == nullptr) {
			throw std::runtime_error{ "av_frame_alloc failed" };
        }


	}
AVFrame* Decoder::decode(const char *filename){
    char buf[1024];
    int ret;
    struct SwsContext* sws_ctx {nullptr};//
    int sts;//
    AVFrame* finalFrame{nullptr};
    ret = avcodec_send_packet(context, packet);
    if (ret < 0) {
        throw std::runtime_error{ "Error sending a packet for decoding" };
    }

    sws_ctx = sws_getContext(context->width,
                             context->height,
                             context->pix_fmt,
                             context->width,
                             context->height,
                             AV_PIX_FMT_RGB24,
                             SWS_BICUBIC,
                             NULL,
                             NULL,
                             NULL);

    if (sws_ctx == nullptr){
        throw std::runtime_error{ "sws_ctx error" };
    }
    AVFrame* pRGBFrame = av_frame_alloc();

    pRGBFrame->format = AV_PIX_FMT_RGB24;
    pRGBFrame->width = context->width;
    pRGBFrame->height = context->height;

    sts = av_frame_get_buffer(pRGBFrame, 0);

    if (sts < 0)
    {
        return finalFrame;  //Error!
    }

    while (ret >= 0) {
        ret = avcodec_receive_frame(context, frame);
        if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
            return finalFrame;
        else if (ret < 0) {
            throw std::runtime_error{ "Error during decoding\n" };
        }

    //    printf("saving frame %3d\n", context->frame_number);
        fflush(stdout);
        sts = sws_scale(sws_ctx,                //struct SwsContext* c,
                        frame->data,            //const uint8_t* const srcSlice[],
                        frame->linesize,        //const int srcStride[],
                        0,                      //int srcSliceY, 
                        frame->height,          //int srcSliceH,
                        pRGBFrame->data,        //uint8_t* const dst[], 
                        pRGBFrame->linesize);   //const int dstStride[]);

        if (sts != frame->height)
        {
            return finalFrame;  //Error!
        }

        /* the picture is allocated by the decoder. no need to
           free it */
        //snprintf(buf, sizeof(buf), "%s-%d.pgm", filename, context->frame_number);
        snprintf(buf, sizeof(buf), "%s-%d.ppm", filename, context->frame_number);
        
        // save_rgb_frame(frame->data[0], frame->linesize[0],
        //          frame->width, frame->height, buf);
        // save_rgb_frame(pRGBFrame->data[0], pRGBFrame->linesize[0], pRGBFrame->width, pRGBFrame->height, buf);
        // return pRGBFrame;
        return frame;
    }
    return finalFrame;
}

AVFrame* Decoder::initDecode(std::span<const char> data){
    int ret;
    char buf[1024];
    auto data_value = const_cast<char*>(data.data());
    AVFrame* returnFrame {nullptr};
   // std::span<const char> part;
    auto size = data.size();
    while (data.size() > 0) {// not needed
        ret = av_parser_parse2(parser, context, &packet->data, &packet->size,
                reinterpret_cast<uint8_t *>(data_value),size, AV_NOPTS_VALUE, AV_NOPTS_VALUE, 0);
        if (ret < 0) {
            throw std::runtime_error{ "ret less than 0" };
        }
        // frame.data()+= ret;
        size -= ret;
        if (packet->size){
            returnFrame = decode("proba");
        }
        else {
            break;
        }
   }
   return returnFrame;
}



Decoder::~Decoder() {
        if(parser){
        av_parser_close(parser);
    }
    if (context) {
        avcodec_free_context(&context);
    }
    if (frame) {
        av_frame_free(&frame);
    }

    if (packet) {
        av_packet_free(&packet);
    }

    
    
}






void Decoder::pgm_save(unsigned char *buf, int wrap, int xsize, int ysize,
                     char *filename)
{
    FILE *f;
    int i;
 
    f = fopen(filename,"wb");
    fprintf(f, "P5\n%d %d\n%d\n", xsize, ysize, 255);
    for (i = 0; i < ysize; i++)
        fwrite(buf + i * wrap, 1, xsize, f);
    fclose(f);
}
 
 void Decoder::save_rgb_frame(unsigned char* buf, int wrap, int xsize, int ysize, char* filename)
{
    FILE* f;
    int i;

    f = fopen(filename, "wb");
    fprintf(f, "P6\n%d %d\n%d\n", xsize, ysize, 255);

    for (i = 0; i < ysize; i++)
    {
        fwrite(buf + i * wrap, 1, xsize*3, f);
    }

    fclose(f);
}


 

