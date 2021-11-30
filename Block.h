//-------------------------------------------------------------
// Block.h
// Header file for a blocked sequence set file object class
//
// Author: Jordan Hebler, Myint Aung
//-------------------------------------------------------------
// A blocked sequence set file object can read data from a file,
// sort records, find a specific record, and add and/or delete
// records from a block.
//-------------------------------------------------------------

/**
 * @file Block.h
 * @brief A blocked sequence set file object
 * @details A blocked sequence set file object can read data from a file, sort records,
 *          find a specific record, and add and/or delete records from a block
 *
 * @author Jordan Hebler
 * @author Myint Aung
 */

#ifndef BLOCK_H
#define BLOCK_H

#include "BlockNode.h"
#include "DelimBuffer.h"
#include <vector>
#include <fstream>
#include <iomanip>
#include <algorithm>

template<class dataType>
class Block
{
	private:
		/// Block count
		int blockCount = 0;
		/// Records count
		int recCount = 0;
		/// Avail list
		vector<int> avail_list;
		/// Current active ptr
		int headBlockNumber;
		/// File name
		string fileName;
		/// Index file name;
		string indexFilename;
		/// Version
		int version;
		/// Stale flag
		int stale;
		
		/// @brief Read header
		/// @param infile A file to read
		void readHeader(istream &infile);
		
		/// @brief Write header
		/// @param outfile A file to write
		void writeHeader(ostream &outfile);
		
		/// @brief Write block information
		/// @param outfile A file to write
		void writeBlockInfo(BlockNode<dataType> curBlock, ostream &outfile);
		
		/// @brief Retrieve headBlockNumber
		/// @return headBlockNumber
		int getHeadBlockNumber() const;
		
		/// @brief Find fit block
		/// @param key A string
		/// @return The desired block for the key
		int findDesiredBlock(const string &key);
			
		/// @brief Update the index file
		void updateIndexFile();
		
		/// @brief Merge 2 block to prevent underflow
		/// @param block1 - The main block
		/// @param block2 - The succeeded block of the main block
		void redistribution(BlockNode<dataType> &block1, BlockNode<dataType> &block2);	
		
	public:
		/// Max block size
		int maxDataSize = 512;
		
		/// Min block size
		int minDataSize = 256;
		
		/// @brief Default constructor
		Block(const string &filename);
		
		/// @brief Read block at a position
		/// @param The block need to be read position
		BlockNode<dataType> readBlock(const int &pos);
		
		/// @brief Read block from file
		/// @pre The file must be in blocked sequence set file format
		void readFile(const string &filename);
		
		/// @brief Count the number of nodes
		/// @return The number of nodes
		int getBlockCount() const;
		
		/// @brief Add new record to a the block
		/// @pre Assume the record is compressed to a string with a correct format
		/// @post The new record is added to the file
		/// @param A record compressed as a string
		/// @return True if added successfully, False otherwise
		bool addData(const dataType &record);
		
		/// @brief Delete a record from the block
		/// @post The record is deleted from the file
		/// @param The key of the record needs to be deleted
		/// @return True if deleted successfully, False otherwise
		bool removeData(const string &key);
		
		/// @brief string to int converter
		/// @return int value of the string s
		int str2int (const string &s) const;

		/// @brief Update the block file with new information
		/// @param outfile Outstream
		void updateBlockFile(const BlockNode<dataType> &mainBlock, const BlockNode<dataType> &newBlock, const BlockNode<dataType> &sMainBlock);
		
		/// @brief Sort records
		void sortRecords();
		
		/// @brief Search for a record
		bool findRecord(const string &keyStr);
    
        /// @brief Dump method showing logical ordering
        void logicalDump();
    
        /// @brief Dump method showing physical ordering
        void physicalDump();
};

#include "Block.cpp"
#endif
