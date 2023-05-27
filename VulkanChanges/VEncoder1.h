#pragma once

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavutil/opt.h>
#include <libavutil/imgutils.h>
}

#include <memory>
#include <string>
#include <array>
#include <fstream>
#include <span>
#include <functional>
#include <span>

namespace ve {

	class VEncoder1 {
	public:
		struct Config {
			int width;
			int height;
			std::string codecName = "mpeg4";
			int64_t bitrate = 400000;
			AVRational timebase = { 1, 25 };
			AVRational framerate = { 25, 1 };
			// the number of pictures in a group of pictures
			int gopSize = 10;
			int maxBFrames = 1;
			AVPixelFormat pixelFormat = AV_PIX_FMT_YUV420P;
			explicit Config(int width, int height) : width{ width }, height{ height } {}
		};	

		struct RGBA {
			uint8_t r, g, b, a;
		};

		explicit VEncoder1(const Config& config);
		~VEncoder1();
		using Callback = std::function<void(unsigned int, const char*, size_t)>;
		int encode(const RGBA* picture, Callback callback);
	private:

		Config config;
		const AVCodec* codec{ nullptr };
		AVCodecContext* context{ nullptr };
		AVPacket* packet{ nullptr };
		AVFrame* frame{ nullptr };
		int64_t frameCounter{ 0 };

	};
}