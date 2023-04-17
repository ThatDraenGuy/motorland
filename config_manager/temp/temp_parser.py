import yaml
import struct

# Define the YAML file path and binary file path
yaml_file = "../examples/example.yaml"
binary_file = "../examples/out/example.bin"

# Load the YAML file
with open(yaml_file, "r") as f:
    config = yaml.safe_load(f)

# Define the binary format using the struct module
fmt = "<6sHHHfI"  # 6 strings, 3 shorts, 1 float, and 1 integer in little-endian byte order
packed_data = b''  # Initialize packed data as an empty byte string

# Loop over each motor in the configuration and pack its data into binary format
for motor in config["motors"]:
    # Extract the motor data from the YAML
    name = motor["name"].encode("utf-8")
    type = motor["type"].encode("utf-8")
    steps_per_rev = motor["steps_per_revolution"]
    max_speed = motor["steps_rev_per_minute"]
    if type == "gpio":
        pins = motor["pins"]
        data = (name, steps_per_rev, max_speed, len(pins), 0, 0)
        packed_data += struct.pack(fmt, *data)
        for pin in pins:
            packed_data += struct.pack("<H", pin)
    elif type == "spi":
        bus = motor["bus"]
        device = motor["device"]
        mode = motor["mode"]
        max_speed_hz = motor["max_speed_hz"]
        data = (name, steps_per_rev, max_speed, bus, device, mode)
        packed_data += struct.pack(fmt, *data)
    elif type == "i2c":
        bus = motor["bus"]
        address = motor["address"]
        data = (name, steps_per_rev, max_speed, bus, address, 0)
        packed_data += struct.pack(fmt, *data)
    else:
        raise ValueError("Invalid motor type: " + type)

# Write the binary data to a file
with open(binary_file, "wb") as f:
    f.write(packed_data)