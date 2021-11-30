//-------------------------------------------------------------
// DelimBuffer.cpp
// Buffer class implementation file
//
// Author: Justine Canlas, Jordan Hebler
//-------------------------------------------------------------
// This class reads in each field from the provided file or
// istream. It reads in each block header and its corresponding
// length-indicated records and places them into the buffer.
//-------------------------------------------------------------

/**
 * @file DelimBuffer.cpp
 * @brief Buffer class implementation file
 *
 * @author Jordan Hebler
 * @author Justine Canlas
 */

#include "DelimBuffer.h"

/// @brief Default Constructor
/// @param char Delimiter character used for the buffer object
DelimBuffer :: DelimBuffer(char del)
{
	delim = del; 
	bufferSize = 0; 
	nextCharIndex = 0;
}


/// @brief Clear the buffer
void DelimBuffer :: clear()
{
	bufferSize = 0;
	blockNumber = 0;
	sBlockNumber = 0;
	pBlockNumber = 0;
	nextCharIndex = 0;
	buffer = "";
	numRecs = 0;
}


/// @brief Method reads from the file stream and tests to see if stream open
bool DelimBuffer :: read(istream& infile)
{
	// clear buffer
	clear();
	
	if(infile.eof()) return false;
	
	// read block size
	string line;
	getline(infile,line);
	
    // Reads the block header and sets the appropriate values
	int dataCount = 1;
	for (int i = 1; i < line.length(); i++)
	{
		if (line[i] == ',') {dataCount++; continue;}
		if (dataCount == 1)
		{
			int num = line[i] - '0';
			blockNumber = blockNumber * 10 + num;
		}
		else if (dataCount == 2)
		{
			int num = line[i] - '0';
			numRecs = numRecs * 10 + num;
		}
		else if (dataCount == 3)
		{
			int num = line[i] - '0';
			pBlockNumber = pBlockNumber * 10 + num;
		}
		else if (dataCount == 4)
		{
			int num = line[i] - '0';
			sBlockNumber = sBlockNumber * 10 + num;
		}
		else
		{
			break;
		}
	}
	
	// get each record in the block to buffer
	for (int i = 0; i < numRecs; i++)
	{
		getline(infile,line);
		bufferSize += line.length();
		buffer += line + '\n';
		if (bufferSize > maxByte)
			{cout << "Buffer overloaded" << endl; return false;}
	}
	bufferSize = buffer.length();
	return true;
}


/// @brief Get the succeeded block's number
/// @return sBlockNumber
int DelimBuffer :: getSBlockNumber() const
{
	return sBlockNumber;
}
	
/// @brief Get the preceded block's number
/// @return pBlockNumber
int DelimBuffer :: getPBlockNumber() const
{
	return pBlockNumber;
}


/// @brief Get the next field from readed file stream
bool DelimBuffer :: unpackField(string &aStr)
{
	aStr = ""; //clear aStr
	if (nextCharIndex > bufferSize) return false;
	
	int len = -1; // length of unpacking string
	int start = nextCharIndex; // first character to be unpacked
	
	// look for delimiter
	for (int i = start; i < bufferSize; i++)
	{
    
        if (buffer[i] == delim || buffer[i] == '\n'){
            len = i - start;
            break;
        }
	}
	
	// delimeter not found
	// last item
	if (len == -1)
	{
		aStr = buffer.substr(nextCharIndex, bufferSize - nextCharIndex);
		return false;
	}
	
	// retrieve the field
	nextCharIndex += len + 1;
	aStr = buffer.substr(start, len);
	return true;
}

/// @brief Increments the index so the Length is not included in the Zip Code
void DelimBuffer :: increment()
{
    nextCharIndex += 2;
}

/// @brief Get the current block's size
int DelimBuffer :: getNumRecs() const {return numRecs;}
/// @brief Set the current block's size to a certain value
void DelimBuffer :: setNumRecs(const int &val) {numRecs = val;}
/// @brief Get the current block's number
int DelimBuffer :: getBlockNumber() const {return blockNumber;}
/// @brief Set the current block's number to a certain value
void DelimBuffer :: setBlockNumber(const int &val) {blockNumber = val;}
