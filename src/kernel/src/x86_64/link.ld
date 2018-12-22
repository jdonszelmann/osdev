
OUTPUT_FORMAT("binary","binary","binary")

SECTIONS {
	. = 0x10000;
	.kern : {
		*(.kernel);
		*(.kernel*);
		*(.game*);
		BYTE(0x42);
		_system_end = .;
	}

	. = ASSERT(_system_end <= 0x20000,"Load more kernel pages from disk (and then change this error)");
	. = ASSERT(_system_end <= 0x78400,"System initialization section too big! (>480 KB)");

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
