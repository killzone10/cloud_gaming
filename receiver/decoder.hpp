#pragma once

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavutil/opt.h>
#include <libavutil/imgutils.h>
#include <libswscale/swscale.h>
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>

}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <iostream>
#include <span>


#define INBUF_SIZE 4096


class Decoder{
    public:

        Decoder();
        ~Decoder();

        AVFrame* initDecode(std::span<const char> frame);
        AVFrame* decode(const char *filename);
        void pgm_save(unsigned char *buf, int wrap, int xsize, int ysize,
                     char *filename);
        void jpg_save(unsigned char *buf, int wrap, int xsize, int ysize,
                     char *filename);

        void save_rgb_frame(unsigned char* buf, int wrap, int xsize, int ysize, char* filename);

    private:

		const AVCodec* codec{ nullptr };
        AVCodecParserContext *parser {nullptr}; 
		AVCodecContext* context{ nullptr };
		AVPacket* packet{ nullptr };
		AVFrame* frame{ nullptr };
        uint8_t inbuf[INBUF_SIZE + AV_INPUT_BUFFER_PADDING_SIZE];


};