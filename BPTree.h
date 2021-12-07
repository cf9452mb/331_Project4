/**
 * @file BPTree.h
 * @brief A B+ Tree class
 * @details This B+ Tree class can take a blocked sequence set file and create a B+ Tree index. This index can be used
 *          for searching and displaying Location objects.
 *
 * @author Jordan Hebler
 */

#ifndef BPTREE_H
#define BPTREE_H

#include <climits>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include "Block.h"
#include "Location.h"


using namespace std;

// Structure used to represent a zip code and RBN pair
struct Pair
{
   int zipCode, RBN;
};

//Index B+ Tree node
class IndexNode {
    private:
        bool IS_LEAF;
        int size;
        Pair *key;
        IndexNode **ptr;
        friend class BPTree;

   public:
        IndexNode();
};

// B+ Tree Class
class BPTree {
    private:
        IndexNode *root;
        void insertInternal(Pair, IndexNode *, IndexNode *);
        IndexNode *findParent(IndexNode *, IndexNode *);
        //void insert(Pair);
        void writeNodes(IndexNode *, int &, int, int, ofstream &);
        Pair *findBlock(int);

   public:
        BPTree();
        void createIndex(string file);
        void writeIndex(string file);
        bool lookUpKey(string);
        void display(IndexNode *);
        IndexNode *getRoot();
        void insert(Pair);
};

#include "BPTree.cpp"
#endif
