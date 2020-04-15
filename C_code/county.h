#ifndef __COUNTY_H__
#define __COUNTY_H__

#include <inttypes.h>
#include <stdbool.h>

#define YEARS 1

//2020-01-01 maps to 1
uint32_t date_to_int(char *date);

//1 maps to 2020-01-01
char *int_to_date(uint32_t daynum);

typedef struct county{
  uint32_t cases[YEARS*365 + YEARS/4 + 1];
  uint32_t deaths[YEARS*365 + YEARS/4 + 1];
  char *name;
  char *state;

  uint32_t last_day;
} county;

county *county_table[57000];

county *update_or_create(char row_entry[6][15]);

bool output_county(uint16_t fips);

void delete_county(county *c);

uint8_t state_name_to_id(char *name);
#endif
