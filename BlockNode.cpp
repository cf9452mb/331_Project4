//-------------------------------------------------------------
// BlockNode.cpp
// Block node class implementation file
//
// Author: Jordan Hebler, Myint Aung, Justine Canlas
//-------------------------------------------------------------
// A block node contains the data in each block and had 2
// pointers which point to its adjacent blocks.
//-------------------------------------------------------------

/**
 * @file BlockNode.cpp
 * @brief A block node with pointers to both adjacent blocks
 *
 * @author Jordan Hebler
 * @author Myint Aung
 * @author Justine Canlas
 */

#include "BlockNode.h"

/// @brief Default constructor
template<class dataType>
BlockNode<dataType> :: BlockNode()
{
	blockNumber = 0;
	setSBlock(0);
	setPBlock(0);
}


/// @brief Constructor with block number
template<class dataType>
BlockNode<dataType> :: BlockNode(const int &bNum)
{
	blockNumber = bNum;
	setSBlock(0);
	setPBlock(0);
}

/// @brief Constructor
template<class dataType>
BlockNode<dataType> :: BlockNode(const int &bNum, const int &sBlock, const int &pBlock)
{
	blockNumber = bNum;
	setSBlock(sBlock);
	setPBlock(pBlock);
}


/// @brief Get block number
template<class dataType>
int BlockNode<dataType> :: getBlockNumber() const
{
	return blockNumber;
}


/// @brief Get i-th data
template<class dataType>
dataType BlockNode<dataType> :: getData(const int num) const
{
    dataType anEntry;
	if ((num < data.size()) && num >= 0)
	{
		anEntry = data[num];
	}
    return anEntry;
}


/// @brief Add new data entry
template<class dataType>
bool BlockNode<dataType> :: addData(const dataType &anEntry)
{
	int position;
	dataType curEntry;
	string key = anEntry.getKey();
	for (position = 0; position < data.size(); position++)
	{
		curEntry = data[position];
		string curKey = curEntry.getKey();
		if(str2int(curKey) > str2int(key)) break;
	}
	auto itPos = data.begin() + position;
	auto newIt = data.insert(itPos, anEntry);
	
	numberOfRecords = data.size();
	return true;
}


/// @brief Remove a data entry by position
template<class dataType>
bool BlockNode<dataType> :: removeData(const int &position)
{
	if (data.size() == 0) return false;
	auto itPos = data.begin() + position;
	auto newIt = data.erase(itPos);
	
	numberOfRecords = data.size();
	return true;
}


/// @brief Get block's size
template<class dataType>
int BlockNode<dataType> :: getDataSize() const
{
	int dataSize = 0;
	for (int i = 0; i < data.size(); i++)
	{
		dataType anEntry;
		anEntry = data[i];
		dataSize += anEntry.getWeight();
	}
	return dataSize;
}

		
/// @brief Get number of records
template<class dataType>
int BlockNode<dataType> :: getNumRecs() const
{
	return numberOfRecords;
}
	
		
/// @brief Get succeeded block
template<class dataType>
int BlockNode<dataType> :: getSBlockNumber() const
{
	return succeededBlock;
}


/// @brief Get preceded block
template<class dataType>
int BlockNode<dataType> :: getPBlockNumber() const
{
	return precededBlock;
}
	
		
/// @brief Set succeeded block
template<class dataType>
void BlockNode<dataType> :: setSBlock(const int &val)
{
	succeededBlock = val;
}
		
		
/// @brief Set preceded block
template<class dataType>
void BlockNode<dataType> :: setPBlock(const int &val)
{
	precededBlock = val;
}


/// @brief string to int converter
template<class dataType>
int BlockNode<dataType> :: str2int (const string &s) const
{
	int result = 0;
	for (int i = 0; i < s.length(); i++)
	{
		int num = s[i] - '0';
		result = result*10 + num;
	}
	return result;
}
