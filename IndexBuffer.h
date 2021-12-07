/**
 * @file Indexbuffer.h
 * @brief Buffer class header file
 * @details This is the buffer class, IndexBuffer.h. This class reads in each field from the provided file or istream
 *          This class reads in each block header and its corresponding indexes
 * @author Justine Canlas
 * @author Jordan Hebler
 * @author Myint Aung
 */

#ifndef INDEX_BUFFER_H
#define INDEX_BUFFER_H
#define UINT_MAX2   4294967295

#include <iostream>
#include <string>
#include <vector>
#include <sstream>

using namespace std;

class IndexBuffer
{
	public:
		/// @brief Default Constructor
		IndexBuffer(char ind = ',');

		/// @brief Read from the file stream block by block
		/// @param infile file stream is passed in
		/// @pre infile must be declared first in the program, the header must be eliminated.
		/// @post Read the data in infile to buffer
		/// @return Returns true if the stream is opened and the buffer is not overflow, false otherwise.
		bool read(istream& infile);
		
		/// @brief Get the next field from readed file stream
		/// @param A string& aStr
		/// @post Pass the next field in buffer to aStr
		/// @return Returns true if the next field is readed, false otherwise.
		bool unpackField(string &aStr);
		
		/// @brief Get the current block's size
		/// @return blockSize
		int getNumIndexes() const;
		
		/// @brief Get the current block's number
		/// @return blockNumber
		int getBlockNumber() const;
		
		/// @brief Get the succeeded block's number
		/// @return sBlockNumber
		int getSBlockNumber() const;
		
		/// @brief Get the preceded block's number
		/// @return pBlockNumber
		int getPBlockNumber() const;
    
        /// @brief Increment the nextChar index
        void increment();

	protected:
		/// @brief Clear the buffer
		/// @param No parameters passed
		/// @post bufferSize and nextCharIndex are set to 0
		void clear();

	private:
		/// Index
		char index = ',';
		/// The buffer
		string buffer; 
		/// Size of the buffer string
		unsigned long bufferSize = 0; 
		/// Largest buffer size, default = size of string
		unsigned int maxByte = UINT_MAX2;
		/// Index of next char
		int nextCharIndex = 0;
		/// Number of indexes
		int numindexes = 0;
		/// Block number
		int blockNumber = 0;
		/// Succeeded block number
		int sBlockNumber = 0;
		/// Succeeded block number
		int pBlockNumber = 0;
		/// @brief Set the current block's size to a certain value
		void setNumIndexes(const int &val);
		/// @brief Set the current block's number to a certain value
		void setBlockNumber(const int &val);
};

#include "IndexBuffer.cpp"
#endif
