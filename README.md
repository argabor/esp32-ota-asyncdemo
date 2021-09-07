# esp32-ota-asyncdemo

Read more [here](https://husarnet.com/blog/internet-ota-esp32)

## Making a custom HTTP request with `cURL`:

```bash
curl -X POST \
>   'http://ota-test:3232/update' \
>   -H 'Accept: */*' \
>   -H 'Accept-Encoding: gzip, deflate' \
>   -H 'Connection: keep-alive' \
>   -H 'Content-Length: 3000000' \
>   -H 'Cookie: visited=1' \
>   -H 'Host: ota-test:3232' \
>   -H 'Origin: http://ota-test:3232' \
>   -H 'Referer: http://ota-test:3232/update' \
>   --form 'MD5="b24c693746dfc55f7088c97758fac61a"' \
>   --form 'firmware=@<FULL_PATH_TO_FIRMWARE_BIN>/firmware.bin'
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