//
// Created by zxh on 2021/6/18.
//

#ifndef VOICECHANGE_PLAYER_H
#define VOICECHANGE_PLAYER_H


#include "com_zxh_voicechange_MainActivity.h"

using namespace FMOD;

static bool is_stop = false;
static bool is_doing = false;

class Player {

private:
    //定义系统
    System *system = 0;
    //定义通道
    Channel *channel = 0;
    //定义声音
    Sound *sound = 0;

    const char *path_ = 0;

    //记录传递过来自定义的值
    float picth_v;
    float echo_v;
    float tremolo_v;
    float frequency_v;


public:
    Player();

    virtual ~Player();

    void play_music(JNIEnv *env, jobject jobject1, jstring path, jint mode, jfloat d, jfloat d1,
                    jfloat d2,
                    jfloat d3);

    void stop_music(JNIEnv *env, jobject job);

    void
    play_special_music(JNIEnv *env, jobject job, jstring path, jfloat picth_value,
                       jfloat echo_value,
                       jfloat tremolo_value, jfloat frequency_value);
};


#endif //VOICECHANGE_PLAYER_H
