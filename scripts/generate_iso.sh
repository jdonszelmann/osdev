



mkdir -p iso/boot/grub

cp $1 iso/boot/kernel

cat > iso/boot/grub/grub.cfg <<EOF
	set timeout=0
	set default=0

	menuentry OSDEV {
		multiboot /boot/kernel   # The multiboot command replaces the kernel command
		boot
	}

EOF

grub-mkrescue -o $2 iso/