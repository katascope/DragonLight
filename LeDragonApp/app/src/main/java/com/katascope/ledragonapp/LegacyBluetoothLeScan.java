package com.katascope.ledragonapp;

import android.app.Activity;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.le.BluetoothLeScanner;
import android.bluetooth.le.ScanCallback;
import android.bluetooth.le.ScanResult;
import android.content.Context;
import android.os.Handler;
import android.util.Log;

public class LegacyBluetoothLeScan {

    private String LogName = "SELF";
    private BluetoothLeScanner bluetoothLeScanner = null;
    private boolean scanning;
    private Handler handler = new Handler();

    // Stops scanning after 10 seconds.
    private static final long SCAN_PERIOD = 10000;

    public void initialize(Context context, Activity activity, BluetoothAdapter bluetoothAdapter)
    {
        Log.d(LogName, "BluetoothLeScan initialize");
        bluetoothLeScanner = bluetoothAdapter.getBluetoothLeScanner();
        Log.d(LogName, "BluetoothLeScan initialized");
    }

    private LegacyLeDeviceListAdapter leDeviceListAdapter = new LegacyLeDeviceListAdapter();
    public LegacyLeDeviceListAdapter GetDeviceListAdapter()
    {
        return leDeviceListAdapter;
    }

    // Device scan callback.
    private ScanCallback leScanCallback =
            new ScanCallback() {
                @Override
                public void onScanResult(int callbackType, ScanResult result) {
                    super.onScanResult(callbackType, result);
                    //Log.d(LogName,"scanLeDeviceResult:"+result);
                    leDeviceListAdapter.addDevice(result.getDevice());
                    //leDeviceListAdapter.notifyDataSetChanged();
                }
            };

    public void scanLeDevice() {
        if (!scanning) {
            // Stops scanning after a predefined scan period.
            handler.postDelayed(new Runnable() {
                @Override
                public void run() {
                    scanning = false;
                    Log.d(LogName,"scanLeDevice:"+scanning);
                    bluetoothLeScanner.stopScan(leScanCallback);
                }
            }, SCAN_PERIOD);

            scanning = true;
            Log.d(LogName,"scanLeDevice:"+scanning);
            bluetoothLeScanner.startScan(leScanCallback);
        } else {
            scanning = false;
            Log.d(LogName,"scanLeDevice:"+scanning);
            bluetoothLeScanner.stopScan(leScanCallback);
        }
    }
}
