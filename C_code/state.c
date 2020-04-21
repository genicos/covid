#include "state.h"
#include <string.h>

state *update_or_create_state(char row[5][15]){
  extern state *state_table[57];

  uint32_t id = state_name_to_id(row[1]);
  state *region = state_table[id];

  if(!region){
    state_table[id] = (state *)calloc(1,sizeof(state));
    region = state_table[id];

    if(!region){
      return NULL;
    }

    uint8_t state_length = strlen(row[1]) + 1;
    
    region->state = (char *)calloc(state_length,sizeof(char));
    for(int h = 0; h < state_length - 1; h++){
      region->state[h] = row[1][h];
    }
    
  }
  
  uint32_t daynumber = date_to_int(row[0]);

  region->cases[daynumber] = atoi(row[3]);
  if(region->cases[daynumber] > region->most_cases_day_count){
    region->most_cases_day_count = region->cases[daynumber];
  }

  region->deaths[daynumber] = atoi(row[4]);
  if(region->deaths[daynumber] > region->most_deaths_day_count){
    region->most_deaths_day_count = region->deaths[daynumber];
  }
  
  region->last_day = daynumber + 1;
  return region;
}


void delete_state(state *s){
  if(s){
    free(s->state);
    free(s);
  }
}


bool output_state(uint8_t id){
  if(!state_table[id]){
    return false;
  }

  char *file_name = (char *)calloc(11, sizeof(char));
  sprintf(file_name,"states/.%d", id);
  
  FILE *out = fopen(file_name, "w");
  free(file_name);

  if(!out){  
    return false;
  }
  
  static char buffer[YEARS*366*3*4 + 34];
  uint32_t buffer_index = 0;

  state *to_write = state_table[id];

  buffer_4bytes(buffer,0, to_write->last_day);
  
   
  for(uint8_t h = 0; h < 14; h++){
    buffer[h + 4] = to_write->state[h];
    if(to_write->state[h] == 0){
      break;
    }
  }
  
  buffer_index = 19;

  //buffering cases array
  for(uint32_t h = 0; h < to_write->last_day; h++, buffer_index+=4){
    if(h > 0 && to_write->cases[h] == 0){
      to_write->cases[h] = to_write->cases[h-1];
    }
    buffer_4bytes(buffer, buffer_index, to_write->cases[h]);
  }
  
  //buffering deaths array
  for(uint32_t h = 0; h < to_write->last_day; h++, buffer_index+=4){
    if(h > 0 && to_write->deaths[h] == 0){
      to_write->deaths[h] = to_write->deaths[h-1];
    }
    buffer_4bytes(buffer, buffer_index, to_write->deaths[h]);
  }
  

  //buffering growth rate * 1000
  for(uint32_t h = 0; h < to_write->last_day; h++, buffer_index+=4){
    if(h<4){
      continue;
    }
    
    uint32_t *cases = &to_write->cases[h];
    
    uint32_t this_change = cases[0]  - cases[-2];
    uint32_t prev_change = cases[-1] - cases[-3];
    
    uint32_t growth_rate = 0;
    if(prev_change != 0){
      growth_rate = (1000)*this_change/prev_change;
    }
    
    buffer_4bytes(buffer, buffer_index, growth_rate);
  }
  
  write(fileno(out), buffer, buffer_index - 4);
  fclose(out);
  return true;
}



uint8_t state_name_to_id(char *name){

  if(!strcmp("Alabama",name))
     return 1;
  if(!strcmp("Alaska",name))
	  return 2;
  if(!strcmp("Arizona",name))
	  return 4;
  if(!strcmp("Arkansas",name))
	  return 5;
  if(!strcmp("California",name))
	  return 6;
  if(!strcmp("Colorado",name))
	  return 8;
  if(!strcmp("Connecticut",name))
	  return 9;
  if(!strcmp("Delaware",name))
	  return 10;
  if(!strcmp("District of Columbia",name))
	  return 11;
  if(!strcmp("Florida",name))
	  return 12;
  if(!strcmp("Georgia",name))
	  return 13;
  if(!strcmp("Hawaii",name))
	  return 15;
  if(!strcmp("Idaho",name))
	  return 16;
  if(!strcmp("Illinois",name))
	  return 17;
  if(!strcmp("Indiana",name))
	  return 18;
  if(!strcmp("Iowa",name))
	  return 19;
  if(!strcmp("Kansas",name))
	  return 20;
  if(!strcmp("Kentucky",name))
	  return 21;
  if(!strcmp("Louisiana",name))
	  return 22;
  if(!strcmp("Maine",name))
	  return 23;
  if(!strcmp("Maryland",name))
	  return 24;
  if(!strcmp("Massachusetts",name))
	  return 25;
  if(!strcmp("Michigan",name))
	  return 26;
  if(!strcmp("Minnesota",name))
	  return 27;
  if(!strcmp("Mississippi",name))
	  return 28;
  if(!strcmp("Missouri",name))
	  return 29;
  if(!strcmp("Montana",name))
	  return 30;
  if(!strcmp("Nebraska",name))
	  return 31;  
  if(!strcmp("Nevada",name))
	  return 32;
  if(!strcmp("New Hampshire",name))
	  return 33;
  if(!strcmp("New Jersey",name))
	  return 34;
  if(!strcmp("New Mexico",name))
	  return 35;
  if(!strcmp("New York",name))
	  return 36;
  if(!strcmp("North Carolina",name))
	  return 37;
  if(!strcmp("North Dakota",name))
	  return 38;
  if(!strcmp("Ohio",name))
	  return 39;
  if(!strcmp("Oklahoma",name))
	  return 40;
  if(!strcmp("Oregon",name))
	  return 41;
  if(!strcmp("Pennsylvania",name))
	  return 42;
  if(!strcmp("Rhode Island",name))
	  return 44;
  if(!strcmp("South Carolina",name))
	  return 45;
  if(!strcmp("South Dakota",name))
	  return 46;
  if(!strcmp("Tennessee",name))
	  return 47;
  if(!strcmp("Texas",name))
	  return 48;
  if(!strcmp("Utah",name))
	  return 49;
  if(!strcmp("Vermont",name))
	  return 50;
  if(!strcmp("Virginia",name))
	  return 51;
  if(!strcmp("Washington",name))
	  return 53;
  if(!strcmp("West Virginia",name))
	  return 54;
  if(!strcmp("Wisconsin",name))
	  return 55;
  if(!strcmp("Wyoming",name))
	  return 56;
  if(!strcmp("Puerto Rico",name))
	  return 3;
  if(!strcmp("Guam",name))
	  return 14;  
  if(!strcmp("Virgin Islands",name))
	  return 43;

  return 0;
}

