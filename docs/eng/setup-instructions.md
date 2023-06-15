# Testing
## Run linux on Orange Pi Zero LTS
1. download Armbian [firmware](https://www.armbian.com/orange-pi-zero/)
2. flash image to SD card (you can use balena-etcher)
3. connect orange pi via ethernet cable
4. get your eth dev name - `ip a` (i.e. `enp0s2`)
5. set static ip address `sudo ip addr add dev enp0s2 192.168.1.222/24`
6. run dhcp server `sudo dnsmasq -d -C /dev/null --port=0 --domain=localdomain --interface=enp0s2 --dhcp-range=192.168.1.1,192.168.1.255,99h --log-queries`
7. connect orange pi to power source
8. wait for logs from `dnsmasq` which contain device ip address
9. connect ssh `ssh root@192.168.1.75` (replace with pi's ip address)
10. enter credentials. login: `root`, password: `1234`


## Cross-compile toolchain for Orange Pi Zero LTS with `crosstool-ng`
### build crosstool-ng from sources and install
1. `./configure`
2. `make`
3. `make install`
### build toolchain
1. `DEFCONFIG=samples/armv6-unknown-linux-gnueabihf/crosstool.config ct-ng defconfig`
2. `ct-ng build`


## Cross-compile gpiod library
Note: 
use `arm-linux-gnueabihf` when compiling for orange pi. Use `arm-linux-musleabi` when compiling for openipc camera. 
This example will use `arm-linux-gnueabihf`.

1. download [link to libgpiod repo](https://git.kernel.org/pub/scm/libs/libgpiod/libgpiod.git/)
2. extract `tar xvf libgpiod-2.0.tar.gz`
3. `cd libgpiod-2.0.tar.gz`
4. orange pi host `export HOST=arm-linux-gnueabihf`
5. set compiler binary `export CC=arm-unknown-linux-gnueabihf-gcc`
6. set toolchain root path `export SYSROOT=$HOME/x-tool/arm-unknown-linux-gnueabihf`
7. `./autogen.sh --enable-tools=yes --host=$HOST --prefix=$SYSROOT`
8. `./configure --build=i386-linux --host=$HOST --target=$HOST --prefix=$SYSROOT`
9. `make ARCH=arm CROSS_COMPILE=arm-unknown-linux-gnueabihf-`
10. install into sysroot `make install`

## Link against libgpiod library
1. `export PATH="$PATH:$EXT_SDK/bin"`
2. `${CC} -I${SYSROOT}/include -L${SYSROOT}/lib src/examples/rotate.c -lgpiod -static`
3. send `a.out` file to your embedded device