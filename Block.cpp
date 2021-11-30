//-------------------------------------------------------------
// Block.cpp
// Implementation file for a blocked sequence set file object class
//
// Author: Jordan Hebler, Myint Aung
//-------------------------------------------------------------
// A blocked sequence set file object can read data from a file,
// sort records, find a specific record, and add and/or delete
// records from a block.
//-------------------------------------------------------------

/**
 * @file Block.cpp
 * @brief A blocked sequence set file object
 *
 * @author Jordan Hebler
 * @author Myint Aung
 */

#include "Block.h"

/// @brief Default constructor
template<class dataType>
Block<dataType> :: Block(const string &filename)
{
	fileName = filename;
}


/// @brief Read block at a specific position
/// @param int Read a block at this specified position
template<class dataType>
BlockNode<dataType> Block<dataType> :: readBlock(const int &pos)
{	
	//the file must be blocked sequence set file
	ifstream infile;
	infile.open(fileName);
	
	//read headers
	readHeader(infile);
	
	//return null if pos > blockCount
	if (pos > blockCount || pos < 1)
	{
		BlockNode<dataType> nullBlock;
		return nullBlock;
	}
	
	//read in data
	DelimBuffer buffer;
	if(buffer.read(infile))
	{
		int curBlockCount = 1;
		while (curBlockCount <= blockCount)
		{
			if(buffer.getBlockNumber() == pos) break;
			buffer.read(infile);
			curBlockCount++;
		}
		if(buffer.getBlockNumber() == pos)
		{
			BlockNode<dataType> thisBlock(buffer.getBlockNumber(), buffer.getSBlockNumber(), buffer.getPBlockNumber());
			for(int i = 0; i < buffer.getNumRecs(); i++)
			{
				dataType newEntry;
				newEntry.unpack(buffer);
				thisBlock.addData(newEntry);
			}
			return thisBlock;
		}
	}
	
	infile.close();
	//return null if not found
	BlockNode<dataType> nullBlock;
	return nullBlock;		
}


/// @brief Count the number of nodes
template<class dataType>
int Block<dataType> :: getBlockCount() const
{
	return blockCount;
}

		
/// @brief Add new record to a the block
template<class dataType>
bool Block<dataType> :: addData(const dataType &record)
{
	int desiredBlockPos = findDesiredBlock(record.getKey());
	BlockNode<dataType> desiredBlock = readBlock(desiredBlockPos);
	BlockNode<dataType> sBlock = readBlock(desiredBlock.getSBlockNumber());
	
	if(desiredBlock.getDataSize() + record.getWeight() <= maxDataSize)
	{	//not overload
		desiredBlock.addData(record);
		stale = 0;
		recCount++;
		BlockNode<dataType> b(-1);
		updateBlockFile(desiredBlock, b, b);
	}
	else
	{	//overload
		//create a new block
		int newBlockNum = avail_list[0];
		auto newIt = avail_list.erase(avail_list.begin());
		
		if (avail_list.size() == 0)
		{
			blockCount++;
			avail_list.push_back(getBlockCount() + 1);
		}
		
		//relink them
		BlockNode<dataType> newBlock(newBlockNum, sBlock.getBlockNumber(), desiredBlock.getBlockNumber());		
		desiredBlock.setSBlock(newBlock.getBlockNumber());
		if (sBlock.getBlockNumber() != 0) sBlock.setPBlock(newBlockNum);
		newBlock.addData(record);

		//redistribute
		redistribution(desiredBlock, newBlock);
        cout << "Split Block " << desiredBlock.getBlockNumber() << " with Block " << newBlock.getBlockNumber() << endl;
		
		//update file
		stale = 0;
		recCount++;
		updateBlockFile(desiredBlock, newBlock, sBlock);
	}
	
	return true;
}
		
		
/// @brief Delete a record from the block
template<class dataType>
bool Block<dataType> :: removeData(const string &key)
{
	int desiredBlockPos = findDesiredBlock(key);
	if (desiredBlockPos < 0) return false;

	BlockNode<dataType> desiredBlock = readBlock(desiredBlockPos);
	int deletePos = 0;
	for(deletePos = 0; deletePos < desiredBlock.getNumRecs(); deletePos++)
	{	//search for data
		dataType anEntry;
		anEntry = desiredBlock.getData(deletePos);
		if (key == anEntry.getKey()) break;
	}
	if (deletePos == desiredBlock.getNumRecs()) return false;
	desiredBlock.removeData(deletePos);
	recCount--;
	stale = 0;

	if (desiredBlock.getNumRecs() == 0)
	{	//delete this block

		BlockNode<dataType> sBlock = readBlock(desiredBlock.getSBlockNumber());
		BlockNode<dataType> pBlock = readBlock(desiredBlock.getPBlockNumber());
		
		desiredBlock.setSBlock(0);
		desiredBlock.setPBlock(0);
		
		sBlock.setPBlock(pBlock.getBlockNumber());
		pBlock.setSBlock(sBlock.getBlockNumber());
		
		avail_list.push_back(desiredBlockPos);
		sort(avail_list.begin(), avail_list.end());
		
		updateBlockFile(pBlock, desiredBlock, sBlock);
	}

	else if (desiredBlock.getDataSize() < minDataSize && desiredBlock.getSBlockNumber() != 0)
	{	//redistribute if neccessary
		int sBlockNum = desiredBlock.getSBlockNumber();
		BlockNode<dataType> sBlock = readBlock(sBlockNum);
		if (sBlock.getDataSize() > minDataSize) redistribution(desiredBlock, sBlock);
        cout << "Redistribution on Block " << desiredBlock.getBlockNumber() << " and Block " << sBlock.getBlockNumber() << endl;
		BlockNode<dataType> b(-1);
		updateBlockFile(desiredBlock, sBlock, b);
	}
	
	else
	{	//no need to do anything
		BlockNode<dataType> b(-1);
		updateBlockFile(desiredBlock, b, b);
	}

	return true;
}


/// @brief Read header
template<class dataType>
void Block<dataType> :: readHeader(istream &infile)
{
	//retrive header
	for (int i = 0; i < 17; i++)
	{
		//read line by line
		string header;
		getline(infile, header);
				
		//check header's type
		if (i == 1) 	  {version = str2int(header) + 1;}
		else if (i == 5)  {maxDataSize = str2int(header);}
		else if (i == 6)  
		{
			float minData = str2int(header) / 100.0 * maxDataSize;
			minDataSize = (int)(minData);
		}
		else if (i == 7)  {indexFilename = header;}
		else if (i == 9)  {recCount = str2int(header);}
		else if (i == 10) {blockCount = str2int(header);}
		else if (i == 14)
		{
			int avail = 0;
			avail_list.clear();
			for (int i = 0; i < header.length(); i++)
			{
				if(header[i] == ' ')
				{
					avail_list.push_back(avail);
					avail = 0;
				}
				else
				{
					int num = header[i] - '0';
					avail = avail * 10 + num;
				}
			}
			avail_list.push_back(avail);
		}
		else if (i == 15) {headBlockNumber = str2int(header);}
		else if (i == 16) {stale = str2int(header);}
	}
}


/// @brief Write header
template<class dataType>
void Block<dataType> :: writeHeader(ostream &outfile)
{
	outfile << "blocked sequence set with comma separated fields, length-indicated records" << endl
		 	<< version << endl
		 	<< "1024" << endl
		 	<< "512" << endl
		 	<< "ASCII" << endl
		 	<< maxDataSize << endl
		 	<< "50" << endl
		  	<< indexFilename << endl
		    << "primaryIndex,blockNumber" << endl
		  	<< recCount << endl
		  	<< blockCount << endl
		  	<< "6" << endl
		  	<< "Zip Code,Place,State,County,Lat,Long" << endl
		  	<< "Zip Code" << endl
		  	<< avail_list[0];
	for (int i = 1; i < avail_list.size(); i++)
	{
		
		outfile << ' ' << avail_list[i];
	}
	outfile	<< endl
		  	<< headBlockNumber<< endl
		 	<< "1" << endl;
}


/// @brief Find fit block
template<class dataType>
int Block<dataType> :: findDesiredBlock(const string &key)
{
	//open file
	ifstream indexFile;
	indexFile.open(indexFilename);
	
	//get the desired block
	int desiredBlock = -1, i;
	string oldline, zip = "", blockStr = "";
	while (!indexFile.eof())
	{
		string line;
		getline(indexFile, line);
		if (line.length() == 0) continue;
		zip = ""; blockStr = "";
		for (i = 0; i < line.length(); i++)
		{
			if (line[i] == ',') break;
			else zip += line[i];
		}
		if (str2int(zip) >= str2int(key))
		{
			i++;
            for ( ; i < line.length(); i++) {
                blockStr += line[i];
            }
			desiredBlock = str2int(blockStr);
			return desiredBlock;
		}
		oldline = line;
	}
	
	for (i = 0; i < oldline.length(); i++)
	{
		if (oldline[i] == ',') break;
		else zip += oldline[i];
	}
	i++;
	for ( ; i < oldline.length(); i++) blockStr += oldline[i];
	desiredBlock = str2int(blockStr);
	return desiredBlock;
	
	//close and return
	indexFile.close();
	return desiredBlock;
}


/// @brief Write block information
template<class dataType>
void Block<dataType> :: writeBlockInfo(BlockNode<dataType> curBlock, ostream &outfile)
{
	outfile << 'B' << curBlock.getBlockNumber() << ',' << curBlock.getNumRecs() << ',';

	int sBlockNum = curBlock.getSBlockNumber();
	int pBlockNum = curBlock.getPBlockNumber();
	outfile << pBlockNum << ',' << sBlockNum << ',' << curBlock.getDataSize() << endl;
}

/// @brief Retrieve headBlockNumber
template<class dataType>
int Block<dataType> :: getHeadBlockNumber() const
{
	return headBlockNumber;
}

/// @brief Update the block file with new information
template<class dataType>
void Block<dataType> :: updateBlockFile(const BlockNode<dataType> &mainBlock, const BlockNode<dataType> &newBlock, const BlockNode<dataType> &sMainBlock)
{	
	ofstream outfile;
	outfile.open("out_temp.txt", ofstream::trunc);

	writeHeader(outfile);
	BlockNode<dataType> curBlock = readBlock(headBlockNumber);
	bool mainRead = false, newRead = false, sRead = false;
	if ((mainBlock.getBlockNumber() == newBlock.getBlockNumber()) || ((newBlock.getBlockNumber() == -1))) newRead = true;
	if ((mainBlock.getBlockNumber() == sMainBlock.getBlockNumber()) || ((sMainBlock.getBlockNumber() == -1))) sRead = true;

	while (curBlock.getBlockNumber() != 0)
	{
		if ((!mainRead) && (curBlock.getBlockNumber() == mainBlock.getBlockNumber()))
		{
			curBlock = mainBlock;
			mainRead = true;
		}
		
		writeBlockInfo(curBlock, outfile);
		for (int j = 0; j < curBlock.getNumRecs(); j++)
		{
			dataType anEntry;
			anEntry = curBlock.getData(j);
			outfile << anEntry;
		}
		
		if ((!newRead) && (curBlock.getBlockNumber() == mainBlock.getBlockNumber()))
		{
			curBlock = newBlock;
			newRead = true;
		}
		else if ((!sRead) && (curBlock.getBlockNumber() == newBlock.getBlockNumber()))
		{
			curBlock = sMainBlock;
			sRead = true;
		}
		else curBlock = readBlock(curBlock.getSBlockNumber());
	}
	
    //cout << avail_list[0] << endl;
	outfile.close();
	remove(fileName.c_str());
	rename("out_temp.txt", fileName.c_str());
	
	updateIndexFile();

}


/// @brief Update the index file
template<class dataType>
void Block<dataType> :: updateIndexFile()
{
	Block<dataType> newBlock(fileName);
	ifstream infile;
	infile.open(fileName.c_str());
	newBlock.readHeader(infile);
		
	ofstream indexFile;
	indexFile.open(indexFilename.c_str(), ofstream::trunc);
	
	BlockNode<dataType> curBlock = newBlock.readBlock(newBlock.getHeadBlockNumber());
	while (curBlock.getBlockNumber() != 0)
	{
		dataType anEntry = curBlock.getData(curBlock.getNumRecs() - 1);
		indexFile << anEntry.getKey() << ',' << curBlock.getBlockNumber() << endl;
		curBlock = newBlock.readBlock(curBlock.getSBlockNumber());
	}
	
	indexFile.close();
}

/// @brief Output to terminal the logical ordering of the blocks
template<class dataType>
void Block<dataType> :: logicalDump()
{
    Block<dataType> newBlock(fileName);
    ifstream infile;
    infile.open(fileName.c_str());
    newBlock.readHeader(infile);
    
    cout << "List Head: " << 'B' << headBlockNumber << endl;
    cout << "Avail Head: " << 'B' << avail_list[0] << endl;
    
    BlockNode<dataType> curBlock = newBlock.readBlock(newBlock.getHeadBlockNumber());
    
    while (curBlock.getBlockNumber() != 0)
    {
        cout << 'B' << curBlock.getPBlockNumber() << ' ';
        for (int i = 0; i < curBlock.getNumRecs(); i++)
        {
            dataType anEntry = curBlock.getData(i);
            cout << anEntry.getKey() << ' ';
        }
        cout << 'B' << curBlock.getSBlockNumber() << endl;
        curBlock = newBlock.readBlock(curBlock.getSBlockNumber());
    }
    
}
	
/// @brief Output to terminal the physical ordering of the blocks
template<class dataType>
void Block<dataType> :: physicalDump()
{
	Block<dataType> newBlock(fileName);
	ifstream infile;
	infile.open(fileName.c_str());
	newBlock.readHeader(infile);
    
    cout << "List Head: " << 'B' << headBlockNumber << endl;
    cout << "Avail Head: " << 'B' << avail_list[0] << endl;
	
	BlockNode<dataType> curBlock = newBlock.readBlock(newBlock.getHeadBlockNumber());
    
	int curBlockCount = 1;
	while (curBlockCount <= newBlock.getBlockCount())
	{
		cout << 'B' << curBlock.getPBlockNumber() << ' ';
		for (int i = 0; i < curBlock.getNumRecs(); i++) 
		{
			dataType anEntry = curBlock.getData(i);
			cout << anEntry.getKey() << ' ';
		}
		cout << 'B' << curBlock.getSBlockNumber() << endl;
		curBlockCount++;
		curBlock = newBlock.readBlock(curBlockCount);
	}
	
}


/// @brief Find records
template<class dataType>
bool Block<dataType> :: findRecord(const string &keyStr)
{
	//get the key zip code
	int key = 0;
	string keyS = "";
    for (int i = 2; i < keyStr.length(); i++){
        int num = keyStr[i] - '0';
        key = key * 10 + num;
        keyS += keyStr[i];
    }
	
	//check if the record is found or not
	int pos = findDesiredBlock(keyS);
    
	if(pos < 0) 
		{cout << "Zip Code " << key << " is not found..." << endl; return false;}
	
	//target = desired output
	BlockNode<dataType> posBlock = readBlock(pos);
	dataType target;
	int i;
	for (i = 0; i < posBlock.getNumRecs(); i++)
	{
		target = posBlock.getData(i);
		if(target.getKey() == keyS) break;
	}
	
	//print result if found
	if (i == posBlock.getNumRecs())
		{cout << "Zip Code " << key << " is not found..." << endl; return false;}
	else
	{
		cout << "+---------------------------------------------------------------------------------------+" << endl
			 << '|' << setw(7) << "Zipcode"
			 << '|' << setw(5) << "State"
			 << '|' << setw(20) << "County"
			 << '|' << setw(30) << "Name"
			 << '|' << setw(10) << "Latitude"
			 << '|' << setw(10) << "Longitude"
			 << '|' << endl
			 << "+---------------------------------------------------------------------------------------+" << endl
			 << '|' << setw(7) << target.getZipCode()
			 << '|' << setw(5) << target.getState()
			 << '|' << setw(20) << target.getCounty()
			 << '|' << setw(30) << target.getPlace()
			 << '|' << setw(10) << target.getLat()
			 << '|' << setw(10) << target.getLong()
			 << '|' << endl
			 << "+---------------------------------------------------------------------------------------+" << endl;
		return true;
	}
}


/// @brief Sort records
template<class dataType>
void Block<dataType> :: sortRecords()
{
	//create a vector to store data
    vector<dataType> dataStore;
    
    
	//read in data and store to vector
	BlockNode<dataType> curBlock = readBlock(headBlockNumber);
   
	while (curBlock.getBlockNumber() != 0)
	{
		for (int k = 0; k < curBlock.getNumRecs(); k++)
		{
			dataType anEntry;
			anEntry = curBlock.getData(k);
			dataStore.push_back(anEntry);
		}
		curBlock = readBlock(curBlock.getSBlockNumber());
	}

	//sort by state
	sort(dataStore.begin(), dataStore.end());
	
	int size = dataStore.size();
	
	// print header
	cout << "+---------------------------------------------------------------------+" << endl;
	cout << '|' << setw(5) << "State"
		 << '|' << setw(15) << "Westernmost"
		 << '|' << setw(15) << "Easternmost"
		 << '|' << setw(15) << "Northernmost"
		 << '|' << setw(15) << "Southernmost" << '|' << endl;	
	cout << "+---------------------------------------------------------------------+" << endl;
	
	// find westernmost, easternmost, northernmost, southernmost
	int currentIndex = 0;
	while (currentIndex < size)
	{
		// get the index of the current location as westernmost, easternmost, northernmost, southernmost
		string currentState = dataStore[currentIndex].getState();
		int   w_most = currentIndex,
			  e_most = currentIndex,
			  s_most = currentIndex,
			  n_most = currentIndex;
		
		// move on the next state
		currentIndex++;
		while (currentState == dataStore[currentIndex].getState())
		{
			// while still in the current state, check westernmost, easternmost, northernmost, southernmost
			if (dataStore[currentIndex].getLong() > dataStore[w_most].getLong()) w_most = currentIndex;
			if (dataStore[currentIndex].getLong() < dataStore[e_most].getLong()) e_most = currentIndex;
			if (dataStore[currentIndex].getLat() > dataStore[n_most].getLat())   n_most = currentIndex;
			if (dataStore[currentIndex].getLat() < dataStore[s_most].getLat())   s_most = currentIndex;
			// move on the next state
			currentIndex++;
			// if reach the end, exit
			if (currentIndex == size) break;
		}
		// print result
		cout << '|' << setw(5) << dataStore[currentIndex - 1].getState() 
			 << '|' << setw(15) << dataStore[w_most].getZipCode() 
			 << '|' << setw(15) << dataStore[e_most].getZipCode() 
			 << '|' << setw(15) << dataStore[n_most].getZipCode() 
			 << '|' << setw(15) << dataStore[s_most].getZipCode()  << '|' << endl;
	}
	cout << "+---------------------------------------------------------------------+" << endl;
}


/// @brief string to int converter
template<class dataType>
int Block<dataType> :: str2int (const string &s) const
{
	int result = 0;
	for (int i = 0; i < s.length(); i++)
	{
		int num = s[i] - '0';
		result = result*10 + num;
	}
	return result;
}


/// @brief Merge 2 block to prevent underflow
template<class dataType>
void Block<dataType> :: redistribution(BlockNode<dataType> &block1, BlockNode<dataType> &block2)
{
	BlockNode<dataType> tempBlock;
	int b1 = block1.getNumRecs();
	int b2 = block2.getNumRecs();
	for (int i = 0; i < b1; i++)
	{
		dataType temp = block1.getData(0);
		tempBlock.addData(temp);
		block1.removeData(0);
	}
	
	for (int i = 0; i < b2; i++)
	{
		dataType temp = block2.getData(0);
		tempBlock.addData(temp);
		block2.removeData(0);
	}

	int count = 0;
	for ( ; count < (tempBlock.getNumRecs()/2 + 1); count++)    // Fills first block with (1/2)records + 1 to ensure 50% capacity
		{block1.addData(tempBlock.getData(count));}
	for ( ; count < tempBlock.getNumRecs(); count++)
		{block2.addData(tempBlock.getData(count));}
}
