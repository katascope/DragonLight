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
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.Spinner;
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



        ((Button)findViewById(R.id.button_sound)).setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                soundOverride = !soundOverride;
                if (soundOverride)
                    ((Button)findViewById(R.id.button_sound)).setBackgroundColor(Color.GREEN);
                else
                    ((Button)findViewById(R.id.button_sound)).setBackgroundColor(Color.MAGENTA);
                }
            });

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


        Spinner spinner = (Spinner)findViewById(R.id.spinner);
        ArrayAdapter<CharSequence> adapter = ArrayAdapter.createFromResource(this,R.array.palette_array, android.R.layout.simple_spinner_item);
        adapter.setDropDownViewResource(android.R.layout.simple_spinner_item);
        spinner.setAdapter(adapter);
        spinner.setOnItemSelectedListener(new AdapterView.OnItemSelectedListener() {
            @Override public void onItemSelected(AdapterView<?> parent, View view, int position, long id) {
                Spinner spinner = (Spinner)findViewById(R.id.spinner);
                spinner.setOnItemSelectedListener(this);
                String choice = spinner.getSelectedItem().toString();
                Log.d(LogName, "Selected " + choice);
                switch (choice)
                {
                    case "DarkRedYellow":       bleService.writePalette(1); break;
                    case "DarkRedGreen":        bleService.writePalette(2); break;
                    case "DarkRedCyan":         bleService.writePalette(3); break;
                    case "DarkRedBlue":         bleService.writePalette(4); break;
                    case "DarkRedMagenta":      bleService.writePalette(5); break;
                    case "WhiteRedYellow":      bleService.writePalette(6); break;
                    case "WhiteRedGreen":       bleService.writePalette(7); break;
                    case "WhiteRedCyan":        bleService.writePalette(8); break;
                    case "WhiteRedBlue":        bleService.writePalette(9); break;
                    case "WhiteRedMagenta":     bleService.writePalette(10); break;
                    case "DarkYellowGreen":    bleService.writePalette(11); break;
                    case "DarkYellowCyan":    bleService.writePalette(12); break;
                    case "DarkYellowBlue":    bleService.writePalette(13); break;
                    case "DarkYellowMagenta":    bleService.writePalette(14); break;
                    case "WhiteYellowGreen":    bleService.writePalette(15); break;
                    case "WhiteYellowCyan":    bleService.writePalette(16); break;
                    case "WhiteYellowBlue":    bleService.writePalette(17); break;
                    case "WhiteYellowMagenta":    bleService.writePalette(18); break;
                    case "DarkGreenCyan":    bleService.writePalette(19); break;
                    case "DarkGreenBlue":    bleService.writePalette(20); break;
                    case "DarkGreenMagenta":    bleService.writePalette(21); break;
                    case "WhiteGreenCyan":    bleService.writePalette(22); break;
                    case "WhiteGreenBlue":    bleService.writePalette(23); break;
                    case "WhiteGreenMagenta":    bleService.writePalette(24); break;
                    case "DarkCyanBlue":    bleService.writePalette(25); break;
                    case "DarkCyanMagenta":    bleService.writePalette(26); break;
                    case "WhiteCyanBlue":    bleService.writePalette(27); break;
                    case "WhiteCyanMagenta":    bleService.writePalette(28); break;
                    case "DarkBlueMagenta":    bleService.writePalette(29); break;
                    case "WhiteBlueMagenta":    bleService.writePalette(30); break;
                    case "RedGreenBlue":    bleService.writePalette(31); break;
                    case "RedBlueMagenta":    bleService.writePalette(32); break;
                    case "CyanMagentaYellow":    bleService.writePalette(33); break;
                    case "CyanBlueMagenta":    bleService.writePalette(34); break;
                    case "Dark":    bleService.writePalette(50); break;
                    case "White":    bleService.writePalette(51); break;
                    case "DarkRed":    bleService.writePalette(101); break;
                    case "Red":    bleService.writePalette(102); break;
                    case "RedYellow":    bleService.writePalette(103); break;
                    case "RedGreen":    bleService.writePalette(104); break;
                    case "RedCyan":    bleService.writePalette(105); break;
                    case "RedBlue":    bleService.writePalette(106); break;
                    case "RedMagenta":    bleService.writePalette(107); break;
                    case "WhiteRed":    bleService.writePalette(108); break;
                    case "DarkYellow":    bleService.writePalette(109); break;
                    case "Yellow":    bleService.writePalette(110); break;
                    case "YellowGreen":    bleService.writePalette(111); break;
                    case "YellowCyan":    bleService.writePalette(112); break;
                    case "YellowBlue":    bleService.writePalette(113); break;
                    case "YellowMagenta":    bleService.writePalette(114); break;
                    case "WhiteYellow":    bleService.writePalette(115); break;
                    case "DarkGreen":    bleService.writePalette(116); break;
                    case "Green":    bleService.writePalette(117); break;
                    case "GreenCyan":    bleService.writePalette(118); break;
                    case "GreenBlue":    bleService.writePalette(119); break;
                    case "GreenMagenta":    bleService.writePalette(120); break;
                    case "WhiteGreen":    bleService.writePalette(121); break;
                    case "DarkCyan":    bleService.writePalette(122); break;
                    case "Cyan":    bleService.writePalette(123); break;
                    case "CyanBlue":    bleService.writePalette(124); break;
                    case "CyanMagenta":    bleService.writePalette(125); break;
                    case "WhiteCyan":    bleService.writePalette(126); break;
                    case "DarkBlue":    bleService.writePalette(127); break;
                    case "Blue":    bleService.writePalette(128); break;
                    case "BlueMagenta":    bleService.writePalette(129); break;
                    case "WhiteBlue":    bleService.writePalette(130); break;
                    case "DarkMagenta":    bleService.writePalette(131); break;
                    case "DarkMagenta":    bleService.writePalette(132); break;
                    case "WhiteMagenta":    bleService.writePalette(133); break;
                    case "Lava":    bleService.writePalette(201); break;
                    case "CLoud":    bleService.writePalette(202); break;
                    case "Ocean":    bleService.writePalette(203); break;
                    case "Forest":    bleService.writePalette(204); break;
                    case "Rainbow":    bleService.writePalette(205); break;
                    case "RainbowStripe":    bleService.writePalette(206); break;
                    case "Party":    bleService.writePalette(207); break;
                    case "Heat":    bleService.writePalette(208); break;
                    case "Seahawks":    bleService.writePalette(209); break;


                }
            }
            @Override public void onNothingSelected(AdapterView<?> parent)
            {
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