# Replace "/mnt/floppy" with the whatever directory is appropriate.


make -f makefile.linux64 clean
make -f makefile.linux64 

sudo mount -o loop dev_kernel_grub.img /mnt/floppy
sudo cp kernel.bin /mnt/floppy
sudo umount /mnt/floppy

bochs -q -f bochsrc.bxrc


