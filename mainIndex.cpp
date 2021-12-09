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
    
  //Get arguments
  blockFile = argv[1];
  string key = argv[2];
  
  //Calls functions from the BPTree class and creates node as an object
  BPTree node;
  //Calls createIndex from BPTree class
  node.createIndex(blockFile);
  //Calls writeIndex from BPTree class
  node.writeIndex(blockFile);
  
  //Calls display from BPTree class
  node.display(node.getRoot());
  //Calls lookUpKey from BPTree class
  node.lookUpKey(key);

   
  return 0;
}
