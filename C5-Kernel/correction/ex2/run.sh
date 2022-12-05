#!/bin/sh
#
#wget https://cdn.kernel.org/pub/linux/kernel/v6.x/linux-6.0.11.tar.xz
#wget https://cdn.kernel.org/pub/linux/kernel/v6.x/linux-6.0.11.tar.sign
#gpg --locate-key greghk@kernel.org
#unxz linux...
#gpg --verify linux-6.0.1.tar.sign
#tar xf linux.... && cd linux.....
#make tinyconfig
#make menuconfig
#cp /boot/config-... .config

make -j8


sudo mkinitramfs ramfs.img

qemu-system-x86_64 -kernel arch/x86/boot/bzImage -nographic -append "console=ttyS0" -initrd ramfs.img -m 1G -smp 4 -cpu host --enable-kvm

# OU
# sudo make modules_install
# sudo make install
# reboot
