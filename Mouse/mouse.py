# pip install hid pyhidapi
import hid
VID = 0x1234 # replace with your mouse vendor id
PID = 0xABCD #replace with your mouse product id

# find device

for dev in hid.enumerate():
    if dev['vendor_id'] == VID and dev['product_id'] == PID:
        print("Found", dev)
        break

# open
device = hid.device()
device.open(VID, PID) # may require admin on some OSes

# Example: send a feature report (first byte = report id)
# Replace payload with the exact bytes your device expects.
report_id = 0x00
payload = bytes([report_id, 0x10, 0x20, 0x30]) # example
device.send_feature_report(payload)

# read feature back (if supported)

try:
    resp = device.get_feature_report(report_id, 64)
    print("Feature report:", resp)
finally:
    device.close()



