# HC-05 Linux Configuration
+ Find the mac address of HC-05
```
hcitool scan
Scanning ...
    00:13:12:25:33:77   HC-05
```
+ Configure `/etc/bluetooth/rfcomm.conf`
```
rfcomm0 {
  bind no;
  device 00:13:12:25:33:77;
  channel 1;
  comment "HC-05 module";
}
```
+ Pair it
```
bluetooth-agent --adapter hci0 1234&
```
+ Connect using rfcomm
```
sudo rfcomm -i hci0 connect 0
```
+ Freely open device and communicate
```
picocom -b <baud-rate> /dev/rfcomm0
or
minicom -b <baud-rate> -D /dev/rfcomm0
```

# Ubuntu 16.04:

- [Ref](https://wiki.debian.org/BluetoothUser)
- On `ubuntu 16.04`, there is no `bluetooth-agent` so we have to pair ourself using `bluetoothctl`:

    ```bash
    # Sometimes we need to restart the bluetooth device to make it works properly
    $ sudo /etc/init.d/bluetooth restart
    $ bluetoothctl
    [bluetooth]# scan on
    Discovery started
    [CHG] Controller 5C:F3:70:80:7D:A9 Discovering: yes
    [NEW] Device 00:14:03:12:34:48 ZEALOT-HC
    [bluetooth]# devices
    Device 00:14:03:12:34:48 ZEALOT-HC
    [bluetooth]# agent on
    Agent is already registered
    [bluetooth]# pair 00:14:03:12:34:48
    Attempting to pair with 00:14:03:12:34:48
    [CHG] Device 00:14:03:12:34:48 Connected: yes
    Request PIN code
    [agent] Enter PIN code: 1234
    [CHG] Device 00:14:03:12:34:48 UUIDs: 00001101-0000-1000-8000-00805f9b34fb
    [CHG] Device 00:14:03:12:34:48 Paired: yes
    Pairing successful
    [CHG] Device 00:14:03:12:34:48 Connected: no
    [CHG] Device 00:14:03:12:34:48 Connected: yes
    # => At this time, the led on HC-05 module should stop blinking, means pair ok

    # On another terminal tab, type this command to attach HC-05 to a rfcomm port
    $ sudo rfcomm connect 1 00:14:03:12:34:48
    Connected /dev/rfcomm1 to 00:14:03:12:34:48 on channel 1
    # => now we can talk to HC-05 through /dev/rfcomm1
    ```
