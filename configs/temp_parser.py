import yaml

with open('gpio_example.yaml', 'r') as fin:
    config = yaml.safe_load(fin)
    with open('gpio_example.txt', 'wb') as fout:

        for motor in config['motors']:
            spr = motor['steps_per_revolution']
            rpm = motor['steps_rev_per_minute']
            fout.write(motor['name'].encode())
            fout.write(motor['type'].encode())
            if motor['type'] == 'gpio':
                pins = motor['pins']
                for pin in pins:
                    fout.write(format(pin, 'o').encode())
            fout.write(format(spr, 'o').encode())
            fout.write(format(rpm, 'o').encode())
