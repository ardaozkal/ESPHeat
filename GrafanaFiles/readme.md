 
# Read this first!

(By device name, I mean the "devicename" you were supposed to replace during setup of the arduino files)

For these to work, you need to do one of these:

### Solution One:

Open JSON by hand and change every mention of "dht11" to what you named your device running dht11, and every mention of "sht30" with what you named your device running sht30.

### Solution Two:

Name the devices "dht11" or "sht30" directly.

### Solution Three:

After adding each file to grafana, open each graph and change sht30 with your proper device name.