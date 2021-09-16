# esp32-ota-asyncdemo

Read more [here](https://husarnet.com/blog/internet-ota-esp32)

## Quick start

```bash
export ENV_WIFI_SSID=YOUR_WIFI_SSID
export ENV_WIFI_PASS=YOUR_WIFI_PASS
export ENV_HUSARNET_JOINCODE=YOUR_JOIN_CODE
pio run -t upload
```

## Making a custom HTTP request with `cURL`:

```bash
curl -# -v -X POST \
  'http://ota-test:3232/update' \
  -H 'Accept: */*' \
  -H 'Accept-Encoding: gzip, deflate' \
  -H 'Connection: keep-alive' \
  --form "MD5="$(md5sum "/home/dominik/tech/esp32-ota-asyncdemo/.pio/build/esp32dev/firmware.bin" | cut -d ' ' -f 1)"" \
  --form 'firmware=@/home/dominik/tech/esp32-ota-asyncdemo/.pio/build/esp32dev/firmware.bin'
```

```bash
curl -# -v \
-H 'Accept: */*' \
-H 'Accept-Encoding: gzip, deflate' \
-H 'Connection: keep-alive' \
-H 'Expect:' \
-H 'Cookie: visited=1' \
-H 'Accept-Language: en-US,en;q=0.5' \
-H 'Origin: http://ota-test:3232' \
-H 'Referer: http://ota-test:3232/update' \
-F "MD5="$(md5sum "/home/dominik/tech/esp32-ota-asyncdemo/.pio/build/esp32dev/firmware.bin" | cut -d ' ' -f 1)"" \
-F 'firmware=@/home/dominik/tech/esp32-ota-asyncdemo/.pio/build/esp32dev/firmware.bin' \
'http://ota-test:3232/update'
```

## Troubleshooting

### Erasing flash memory of ESP32

1. Connect ESP32 to your laptop

2. Install platformio CLI

```bash
pip install -U platformio
```

3. Make flash erase:

```bash
pio run --target erase
```

### Monitoring network traffic on `hnet0` interface

```bash
sudo tcpflow -p -c -i hnet0
```