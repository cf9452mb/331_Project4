//-------------------------------------------------------------
// BPTree.h
// Header file for a B+ Tree class
//
// Author: Jordan Hebler
//-------------------------------------------------------------
// This B+ Tree class can take a blocked sequence set file and
// create a B+ Tree index. This class can be used for searching
// and displaying Location objects, sorting Location objects,
// and adding/deleting records from the blocked/index file.
//-------------------------------------------------------------

/**
 * @file BPTree.h
 * @brief A B+ Tree class
 * @details This B+ Tree class can take a blocked sequence set file and create a B+ Tree index.
 *          This class can be used for searching and displaying Location objects, sorting Location
 *          objects, and adding/deleting records from the blocked/index file.
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

/// @brief Structure used to represent a zip code and RBN pair
struct Pair
{
   int zipCode, RBN;
};

/// @brief Index B+ Tree node
class IndexNode {
    private:
        /// Determines if this node is a leaf
        bool IS_LEAF;
        /// Size of the Node
        int size;
        /// The Node contains Pairs
        Pair *key;
        /// Pointers to other Nodes
        IndexNode **ptr;
        /// friends with the BPTree class
        friend class BPTree;

   public:
        /// @brief constructor
        IndexNode();
};

/// @brief B+ Tree Class
class BPTree {
    private:
        /// Root Node of the Tree
        IndexNode *root;
        /// @brief Clear function to reset the Tree
        void clear();
        /// @brief Helper function for internal insertion
        /// @param Pair, IndexNode *, IndexNode *
        /// @return The specified Pair and nodes are inserted
        void insertInternal(Pair, IndexNode *, IndexNode *);
        /// @brief The parent of the indexnode is found
        /// @param IndexNode *, IndexNode *
        /// @return The parent of the indexnode is returned
        IndexNode *findParent(IndexNode *, IndexNode *);
        /// @brief Writes the indexnodes to the blocked sequence file
        /// @param A string file
        /// @return The blocked sequence file with the indexnodes
        void writeIndex(string file);
        /// @brief Inserting the pair to the B+ Tree
        /// @param Pair To be inserted
        void insert(Pair);
        /// @brief Helper function to write the IndexNodes to the blocked sequence file
        /// @param IndexNode *, int &, int, int, ofstream &
        /// @post The indexnodes are written to the blocked sequence file
        void writeNodes(IndexNode *, int, int, int, ofstream &);
        /// @brief Find block that the key passed in may be contained in
        /// @param A int int
        /// @return The specified Pair that includes the Block
        Pair *findBlock(int);

   public:
        /// @brief constructor
        BPTree();
        /// @brief Accessor for the root node
        /// @return This function returns the root Node
        IndexNode *getRoot();
        /// @brief Create a B+ Tree index from the Blocked sequence set file
        /// @param A string file
        /// @post The B+Tree index is created
        void createIndex(string file);
        /// @brief A key is looked up in the blocked/index file and outputted if found
        /// @param string ZipCode to search for
        /// @post The record corresponding to the key is outputted if found
        void lookUpKey(string);
        /// @brief Sort records
        void sortRecords();
        /// @brief Outputs the B+ Tree
        void display(IndexNode *);
        /// @brief Add new record to the blocked/index file
        /// @pre Assume the record is compressed to a string with correct format
        /// @post The new record is added to the blocked/index file
        /// @param Location A record that is to be added to file
        void add(Location);
        /// @brief Delete a record from the blocked/index file
        /// @post The record is deleted from the file if found
        /// @param string The key of the record to be deleted
        void remove(string);
        
};

#include "BPTree.cpp"
#endif

