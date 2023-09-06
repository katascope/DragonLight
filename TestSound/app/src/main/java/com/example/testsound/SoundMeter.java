package com.example.testsound;
import android.media.MediaRecorder;

public class SoundMeter {
    private MediaRecorder mRecorder = null;
    public void start()
    {
        if (mRecorder == null) {
            mRecorder = new MediaRecorder();
            mRecorder.setAudioSource(MediaRecorder.AudioSource.MIC);
            mRecorder.setOutputFormat(MediaRecorder.OutputFormat.THREE_GPP);
            mRecorder.setAudioEncoder(MediaRecorder.AudioEncoder.AMR_NB);
            mRecorder.setOutputFile("/dev/null");
            try {
                mRecorder.prepare();
            } catch (IllegalStateException e){
                e.printStackTrace();
            } catch (java.io.IOException e) {
                e.printStackTrace();
            }
            mRecorder.start();
        }
    }

    public void stop(){
        if (mRecorder != null) {
            mRecorder.stop();
            mRecorder.release();
            mRecorder = null;
        }
    }

    public double getAmplitude() {
        if (mRecorder != null)
            return mRecorder.getMaxAmplitude();
        else return 0;
    }
}
