#include "county.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>

#define YEAR_0 2020

bool is_leap_year(uint32_t year){
  return (year%4 == 0 &&(!year%100 == 0 || year%400 == 0));
}

static const uint16_t zeroeth_day_of[12] = {
  0,
  31,
  59,
  90,
  120,
  151,
  181,
  212,
  243,
  273,
  304,
  334,
};

uint32_t date_to_int(char *date){
  
  uint16_t year  = atoi(date);
  uint8_t month = atoi(&date[5]);
  uint8_t day   = atoi(&date[8]);
  

  uint32_t ans = 0;
  
  if(is_leap_year(year) && month>2){
    ans++;
  } 

  while(year > YEAR_0){
    ans+=365;
    year--;
    if(is_leap_year(year)){ //leap years
      ans++;
    }
  }
  
  ans += zeroeth_day_of[month-1];
   
  ans += day;
  
  return ans;
}



char *int_to_date(uint32_t daynum){
  char *ans = (char *)calloc(11, sizeof(char));
  if(!ans){
    return NULL;
  }
  
  uint16_t year = YEAR_0 + daynum/146097 * 400; //as if my code will last 400 years
  daynum%=146097;
  
  for(uint32_t h = YEAR_0; daynum > 365; h++){
    if(is_leap_year(h)){
      daynum--;
    }
    daynum-=365;
    year++;
  }
  
  if(daynum == 0){
    daynum +=366;
    year--;
  }
  
  uint8_t month = 0;

  for(uint8_t mon = 0; mon < 12; mon++){
    uint32_t zeroeth_day_of_month = zeroeth_day_of[mon];
    
    if(is_leap_year(year) && mon>1){
      zeroeth_day_of_month++;
    }
    
    if(zeroeth_day_of_month >= daynum){
      month = mon;
      break;
    }
    
    if(mon == 11){
      month = 12;
    }
  }
  
  if(is_leap_year(year) && month > 2){
    daynum--;
  }
  
  daynum-=zeroeth_day_of[month-1];
  
  uint8_t day = daynum;

  sprintf(ans, "%04d-%02d-%02d", year%10000,month%100,day%100);
  
  return ans;
}


county *update_or_create(char row[6][15]){
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
  
  area->deaths[daynumber] = atoi(row[5]);

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

void buffer_4bytes(char *buffer, uint32_t index, uint32_t data){
  buffer[index + 0] = data       & 0xFF;
  buffer[index + 1] = data >>  8 & 0xFF;
  buffer[index + 2] = data >> 16 & 0xFF;
  buffer[index + 3] = data >> 24 & 0xFF;
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
