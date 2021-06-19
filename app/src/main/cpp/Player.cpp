//
// Created by zxh on 2021/6/18.
//

#include "Player.h"


Player::Player() {

}

Player::~Player() {

}

void Player::play_music(JNIEnv *env, jobject jobject1, jstring path, jint mode, jfloat picth_value,
                        jfloat echo_value, jfloat tremolo_value, jfloat frequency_value) {

    picth_v = picth_value;
    echo_v = echo_value;
    tremolo_v = tremolo_value;
    frequency_v = frequency_value;

    //TODO 1. 创建系统
    System_Create(&system);

    //TODO 2. 系统初始化
    system->init(32, FMOD_INIT_NORMAL, 0);

    //获取c 识别的path
    path_ = env->GetStringUTFChars(path, NULL);

    //TODO 3. 创建声音
    system->createSound(path_, FMOD_DEFAULT, 0, &sound);

    //TODO 4. 播放声音
    system->playSound(sound, 0, false, &channel);

    DSP *dsp = 0;

    char *content = "默认 播放完毕";
    //不同声音模式
    switch (mode) {
        case com_zxh_voicechange_MainActivity_VOICE_NORMAL:
            content = "原声模式播放完毕";
            //不做任务处理
            break;
        case com_zxh_voicechange_MainActivity_VOICE_LUOLI:
            content = "萝莉模式播放完毕";
            // 音调调高
            //创建pitch dsp
            system->createDSPByType(FMOD_DSP_TYPE_PITCHSHIFT, &dsp);
            //设置pitch的值，范围0.5 to 2.0
            dsp->setParameterFloat(FMOD_DSP_PITCHSHIFT_PITCH, 1.8F);
            //设置到通道中去
            channel->addDSP(0, dsp);

            break;
        case com_zxh_voicechange_MainActivity_VOICE_DASHU:
            content = "大叔模式播放完毕";
            //创建pitch dsp
            system->createDSPByType(FMOD_DSP_TYPE_PITCHSHIFT, &dsp);
            //设置pitch的值，范围0.5 to 2.0
            dsp->setParameterFloat(FMOD_DSP_PITCHSHIFT_PITCH, 0.8F);
            //设置到通道中去
            channel->addDSP(0, dsp);
            break;
        case com_zxh_voicechange_MainActivity_VOICE_JINGSONG:
            content = "惊悚模式播放完毕";
            //音调放低
            system->createDSPByType(FMOD_DSP_TYPE_PITCHSHIFT, &dsp);
            dsp->setParameterFloat(FMOD_DSP_PITCHSHIFT_PITCH, 0.5F);
            channel->addDSP(0, dsp);

            //回声
            system->createDSPByType(FMOD_DSP_TYPE_ECHO, &dsp);
            dsp->setParameterFloat(FMOD_DSP_ECHO_DELAY, 200.f);
            channel->addDSP(1, dsp);

            //颤抖
            system->createDSPByType(FMOD_DSP_TYPE_TREMOLO, &dsp);
            dsp->setParameterFloat(FMOD_DSP_TREMOLO_SKEW, 0.5F);
            channel->addDSP(2, dsp);

            break;
        case com_zxh_voicechange_MainActivity_VOICE_GAOGUAI:
            content = "搞怪模式播放完毕";
            // 调整频率快些
            //获取频率
            float frequency;
            channel->getFrequency(&frequency);
            //设置频率
            channel->setFrequency(frequency * 1.5f);

            break;
        case com_zxh_voicechange_MainActivity_VOICE_KONGLING:
            content = "空灵模式播放完毕";
            // 设置回声
            system->createDSPByType(FMOD_DSP_TYPE_ECHO, &dsp);
            dsp->setParameterFloat(FMOD_DSP_ECHO_DELAY, 200.0f);
            dsp->setParameterFloat(FMOD_DSP_ECHO_FEEDBACK, 2.0f);
            channel->addDSP(0, dsp);

            break;

        case com_zxh_voicechange_MainActivity_VOICE_CUSTOM:
            content = "自定义模式播放完毕";

            //获取频率
            float frequency2;
            channel->getFrequency(&frequency2);
            //设置频率
            channel->setFrequency(frequency2 * frequency_v);

            //音调放低
            system->createDSPByType(FMOD_DSP_TYPE_PITCHSHIFT, &dsp);
            dsp->setParameterFloat(FMOD_DSP_PITCHSHIFT_PITCH, picth_v);
            channel->addDSP(0, dsp);

            //回声
            system->createDSPByType(FMOD_DSP_TYPE_ECHO, &dsp);
            dsp->setParameterFloat(FMOD_DSP_ECHO_DELAY, echo_v);
            channel->addDSP(1, dsp);

            //颤抖
            system->createDSPByType(FMOD_DSP_TYPE_TREMOLO, &dsp);
            dsp->setParameterFloat(FMOD_DSP_TREMOLO_SKEW, tremolo_v);
            channel->addDSP(2, dsp);

            break;
    }

    bool is_playing = true;
    while (is_playing) {
        channel->isPlaying(&is_playing);
        if (is_stop) {
            channel->stop();
            break;
        }
        is_doing = true;
        //单位微妙
        usleep(1000);
    }

    is_stop = false;
    is_doing = false;
    //释放操作
    sound->release();
    system->close();
    system->release();
    env->ReleaseStringUTFChars(path, path_);

    //回调java函数
    jclass clazz = env->GetObjectClass(jobject1);
    jmethodID method_id = env->GetMethodID(clazz, "showToast", "(Ljava/lang/String;)V");
    jstring value = env->NewStringUTF(content);
    env->CallVoidMethod(jobject1, method_id, value);
}


void Player::stop_music(JNIEnv *env, jobject jobject1) {
    char *str = 0;
    if (is_doing) {
        str = "is_doing=true" ;
        is_stop = true;
    } else {
        str = "is_doing=false";
    }


    //回调java函数
    jclass clazz = env->GetObjectClass(jobject1);
    jmethodID method_id = env->GetMethodID(clazz, "showToast", "(Ljava/lang/String;)V");
    jstring value = env->NewStringUTF(str);
    env->CallVoidMethod(jobject1, method_id, value);
}

void Player::play_special_music(JNIEnv *env, jobject jobject1, jstring path, jfloat picth_value,
                                jfloat echo_value, jfloat tremolo_value, jfloat frequency_value) {



    play_music(env, jobject1, path, com_zxh_voicechange_MainActivity_VOICE_CUSTOM, 0, 0, 0, 0);

}

