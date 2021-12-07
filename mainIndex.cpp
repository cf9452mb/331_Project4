//------------------------------------------------------------
// mainIndex.cpp
// Test program for a B+ Tree index using a blocked sequence
// file
//
// Author: Jordan Hebler
//------------------------------------------------------------
// This main program is the interface to use for testing the
// features from the B+ Tree class (BPTree). This includes
// searching for a record, sorting the records, and adding
// and/or deleting records from a block.
//------------------------------------------------------------

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
  string key = argv[2];
  
  BPTree node;
  node.createIndex(blockFile);
  node.writeIndex(blockFile);
    
  node.display(node.getRoot());
  node.lookUpKey(key);

   
  return 0;
}
