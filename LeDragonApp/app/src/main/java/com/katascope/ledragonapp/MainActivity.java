package com.katascope.ledragonapp;

import static androidx.navigation.ActivityNavigatorDestinationBuilderKt.activity;

import android.app.Activity;
import android.bluetooth.le.BluetoothLeScanner;
import android.content.Context;
import android.content.pm.PackageManager;
import android.graphics.Color;
import android.media.MediaRecorder;
import android.os.Bundle;

import com.google.android.material.snackbar.Snackbar;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;

import android.util.Log;
import android.view.View;

import androidx.core.app.ActivityCompat;
import androidx.core.view.WindowCompat;
import androidx.navigation.NavController;
import androidx.navigation.Navigation;
import androidx.navigation.ui.AppBarConfiguration;
import androidx.navigation.ui.NavigationUI;

import com.katascope.ledragonapp.databinding.ActivityMainBinding;

import android.view.Menu;
import android.view.MenuItem;
import android.widget.Button;
import android.widget.Toast;

import android.Manifest;


/*

        static BluetoothUuid mainServiceUuid   = BluetoothUuid.FromGuid(new Guid("02FE4875-5056-48B5-AD15-36E30665D9B4"));
        static BluetoothUuid mainCommandUuid   = BluetoothUuid.FromGuid(new Guid("220154BF-1DCE-4F03-85F0-7BA905D2D6B0"));
        static BluetoothUuid mainAuthenticateUuid = BluetoothUuid.FromGuid(new Guid("4C75BB42-5365-458D-A3EA-2B91339646B7"));
        static BluetoothUuid mainPlayUuid      = BluetoothUuid.FromGuid(new Guid("3B140EF5-0A72-4891-AD38-83B5A2595622"));
        static BluetoothUuid mainStatusUuid    = BluetoothUuid.FromGuid(new Guid("D01C9106-91BD-4998-9554-85264D33ACB2"));
        static BluetoothUuid mainVolumeUuid    = BluetoothUuid.FromGuid(new Guid("04E92E16-47AF-11EE-BE56-0242AC120002"));
        static BluetoothUuid mainPaletteUuid   = BluetoothUuid.FromGuid(new Guid("7a0cca4f-9124-4831-923c-6ffdd6042ff2"));
        static BluetoothUuid mainToggleUuid    = BluetoothUuid.FromGuid(new Guid("FBDD5000-4B48-4A1A-9E40-98E84FD69245"));
        static BluetoothUuid mainToggleOnUuid  = BluetoothUuid.FromGuid(new Guid("9677B246-7355-4130-A770-769CD4732677"));
        static BluetoothUuid mainToggleOffUuid = BluetoothUuid.FromGuid(new Guid("CB9B8B39-96A8-498D-8420-4C9FC558894D"));
        static BluetoothUuid mainExciteUuid    = BluetoothUuid.FromGuid(new Guid("5549a237-ede8-4b5e-abb0-b233cebe0e52"));
        static BluetoothUuid mainResetUuid     = BluetoothUuid.FromGuid(new Guid("a8907f1f-09ea-4caf-8f73-3acfad5ace43"));
        static BluetoothUuid mainFxPresetUuid  = BluetoothUuid.FromGuid(new Guid("b47cb504-39df-489f-9bfa-2434082f6285"));

DeviceId=91CDE3A4B695 #LightSuitAngelA
DeviceId=B872B08E0D8E #LightSuitAngelB
DeviceId=F6426025BEE  #LightSuitAngelJ
DeviceId=2198D30EA040 #LightSuitAngelD
DeviceId=77C648CCE101 #LightSuitAngel?


        */

public class MainActivity extends AppCompatActivity {

    private AppBarConfiguration appBarConfiguration;
    private ActivityMainBinding binding;
    //    private SoundMeter soundMeter;
    private MediaRecorder mRecorder = null;
    private BluetoothLeService bleService = null;

    private BluetoothLeScanner bluetoothLeScanner = null;

    private Context context;
    private Activity activity;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        binding = ActivityMainBinding.inflate(getLayoutInflater());
        setContentView(binding.getRoot());

        setSupportActionBar(binding.toolbar);

        NavController navController = Navigation.findNavController(this, R.id.nav_host_fragment_content_main);
        appBarConfiguration = new AppBarConfiguration.Builder(navController.getGraph()).build();
        NavigationUI.setupActionBarWithNavController(this, navController, appBarConfiguration);

        binding.fab.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Snackbar.make(view, "Replace with your own action", Snackbar.LENGTH_LONG)
                        .setAnchorView(R.id.fab)
                        .setAction("Action", null).show();
            }
        });

        context = this;
        activity = this;



        /*
        if (ActivityCompat.checkSelfPermission(this, Manifest.permission.BLUETOOTH_CONNECT)
                != PackageManager.PERMISSION_GRANTED) {
            Log.d("LEDRAGON", "Requesting permissions");
            ActivityCompat.requestPermissions(this, new String[]{Manifest.permission.BLUETOOTH_CONNECT}, 10);
        }*/

        /*
        if (ActivityCompat.checkSelfPermission(this, Manifest.permission.BLUETOOTH_CONNECT)
                != PackageManager.PERMISSION_GRANTED) {
            Log.d("LEDRAGON", "Requesting permissions");
            ActivityCompat.requestPermissions(this, new String[]{Manifest.permission.BLUETOOTH_CONNECT}, 10);
        } else {
            boolean result = bleService.connect(this, "21:98:D3:0E:A0:40");
            if (result == true) {
                Log.d("LEDDRAGON", "Connected to BLE");
            } else Log.d("LEDDRAGON", "NO connection to BLE");
        }
*/
        if (ActivityCompat.checkSelfPermission(this, Manifest.permission.RECORD_AUDIO)
                != PackageManager.PERMISSION_GRANTED) {
            Log.d("LEDRAGON", "Requesting permission RECORD_AUDIO");
            ActivityCompat.requestPermissions(this, new String[]{Manifest.permission.RECORD_AUDIO}, 10);
        }
        else {
            Log.d("LEDRAGON", "Requesting permission RECORD_AUDIO");
            microphoneStart();

            Thread audioThread = new Thread() {
                public void run() {
                    while (true) {
                        try {
                            //Log.d("STATE","SOUND:"+getAmplitude());
                            sleep(1000);
                        } catch (InterruptedException e) {
                            throw new RuntimeException(e);
                        }
                    }
                }
            };
            audioThread.start();
        }

        Button buttonFindBLE = (Button)findViewById(R.id.button_findble);
        buttonFindBLE.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v)
            {
                //bleService.scanLeDevice(true);
                GattConnect();
            }
        });

        Button buttonConnect = (Button)findViewById(R.id.button_connect);
        buttonConnect.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v)
            {
                BLEConnect();
            }
        });
    }

    private void BLEConnect()
    {
        Log.d("LEDRAGON", "Starting BLE Service");
        bleService = new BluetoothLeService();
        bleService.initialize(this, this);
        Log.d("LEDRAGON", "Started BLE Service");
        Button buttonConnect = (Button)findViewById(R.id.button_connect);
        buttonConnect.setBackgroundColor(Color.GREEN);
    }

    private void GattConnect() {
        if (ActivityCompat.checkSelfPermission(this, Manifest.permission.BLUETOOTH_CONNECT)
                != PackageManager.PERMISSION_GRANTED) {
            Log.d("LEDRAGON", "Requesting permission BLUETOOTH_CONNECT");
            ActivityCompat.requestPermissions(this, new String[]{Manifest.permission.BLUETOOTH_CONNECT}, 10);
        } else {
            Log.d("LEDRAGON", "Have permission BLUETOOTH_CONNECT");
            //boolean result = bleService.connectGatt(this, "21:98:D3:0E:A0:40");
            boolean result = bleService.discoverGatt(this, "21:98:D3:0E:A0:40");

            if (result == true) {
                Log.d("LEDDRAGON", "Connected GATT to BLE");
            } else Log.d("LEDDRAGON", "NO GATT connection to BLE");
        }
    }


    private void microphoneStart()
    {
        if (mRecorder == null) {
            Log.d("LEDRAGON", "Recording1");
            mRecorder = new MediaRecorder(this);
            mRecorder.setAudioSource(MediaRecorder.AudioSource.MIC);
            mRecorder.setOutputFormat(MediaRecorder.OutputFormat.THREE_GPP);
            mRecorder.setAudioEncoder(MediaRecorder.AudioEncoder.AMR_NB);
            mRecorder.setOutputFile(getExternalCacheDir().getAbsolutePath()+"/temp.3gp");

            Log.d("LEDRAGON", "Recording2");
            try {
                mRecorder.prepare();
            } catch (IllegalStateException e){
                e.printStackTrace();
            } catch (java.io.IOException e) {
                e.printStackTrace();
            }
            Log.d("LEDRAGON", "Recording3");
            try {
                mRecorder.start();
                Toast.makeText(getBaseContext(), "Sound sensor initiated", Toast.LENGTH_SHORT).show();
            }
            catch (IllegalStateException e) {
                Toast.makeText(getBaseContext(), "Sound sensor FAILED", Toast.LENGTH_SHORT).show();
                e.printStackTrace();
            }

            Log.d("LEDRAGON", "Sound"+getAmplitude());
        }
    }

    public double getAmplitude()
    {
        if (mRecorder != null)
            return mRecorder.getMaxAmplitude();
        else return 0;
    }
    @Override public void onRequestPermissionsResult(int requestCode, @NonNull String[] permissions,
                                                     @NonNull int[] grantResults) {
        Log.d("LEDRAGON", "onRequestPermissionsResult");
        super.onRequestPermissionsResult(requestCode, permissions, grantResults);
        if (requestCode == 10) {
            Log.d("LEDRAGON", "onRequestPermissionsResult " + permissions + " : " + grantResults[0]);
            if (grantResults[0] == PackageManager.PERMISSION_GRANTED) {
                Log.d("LEDRAGON", "onRequestPermissionsResult Good " + permissions + " : " + grantResults[0]);
            }else{
                //User denied Permission.
                Log.d("LEDRAGON", "onRequestPermissionsResult Denied " + permissions + " : " + grantResults[0]);
            }
        }
    }


    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.menu_main, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();

        //noinspection SimplifiableIfStatement
        if (id == R.id.action_settings) {
            return true;
        }

        return super.onOptionsItemSelected(item);
    }

    @Override
    public boolean onSupportNavigateUp() {
        NavController navController = Navigation.findNavController(this, R.id.nav_host_fragment_content_main);
        return NavigationUI.navigateUp(navController, appBarConfiguration)
                || super.onSupportNavigateUp();
    }
}