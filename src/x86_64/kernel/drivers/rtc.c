
#include <rtc.h>

#define CURRENT_YEAR 2018 // Change this each year!

uint8_t century_register = 0x00; // Set by ACPI table parsing code if possible

datetime_t get_datetime() {
	read_rtc();
	datetime_t dt;
	dt.second = second;
	dt.minute = minute;
	dt.hour = hour;
	dt.day = day;
	dt.month = month;
	dt.year = year;
	return dt;
}

uint8_t read_cmos(uint8_t address) {
	uint8_t data;
	outportb(CMOS_ADDRESS, address);
	data = inportb(CMOS_DATA);
	return data;
}

uint8_t get_update_in_progress_flag() { return read_cmos(0x0A) & 0x80; }

void read_rtc() {
	uint8_t century;
	uint8_t last_second;
	uint8_t last_minute;
	uint8_t last_hour;
	uint8_t last_day;
	uint8_t last_month;
	uint8_t last_year;
	uint8_t last_century;
	uint8_t registerB;

	// Note: This uses the "read registers until you get the same values twice
	// in a row" technique
	//       to avoid getting dodgy/inconsistent values due to RTC updates

	while(get_update_in_progress_flag())
		; // Make sure an update isn't in progress
	second = read_cmos(0x00);
	minute = read_cmos(0x02);
	hour = read_cmos(0x04);
	day = read_cmos(0x07);
	month = read_cmos(0x08);
	year = read_cmos(0x09);
	if(century_register != 0) {
		century = read_cmos(century_register);
	}

	do {
		last_second = second;
		last_minute = minute;
		last_hour = hour;
		last_day = day;
		last_month = month;
		last_year = year;
		last_century = century;

		while(get_update_in_progress_flag())
			; // Make sure an update isn't in progress
		second = read_cmos(0x00);
		minute = read_cmos(0x02);
		hour = read_cmos(0x04);
		day = read_cmos(0x07);
		month = read_cmos(0x08);
		year = read_cmos(0x09);
		if(century_register != 0) {
			century = read_cmos(century_register);
		}
	} while((last_second != second) || (last_minute != minute) ||
			(last_hour != hour) || (last_day != day) || (last_month != month) ||
			(last_year != year) || (last_century != century));

	registerB = read_cmos(0x0B);

	// Convert BCD to binary values if necessary

	if(!(registerB & 0x04)) {
		second = (second & 0x0F) + ((second / 16) * 10);
		minute = (minute & 0x0F) + ((minute / 16) * 10);
		hour = ((hour & 0x0F) + (((hour & 0x70) / 16) * 10)) | (hour & 0x80);
		day = (day & 0x0F) + ((day / 16) * 10);
		month = (month & 0x0F) + ((month / 16) * 10);
		year = (year & 0x0F) + ((year / 16) * 10);
		if(century_register != 0) {
			century = (century & 0x0F) + ((century / 16) * 10);
		}
	}

	// Convert 12 hour clock to 24 hour clock if necessary

	if(!(registerB & 0x02) && (hour & 0x80)) {
		hour = ((hour & 0x7F) + 12) % 24;
	}

	// Calculate the full (4-digit) year

	if(century_register != 0) {
		year += century * 100;
	} else {
		year += (CURRENT_YEAR / 100) * 100;
		if(year < CURRENT_YEAR) year += 100;
	}
}