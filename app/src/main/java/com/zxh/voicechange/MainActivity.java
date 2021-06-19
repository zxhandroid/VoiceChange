package com.zxh.voicechange;


import android.os.Bundle;
import android.view.View;
import android.widget.SeekBar;
import android.widget.TextView;

import org.fmod.FMOD;

import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

import androidx.appcompat.app.AppCompatActivity;

/**
 * 用javah 生成头文件，首先要保证是使用jdk 8
 * 然后cd 到java 文件夹下
 * 然后javah com.zxh.voicechange.MainActivity 回车就可以生成相应的.h文件
 */
public class MainActivity extends AppCompatActivity {

    //定义一些常量
    private static final int VOICE_NORMAL = 0;
    private static final int VOICE_LUOLI = 1;
    private static final int VOICE_DASHU = 2;
    private static final int VOICE_JINGSONG = 3;
    private static final int VOICE_GAOGUAI = 4;
    private static final int VOICE_KONGLING = 5;
    private static final int VOICE_CUSTOM = 6;
    private static final String TAG = "MainActivity";

    private String path;
    private int voiceType;

    // TODO 应用开始时加载总库
    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    private ExecutorService service;
    private TextView tvStatus;
    private SeekBar sbEcho;
    private SeekBar sbPitch;
    private SeekBar sbTremolo;
    private SeekBar sbFrequency;
    private TextView tvPitch;
    private TextView tvEcho;
    private TextView tvTremolo;
    private TextView tvFrequency;
    private float pitchValue = 1.0f;
    private float echoValue = 5000f;
    private float tremoloValue = 0;
    private float frequencyValue = 1;


    //-----------native 方法 start ----
    private native void playMusicVoiceNative(String path, int voiceType, float pitchValue, float echoValue, float tremoloValue, float frequencyValue);

    private native void pauseOtherVoice();

    //-----------native 方法 end ----

    /**
     * 供native函数调用
     *
     * @param message
     */
    public void showToast(final String message) {
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                tvStatus.setText(message);
            }
        });
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        path = "file:///android_asset/music.mp3";
        service = Executors.newFixedThreadPool(Runtime.getRuntime().availableProcessors() + 1);
        tvStatus = findViewById(R.id.tv_status);

        sbPitch = findViewById(R.id.sb_pitch);
        sbEcho = findViewById(R.id.sb_echo);
        sbTremolo = findViewById(R.id.sb_tremolo);
        sbFrequency = findViewById(R.id.sb_frequency);

        tvPitch = findViewById(R.id.tv_pitch);
        tvEcho = findViewById(R.id.tv_echo);
        tvTremolo = findViewById(R.id.tv_tremolo);
        tvFrequency = findViewById(R.id.tv_frequency);

        initSbEvent();

        //初始化环境
        FMOD.init(this);
    }

    private void initSbEvent() {
        sbPitch.setOnSeekBarChangeListener(new DelegateSeekBarChangeListener());
        sbEcho.setOnSeekBarChangeListener(new DelegateSeekBarChangeListener());
        sbTremolo.setOnSeekBarChangeListener(new DelegateSeekBarChangeListener());
        sbFrequency.setOnSeekBarChangeListener(new DelegateSeekBarChangeListener());
    }

    //适配一下
    class DelegateSeekBarChangeListener implements SeekBar.OnSeekBarChangeListener {
        @Override
        public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
            switch (seekBar.getId()) {
                case R.id.sb_pitch:
//                    音调 0.5 to 2.0  default 1.0
                    pitchValue = (2.0f - 0.5f) / 100 * progress;
                    tvPitch.setText("音调:" + pitchValue );

                    break;
                case R.id.sb_echo:
//                    回声延迟 10  to 5000.  Default = 5000
                    echoValue = (5000f - 10f) / 100 * progress;
                    tvEcho.setText("回声延迟:" + echoValue );
                    break;
                case R.id.sb_tremolo:
//                    颤音的扭曲程度 -1 to 1.  Default = 0
                    tremoloValue = (1.f + 1.0f) / 100 * progress;
                    tvTremolo.setText("颤音扭曲:" + tremoloValue );
                    break;
                case R.id.sb_frequency:
                    //频率
                    frequencyValue = 5.0f / 100 * progress;
                    tvFrequency.setText("频率:" + frequencyValue );
                    break;

                default:
                    break;
            }

        }

        @Override
        public void onStartTrackingTouch(SeekBar seekBar) {

        }

        @Override
        public void onStopTrackingTouch(SeekBar seekBar) {

        }
    }


    class PlayTask implements Runnable {

        @Override
        public void run() {
            //如果正在播放，则先停止
            pauseOtherVoice();
            //再播放
            playMusicVoiceNative(path, voiceType,pitchValue,echoValue,tremoloValue,frequencyValue);
        }
    }



    //点击各种图标事件
    public void onPlay(View view) {
        switch (view.getId()) {
            case R.id.btn_normal:
                voiceType = VOICE_NORMAL;
                break;
            case R.id.btn_luoli:
                voiceType = VOICE_LUOLI;
                break;
            case R.id.btn_dashu:
                voiceType = VOICE_DASHU;
                break;
            case R.id.btn_jingsong:
                voiceType = VOICE_JINGSONG;
                break;
            case R.id.btn_gaoguai:
                voiceType = VOICE_GAOGUAI;
                break;
            case R.id.btn_kongling:
                voiceType = VOICE_KONGLING;
                break;
            case R.id.bt_custom_play:
                voiceType = VOICE_CUSTOM;
                break;
            default:
                break;
        }
        service.execute(new PlayTask());
    }

    /**
     * 暂停所有的音乐
     * @param view
     */
    public void onStop(View view) {
        pauseOtherVoice();
    }


    @Override
    protected void onDestroy() {
        super.onDestroy();
        //页面销毁也停止播放
        pauseOtherVoice();
        //关闭一下
        FMOD.close();
    }
}
