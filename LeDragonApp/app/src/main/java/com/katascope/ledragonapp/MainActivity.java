package com.katascope.ledragonapp;

import static androidx.navigation.ActivityNavigatorDestinationBuilderKt.activity;

import android.app.Activity;
import android.bluetooth.BluetoothDevice;
import android.content.Context;
import android.content.pm.PackageManager;
import android.graphics.Color;
import android.os.Bundle;

import com.google.android.material.snackbar.Snackbar;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;

import android.os.Handler;
import android.util.Log;
import android.view.View;

import androidx.core.app.ActivityCompat;
import androidx.navigation.NavController;
import androidx.navigation.Navigation;
import androidx.navigation.ui.AppBarConfiguration;
import androidx.navigation.ui.NavigationUI;

import com.katascope.ledragonapp.databinding.ActivityMainBinding;

import android.view.Menu;
import android.view.MenuItem;
import android.widget.Button;
import android.widget.TextView;

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

    private String arduinoUuids[] = {
            "21:98:D3:0E:A0:40", //LedOnion
            "B1:5B:A0:80:48:DD"  //LeDragon
    };
    private String arduinoUuid = null;


    private Context context;
    private Activity activity;

    private boolean soundOverride = false;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        binding = ActivityMainBinding.inflate(getLayoutInflater());
        setContentView(binding.getRoot());

        setSupportActionBar(binding.toolbar);

        NavController navController = Navigation.findNavController(this, R.id.nav_host_fragment_content_main);
        appBarConfiguration = new AppBarConfiguration.Builder(navController.getGraph()).build();
        NavigationUI.setupActionBarWithNavController(this, navController, appBarConfiguration);


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
            if (result == true) {
                Log.d(LogName, "Connected to BLE");
                ((Button)findViewById(R.id.button_connect)).setBackgroundColor(Color.YELLOW);
                ((Button)findViewById(R.id.button_connect)).setText("Search");
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



        ((Button)findViewById(R.id.button_preset0)).setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) { bleService.writePreset(0); }
        });
        ((Button)findViewById(R.id.button_preset1)).setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) { bleService.writePreset(1); }
        });
        ((Button)findViewById(R.id.button_preset2)).setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) { bleService.writePreset(2); }
        });
        ((Button)findViewById(R.id.button_preset3)).setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) { bleService.writePreset(3); }
        });
        ((Button)findViewById(R.id.button_preset4)).setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) { bleService.writePreset(4); }
        });
        ((Button)findViewById(R.id.button_preset5)).setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) { bleService.writePreset(5); }
        });
        ((Button)findViewById(R.id.button_preset6)).setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) { bleService.writePreset(6); }
        });

        ((Button)findViewById(R.id.button_paletteCM)).setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) { bleService.writePalette(125); } });
        ((Button)findViewById(R.id.button_paletteRB)).setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) { bleService.writePalette(106); } });
        ((Button)findViewById(R.id.button_paletteWYM)).setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) { bleService.writePalette(18); } });

        ((Button)findViewById(R.id.button_sound)).setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                soundOverride = !soundOverride;
                if (soundOverride)
                    ((Button)findViewById(R.id.button_sound)).setBackgroundColor(Color.GREEN);
                else
                    ((Button)findViewById(R.id.button_sound)).setBackgroundColor(Color.MAGENTA);
                }
            });

        ((Button)findViewById(R.id.button_paletteRBM)).setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) { bleService.writePalette(150); } });
        ((Button)findViewById(R.id.button_paletteCMY)).setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) { bleService.writePalette(152); } });
        ((Button)findViewById(R.id.button_paletteCBM)).setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) { bleService.writePalette(153); } });
        ((Button)findViewById(R.id.button_paletteLAVA)).setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) { bleService.writePalette(201); } });
        ((Button)findViewById(R.id.button_paletteCLOUD)).setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) { bleService.writePalette(202); } });
        ((Button)findViewById(R.id.button_paletteOCEAN)).setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) { bleService.writePalette(203); } });
        ((Button)findViewById(R.id.button_paletteFOREST)).setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) { bleService.writePalette(204); } });
        ((Button)findViewById(R.id.button_paletteRAINBOW)).setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) { bleService.writePalette(205); } });
        ((Button)findViewById(R.id.button_paletteRAINBOWSTRIPE)).setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) { bleService.writePalette(206); } });
        ((Button)findViewById(R.id.button_palettePARTY)).setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) { bleService.writePalette(207); } });
        ((Button)findViewById(R.id.button_paletteHEAT)).setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) { bleService.writePalette(208); } });
        ((Button)findViewById(R.id.button_paletteSEAHAWKS)).setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) { bleService.writePalette(209); } });

        ((Button)findViewById(R.id.button_channel1)).setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) { bleService.writeToggle(1); } });
        ((Button)findViewById(R.id.button_channel2)).setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) { bleService.writeToggle(2); } });
        ((Button)findViewById(R.id.button_channel3)).setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) { bleService.writeToggle(3); } });
        ((Button)findViewById(R.id.button_channel4)).setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) { bleService.writeToggle(4); } });
        ((Button)findViewById(R.id.button_channel5)).setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) { bleService.writeToggle(5); } });
        ((Button)findViewById(R.id.button_channel6)).setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) { bleService.writeToggle(6); } });
        ((Button)findViewById(R.id.button_channel7)).setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) { bleService.writeToggle(7); } });

        ((Button)findViewById(R.id.button_speedKill)).setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) { bleService.writeCommand('|'); } });
        ((Button)findViewById(R.id.button_speedSlower)).setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) { bleService.writeCommand('-'); } });
        ((Button)findViewById(R.id.button_speedMore)).setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) { bleService.writeCommand('='); } });
        ((Button)findViewById(R.id.button_speedReset)).setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) { bleService.writeCommand('['); } });

        ((Button)findViewById(R.id.button_brightHalf)).setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) { bleService.writeCommand('$'); } });
        ((Button)findViewById(R.id.button_brightNorm)).setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) { bleService.writeCommand('%'); } });
        ((Button)findViewById(R.id.button_brightMax)).setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) { bleService.writeCommand('^'); } });

        ((Button)findViewById(R.id.button_sidefx)).setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) { bleService.writeCommand(']'); } });

        Button buttonGatt = (Button)findViewById(R.id.button_gatt);
        buttonGatt.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v)
            {
                bleService.connectGatt(context,arduinoUuid);
            }
        });

        ((Button)findViewById(R.id.button_exit)).setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) { finishAffinity(); } });


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
                        Thread.sleep(100);
                    }
                    catch (InterruptedException e){
                    }
                    handler.post(new Runnable(){
                        public void run() {
                            if (arduinoDevice == null) {
                                for (int i=0;i<arduinoUuids.length;i++) {
                                    if (arduinoDevice == null) {
                                        Log.d(LogName, "Searching for " + arduinoUuids[i]);
                                        arduinoDevice = bluetoothLeScanner.GetDeviceListAdapter().findDevice(arduinoUuids[i]);
                                        if (arduinoDevice != null)//found it
                                        {
                                            arduinoUuid = arduinoUuids[i];
                                        }
                                    }
                                }
                                if (arduinoDevice != null)
                                    Log.d(LogName, "FOUND " + arduinoDevice);
                            }
                            else {
                                if ((Button)findViewById(R.id.button_connect) != null) {
                                    ((Button) findViewById(R.id.button_connect)).setBackgroundColor(Color.GREEN);
                                    ((Button) findViewById(R.id.button_connect)).setText("Online");
                                }
                            }

                            if (bleService.HasCharacteristics()) {
                                if ((Button)findViewById(R.id.button_gatt) != null)
                                    ((Button)findViewById(R.id.button_gatt)).setBackgroundColor(Color.GREEN);
                            }

                            double amplitude = audioInput.getAmplitude();
                            if (soundOverride)
                                amplitude = 1.0;
                            double bias = 20;
                            int writeAmplitude = (int)((double)amplitude*(double)255.0 * bias);
                            if (writeAmplitude > 255)
                                writeAmplitude = 255;
                            if ((TextView)findViewById(R.id.textview_sound) != null) {
                                TextView textSound = (TextView) findViewById(R.id.textview_sound);
                                textSound.setText("Snd=" + (int) ((double) amplitude * (double) 100.0));
                            }
                            //Log.d(LogName,"WriteAmplitude2="+writeAmplitude);
                            bleService.writeVolume(writeAmplitude);
                        }
                    });
                }
            }
        };
        new Thread(runnable).start();
    }

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