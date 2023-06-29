# Geographic Information System

## Description
The system indexes and provides search features for GIS records from the website for the USGS Board on Geographic Names 
(http://geonames.usgs.gov/).

The system builds and maintains several in-memory indexDatabaseByName data structures to support the following operations:
* Importing new GIS records into the databaseService file
* Retrieving data for all GIS records matching a given feature name and state
* Retrieving data for all GIS records that fall within a given (rectangular) geographic region
* Display the in-memory indices in a human-readable manner


### File Formats for Domestic Geographic Names
https://geonames.usgs.gov/docs/pubs/Nat_State_Topic_File_formats.pdf 

The type specifications used here have been modified from the source.


## Usage
```
./GIS <databaseService file name> <command script file name> <log file name>
```

Example:
```
./GIS /home/user/CLionProjects/comp8042/data/db0001.txt /home/user/CLionProjects/comp8042/data/script01.txt /home/user/CLionProjects/comp8042/data/log01.txt