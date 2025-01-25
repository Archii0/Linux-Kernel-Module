Load kernel module:
```
sudo insmod <modulename>.ko
```


Unload kernel module:
```
sudo rmmod <modulename>.ko
```


Monitor kernel output:
```
sudo dmesg -W
```

If system is booted with safeboot enabled, modules need to be signed or loading them will fail.