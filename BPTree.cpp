/**
 * @file BPTree.cpp
 * @brief A B+ Tree class
 *
 * @author Jordan Hebler
 */

#include "BPTree.h"

using namespace std;

//Max size of each Index Node
int MAX = 4;
//Blocked file used in creating a B+ Tree index
string blockFile;

IndexNode::IndexNode() {
    key = new Pair[MAX];
    ptr = new IndexNode *[MAX + 1];
}

BPTree::BPTree() {
  root = NULL;
}

///@brief Find block that the key passed in may be contained in
Pair *BPTree::findBlock(int key){
    Pair *search = new Pair;
    
    //Initialize default data if the key is not found
    search->zipCode = 0;
    search->RBN = 0;
    
    if (root == NULL) {
        return search;      // Tree is empty
    }
    else {
      IndexNode *cursor = root;
      while (cursor->IS_LEAF == false) {
        for (int i = 0; i < cursor->size; i++) {
          if ((key <= cursor->key[i].zipCode)) {
            cursor = cursor->ptr[i];
            break;
          }
          if (i == cursor->size - 1) {
            cursor = cursor->ptr[i + 1];
            break;
          }
        }
      }
      for (int i = 0; i < cursor->size; i++) {
        if (cursor->key[i].zipCode >= key) {
          search = &cursor->key[i];
          break;
        }
      }
    }
    return search;
}

///@brief Insert Operation
void BPTree::insert(Pair x) {
  // Inserting into an empty B+ Tree
  if (root == NULL) {
    root = new IndexNode;
    root->key[0] = x;
    root->IS_LEAF = true;
    root->size = 1;
  }
  // If the tree is non-empty
  else {
    IndexNode *cursor = root;
    IndexNode *parent;
    while (cursor->IS_LEAF == false) {
      parent = cursor;
      for (int i = 0; i < cursor->size; i++) {
        if (x.zipCode < cursor->key[i].zipCode) {
          cursor = cursor->ptr[i];
          break;
        }
        if (i == cursor->size - 1) {
          cursor = cursor->ptr[i + 1];
          break;
        }
      }
    }
    // If there is still room in the current IndexNode
    if (cursor->size < MAX) {
      int i = 0;
      while (x.zipCode > cursor->key[i].zipCode && i < cursor->size)
        i++;
      for (int j = cursor->size; j > i; j--) {
        cursor->key[j] = cursor->key[j - 1];
      }
      cursor->key[i] = x;
      cursor->size++;
      cursor->ptr[cursor->size] = cursor->ptr[cursor->size - 1];
      cursor->ptr[cursor->size - 1] = NULL;
    } else {
      IndexNode *newLeaf = new IndexNode;
      Pair virtualNode[MAX + 1];
      for (int i = 0; i < MAX; i++) {
        virtualNode[i] = cursor->key[i];
      }
      int i = 0, j;
      while (x.zipCode > virtualNode[i].zipCode && i < MAX)
        i++;
      for (int j = MAX + 1; j > i; j--) {
        virtualNode[j] = virtualNode[j - 1];
      }
      virtualNode[i] = x;
      newLeaf->IS_LEAF = true;
      cursor->size = MAX + 1 - (MAX + 1) / 2;
      newLeaf->size = (MAX + 1) / 2;
      cursor->ptr[cursor->size] = newLeaf;
      newLeaf->ptr[newLeaf->size] = cursor->ptr[MAX];
      cursor->ptr[MAX] = NULL;
      for (i = 0; i < cursor->size; i++) {
        cursor->key[i] = virtualNode[i];
      }
      for (i = 0, j = cursor->size; i < newLeaf->size; i++, j++) {
        newLeaf->key[i] = virtualNode[j];
      }
      if (cursor == root) {
        IndexNode *newRoot = new IndexNode;
        newRoot->key[0] = cursor->key[cursor->size - 1];
        newRoot->ptr[0] = cursor;
        newRoot->ptr[1] = newLeaf;
        newRoot->IS_LEAF = false;
        newRoot->size = 1;
        root = newRoot;
      } else {
        insertInternal(cursor->key[cursor->size - 1], parent, newLeaf);
      }
    }
  }
}

///@brief Helper function for internal insertion
void BPTree::insertInternal(Pair x, IndexNode *cursor, IndexNode *child) {
  if (cursor->size < MAX) {
    int i = 0;
    while (x.zipCode > cursor->key[i].zipCode && i < cursor->size)
      i++;
    for (int j = cursor->size; j > i; j--) {
      cursor->key[j] = cursor->key[j - 1];
    }
    for (int j = cursor->size + 1; j > i + 1; j--) {
      cursor->ptr[j] = cursor->ptr[j - 1];
    }
    cursor->key[i] = x;
    cursor->size++;
    cursor->ptr[i + 1] = child;
  } else {
    IndexNode *newInternal = new IndexNode;
    Pair virtualKey[MAX + 1];
    IndexNode *virtualPtr[MAX + 2];
    for (int i = 0; i < MAX; i++) {
      virtualKey[i] = cursor->key[i];
    }
    for (int i = 0; i < MAX + 1; i++) {
      virtualPtr[i] = cursor->ptr[i];
    }
    int i = 0, j;
    while (x.zipCode > virtualKey[i].zipCode && i < MAX)
      i++;
    for (int j = MAX + 1; j > i; j--) {
      virtualKey[j] = virtualKey[j - 1];
    }
    virtualKey[i] = x;
    for (int j = MAX + 2; j > i + 1; j--) {
      virtualPtr[j] = virtualPtr[j - 1];
    }
    virtualPtr[i + 1] = child;
    newInternal->IS_LEAF = false;
    cursor->size = (MAX + 1) / 2;
    newInternal->size = MAX - (MAX + 1) / 2;
    for (i = 0, j = cursor->size + 1; i < newInternal->size; i++, j++) {
      newInternal->key[i] = virtualKey[j];
    }
    for (i = 0, j = cursor->size + 1; i < newInternal->size + 1; i++, j++) {
      newInternal->ptr[i] = virtualPtr[j];
    }
    if (cursor == root) {
      IndexNode *newRoot = new IndexNode;
      newRoot->key[0] = cursor->key[cursor->size];
      newRoot->ptr[0] = cursor;
      newRoot->ptr[1] = newInternal;
      newRoot->IS_LEAF = false;
      newRoot->size = 1;
      root = newRoot;
    } else {
      insertInternal(cursor->key[cursor->size], findParent(root, cursor), newInternal);
    }
  }
}

///@brief Find the parent IndexNode
IndexNode *BPTree::findParent(IndexNode *cursor, IndexNode *child) {
  IndexNode *parent;
  if (cursor->IS_LEAF || (cursor->ptr[0])->IS_LEAF) {
    return NULL;
  }
  for (int i = 0; i < cursor->size + 1; i++) {
    if (cursor->ptr[i] == child) {
      parent = cursor;
      return parent;
    } else {
      parent = findParent(cursor->ptr[i], child);
      if (parent != NULL)
        return parent;
    }
  }
  return parent;
}

///@brief Print the tree
void BPTree::display(IndexNode *cursor) {
  if (cursor != NULL) {
    for (int i = 0; i < cursor->size; i++) {
      cout << cursor->key[i].zipCode << " ";
    }
    cout << "\n";
    if (cursor->IS_LEAF != true) {
      for (int i = 0; i < cursor->size + 1; i++) {
        display(cursor->ptr[i]);
      }
    }
  }
}

///@brief Accessor for the root node
IndexNode *BPTree::getRoot() {
  return root;
}

///@brief Create a B+ Tree index from the Blocked sequence set file
void BPTree::createIndex(string name) {
    ifstream file;
    file.open(name);
    Block<Location> aBlock(name);
    
    aBlock.readHeader(file);
    
    // Sets the max number of Pairs per node
    //MAX = (aBlock.maxDataSize/12);
    
    BlockNode<Location> curBlock = aBlock.readBlock(aBlock.headBlockNumber);
    
    while (curBlock.getSBlockNumber() != 0) {
        Location anEntry = curBlock.getData(curBlock.getNumRecs() - 1);
        Pair p;
        p.zipCode = aBlock.str2int(anEntry.getZipCode());
        p.RBN = curBlock.getBlockNumber();
        insert(p);
        
        curBlock = aBlock.readBlock(curBlock.getSBlockNumber());
    }
    
    Location anEntry = curBlock.getData(curBlock.getNumRecs() - 1);
    Pair p;
    p.zipCode = aBlock.str2int(anEntry.getZipCode());
    p.RBN = curBlock.getBlockNumber();
    insert(p);
    
    file.close();
    
}

///@brief Helper function to write the IndexNodes to the blocked sequence file
void BPTree::writeNodes(IndexNode *cursor, int &count, int blockSize, int headerSize, ofstream &out) {
  if (cursor != NULL) {
      out.seekp((blockSize * count) + headerSize - blockSize);
      out << 'B' << count << ',' << cursor->size << endl;
    for (int i = 0; i < cursor->size; i++) {
        out << cursor->key[i].zipCode << ',' << cursor->key[i].RBN << endl;
    }
    if (cursor->IS_LEAF != true) {
      for (int i = 0; i < cursor->size + 1; i++) {
        count++;
        writeNodes(cursor->ptr[i], count, blockSize, headerSize, out);
      }
    }
  }
}

///@brief Writes the IndexNodes to the blocked dequence file
void BPTree::writeIndex(string name) {
    ifstream ifile;
    string line;
    ifile.open(name);
    Block<Location> aBlock(name);
    
    aBlock.readHeader(ifile);
    ifile.seekg(0, ios::beg);
    
    ofstream outfile;
    outfile.open("temp.txt");
    
    while(getline(ifile,line)){
        outfile << line << "\n";
    }

    aBlock.blockCount++;
    outfile.seekp((aBlock.blockSize * aBlock.blockCount) + aBlock.headerSize - aBlock.blockSize);

    //Write the Index Nodes to the file
    writeNodes(getRoot(), aBlock.blockCount, aBlock.blockSize, aBlock.headerSize, outfile);
    
    ifile.close();
    outfile.close();
    
    rename("temp.txt", name.c_str());
    
}

///@brief Search for a key in the blocked/index file
bool BPTree::lookUpKey(string key) {
    
    
    ifstream ifile;
    ifile.open(blockFile);
    Block<Location> aBlock(blockFile);
    
    
    Pair *p = findBlock(aBlock.str2int(key));;
    
    //target = desired output
    cout << p->RBN << endl;
    BlockNode<Location> posBlock = aBlock.readBlock(p->RBN);
    
    if(posBlock.getBlockNumber() == 0)
        {cout << "Zip Code " << key << " is not found..." << endl; return false;}
    
    Location target;
    int i;
    for (i = 0; i < posBlock.getNumRecs(); i++)
    {
        target = posBlock.getData(i);
        if(target.getKey() == key) break;
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

