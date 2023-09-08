package com.katascope.ledragonapp;

import android.Manifest;
import android.app.Activity;
import android.content.Context;
import android.content.pm.PackageManager;
import android.media.MediaRecorder;
import android.util.Log;
import android.widget.Toast;

import androidx.core.app.ActivityCompat;

public class AudioInput {
    private String TAG;
    private MediaRecorder mRecorder = null;

    public void initialize(String tag, Context context, Activity activity)
    {
        if (ActivityCompat.checkSelfPermission(context, android.Manifest.permission.RECORD_AUDIO)
                != PackageManager.PERMISSION_GRANTED) {
            Log.d(TAG, "Requesting permission RECORD_AUDIO");
            ActivityCompat.requestPermissions(activity, new String[]{Manifest.permission.RECORD_AUDIO}, 10);
        }
        else {
            Log.d(TAG, "Have permission RECORD_AUDIO");
            microphoneStart(context, activity);
        }
    }

    private void microphoneStart(Context context, Activity activity)
    {
        if (mRecorder == null) {
            Log.d(TAG, "Recording1");
            mRecorder = new MediaRecorder(context);
            mRecorder.setAudioSource(MediaRecorder.AudioSource.MIC);
            mRecorder.setOutputFormat(MediaRecorder.OutputFormat.THREE_GPP);
            mRecorder.setAudioEncoder(MediaRecorder.AudioEncoder.AMR_NB);
            mRecorder.setOutputFile(context.getExternalCacheDir().getAbsolutePath()+"/temp.3gp");

            Log.d(TAG, "Recording2");
            try {
                mRecorder.prepare();
            } catch (IllegalStateException e){
                e.printStackTrace();
            } catch (java.io.IOException e) {
                e.printStackTrace();
            }
            Log.d(TAG, "Recording3");
            try {
                mRecorder.start();
                Toast.makeText(context, "Sound sensor initiated", Toast.LENGTH_SHORT).show();
            }
            catch (IllegalStateException e) {
                Toast.makeText(context, "Sound sensor FAILED", Toast.LENGTH_SHORT).show();
                e.printStackTrace();
            }

            Log.d(TAG, "Sound"+getAmplitude());
        }
    }

    public double getAmplitude()
    {
        if (mRecorder != null)
            return mRecorder.getMaxAmplitude();
        else return 0;
    }
}
