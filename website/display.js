var YEAR_0 = 2020;
/*

~~(a/b) is used for integer division (the bit operation ~ tells javascript to work with integers)

*/

function is_leap_year(year){
  return (year%4==0) && (year%100!=0 || year%400==0);
}


function IntToDate(daynum){
  let zeroeth_day_of = [
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
  ];
  
  var year = YEAR_0 + ~~(daynum/146097) * 400; //as if my code will last 400 years
  daynum%=146097;
  
  for(h = YEAR_0; daynum > 365; h++){
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
  
  var month = 0;

  for(mon = 0; mon < 12; mon++){
    var zeroeth_day_of_month = zeroeth_day_of[mon];
    
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
  
  var day = daynum;
  
  var year_string = (year%10000)+"";
  while (year_string.length < 4) year_string = "0" + year_string;
  
  var month_string = month+"";
  while (month_string.length < 2) month_string = "0" + month_string;
  
  var day_string = day+"";
  while (day_string.length < 2) day_string = "0" + day_string;
  
  
  var ans = year_string + "-" + month_string + "-" + day_string;
  
  return ans;
}


/*
const fs = require('fs')



fs.readFile('../united_states', (err,data) => {
  if(err) throw err;
  
  days = data.readUInt32LE();
  
  cases       = new Array(days);
  deaths      = new Array(days);
  growth_rate = new Array(days);
  
  for(i = 0; i<days; i++){
    cases      [i] = data.readUInt32LE( 4*(1 + days*0 + i) );
  }
  
  for(i = 0; i<days; i++){
    deaths     [i] = data.readUInt32LE( 4*(1 + days*1 + i) );
  }
  
  for(i = 0; i<days; i++){
    growth_rate[i] = data.readUInt32LE( 4*(1 + days*2 + i) );
    console.log(growth_rate[i]);
  }
  
  
})
*/
var c = document.getElementById("draw");
var ctx = c.getContext("2d");

ctx.fillStyle = "#0000FF";
ctx.fillRect(0,0,100,50);

