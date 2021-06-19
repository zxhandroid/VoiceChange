
#include "Player.h"



//全局的player
Player *player = 0;

extern "C" JNIEXPORT void JNICALL Java_com_zxh_voicechange_MainActivity_playMusicVoiceNative
        (JNIEnv *env, jobject jobject1, jstring path, jint mode, jfloat pitch_value,
         jfloat echo_value, jfloat tremolo_value, jfloat frequency_value) {

    player = new Player();

    player->play_music(env, jobject1, path, mode, pitch_value, echo_value, tremolo_value,
                       frequency_value);

}

extern "C"
JNIEXPORT void JNICALL
Java_com_zxh_voicechange_MainActivity_pauseOtherVoice(JNIEnv *env, jobject thiz) {
    if (!player) {
        player = new Player();
    }
    player->stop_music(env,thiz);
}
