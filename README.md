# esp32-ota-asyncdemo

Read more [here](https://husarnet.com/blog/internet-ota-esp32)

## Making a custom HTTP request with `cURL`:

```bash
curl -# -v -X POST \
  'http://ota-test:3232/update' \
  -H 'Accept: */*' \
  -H 'Accept-Encoding: gzip, deflate' \
  -H 'Connection: keep-alive' \
  --form "MD5="$(md5sum "/home/dominik/tech/esp32_workspace/esp32-ota-asyncdemo/firmware.bin" | cut -d ' ' -f 1)"" \
  --form 'firmware=@/home/dominik/tech/esp32_workspace/esp32-ota-asyncdemo/firmware.bin'
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