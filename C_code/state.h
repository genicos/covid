#ifndef __STATE_H__
#define __STATE_H__

#include "us.h"

typedef struct state{
  uint32_t cases[YEARS*365 + YEARS/4 + 1];
  uint32_t deaths[YEARS*365 + YEARS/4 + 1];
  char *state;

  uint32_t most_cases_day_count;
  uint32_t most_deaths_day_count;
  uint32_t last_day;
} state;

state *state_table[57];

//Takes a row from the us-states.csv and adds the info to the state
state *update_or_create_state(char row[5][15]);

//creates a file containing state information,
//in the following format, all numbers are 4 byte unsigned ints, little endian
//  4 bytes, number of days
//  15 bytes, state name
//  4 * days bytes, cases for each day
//  4 * days bytes, deaths for each day
//  4 * days bytes, growth rate for each day
bool output_state(uint8_t id);

void delete_state(state *s);

uint8_t state_name_to_id(char *name);

#endif
