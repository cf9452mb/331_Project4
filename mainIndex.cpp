//------------------------------------------------------------
// mainIndex.cpp
// Test program for a B+ Tree index
//
// Author: Jordan Hebler
//------------------------------------------------------------
// This main program is the interface to use for testing the
// features from the B+ Tree class (BPTree). This includes
// searching for a record, sorting the records, and adding
// and/or deleting records from a blocked/index file.
//------------------------------------------------------------

/**
 * @file mainIndex.cpp
 * @brief The test program for a B+ Tree that uses a blocked/index file
 * @details This main program is the interface to use for testing the features from the
 *          B+ Tree class (BPTree). This includes searching for a record, sorting the
 *          records, and adding and/or deleting records from a blocked/index file.
 *
 * @author Jordan Hebler
 */

#include "BPTree.h"

using namespace std;

int main(int argc, const char* argv[])
{
  
    if(argc < 3) {
        cout << "Invalid Number of arguments!" << endl;
        exit(1);
    }
    
    //get arguments
    blockFile = argv[1];
    string command = argv[2];
    
    BPTree node;
    node.createIndex(blockFile);
    
    //commands
    if (command[0] == '-')
    {
        if(command[1] == 'z')             node.lookUpKey(argv[2]);
        else if (command[1] == 's')        node.sortRecords();
        else if (command[1] == 't')        node.display(node.getRoot());
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
            node.add(loc);
        }
        else if (command[1] == 'd')
        {
            string key = "";
            for (int i = 2; i < command.length(); i++) {
                key += argv[2][i];
            }
            node.remove(key);
        }
    }
    else
    {
        cout << "Invalid command..." << endl;
        return 0;
    }
     
  return 0;
}
