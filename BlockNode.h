//-------------------------------------------------------------
// BlockNode.h
// Block node class header file
//
// Author: Jordan Hebler, Myint Aung, Justine Canlas
//-------------------------------------------------------------
// A block node contains the data in each block and had 2
// pointers which point to its adjacent blocks.
//-------------------------------------------------------------

/**
 * @file BlockNode.h
 * @brief A block node with pointers to both adjacent blocks
 * @details A block node contains the data in each block and has 2 pointers which
 *          point to its adjacent blocks
 *
 * @author Jordan Hebler
 * @author Myint Aung
 * @author Justine Canlas
 */

#ifndef BLOCK_NODE_H
#define BLOCK_NODE_H

#include <string>
#include <vector>
#include <iostream>
using namespace std;

template<class dataType>
class BlockNode
{
	private:
		/// String stores data
		vector<dataType> data;
		/// Block number
		int blockNumber = 0;
		/// Number of records in this block
		int numberOfRecords = 0;
		/// Succeeded block
		int succeededBlock = 0;
		/// Preceded block
		int precededBlock = 0;
		/// @brief string to int converter
		/// @return int value of the string s
		int str2int (const string &s) const;
		
	public:		
		/// @brief Default constructor
		BlockNode();
		
		/// @brief Constructor with block number
		BlockNode(const int &bNum);
		
		/// @brief Constructor
		BlockNode(const int &bNum, const int &sBlock, const int &pBlock);
				
		/// @brief Get block number
		/// @return Block number
		int getBlockNumber() const;
		
		/// @brief Get block's size
		/// @return An int of block size
		int getDataSize() const;
		
		/// @brief Get number of records
		/// @return An int of number of records
		int getNumRecs() const;
		
		/// @brief Get i-th data
		/// @return A record
		dataType getData(const int num) const;

		/// @brief Add new data entry
		/// @param anEntry An data entry
		bool addData(const dataType &anEntry);
		
		/// @brief Remove a data entry by position
		/// @param position The position need to be removed
		bool removeData(const int &position);
		
		/// @brief Get succeeded block
		/// @return A block
		int getSBlockNumber() const;
		
		/// @brief Get preceded block
		/// @return A block
		int getPBlockNumber() const;
		
		/// @brief Set succeeded block
		/// @param A block
		void setSBlock(const int &val);
		
		/// @brief Set preceded block
		/// @param A block
		void setPBlock(const int &val);
};

#include "BlockNode.cpp"
#endif
