
OUTPUT_FORMAT("binary","binary","binary")

SECTIONS {
	. = 0x7C00;
	.bootloader : {
		*(.stage1);
		*(.stage1.data);
		. = 0x1fe;
		BYTE(0x55);
		BYTE(0xaa);
		_bootstrap_end = .;
		*(.stage12);
		*(.stage12*);
		
		*(.stage2);
		*(.stage2*);
		*(.stage3);
		*(.stage3*);
		_bootloader_end = .;
	}

	. = ASSERT(_bootstrap_end <= 0x7E00,"Bootstrap section too big! (>510 bytes)");
	. = ASSERT(_bootloader_end <= 0x9FFF,"Bootloader section too big! (>8.5 kilobytes)");
	. = ASSERT(_bootloader_end <= 0x8600,"Load more boot pages from disk (and then change this error)");

	/DISCARD/ : {
		*(.debug_*)
		*(.note*)
		*(.indent)
		*(.comment)
		*(.stab)
		*(.stabstr)
		*(.eh_frame)
		*(.eh_frame_hdr)
	}
}

