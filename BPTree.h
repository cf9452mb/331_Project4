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
	    ///leaf
        bool IS_LEAF;
		///size of the leaves
        int size;
		///pairings of the leaves
        Pair *key;
		/// pointer to the leaves
        IndexNode **ptr;
		///BPTree class
        friend class BPTree;

   public:
        IndexNode();
};

// B+ Tree Class
class BPTree {
    private:
		///root
        IndexNode *root;
		/// @brief Helper function for internal insertion
		/// @param Pair, IndexNode *, IndexNode *
		/// @return The specified Pair and nodes are inserted
        void insertInternal(Pair, IndexNode *, IndexNode *);
		/// @brief The parent of the indexnode is found
		/// @param IndexNode *, IndexNode *
		/// @return The parent of the indexnode is returned
        IndexNode *findParent(IndexNode *, IndexNode *);
        //void insert(Pair);
		///@brief Helper function to write the IndexNodes to the blocked sequence file
		///@param IndexNode *, int &, int, int, ofstream &
		/// @return The indexnodes are written to the blocked sequence file
        void writeNodes(IndexNode *, int &, int, int, ofstream &);
		/// @brief Find block that the key passed in may be contained in
		/// @param A int int
		/// @return The specified block
        Pair *findBlock(int);

   public:
		/// @brief default constructor for BPTree
        BPTree();
		/// @brief Create a B+ Tree index from the Blocked sequence set file
		/// @param A string file
		/// @return The B+Tree index is created
        void createIndex(string file);
		/// @brief Writes the indexnodes to the blocked sequence file
		/// @param A string file
		/// @return The blocked sequence file with the indexnodes
        void writeIndex(string file);
		/// @brief A key is looked up in the blocked/index file
		/// @param A string string
		/// @return A key is returned
        bool lookUpKey(string);
		/// @brief The function to print the tree
		/// @param IndexNode *
		/// @return The tree is printed
        void display(IndexNode *);
        IndexNode *getRoot();
		/// @brief Inserting the pair to the B+ Tree
		/// @param Pair 
		/// @return The specified pair is inserted to the B+ Tree
        void insert(Pair);
};

#include "BPTree.cpp"
#endif
