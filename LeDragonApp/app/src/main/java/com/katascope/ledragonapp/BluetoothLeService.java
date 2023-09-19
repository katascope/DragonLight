package com.katascope.ledragonapp;

import android.app.Activity;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothGatt;
import android.bluetooth.BluetoothGattCallback;
import android.bluetooth.BluetoothGattCharacteristic;
import android.bluetooth.BluetoothGattService;
import android.bluetooth.BluetoothManager;
import android.bluetooth.BluetoothProfile;
import android.bluetooth.le.ScanCallback;
import android.bluetooth.le.ScanResult;
import android.content.Context;
import android.os.Handler;
import android.util.Log;

import java.util.UUID;


public class BluetoothLeService {
    private UUID mainServiceUuid      = UUID.fromString("02FE4875-5056-48B5-AD15-36E30665D9B4");
    private UUID mainCommandUuid      = UUID.fromString("220154BF-1DCE-4F03-85F0-7BA905D2D6B0");
    private UUID mainAuthenticateUuid = UUID.fromString("4C75BB42-5365-458D-A3EA-2B91339646B7");
    private UUID mainPlayUuid         = UUID.fromString("3B140EF5-0A72-4891-AD38-83B5A2595622");
    private UUID mainStatusUuid       = UUID.fromString("D01C9106-91BD-4998-9554-85264D33ACB2");
    private UUID mainVolumeUuid       = UUID.fromString("04E92E16-47AF-11EE-BE56-0242AC120002");
    private UUID mainPaletteUuid      = UUID.fromString("7a0cca4f-9124-4831-923c-6ffdd6042ff2");
    private UUID mainToggleUuid       = UUID.fromString("FBDD5000-4B48-4A1A-9E40-98E84FD69245");
    private UUID mainToggleOnUuid     = UUID.fromString("9677B246-7355-4130-A770-769CD4732677");
    private UUID mainToggleOffUuid    = UUID.fromString("CB9B8B39-96A8-498D-8420-4C9FC558894D");
    private UUID mainExciteUuid       = UUID.fromString("5549a237-ede8-4b5e-abb0-b233cebe0e52");
    private UUID mainResetUuid        = UUID.fromString("a8907f1f-09ea-4caf-8f73-3acfad5ace43");
    private UUID mainFxPresetUuid     = UUID.fromString("b47cb504-39df-489f-9bfa-2434082f6285");

    private String LogName = "SELF";
    public static final String TAG = "BluetoothLeService";

    private boolean scanning;
    private Handler handler;
    private BluetoothAdapter bluetoothAdapter;
    private static final long SCAN_PERIOD = 10000;

    private BluetoothGattCharacteristic characteristicCommand = null;
    private BluetoothGattCharacteristic characteristicFxPreset = null;
    private BluetoothGattCharacteristic characteristicVolume = null;
    private BluetoothGattCharacteristic characteristicPalette = null;
    private BluetoothGattCharacteristic characteristicToggle = null;
    private BluetoothGattCharacteristic characteristicAuth = null;


    public BluetoothAdapter getBluetoothAdapter()
    {
        return bluetoothAdapter;
    }

    private boolean foundCharacteristics = false;
    public boolean HasCharacteristics()
    {
        return foundCharacteristics;
    }

    //private LeDeviceListAdapter leDeviceListAdapter = new LeDeviceListAdapter();
    public boolean initialize(Context context, Activity activity) {
        handler = new Handler();

        BluetoothManager bluetoothManager = (BluetoothManager) context.getSystemService(context.BLUETOOTH_SERVICE);
        bluetoothAdapter = bluetoothManager.getAdapter();
        return true;
    }

    private BluetoothGatt savedGatt = null;
    public void writePreset(int presetId)
    {
        if (savedGatt != null) {
            characteristicFxPreset.setValue(presetId, BluetoothGattCharacteristic.FORMAT_UINT8, 0);
            savedGatt.writeCharacteristic(characteristicFxPreset);
        }
    }

    public void writeVolume(int volume)
    {
        if (savedGatt != null) {
//            Log.d(LogName, "writeVolume=" + volume);
            characteristicVolume.setValue(volume, BluetoothGattCharacteristic.FORMAT_UINT8, 0);
            savedGatt.writeCharacteristic(characteristicVolume);
        }
    }

    public void writePalette(int choice)
    {
        if (savedGatt != null) {
            Log.d(LogName, "writePalette=" + choice);
            characteristicPalette.setValue(choice, BluetoothGattCharacteristic.FORMAT_UINT8, 0);
            savedGatt.writeCharacteristic(characteristicPalette);
        }
    }

    public void writeToggle(int volume)
    {
        if (savedGatt != null) {
            Log.d(LogName, "writeToggle=" + volume);
            characteristicToggle.setValue(volume, BluetoothGattCharacteristic.FORMAT_UINT8, 0);
            savedGatt.writeCharacteristic(characteristicToggle);
        }
    }

    public void writeAuth(int auth)
    {
        if (savedGatt != null) {
            Log.d(LogName, "writeAuih=" + auth);
            characteristicAuth.setValue(auth, BluetoothGattCharacteristic.FORMAT_UINT32, 0);
            savedGatt.writeCharacteristic(characteristicAuth);
        }
    }
    public void writeCommand(int volume)
    {
        if (savedGatt != null) {
            Log.d(LogName, "writeCommand=" + volume);
            characteristicCommand.setValue(volume, BluetoothGattCharacteristic.FORMAT_UINT8, 0);
            savedGatt.writeCharacteristic(characteristicCommand);
        }
    }

    private final BluetoothGattCallback bluetoothGattCallback = new BluetoothGattCallback() {
        @Override
        public void onConnectionStateChange(BluetoothGatt gatt, int status, int newState) {
            Log.d(LogName, "GattServerState=" + newState);
            if (newState == BluetoothProfile.STATE_CONNECTED) {
                // successfully connected to the GATT Server
                Log.w(LogName, "GattServerConnect");
                Log.d(LogName, "GATT=" + gatt.getServices());
                gatt.discoverServices();
                Log.d(LogName, "GATT Discovering services");

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
                savedGatt = gatt;
                //broadcastUpdate(ACTION_GATT_SERVICES_DISCOVERED);
                Log.w(LogName, "onServicesDiscovered success: " + status);
                Log.d(LogName, "onServicesDiscovered=" + gatt.getServices());
                //BluetoothGattCharacteristic characteristic = null;
                for (BluetoothGattService service : gatt.getServices())
                {
                    Log.d(LogName, "GattService: " +service.getUuid());
                    for (BluetoothGattCharacteristic characteristic : service.getCharacteristics())
                    {
                        Log.d(LogName, "GattServiceCharacteristic: " +characteristic.getUuid());
                    }
                }

                characteristicFxPreset =
                        gatt.getService(mainServiceUuid).getCharacteristic(mainFxPresetUuid);
                if (characteristicFxPreset != null)
                    Log.d(LogName, "Found characteristicFxPreset");

                characteristicVolume =
                        gatt.getService(mainServiceUuid).getCharacteristic(mainVolumeUuid);
                if (characteristicVolume != null)
                    Log.d(LogName, "Found characteristicVolume");

                characteristicPalette =
                        gatt.getService(mainServiceUuid).getCharacteristic(mainPaletteUuid);
                if (characteristicPalette != null)
                    Log.d(LogName, "Found characteristicPalette");

                characteristicToggle =
                        gatt.getService(mainServiceUuid).getCharacteristic(mainToggleUuid);
                if (characteristicToggle != null)
                    Log.d(LogName, "Found characteristicToggle");

                characteristicCommand =
                        gatt.getService(mainServiceUuid).getCharacteristic(mainCommandUuid);
                if (characteristicCommand != null)
                    Log.d(LogName, "Found characteristicCommand");

                characteristicAuth =
                        gatt.getService(mainServiceUuid).getCharacteristic(mainAuthenticateUuid);
                if (characteristicAuth != null) {
                    Log.d(LogName, "Found characteristicAuth");
                    writeAuth(3838);
                    Log.d(LogName, "Wrote initial characteristicAuth");
                }


                foundCharacteristics = true;

            }
            else { Log.w(LogName, "onServicesDiscovered received: " + status); }
        }
    };

    /*public boolean connect(Context context, final String addresses[]) {
        if (bluetoothAdapter == null) {
            Log.w(LogName, "Bad Bluetooth adapter");
            return false;
        }
        for (int address = 0;address < addresses.length;address++) {
            Log.d(LogName, "Trying connect to BLE on address " + addresses[address]);
            try {
                final BluetoothDevice device = bluetoothAdapter.getRemoteDevice(addresses[address]);
                Log.d(LogName, "Successful connect to BLE on address " + addresses[address]);
                return true;
            } catch (IllegalArgumentException exception) {
                Log.w(LogName, "Device not found on address");
            }
        }
        return false;
    }*/

    public String connectGatt(Context context, final String address) {
        Log.d(LogName, "GATT connecting..");
            Log.d(LogName, "Trying connect to BLE on address " + address);
            try {
                final BluetoothDevice device = bluetoothAdapter.getRemoteDevice(address);
                Log.d(LogName, "GATT Device=" + device.getAddress());
                BluetoothGatt bluetoothGatt = device.connectGatt(context, true, bluetoothGattCallback);
                Log.d(LogName, "GATT=" + bluetoothGatt.toString());
                return address;
            } catch (IllegalArgumentException exception) {
                Log.w(LogName, "GattError-Refused");
            } catch (SecurityException exception) {
                Log.w(LogName, "GattError-Refused");
            }

        Log.d(LogName, "connectGatt done. No valid address found");
        return null;
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
                    Log.d(LogName, "scanLeDevice Scanning Run");
                    bluetoothAdapter.stopLeScan(mLeScanCallback);
                    //invalidateOptionsMenu();
                }
            }, SCAN_PERIOD);

            scanning = true;
            Log.d(LogName, "scanLeDevice Scanning Start");
            bluetoothAdapter.startLeScan(mLeScanCallback);
        } else {
            scanning = false;
            Log.d(LogName, "scanLeDevice Scanning Stop");
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
                    Log.d(LogName, "scanLeDevice Result " + device);
                }
            };
}
