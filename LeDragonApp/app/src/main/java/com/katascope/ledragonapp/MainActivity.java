package com.katascope.ledragonapp;

import static androidx.navigation.ActivityNavigatorDestinationBuilderKt.activity;

import android.app.Activity;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.le.BluetoothLeScanner;
import android.content.Context;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.graphics.Color;
import android.media.MediaRecorder;
import android.os.Bundle;

import com.google.android.material.snackbar.Snackbar;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;

import android.os.CountDownTimer;
import android.os.Handler;
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
import android.widget.TextView;
import android.widget.Toast;

import android.Manifest;


/*


DeviceId=91CDE3A4B695 #LightSuitAngelA
DeviceId=B872B08E0D8E #LightSuitAngelB
DeviceId=F6426025BEE  #LightSuitAngelJ
DeviceId=2198D30EA040 #LightSuitAngelD
DeviceId=77C648CCE101 #LightSuitAngel?


        */

public class MainActivity extends AppCompatActivity {

    private String LogName = "SELF";
    private AppBarConfiguration appBarConfiguration;
    private ActivityMainBinding binding;

    private AudioInput audioInput = null;
    private BluetoothLeService bleService = null;
    private BluetoothLeScan bluetoothLeScanner = null;

    private String arduinoUuid = "21:98:D3:0E:A0:40";

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

        audioInput = new AudioInput();
        audioInput.initialize(LogName, this,this);

        if (ActivityCompat.checkSelfPermission(this, Manifest.permission.BLUETOOTH_CONNECT)
                != PackageManager.PERMISSION_GRANTED) {
            Log.d(LogName, "Requesting permission BLUETOOTH_CONNECT");
            ActivityCompat.requestPermissions(this, new String[]{Manifest.permission.BLUETOOTH_CONNECT}, 10);
        } else {
            Log.d(LogName, "Have permission BLUETOOTH_CONNECT");
        }

        bleService = new BluetoothLeService();
        boolean result = bleService.initialize(this, this);
//        boolean result = bleService.connect(this, "21:98:D3:0E:A0:40");
            if (result == true) {
                Log.d(LogName, "Connected to BLE");
                ((Button)findViewById(R.id.button_connect)).setBackgroundColor(Color.YELLOW);
                ((Button)findViewById(R.id.button_connect)).setText("Searching");
                Log.d(LogName, "Checking permission BLUETOOTH_SCAN");
                if (ActivityCompat.checkSelfPermission(context, Manifest.permission.BLUETOOTH_SCAN)
                        != PackageManager.PERMISSION_GRANTED) {
                    Log.d(LogName, "Requesting permission BLUETOOTH_SCAN");
                    ActivityCompat.requestPermissions(activity, new String[]{Manifest.permission.BLUETOOTH_SCAN}, 10);
                }
                else
                {
                    if (ActivityCompat.checkSelfPermission(context, Manifest.permission.ACCESS_FINE_LOCATION)
                            != PackageManager.PERMISSION_GRANTED) {
                        Log.d(LogName, "Requesting permission BLUETOOTH_SCAN");
                        ActivityCompat.requestPermissions(activity, new String[]{Manifest.permission.ACCESS_FINE_LOCATION}, 10);
                    }
                    else if (ActivityCompat.checkSelfPermission(context, Manifest.permission.ACCESS_BACKGROUND_LOCATION)
                            != PackageManager.PERMISSION_GRANTED) {
                        Log.d(LogName, "Requesting permission BLUETOOTH_SCAN");
                        ActivityCompat.requestPermissions(activity, new String[]{Manifest.permission.ACCESS_BACKGROUND_LOCATION}, 10);
                    }

                    Log.d(LogName, "Have permission BLUETOOTH_SCAN");
                    bluetoothLeScanner = new BluetoothLeScan();
                    bluetoothLeScanner.initialize(this,this,bleService.getBluetoothAdapter());
                    bluetoothLeScanner.scanLeDevice();
                }
            } else Log.d(LogName, "NO connection to BLE");




        Button buttonGatt = (Button)findViewById(R.id.button_gatt);
        buttonGatt.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v)
            {
                bleService.connectGatt(context,arduinoUuid);
            }
        });

        Button buttonConnect = (Button)findViewById(R.id.button_connect);
        buttonConnect.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v)
            {
                //BLEConnect();
            }
        });

        startTimerThread();
   }

   BluetoothDevice arduinoDevice = null;

    private void startTimerThread()
    {
        Handler handler = new Handler();
        Runnable runnable = new Runnable()
        {
            public void run()
            {
                while (true)
                {
                    try {
                        Thread.sleep(1000);
                    }
                    catch (InterruptedException e){
                    }
                    handler.post(new Runnable(){
                        public void run() {
                            if (arduinoDevice == null) {
                                Log.d(LogName, "Searching for " + arduinoUuid);
                                arduinoDevice = bluetoothLeScanner.GetDeviceListAdapter().findDevice(arduinoUuid);
                                if (arduinoDevice != null)
                                    Log.d(LogName, "FOUND " + arduinoUuid);
                            }
                            else {
                                ((Button)findViewById(R.id.button_connect)).setBackgroundColor(Color.GREEN);
                                ((Button)findViewById(R.id.button_connect)).setText("Online");
                            }
                            TextView textSound = (TextView)findViewById(R.id.textview_sound);
                            textSound.setText("Snd="+audioInput.getAmplitude());
                        }
                    });
                }
            }
        };
        new Thread(runnable).start();
    }
/*
    private void BLEConnect()
    {
        Log.d(LogName, "Starting BLE Service");
        bleService = new BluetoothLeService();
        bleService.initialize(this, this);
        Log.d(LogName, "Started BLE Service");
        Button buttonConnect = (Button)findViewById(R.id.button_connect);
        buttonConnect.setBackgroundColor(Color.GREEN);
    }

    private void GattConnect() {
        if (ActivityCompat.checkSelfPermission(this, Manifest.permission.BLUETOOTH_CONNECT)
                != PackageManager.PERMISSION_GRANTED) {
            Log.d(LogName, "Requesting permission BLUETOOTH_CONNECT");
            ActivityCompat.requestPermissions(this, new String[]{Manifest.permission.BLUETOOTH_CONNECT}, 10);
        } else {
            Log.d(LogName, "Have permission BLUETOOTH_CONNECT");
            boolean result = bleService.connectGatt(this, "21:98:D3:0E:A0:40");
            //boolean result = bleService.discoverGatt(this, "21:98:D3:0E:A0:40");

            if (result == true) {
                Log.d(LogName, "Connected GATT to BLE");
            } else Log.d(LogName, "NO GATT connection to BLE");
        }
    }
*/
    @Override public void onRequestPermissionsResult(int requestCode, @NonNull String[] permissions,
                                                     @NonNull int[] grantResults) {
        Log.d(LogName, "onRequestPermissionsResult");
        super.onRequestPermissionsResult(requestCode, permissions, grantResults);
        if (requestCode == 10) {
            Log.d(LogName, "onRequestPermissionsResult " + permissions + " : " + grantResults[0]);
            if (grantResults[0] == PackageManager.PERMISSION_GRANTED) {
                Log.d(LogName, "onRequestPermissionsResult Good " + permissions + " : " + grantResults[0]);
            }else{
                //User denied Permission.
                Log.d(LogName, "onRequestPermissionsResult Denied " + permissions + " : " + grantResults[0]);
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