#include "county.h"





county *update_or_create_county(char row[6][15]){
  extern county *county_table[57000];
  
  uint32_t fips = atoi(row[3]);
  
  county *area = county_table[fips];
  
  if(!area){
    
    county_table[fips] = (county *)calloc(1,sizeof(county));
    area = county_table[fips];

    if(!area){
      return NULL;
    }
    

    uint8_t name_length = strlen(row[1]) + 1;

    area->name = (char *)calloc(name_length, sizeof(char));
    for(int h = 0; h < name_length - 1; h++){
      area->name[h] = row[1][h];
    }
    

    uint8_t state_length = strlen(row[2]) + 1;
    
    area->state = (char *)calloc(state_length, sizeof(char));
    for(int h = 0; h < state_length - 1; h++){
      area->state[h] = row[2][h];
    }
    
  }
   
  uint32_t daynumber = date_to_int(row[0]);

  area->cases[daynumber]  = atoi(row[4]);
  if(area->cases[daynumber] > area->most_cases_day_count){
    area->most_cases_day_count = area->cases[daynumber];
  } 

  area->deaths[daynumber] = atoi(row[5]);
  if(area->deaths[daynumber] > area->most_deaths_day_count){
    area->most_deaths_day_count = area->deaths[daynumber];
  }

  area->last_day = daynumber + 1;
  
  return area;
}


void delete_county(county *c){
  if(c){
    free(c->name);
    free(c->state);
    free(c);
  }
}




bool output_county(uint16_t fips){
  if(!county_table[fips]){
    return false;
  }
  

  char *file_name = (char *)calloc(16, sizeof(char));
  sprintf(file_name,"counties/.%d", fips);
  
  FILE *out = fopen(file_name, "w");
  free(file_name);

  if(!out){  
    return false;
  }
  
  static char buffer[YEARS*366*3*4 + 34];
  uint32_t buffer_index = 0;

  county *to_write = county_table[fips];

  buffer_4bytes(buffer,0, to_write->last_day);
  
  
  for(uint8_t h = 0; h < 14; h++){
    buffer[h + 4] = to_write->name[h];
    if(to_write->name[h] == 0){
      break;
    }
  }
   
  for(uint8_t h = 0; h < 14; h++){
    buffer[h + 19] = to_write->state[h];
    if(to_write->state[h] == 0){
      break;
    }
  }
  
  buffer_index = 34;

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



