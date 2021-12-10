//------------------------------------------------------------
// Location.cpp
// Location class implementation file
//
// Author: Jordan Hebler
//------------------------------------------------------------
// The Location class describes a record type that stores a
// Zipcode, City, County, and Coordinates
//------------------------------------------------------------

/**
 * @file Location.cpp
 * @brief Implementation file for Location.h
 *
 * @author Jordan Hebler
 */

#include "Location.h"

/// @brief Default constructor
Location :: Location()
{
    zipcode = " ";
    state = " ";
    latitude = 0;
    longitude = 0;
    weight = 0;
};


/// @brief Copy constructor
Location :: Location(const Location& loc)
{
    zipcode = loc.getZipCode();
    state = loc.getState();
    place = loc.getPlace();
    county = loc.getCounty();
    latitude = loc.getLat();
    longitude = loc.getLong();
    weight = loc.getWeight();
};


/// @brief Constructor
 Location :: Location(const string &s)
{
	weight = s.length() + 2;
	string temp[6];
	int field = 0;
	for (int i = 2; i < s.length(); i++)
	{
		if (s[i] == ',') field++;
		else temp[field] += s[i];
	}
	setZipCode(temp[0]);
	setPlace(temp[1]);
	setState(temp[2]);
	setCounty(temp[3]);
	stringstream(temp[4]) >> latitude; //convert to float
	stringstream(temp[5]) >> longitude; //convert to float	
}

/// @brief Access primary key
string Location :: getKey() const { return zipcode; } 
/// @brief Access zip code
string Location :: getZipCode() const { return zipcode; } 
/// @brief Access place name
string Location :: getPlace() const { return place; } 
/// @brief Access state
string Location :: getState() const { return state; } 
/// @brief Access county
string Location :: getCounty() const { return county; } 
/// @brief Access latitude
float Location :: getLat() const { return latitude; } 
/// @brief Access longitude
float Location :: getLong() const { return longitude; }


/// @brief Set zip code
void Location :: setZipCode(const string &val) { zipcode = val; }
/// @briefSet place name
void Location :: setPlace(const string &val) { place = val; }
/// @brief Set state
void Location :: setState(const string &val) { state = val; }
/// @brief Set county
void Location :: setCounty(const string &val) { county = val; }
/// @brief Set latitude
void Location :: setLat(const float &val) { latitude = val; }
/// @brief Set longtitude
void Location :: setLong(const float &val) { longitude = val; }


/// @brief This method unpacks data from a buffer and pass it into the Location object
bool Location :: unpack(DelimBuffer &buffer)
{
	bool result;
	
    buffer.increment();
	result = buffer.unpackField(zipcode);
	result = result && buffer.unpackField(place);
	result = result && buffer.unpackField(state);
	result = result && buffer.unpackField(county);
	string lat_str, long_str;
	result = result && buffer.unpackField(lat_str);
	result = result && buffer.unpackField(long_str);
	stringstream(lat_str) >> latitude; //convert to float
	stringstream(long_str) >> longitude; //convert to float
	
	weight = zipcode.length() +  place.length() +  state.length()
            + county.length()  + lat_str.length() +  long_str.length() + 7; // Add 7 for 5 ',' and the 2 byte length
	
	return result;
}


/// @brief Overloaded assignment operator for a location object
void Location :: operator= (const Location &loc)
{
	setZipCode(loc.getZipCode());
	setPlace(loc.getPlace());
	setState(loc.getState());
	setCounty(loc.getCounty());
	setLat(loc.getLat());
	setLong(loc.getLong());
	weight = loc.getWeight();
}


/// @brief Outstream operator overloaded
ostream& operator<< (ostream& out, const Location &loc)
{
    ostringstream w;
    w << loc.getWeight();
	string s = "";
	
	s += w.str() +
         loc.getZipCode() + ','
	   + loc.getPlace() + ','
	   + loc.getState() + ','
	   + loc.getCounty() + ',';
	
	ostringstream s1, s2;	
	s1 << loc.getLat();
	string lat(s1.str());	
	s2 << loc.getLong();
	string lon(s2.str());
	
	s += lat + ',' + lon;
	
	out << s << endl;
    
    return out;
}


/// @brief Less than comparison operator overloaded
bool  Location :: operator< (const Location &loc) const
{
	return getState() < loc.getState();
}


/// @brief Greater than comparison operator overloaded
bool  Location :: operator> (const Location &loc) const
{
	return getState() > loc.getState();
}


/// @brief This method get the weight of the Location object
int Location :: getWeight() const
{
	return weight;
}
