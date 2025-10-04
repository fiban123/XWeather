# XWeather
C++ based Weather station software running on raspberry pi

## continuous entry type
entries that can be read and stored at any time, not dependant on any events. For example temperature, humidity, etc...

## event-based entry type
entries based on events at certain timestamps. For example a lightning strike


## data file architecture
data is stored in ~/xweather_data

there are directories for each year and files for each day.

Each data file contains data for each continuous entry every second
and each data file contains all timestamps of event-based entries

## data cache
continuous data entries are added every second to the data. It would be unreasonable to write <100B to a data file every second. So it is put in a cache and the contents of the cache are written into the data files every second. So the sache size is 1800 entries.

for event-based entries, they will just be cached in the current cache for continuous entries.

## data entries
temperature
humidity
wind speed
sunlight
UV-light
rain
wind direction

lightning strike

## data file format

// continuous data entries

{number of continuous data entries as uint64}
{number of continuous data entry types as uint64}
{temperatrue as float at t+0}{temperature as float at t+1}...{temperature as flaot at t+86400} // number of seconds per day = 86400
// size of single continuous data entry: 2.7648 Mbit / day = 345.600 KB / day
... 

// event-based data entries
{number of event-based entry types as uint64}
{number of event-based entries as uint64}

{x as uint64}
{timestamp of lightning strike 1 as uint64}
{timestamp of lightning strike 2 as uint64}
...
{timestamp of lightning strike x as uint64}