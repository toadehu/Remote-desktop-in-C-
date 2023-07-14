#pragma once


#include <libavcodec/avcodec.h>
#include <libavutil/imgutils.h>
#include <libavutil/opt.h>
#include <libavutil/samplefmt.h>

void save_yuv420p_frame(AVFrame *frame, const char *filename) {
    FILE *file = fopen(filename, "wb");
    if (!file) {
        fprintf(stderr, "Could not open file: %s\n", filename);
        exit(1);
    }

    for (int plane = 0; plane < 3; plane++) {
        unsigned char *data = frame->data[plane];
        int linesize = frame->linesize[plane];
        int height = plane == 0 ? frame->height : frame->height / 2;

        for (int y = 0; y < height; y++) {
            fwrite(data + y * linesize, 1, linesize, file);
        }
    }

    fclose(file);
}


typedef struct video_decoder
{
    AVCodec* codec;
    AVCodecContext* codec_ctx;
    AVFrame* frame;
    AVPacket* packet;
} video_decoder;


/***/

video_decoder* create_video_decoder(int width, int height) 
{
    video_decoder* _decoder = malloc(sizeof(video_decoder));

    avcodec_register_all();

    _decoder->codec = avcodec_find_decoder(AV_CODEC_ID_H264);
    if (!_decoder->codec)
    {
        fprintf(stderr, "Codec not found\n");
        return NULL;
    }

    _decoder->codec_ctx = avcodec_alloc_context3(_decoder->codec);
    if (!_decoder->codec_ctx)
    {
        fprintf(stderr, "Could not allocate video codec context\n");
        return NULL;
    }

    _decoder->codec_ctx->width = width;
    _decoder->codec_ctx->height = height;
    _decoder->codec_ctx->pix_fmt = AV_PIX_FMT_YUV420P;

    if (avcodec_open2(_decoder->codec_ctx, _decoder->codec, NULL) < 0)
    {
        fprintf(stderr, "Could not open codec\n");
        return NULL;
    }

    _decoder->frame = av_frame_alloc();
    if (!_decoder->frame)
    {
        fprintf(stderr, "Could not allocate video frame\n");
        return NULL;
    }

    _decoder->packet = av_packet_alloc();
    if (!_decoder->packet)
    {
        fprintf(stderr, "Could not allocate video packet\n");
        return NULL;
    }

    return _decoder;
}

int decode_frame(video_decoder* _decoder, unsigned char* data, int size, bool special)/*, (bool*)(unsigned char*) callee) */
{
    int ret;

    _decoder->packet->data = data;
    _decoder->packet->size = size;

    ret = avcodec_send_packet(_decoder->codec_ctx, _decoder->packet);
    if (ret < 0)
    {
        fprintf(stderr, "Error sending a packet for decoding\n");
        return -1;
    }

    /* Normally ret is 1*/
    while (ret >= 0)
    {
        ret = avcodec_receive_frame(_decoder->codec_ctx, _decoder->frame);
        if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
            return 0;
        else if (ret < 0)
        {
            fprintf(stderr, "Error during decoding\n");
            return -1;
        }
        if (special == true)
        {
            save_yuv420p_frame(_decoder->frame, "output.yuv");
        }
        av_frame_unref(_decoder->frame);
    }

    return 0;
}

void free_decoder(video_decoder* _decoder) 
{
    avcodec_free_context(&_decoder->codec_ctx);
    av_frame_free(&_decoder->frame);
    av_packet_free(&_decoder->packet);
    free(_decoder);
}

void save_yuv420_buffer(const char *filename, unsigned char *buffer, int width, int height)
{
    FILE *f = fopen(filename, "wb");
    if (f == NULL) {
        printf("Error opening file for writing\n");
        return;
    }

    /* Write Y plane data*/
    int y_size = width * height;
    fwrite(buffer, 1, y_size, f);

    /* Write U plane data*/
    int u_size = y_size / 4;
    fwrite(buffer + y_size, 1, u_size, f);

    /* Write V plane data*/
    fwrite(buffer + y_size + u_size, 1, u_size, f);

    fclose(f);
}


//*/
