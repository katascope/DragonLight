package com.katascope.ledragonapp;

import android.bluetooth.BluetoothDevice;
import android.util.Log;
import android.view.LayoutInflater;

import java.util.ArrayList;

// Adapter for holding devices found through scanning.
class LegacyLeDeviceListAdapter {
    private ArrayList<BluetoothDevice> mLeDevices;
    private LayoutInflater mInflator;
    public LegacyLeDeviceListAdapter() {
        super();
        mLeDevices = new ArrayList<BluetoothDevice>();
        //mInflator = DeviceScanActivity.this.getLayoutInflater();
    }
    public void addDevice(BluetoothDevice device) {
        if(!mLeDevices.contains(device)) {
            mLeDevices.add(device);
        }
    }
    public BluetoothDevice findDevice(String uuid)
    {
        for (int i=0;i<mLeDevices.size();i++) {
            BluetoothDevice bd = mLeDevices.get(i);
            if (bd.getAddress().toString().equals(uuid))
            {
                Log.d("SELF","BLE found " + uuid);
                return bd;
            }
        }
        return null;
    }
    public BluetoothDevice getDevice(int position) {
        return mLeDevices.get(position);
    }
    public void clear() {
        mLeDevices.clear();
    }
    public int getCount() {
        return mLeDevices.size();
    }
    public Object getItem(int i) {
        return mLeDevices.get(i);
    }
    public long getItemId(int i) {
        return i;
    }
    /*@Override
    public View getView(int i, View view, ViewGroup viewGroup) {
        ViewHolder viewHolder;
        // General ListView optimization code.
        if (view == null) {
            view = mInflator.inflate(R.layout.listitem_device, null);
            viewHolder = new ViewHolder();
            viewHolder.deviceAddress = (TextView) view.findViewById(R.id.device_address);
            viewHolder.deviceName = (TextView) view.findViewById(R.id.device_name);
            view.setTag(viewHolder);
        } else {
            viewHolder = (ViewHolder) view.getTag();
        }
        BluetoothDevice device = mLeDevices.get(i);
        final String deviceName = device.getName();
        if (deviceName != null && deviceName.length() > 0)
            viewHolder.deviceName.setText(deviceName);
        else
            viewHolder.deviceName.setText(R.string.unknown_device);
        viewHolder.deviceAddress.setText(device.getAddress());
        return view;
    }*/
}