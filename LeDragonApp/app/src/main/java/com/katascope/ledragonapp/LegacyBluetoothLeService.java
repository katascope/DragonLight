package com.katascope.ledragonapp;

import android.app.Activity;
import android.app.Service;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothGatt;
import android.bluetooth.BluetoothGattCallback;
import android.bluetooth.BluetoothGattCharacteristic;
import android.bluetooth.BluetoothManager;
import android.bluetooth.BluetoothProfile;
import android.bluetooth.BluetoothGattService;
import android.bluetooth.le.ScanCallback;
import android.bluetooth.le.ScanResult;
import android.content.Context;
import android.content.Intent;
import android.os.Handler;
import android.os.Binder;
import android.os.IBinder;
import android.util.Log;

import java.util.List;

public class LegacyBluetoothLeService extends Service{
    private String LogName = "SELF";
    public static final String TAG = "BluetoothLeService";

    private final IBinder mBinder = new LocalBinder();

    private boolean scanning;
    private Handler handler;
    private BluetoothAdapter bluetoothAdapter;
    private static final long SCAN_PERIOD = 10000;

    private BluetoothGatt mBluetoothGatt;
    public BluetoothAdapter getBluetoothAdapter()
    {
        return bluetoothAdapter;
    }

    //private LeDeviceListAdapter leDeviceListAdapter = new LeDeviceListAdapter();
    public boolean initialize(Context context, Activity activity) {
        handler = new Handler();

        BluetoothManager bluetoothManager = (BluetoothManager) context.getSystemService(context.BLUETOOTH_SERVICE);
        bluetoothAdapter = bluetoothManager.getAdapter();
        return true;
    }

    public class LocalBinder extends Binder {
        LegacyBluetoothLeService getService() {
            return LegacyBluetoothLeService.this;
        }
    }

    @Override public IBinder onBind(Intent intent)
    {
        return mBinder;
    }

    @Override public boolean onUnbind(Intent intent)
    {
        close();
        return super.onUnbind((intent));
    }

    public void close()
    {
        if (mBluetoothGatt == null) {
            return;
        }
        mBluetoothGatt.close();
        mBluetoothGatt = null;
    }
    private final BluetoothGattCallback bluetoothGattCallback = new BluetoothGattCallback() {
        @Override
        public void onConnectionStateChange(BluetoothGatt gatt, int status, int newState) {
            Log.d(LogName, "GattServerState=" + newState);
            if (newState == BluetoothProfile.STATE_CONNECTED) {
                // successfully connected to the GATT Server
                Log.w(LogName, "GattServerConnect");
                mBluetoothGatt = gatt;
                Log.d(LogName, "GATT=" + gatt.getServices());

            } else if (newState == BluetoothProfile.STATE_DISCONNECTED) {
                // disconnected from the GATT Server
                Log.w(LogName, "GattServerDisconnect");
            }
            else {
                Log.w(LogName, "GattServerUnknown");
            }
        }

        @Override public void onServicesDiscovered(BluetoothGatt gatt, int status)
        {
            if (status == BluetoothGatt.GATT_SUCCESS) {
                //broadcastUpdate(ACTION_GATT_SERVICES_DISCOVERED);
                Log.w(LogName, "onServicesDiscovered success: " + status);
                Log.d(LogName, "onServicesDiscovered=" + gatt.getServices());

            }
            else { Log.w(LogName, "onServicesDiscovered received: " + status); }
        }
    };

    public boolean connect(Context context, final String address) {
        if (bluetoothAdapter == null) {
            Log.w(LogName, "Bad Bluetooth adapter");
            return false;
        }
        try {
            final BluetoothDevice device = bluetoothAdapter.getRemoteDevice(address);
        } catch (IllegalArgumentException exception) {
            Log.w(LogName, "Device not found on address");
            return false;
        }
        Log.d(LogName, "Successful connect to BLE on address " + address);
        return true;
    }

    public boolean discoverGatt(Context context, final String address) {
        Log.d(LogName, "GATT Discovery..");
        final BluetoothDevice device = bluetoothAdapter.getRemoteDevice(address);
        Log.d(LogName, "GATT Discovery Device=" + device.getAddress());
        BluetoothGatt bluetoothGatt = device.connectGatt(context, false, bluetoothGattCallback);
        Log.d(LogName, "GATT Discovery Discovering " + bluetoothGatt);
        //bluetoothGatt.discoverServices();
        //Log.d(LogName, "GATT Discovery Done");
        return false;
    }

    public boolean connectGatt(Context context, final String address) {
        Log.d(LogName, "GATT connecting..");
        try {
            final BluetoothDevice device = bluetoothAdapter.getRemoteDevice(address);
            Log.d(LogName, "GATT Device=" + device.getAddress());
            BluetoothGatt bluetoothGatt = device.connectGatt(context, false, bluetoothGattCallback);
            Log.d(LogName, "GATT=" + bluetoothGatt.toString());
            bluetoothGatt.discoverServices();
            Log.d(LogName, "GATT Discovering services");
            return true;
        } catch (IllegalArgumentException exception) {
            Log.w(LogName, "GattError-Refused");
        } catch (SecurityException exception) {
            Log.w(LogName, "GattError-Refused");
        }
        Log.d(LogName, "connectGatt done");
        return false;
    }

    public ScanCallback leScanCallBack = new ScanCallback() {
        @Override
        public void onScanResult(int callbackType, ScanResult result) {
            super.onScanResult(callbackType, result);
            Log.d(LogName, "Adding device " + result.getDevice());
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
                    Log.d(LogName, "GATT Scanning Run");
                    bluetoothAdapter.stopLeScan(mLeScanCallback);
                    //invalidateOptionsMenu();
                }
            }, SCAN_PERIOD);

            scanning = true;
            Log.d(LogName, "GATT Scanning Start");
            bluetoothAdapter.startLeScan(mLeScanCallback);
        } else {
            scanning = false;
            Log.d(LogName, "GATT Scanning Stop");
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
                    Log.d(LogName, "GATT Result " + device);
                }
            };

    /**
     * Request a read on a given {@code BluetoothGattCharacteristic}. The read result is reported
     * asynchronously through the {@code BluetoothGattCallback#onCharacteristicRead(android.bluetooth.BluetoothGatt, android.bluetooth.BluetoothGattCharacteristic, int)}
     * callback.
     *
     * @param characteristic The characteristic to read from.
     */
    public void readCharacteristic(BluetoothGattCharacteristic characteristic) {
        if (bluetoothAdapter == null || mBluetoothGatt == null) {
            Log.w(TAG, "BluetoothAdapter not initialized");
            return;
        }
        mBluetoothGatt.readCharacteristic(characteristic);
    }

    /**
     * Enables or disables notification on a give characteristic.
     *
     * @param characteristic Characteristic to act on.
     * @param enabled If true, enable notification.  False otherwise.
     */
    public void setCharacteristicNotification(BluetoothGattCharacteristic characteristic,
                                              boolean enabled) {
        if (bluetoothAdapter == null || mBluetoothGatt == null) {
            Log.w(TAG, "BluetoothAdapter not initialized");
            return;
        }
        mBluetoothGatt.setCharacteristicNotification(characteristic, enabled);
/*
        // This is specific to Heart Rate Measurement.
        if (UUID_HEART_RATE_MEASUREMENT.equals(characteristic.getUuid())) {
            BluetoothGattDescriptor descriptor = characteristic.getDescriptor(
                    UUID.fromString(SampleGattAttributes.CLIENT_CHARACTERISTIC_CONFIG));
            descriptor.setValue(BluetoothGattDescriptor.ENABLE_NOTIFICATION_VALUE);
            mBluetoothGatt.writeDescriptor(descriptor);
        }*/
    }

    /**
     * Retrieves a list of supported GATT services on the connected device. This should be
     * invoked only after {@code BluetoothGatt#discoverServices()} completes successfully.
     *
     * @return A {@code List} of supported services.
     */
    public List<BluetoothGattService> getSupportedGattServices() {
        if (mBluetoothGatt == null) return null;

        return mBluetoothGatt.getServices();
    }
}
