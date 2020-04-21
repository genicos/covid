#ifndef __US_H__
#define __US_H__

#include <inttypes.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define YEARS  1   //number of years that data spans, rounded up
#define YEAR_0 2020

//2020-01-01 maps to 1
uint32_t date_to_int(char *date);

//1 maps to 2020-01-01
char *int_to_date(uint32_t daynum);



typedef struct us{
  uint32_t cases[YEARS*365 + YEARS/4 + 1];
  uint32_t deaths[YEARS*365 + YEARS/4 + 1];
  
  uint32_t most_cases_day_count;
  uint32_t most_deaths_day_count;
  uint32_t last_day;
} us;

us *united_states;

//Takes a row from the us.csv and adds the info to the us
us *update_or_create_us(char row[6][15]);

//Buffers 4 bytes from data, little endian
void buffer_4bytes(char *buffer, uint32_t index, uint32_t data);

//creates a file containing us information
//in the following format, all numbers are 4 byte unsigned ints, little endian
//  4 bytes, number of days
//  4 * days bytes, cases for each day
//  4 * days bytes, deaths for each day
//  4 * days bytes, growth rate for each day
bool output_us(void);

void delete_us(void);

#endif
