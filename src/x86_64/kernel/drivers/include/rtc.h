
#ifndef RTC_H
#define RTC_H
#include <stdint.h>
#include <IO.h>
#include <stdio.h>

#define CMOS_ADDRESS 0x70
#define CMOS_DATA 0x71

uint8_t second;
uint8_t minute;
uint8_t hour;
uint8_t day;
uint8_t month;
uint32_t year;

typedef struct datetime {
	uint8_t second;
	uint8_t minute;
	uint8_t hour;
	uint8_t day;
	uint8_t month;
	uint32_t year;
} datetime_t;

uint8_t read_cmos(uint8_t address);
uint8_t get_update_in_progress_flag();

void read_rtc();

datetime_t get_datetime();

#endif