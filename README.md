IN THIS DIRECTORY
-----------------

C_code - holds the c code for this project

counties - holds county byte files

states - holds state byte files

united_states - country byte file

us-counties.csv - Data provided by The New York times in github.com/nytimes/covid-19-data
us-states.csv
us.csv

grapher.py - python code, turns county information into graphs


TO BUILD
--------

in this directory, enter the command 'make covid'


MAKE COMMANDS
-------------

'make covid' builds covids, csv parser program

'make covidval' test covid with valgrind


COUNTY BYTE FILE FORMAT
-----------------------
All numbers are unsigned, 4 bytes, little endian

[4 bytes]      - number of days, call it DAYS
[15 bytes]     - county name
[15 bytes]     - state name
[4*DAYS bytes] - cases for each day
[4*DAYS bytes] - deaths for each day
[4*DAYS bytes] - growth rate for each day, multiplied by 1000 and truncated


STATE BYTE FILE FORMAT
-----------------------
All numbers are unsigned, 4 bytes, little endian

[4 bytes]      - number of days, call it DAYS
[15 bytes]     - state name
[4*DAYS bytes] - cases for each day
[4*DAYS bytes] - deaths for each day
[4*DAYS bytes] - growth rate for each day, multiplied by 1000 and truncated


UNITED_STATES BYTE FILE FORMAT
-----------------------
All numbers are unsigned, 4 bytes, little endian

[4 bytes]      - number of days, call it DAYS
[4*DAYS bytes] - cases for each day
[4*DAYS bytes] - deaths for each day
[4*DAYS bytes] - growth rate for each day, multiplied by 1000 and truncated



