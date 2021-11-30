//------------------------------------------------------------
// Location.h
// Location class header file
//
// Author: Jordan Hebler
//------------------------------------------------------------
// The Location class describes a record type that stores a
// Zipcode, City, County, and Coordinates
//------------------------------------------------------------

/**
 * @file Location.h
 * @brief Location class header
 * @details The Location class describes a record type that stores a Zipcode, City, County, and Coordinates
 *
 * @author Jordan Hebler
 */

#ifndef LOCATION_HEADER
#define LOCATION_HEADER

#include "DelimBuffer.h"
#include <string>
#include <iostream>
#include <vector>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <fstream>

using namespace std;

class Location
{       
    public:
        /// @brief Default constructor
        Location();

        /// @brief Copy constructor
        /// @param A Location object to create another object
        /// @post Creat a new Location object with the parameter from the given Location object
        Location(const Location &loc);
 
        /// @brief Constructor
        /// @param s A string of compressed location
        /// @post Creat a new Location object with the parameter from the given string
        Location(const string &s);
        
        /// @brief Access primary key
        /// @return The current value of zip code
        string getKey() const;
        
		/// @brief Access zip code
        /// @return The current value of zip code
        string getZipCode() const;
		
		/// @brief Set zip code
        /// @param New value of zipcode
        /// @post Sets val to zipCode
        void setZipCode(const string &val);
      
		/// @brief Access place name
        /// @return The current value of place name
        string getPlace() const;       

		/// @brief Set place name
        /// @param New value of place
        /// @post Sets val to place
        void setPlace(const string &val);
	    
	    /// @brief Access state
        /// @return The current value of state
        string getState() const;
		      
	    /// @brief Set state
        /// @param New value of state
        /// @post Sets val to state
        void setState(const string &val);

		/// @brief Access county
        /// @return The current value of county
        string getCounty() const;    

	    /// @brief Set county
        /// @param New value of county
        /// @post Sets val to county
        void setCounty(const string &val);
		   
	    /// @brief Access latitude
        /// @return The current Latitude value
        float getLat() const;
      
	    /// @brief Set latitude
        /// @param New value of latitude
        /// @post Sets val to latitude
        void setLat(const float &val);
       
	    /// @brief Access longitude
        /// @return The current Longitude value
        float getLong() const;
      
	    /// @brief Set longitude
        /// @param New value of longtitude
        /// @post Sets val to longtitude
        void setLong(const float &val);
       
		/// @brief This method unpacks data from a buffer and pass it into the Location object
		/// @param DelimBuffer object buffer
		/// @pre buffer must be declared and readed the data file before calling this method
		/// @post Pass the next fields data to the Location object
		/// @return Returns true if unpacked successfully
        bool unpack(DelimBuffer &buffer);
        
		/// @brief Overloaded assignment operator for a location object
		/// @param Location class object
		/// @post The location object in the left now has the same data as the Location object in the right
        void operator= (const Location &loc);

		/// @brief Less than comparison operator overloaded
		/// @param Location class object
		/// @return True if the left state come later than the right state in alphabet order, false otherwise.	
		bool operator< (const Location &loc) const;

		/// @brief Greater than comparison operator overloaded
		/// @param Location class object
		/// @return true if the left state come sooner than the right state in alphabet order, false otherwise.		
		bool operator> (const Location &loc) const;
        
        /// @brief outstream operator overloaded
		/// @param ostream 
		/// @param Location class object
		/// @return returns the overloaded operator right hand operand
        friend ostream& operator<< (ostream& out, const Location &loc);
        
        /// @brief This method get the weight of the Location object
		/// @return The weight of the Location object
        int getWeight() const;

    private:
    	/// Member variable for zip code
        string zipcode; 
        /// Member variable for place name
        string place;
        /// Member variable for state
        string state;
		/// Member variable for county
		string county;
        /// Member variable for latitiude
        float latitude; 
        /// Member variable for longitude
        float longitude;
        /// Weight of the entry
        int weight = 0;
};

#include "Location.cpp"
#endif
