#ifndef __COUNTY_H__
#define __COUNTY_H__

#include "state.h"


typedef struct county{
  uint32_t cases[YEARS*365 + YEARS/4 + 1];
  uint32_t deaths[YEARS*365 + YEARS/4 + 1];
  
  uint32_t most_cases_day_count;
  uint32_t most_deaths_day_count;
  uint32_t last_day;

  char *state;
  char *name;
} county;

county *county_table[57000];

//Takes a row from the csv and adds the info to the county
county *update_or_create_county(char row_entry[6][15]);

//creates a file containing county information,
//in the following format, all numbers are 4 byte unsigned ints, little endian:
//  4 bytes, number of days
//  15 bytes, county name
//  15 bytes, state
//  4 * days bytes, cases for each day
//  4 * days bytes, deaths for each day
//  4 * days bytes, growth rate for each day
bool output_county(uint16_t fips);

void delete_county(county *c);

uint8_t state_name_to_id(char *name);
#endif
