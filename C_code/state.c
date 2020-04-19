#include "state.h"
#include <string.h>

state *update_or_create_state(char row[5][15]){
  row++;
  return NULL;
}

bool output_state(uint8_t id){
  id++;
  return false;
}

void delete_state(state *s){
  s++;
}

uint8_t state_name_to_id(char *name){
  if(strcmp("Alabama",name))
	  return 1;
  if(strcmp("Alaska",name))
	  return 2;
  if(strcmp("Arizona",name))
	  return 4;
  if(strcmp("Arkansas",name))
	  return 5;
  if(strcmp("California",name))
	  return 6;
  if(strcmp("Colorado",name))
	  return 8;
  if(strcmp("Connecticut",name))
	  return 9;
  if(strcmp("Delaware",name))
	  return 10;
  if(strcmp("District of Columbia",name))
	  return 11;
  if(strcmp("Florida",name))
	  return 12;
  if(strcmp("Georgia",name))
	  return 13;
  if(strcmp("Hawaii",name))
	  return 15;
  if(strcmp("Idaho",name))
	  return 16;
  if(strcmp("Illinois",name))
	  return 17;
  if(strcmp("Indiana",name))
	  return 18;
  if(strcmp("Iowa",name))
	  return 19;
  if(strcmp("Kansas",name))
	  return 20;
  if(strcmp("Kentucky",name))
	  return 21;
  if(strcmp("Louisiana",name))
	  return 22;
  if(strcmp("Maine",name))
	  return 23;
  if(strcmp("Maryland",name))
	  return 24;
  if(strcmp("Massachusetts",name))
	  return 25;
  if(strcmp("Michigan",name))
	  return 26;
  if(strcmp("Minnesota",name))
	  return 27;
  if(strcmp("Mississippi",name))
	  return 28;
  if(strcmp("Missouri",name))
	  return 29;
  if(strcmp("Montana",name))
	  return 30;
  if(strcmp("Nebraska",name))
	  return 31;  
  if(strcmp("Nevada",name))
	  return 32;
  if(strcmp("New Hampshire",name))
	  return 33;
  if(strcmp("New Jersey",name))
	  return 34;
  if(strcmp("New Mexico",name))
	  return 35;
  if(strcmp("New York",name))
	  return 36;
  if(strcmp("North Carolina",name))
	  return 37;
  if(strcmp("North Dakota",name))
	  return 38;
  if(strcmp("Ohio",name))
	  return 39;
  if(strcmp("Oklahoma",name))
	  return 40;
  if(strcmp("Oregon",name))
	  return 41;
  if(strcmp("Pennsylvania",name))
	  return 42;
  if(strcmp("Rhode Island",name))
	  return 44;
  if(strcmp("South Carolina",name))
	  return 45;
  if(strcmp("South Dakota",name))
	  return 46;
  if(strcmp("Tennessee",name))
	  return 47;
  if(strcmp("Texas",name))
	  return 48;
  if(strcmp("Utah",name))
	  return 49;
  if(strcmp("Vermont",name))
	  return 50;
  if(strcmp("Virginia",name))
	  return 51;
  if(strcmp("Washington",name))
	  return 53;
  if(strcmp("West Virginia",name))
	  return 54;
  if(strcmp("Wisconsin",name))
	  return 55;
  if(strcmp("Wyoming",name))
	  return 56;
  if(strcmp("Puerto Rico",name))
	  return 3;
  if(strcmp("Guam",name))
	  return 14;  
  if(strcmp("Virgin Islands",name))
	  return 43;

  return 0;
}

