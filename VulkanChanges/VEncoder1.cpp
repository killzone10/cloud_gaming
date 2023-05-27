#include "VEncoder1.h"
#include <iostream>
#include <stdexcept>

namespace ve {



	VEncoder1::VEncoder1(const Config& config) : config{ config } {
		codec = avcodec_find_encoder_by_name(config.codecName.c_str());

		if (codec == nullptr) {
			throw std::runtime_error{ "avcodec_find_encoder_by_name failed" };
		}

		context = avcodec_alloc_context3(codec);

		if (context == nullptr) {
			throw std::runtime_error{ "avcodec_alloc_context3 failed" };
		}

		packet = av_packet_alloc();

		if (packet == nullptr) {
			throw std::runtime_error{ "av_packet_alloc failed" };
		}

		context->bit_rate = config.bitrate;
		context->width = config.width;
		context->height = config.height;
		context->time_base = config.timebase;
		context->framerate = config.framerate;
		context->gop_size = config.gopSize;
		context->max_b_frames = config.maxBFrames;
		context->pix_fmt = config.pixelFormat;

		if (codec->id == AV_CODEC_ID_H264) {
			av_opt_set(context->priv_data, "preset", "slow", 0);
		}

		if (avcodec_open2(context, codec, nullptr) < 0) {
			throw std::runtime_error{ "avcodec_open2 failed" };
		}

		frame = av_frame_alloc();

		if (frame == nullptr) {
			throw std::runtime_error{ "av_frame_alloc failed" };
		}

		frame->format = context->pix_fmt;
		frame->width = context->width;
		frame->height = context->height;

		if (av_frame_get_buffer(frame, 0) < 0) {
			throw std::runtime_error{ "av_frame_get_buffer failed" };
		}
	}

	VEncoder1::~VEncoder1() {
		if (frame) {
			av_frame_free(&frame);
		}

		if (packet) {
			av_packet_free(&packet);
		}

		if (context) {
			avcodec_free_context(&context);
		}
	}


	int VEncoder1::encode(const RGBA* picture, Callback callback) {
	

		if (av_frame_make_writable(frame) < 0) {
			return 1;
		}


		if (picture) {
			const RGBA* pixel = picture;

			for (int y = 0; y < context->height; y++) {
				for (int x = 0; x < context->width; x++) {
					frame->data[0][y * frame->linesize[0] + x] =
						0.299 * pixel->r + 0.587 * pixel->g + 0.114 * pixel->b;
					pixel++;
				}
			}

			for (int y = 0; y < context->height / 2; y++) {
				pixel = &picture[2 * y * context->width];

				for (int x = 0; x < context->width / 2; x++) {
					frame->data[1][y * frame->linesize[1] + x] =
						128 - 0.168736 * pixel->r - 0.331264 * pixel->g + 0.5 * pixel->b;
					frame->data[2][y * frame->linesize[2] + x] =
						128 + 0.5 * pixel->r - 0.418688 * pixel->g - 0.081312 * pixel->b;
					pixel += 2;
				}
			}
		}

		frame->pts = frameCounter++;

		if (avcodec_send_frame(context, frame) < 0) {
			return 2;
		}

		for (int ret = 1; ret >= 0;) {
			ret = avcodec_receive_packet(context, packet);

			if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
				return 0;
			}
			else if (ret < 0) {
				return 4;
			}	

			//file->write(reinterpret_cast<char*>(packet->data), packet->size);
			callback(frameCounter, reinterpret_cast<const char*>(packet->data), packet->size);
			av_packet_unref(packet);
		}

		return 0;
	}

	
}