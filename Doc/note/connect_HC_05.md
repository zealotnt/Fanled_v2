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
sudo rfcomm connect 0
```
+ Freely open device and communicate
```
picocom -b <baud-rate> /dev/rfcomm0
or
minicom -b <baud-rate> -D /dev/rfcomm0
```
