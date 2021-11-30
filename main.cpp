//------------------------------------------------------------
// main.cpp
// Test program for a Blocked sequence set file
//
// Author: Jordan Hebler, Justine Canlas
//------------------------------------------------------------
// This main program is the interface to use for testing the
// features from the Block class. This includes searching for
// a record, sorting the records, and adding and/or deleting
// records from a block.
//------------------------------------------------------------

/**
 * @file main.cpp
 * @brief The test program for a Blocked sequence set file
 * @details This main program is the interface to use for testing the features from the
 *          Block class. This includes searching for a record, sorting the records, and
 *          adding and/or deleting records from a block.
 *
 * @author Jordan Hebler
 * @author Justine Canlas
 */

#include "Block.h"
#include "BlockNode.h"
#include "Location.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

int main(int argc, const char* argv[])
{
	//get arguments
	string fileName = argv[1];
	Block<Location> aBlock(fileName);
	BlockNode<Location> aNode = aBlock.readBlock(1);
	string command = argv[2];
	
	//commands
	if (command[0] == '-')
	{
		if(command[1] == 'z') 			aBlock.findRecord(argv[2]);
		else if (command[1] == 's')		aBlock.sortRecords();
		else if (command[1] == 'a')
		{
			string fileAdd = argv[3];
			int position = 0;
			for (int i = 2; i < command.length(); i++)
			{
				int num = command[i] - '0';
				position = position * 10 + num;
			}
			ifstream infile;
			infile.open(fileAdd);
			string dataLine;
			for (int i = 0; i < position; i++)
			{
				getline(infile, dataLine);
			}
			Location loc(dataLine);
			aBlock.addData(loc);
		}
		else if (command[1] == 'd')
		{
			string key = "";
			for (int i = 2; i < command.length(); i++) key += argv[2][i];
			if (!aBlock.removeData(key)) cout << "Invalid key... " << key << endl;
		}
        	else if (command[1] == 'l')
        	{
            		aBlock.logicalDump();
        	}
        	else if (command[1] == 'p')
        	{
            		aBlock.physicalDump();
        	}
		else
		{ 
			cout << "Invalid command..." << endl;
			return 0;
		}
	}
	else
	{ 
		cout << "Invalid command..." << endl;
		return 0;	
	}
}
