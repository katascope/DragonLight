package com.example.testsound;

import android.Manifest;
import android.app.Activity;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothManager;
import android.bluetooth.le.BluetoothLeScanner;
import android.bluetooth.le.ScanCallback;
import android.bluetooth.le.ScanResult;
import android.content.Context;
import android.content.pm.PackageManager;
import android.os.Handler;
import android.util.Log;

import androidx.core.app.ActivityCompat;

public class BluetoothLeService {
    public static final String TAG = "BluetoothLeService";

    private boolean scanning;
    private Handler handler = new Handler();
    private BluetoothAdapter bluetoothAdapter;
    private static final long SCAN_PERIOD = 5000;

    private LeDeviceListAdapter leDeviceListAdapter = new LeDeviceListAdapter();
    public boolean initialize(Context context, Activity activity) {
        if (ActivityCompat.checkSelfPermission(context, Manifest.permission.BLUETOOTH_SCAN)
                != PackageManager.PERMISSION_GRANTED) {
            Log.d("STATE", "Requesting permissions");
            ActivityCompat.requestPermissions(activity, new String[]{Manifest.permission.BLUETOOTH_SCAN}, 10);
        }

        BluetoothManager bluetoothManager = (BluetoothManager)context.getSystemService(context.BLUETOOTH_SERVICE);
        bluetoothAdapter = bluetoothManager.getAdapter();
        return false;
    }


    public ScanCallback leScanCallBack = new ScanCallback() {
        @Override public void onScanResult(int callbackType, ScanResult result) {
            super.onScanResult(callbackType, result);
            Log.d("STATE", "Adding device " + result.getDevice());
            leDeviceListAdapter.addDevice(result.getDevice());
            //leDeviceListAdapter.notifyDataSetChanged();
        }
    };

    public void scanForLeDevices(Context context)
    {
        BluetoothLeScanner bluetoothLeScanner = bluetoothAdapter.getBluetoothLeScanner();
        try
        {
            if (!scanning)
            {
                handler.postDelayed(() -> {
                    scanning = false;
                    //bluetoothLeScanner.stopScan(leScanCallBack);
                }, SCAN_PERIOD);
                Log.d("STATE", "Scanning");
                scanning = true;
                bluetoothLeScanner.startScan(leScanCallBack);
            }
            else {
                Log.d("STATE", "StoppedScanning");
                scanning = false;
                bluetoothLeScanner.stopScan(leScanCallBack);
            }
        }
        catch (SecurityException se){
            Log.d("ERROR", "Exception in BLE Scanner "+ se);
        }
    }



}
