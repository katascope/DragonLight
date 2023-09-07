package com.katascope.ledragonapp;

import static android.bluetooth.le.ScanSettings.SCAN_MODE_LOW_LATENCY;

import android.Manifest;
import android.app.Activity;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothGatt;
import android.bluetooth.BluetoothGattCallback;
import android.bluetooth.BluetoothManager;
import android.bluetooth.BluetoothProfile;
import android.bluetooth.le.BluetoothLeScanner;
import android.bluetooth.le.ScanCallback;
import android.bluetooth.le.ScanFilter;
import android.bluetooth.le.ScanResult;
import android.bluetooth.le.ScanSettings;
import android.content.Context;
import android.content.pm.PackageManager;
import android.os.Handler;
import android.util.Log;

import androidx.core.app.ActivityCompat;

import java.util.ArrayList;
import java.util.List;

public class BluetoothLeService {
    public static final String TAG = "BluetoothLeService";

    private boolean scanning;
    private Handler handler;
    private BluetoothAdapter bluetoothAdapter;
    private static final long SCAN_PERIOD = 10000;

    //private LeDeviceListAdapter leDeviceListAdapter = new LeDeviceListAdapter();
    public boolean initialize(Context context, Activity activity) {
        if (ActivityCompat.checkSelfPermission(context, Manifest.permission.BLUETOOTH_SCAN)
                != PackageManager.PERMISSION_GRANTED) {
            Log.d("LEDRAGON", "Requesting permissions");
            ActivityCompat.requestPermissions(activity, new String[]{Manifest.permission.BLUETOOTH_SCAN}, 10);
        }

        handler = new Handler();

        BluetoothManager bluetoothManager = (BluetoothManager) context.getSystemService(context.BLUETOOTH_SERVICE);
        bluetoothAdapter = bluetoothManager.getAdapter();
        return false;
    }

    private final BluetoothGattCallback bluetoothGattCallback = new BluetoothGattCallback() {
        @Override
        public void onConnectionStateChange(BluetoothGatt gatt, int status, int newState) {
            Log.d("LEDRAGON", "GattServerState=" + newState);
            if (newState == BluetoothProfile.STATE_CONNECTED) {
                // successfully connected to the GATT Server
                Log.w("LEDRAGON", "GattServerConnect");
            } else if (newState == BluetoothProfile.STATE_DISCONNECTED) {
                // disconnected from the GATT Server
                Log.w("LEDRAGON", "GattServerDisconnect");
            }
        }
    };

    public boolean connect(Context context, final String address) {
        if (bluetoothAdapter == null) {
            Log.w("LEDRAGON", "Bad Bluetooth adapter");
            return false;
        }
        try {
            final BluetoothDevice device = bluetoothAdapter.getRemoteDevice(address);
        } catch (IllegalArgumentException exception) {
            Log.w("LEDRAGON", "Device not found on address");
            return false;
        }
        Log.d("LEDRAGON", "Successful connect to BLE on address " + address);
        return true;
    }

    public boolean discoverGatt(Context context, final String address) {
        Log.d("LEDRAGON", "GATT Discovery..");
        final BluetoothDevice device = bluetoothAdapter.getRemoteDevice(address);
        Log.d("LEDRAGON", "GATT Discovery Device=" + device.getAddress());
        BluetoothGatt bluetoothGatt = device.connectGatt(context, false, bluetoothGattCallback);
        Log.d("LEDRAGON", "GATT Discovery Discovering");
        bluetoothGatt.discoverServices();
        Log.d("LEDRAGON", "GATT Discovery Done");
        return false;
    }

    public boolean connectGatt(Context context, final String address) {
        Log.d("LEDRAGON", "GATT connecting..");
        try {
            final BluetoothDevice device = bluetoothAdapter.getRemoteDevice(address);
            Log.d("LEDRAGON", "GATT Device=" + device.getAddress());
            BluetoothGatt bluetoothGatt = device.connectGatt(context, false, bluetoothGattCallback);
            Log.d("LEDRAGON", "GATT=" + bluetoothGatt.toString());
            bluetoothGatt.discoverServices();
            Log.d("LEDRAGON", "GATT=" + bluetoothGatt.getServices());
            return true;
        } catch (IllegalArgumentException exception) {
            Log.w("LEDRAGON", "GattError-Refused");
        } catch (SecurityException exception) {
            Log.w("LEDRAGON", "GattError-Refused");
        }
        Log.d("LEDRAGON", "connectGatt done");
        return false;
    }

    public ScanCallback leScanCallBack = new ScanCallback() {
        @Override
        public void onScanResult(int callbackType, ScanResult result) {
            super.onScanResult(callbackType, result);
            Log.d("LEDRAGON", "Adding device " + result.getDevice());
            //      leDeviceListAdapter.addDevice(result.getDevice());
            //leDeviceListAdapter.notifyDataSetChanged();
        }
    };

    public void scanLeDevice(final boolean enable) {
        if (enable) {
            // Stops scanning after a pre-defined scan period.
            handler.postDelayed(new Runnable() {
                @Override
                public void run() {
                    scanning = false;
                    Log.d("LEDRAGON", "GATT Scanning Run");
                    bluetoothAdapter.stopLeScan(mLeScanCallback);
                    //invalidateOptionsMenu();
                }
            }, SCAN_PERIOD);

            scanning = true;
            Log.d("LEDRAGON", "GATT Scanning Start");
            bluetoothAdapter.startLeScan(mLeScanCallback);
        } else {
            scanning = false;
            Log.d("LEDRAGON", "GATT Scanning Stop");
            bluetoothAdapter.stopLeScan(mLeScanCallback);
        }
    }

    // Device scan callback.
    private BluetoothAdapter.LeScanCallback mLeScanCallback =
            new BluetoothAdapter.LeScanCallback() {

                @Override
                public void onLeScan(final BluetoothDevice device, int rssi, byte[] scanRecord) {
                    /*runOnUiThread(new Runnable() {
                        @Override
                        public void run() {
                            mLeDeviceListAdapter.addDevice(device);
                            mLeDeviceListAdapter.notifyDataSetChanged();
                        }
                    });*/
                    Log.d("LEDRAGON", "GATT Result " + device);
                }
            };
}
