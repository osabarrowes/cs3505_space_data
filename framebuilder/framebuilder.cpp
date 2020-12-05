extern "C" {
    #include <libavcodec/avcodec.h>
    #include <libavformat/avformat.h>
    #include <libswscale/swscale.h>
    #include <libavfilter/avfilter.h>
    #include <libavutil/opt.h>
    #include <stdio.h>
    #include <stdlib.h>
}

#include "peek_directory_iterator.h"
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <filesystem>

/**
 * this encodes and writes 1 frame of the "video"
 * this was mostly taken from the example code
 * */

static void encode(AVCodecContext *enc_ctx, AVFrame *frame, AVPacket *pkt,
                   FILE *outfile)
{
    // used for understanding success/fail returns
    int ret;

    // puts the provided frame into the encoder
    ret = avcodec_send_frame(enc_ctx, frame);
    if (ret < 0) {
        fprintf(stderr, "Error sending a frame for encoding\n");
        av_log(NULL, AV_LOG_ERROR, "Error sending a frame for encoding\n");
        printf("error code: %d \n", ret);

        // arcane function gets the error in readable form
        char err[1024] = {0};
        av_strerror(ret, err, 1024);

        ret = AVERROR(EAGAIN);
        ret = AVERROR(EINVAL);
        exit(1);
    }

    while (ret >= 0) {
        // this gets the packet out of the encoder, which contains a bunch of info
        ret = avcodec_receive_packet(enc_ctx, pkt);
        if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
            return;
        else if (ret < 0) {
            fprintf(stderr, "Error during encoding\n");
            exit(1);
        }
	
        // this writes the info in the packet to the file
        fwrite(pkt->data, 1, pkt->size, outfile);
        
    }
}

/**
 * taken from example code in muxing.c in the ffmpeg examples library
 */
 
static AVFrame *alloc_picture(enum AVPixelFormat pix_fmt, int width, int height)
{
    AVFrame *picture;
    int ret;

    picture = av_frame_alloc();
    if (!picture)
        return NULL;

    picture->format = pix_fmt;
    picture->width  = width;
    picture->height = height;

    /* allocate the buffers for the frame data */
    ret = av_frame_get_buffer(picture, 0);
    if (ret < 0) {
        fprintf(stderr, "Could not allocate frame data.\n");
        exit(1);
    }

    return picture;
}

// this copies the input frame into the output frame at the specified position
// this function only works in RGBA currently
static void copy_frame(AVFrame *output_frame, AVFrame *input_frame, int posx, int posy)
{
    int output_width = output_frame->width;
    int output_height = output_frame->height;
    int input_width = input_frame->width;
    int input_height = input_frame->height;

    // in RGBA each pixel is 4 bytes, so this position needs to be converted to bytes instead of pixels
    posx *= 4;
    // this loop iterates over all the contents of the input frame and copies them into the output frame
    // the copy is offset by the provided position coordinates
    for(int y = 0; y < input_height && (y + posy) < output_height; y++)
    {
        for(int x = 0; x < input_width*4 && (x + posx) < output_width * 4; x++)
        {
            output_frame->data[0][(y + posy) * output_frame->linesize[0] + x + posx] = input_frame->data[0][y * input_frame->linesize[0] + x];
        }
    }
}

// this function does all of the loading necessary for the provided image
static AVFrame* load_frame(const char *filename)
{
    int ret;
    AVCodec *dec;
    AVPacket *pkt = av_packet_alloc();
    AVFrame *output_frame = av_frame_alloc();
    AVFormatContext *format = NULL;
    AVCodecContext *codec = NULL;

    // this opens the provided file as a filestream-like thing probably
    if ((ret = avformat_open_input(&format, filename, NULL, NULL)) < 0) {
        av_log(NULL, AV_LOG_ERROR, "Cannot open input file\n");
        // this arcane function gets us the error in text form so we know what's going on
        char err[1024] = {0};
        av_strerror(ret, err, 1024);
        std::cout << err << std::endl;

        return NULL;
    }

    // currently not understood
    int stream_index = av_find_best_stream(format, AVMEDIA_TYPE_VIDEO, -1, -1, &dec, 0);
    if (stream_index < 0) {
        av_log(NULL, AV_LOG_ERROR, "Cannot find a video stream in the input file\n");
        return NULL;
    }

    // this allocates the decoder context
    codec = avcodec_alloc_context3(dec);
    if (!codec)
        return NULL;

    // this builds the decoder context using info from the format context
    avcodec_parameters_to_context(codec, format->streams[stream_index]->codecpar);

    // this actually opens the decoder
    if ((ret = avcodec_open2(codec, dec, NULL)) < 0) {
        av_log(NULL, AV_LOG_ERROR, "Cannot open video decoder\n");
        return NULL;
    }

    // this reads the next frame in the file, like an iterator for video frames, and puts it into the packet
    if ((ret = av_read_frame(format, pkt)) < 0) {
        av_log(NULL, AV_LOG_ERROR, "Failure at read frame\n");
        return NULL;
    }

    // this puts the data in the packet through the decoder
    ret = avcodec_send_packet(codec, pkt);
    if (ret < 0) {
        av_log(NULL, AV_LOG_ERROR, "Error while sending a packet to the decoder\n");
        return NULL;
    }

    // this pulls out the frame from the decoder that it decoded and puts it into a frame
    ret = avcodec_receive_frame(codec, output_frame);
    if (ret < 0) {
        av_log(NULL, AV_LOG_ERROR, "Error while receiving a frame from the decoder\n");
        return NULL;
    }

    // close and free up the format and codec, otherwise we'll have too many files open
    avformat_close_input(&format);
    avcodec_close(codec);
    avcodec_free_context(&codec);
    avformat_free_context(format);


    return output_frame;
    }

// saves the frame as a png image
static void save_frame(char* filename, AVFrame *frame, const AVCodec *codec)
{
    int ret;
    AVPacket *pkt = av_packet_alloc();
    // allocates an encoder so we can save the frame 
    AVCodecContext *encode_context = avcodec_alloc_context3(codec);
    if (!encode_context) {
        fprintf(stderr, "Could not allocate video codec context\n");
        exit(1);
    }

    // this block sets up the relevant information into our encoder

    encode_context->bit_rate = 400000;
    encode_context->width = frame->width;
    encode_context->height = frame->height;
    encode_context->time_base.num = 1;
    encode_context->time_base.den = 25;
    encode_context->framerate = (AVRational){25, 1};
    encode_context->gop_size = 10;
    encode_context->max_b_frames = 1;
    encode_context->pix_fmt = AV_PIX_FMT_RGBA;

    // this actually opens the encoder
    ret = avcodec_open2(encode_context, codec, NULL);
    if (ret < 0) {
        // this debug print was written by the example code and for some reason it doesn't compile
        // I've just removed it so we don't have to worry about it
        //fprintf(stderr, "Could not open codec: %s\n", av_err2str(ret));
        std::cout << "could not open codec" << std::endl;
        exit(1);
    }

    FILE *file = fopen(filename, "wb");
    // this encodes the frame and saves it to the file
    encode(encode_context, frame, pkt, file);
    fclose(file);

    av_packet_free(&pkt);
    avcodec_close(encode_context);
    avcodec_free_context(&encode_context);
  
}

// this pulls out the filename from the filepath based on our folder architecture
std::string extract_filename(std::string filename)
{
    // our file structure is of the format "framesegments/<datatype>/<time>.png"
    // strips off the "framesegments/"
    int pos = filename.find("/");
    filename = filename.substr(pos + 1, filename.length() - 1);
    // strips off the "<datatype>/"
    pos = filename.find("/");
    filename = filename.substr(pos + 1, filename.length() - 1);
    // strips off the ".png"
    pos = filename.find(".");
    filename = filename.substr(0, pos);
    return filename;
}

// this advances the peek iterator up to the starting time in the directory
void advance_peek_iterator(peek_directory_iterator &iter, int time)
{

    // while the filename number of the next item is less than the starting time
    while(iter.peek() != "" && std::stoi(extract_filename(iter.peek())) < time)
    {
        iter.next();   
    }
}

// this converts our date/time strings into unix timestamp values
int unix_convert(std::string input)
{

    // used https://stackoverflow.com/questions/17681439/convert-string-time-to-unix-timestamp/17686222
    // as a guide for this, doing it with math wasn't working reeeeee
    std::tm t{};
    std::istringstream ss(input);

    ss >> std::get_time(&t, "%m.%d.%Y.%H.%M.%S");
    if (ss.fail()) {
        throw std::runtime_error{"failed to parse time string"};
    } 
    std::time_t time_stamp = mktime(&t);

    return time_stamp;

}

int main(int argc, char **argv) 
{
    // check for correct command line usage
    if(argc <= 2)
    {
        std::cout << "usage: <start time> <end time>" << std::endl;
        std::cout << "time format: MM.DD.YYYY.HH.mm" << std::endl;
        std::cout << "m = minutes, use 24 hour time" << std::endl;
        exit(0);
    }

    // 11.01.2020.00.00 11.08.2020.00.00
    // parse the given date/times into unix timestamp
    double start_time = unix_convert(argv[1]);
    double end_time = unix_convert(argv[2]);    

    // make and initialize the peek iterators
    peek_directory_iterator isscoords_iter("framesegments/isscoords");
    peek_directory_iterator issfeed_iter("framesegments/issfeed");
    peek_directory_iterator lightningmap_iter("framesegments/lightningmap");
    peek_directory_iterator supernovae_iter("framesegments/supernovae");
    peek_directory_iterator twitch_iter("framesegments/twitch");
    peek_directory_iterator weather_iter("framesegments/weather");

    advance_peek_iterator(isscoords_iter, start_time);
    advance_peek_iterator(issfeed_iter, start_time);
    advance_peek_iterator(lightningmap_iter, start_time);
    advance_peek_iterator(supernovae_iter, start_time);
    advance_peek_iterator(twitch_iter, start_time);
    advance_peek_iterator(weather_iter, start_time);

    // codec used for encoding the frames
    const AVCodec *encoder = avcodec_find_encoder_by_name("png"); 

    // for loop that scales with times
    double scaled_time;
    for(int i = 0; i < 600; i++)
    {
        scaled_time = (((end_time - start_time) / 600) * i) + start_time;
        
        // advance all iterators to the new point in time
        advance_peek_iterator(isscoords_iter, scaled_time);
        advance_peek_iterator(issfeed_iter, scaled_time);
        advance_peek_iterator(lightningmap_iter, scaled_time);
        advance_peek_iterator(supernovae_iter, scaled_time);
        advance_peek_iterator(twitch_iter, scaled_time);
        advance_peek_iterator(weather_iter, scaled_time);

        // load the frame segments
        AVFrame *bg_frame           = load_frame("framesegments/bg.png");
        AVFrame *isscoords_frame    = load_frame(isscoords_iter.get().c_str());
        AVFrame *issfeed_frame      = load_frame(issfeed_iter.get().c_str());
        AVFrame *lightningmap_frame = load_frame(lightningmap_iter.get().c_str());
        AVFrame *supernovae_frame   = load_frame(supernovae_iter.get().c_str());
        AVFrame *twitch_frame       = load_frame(twitch_iter.get().c_str());
        AVFrame *weather_frame      = load_frame(weather_iter.get().c_str());

        // build the compiled frame from those segments
        AVFrame *build_frame = alloc_picture(AV_PIX_FMT_RGBA, 1280, 1024);
        copy_frame(build_frame, bg_frame,           0,   0);
        copy_frame(build_frame, isscoords_frame,    0,   475);
        copy_frame(build_frame, issfeed_frame,      0,   75 );
        copy_frame(build_frame, lightningmap_frame, 720, 70 );
        copy_frame(build_frame, supernovae_frame,   50,  810);
        copy_frame(build_frame, twitch_frame,       690, 810);
        copy_frame(build_frame, weather_frame,      260, 570);

        // add padding to the filename
        std::ostringstream output_string;
        if(i < 10)
            output_string << "framesegments/frames/frame00" << i << ".png";
        else if(i < 100)
            output_string << "framesegments/frames/frame0" << i << ".png";
        else
            output_string << "framesegments/frames/frame" << i << ".png";

        std::string output_filename = output_string.str();

        // save the finished frame
        save_frame(&output_filename[0], build_frame, encoder);

        // delete frames
        av_frame_free(&bg_frame);
        av_frame_free(&isscoords_frame);
        av_frame_free(&issfeed_frame);
        av_frame_free(&lightningmap_frame);
        av_frame_free(&supernovae_frame);
        av_frame_free(&twitch_frame);
        av_frame_free(&weather_frame);
        av_frame_free(&build_frame);
        
    }
}
