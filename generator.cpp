//------------------------------------------------------------
// generator.cpp
// Application program to generate a blocked sequence set file
//
// Author: Jordan Hebler
//------------------------------------------------------------
// This program converts a .csv file to a .txt file in blocked
// sequence set format with a header record, comma separated
// fields, and records are length-indicated.
//------------------------------------------------------------

/**
 * @file generator.cpp
 * @brief Application program to generate a blocked sequence set file
 * @details This program converts a .csv file to a .txt file in blocked sequence set
            format with comma separated fields. Records are length-indicated
 *
 * @author Jordan Hebler
 */

#include <fstream>
#include <iostream>
#include <sstream>
#include <stdio.h>

using namespace std;

int main(int argc, const char* argv[])
{
	//open the old file and create a new file
	string oldfile = argv[1];
	string newfile = argv[2];
    	string indexFile = "simpleIndex.txt";
	char blockFlag = 'B';
	int maxDataSize, blockSize, counter = 0, headerSize = 256;
	stringstream s;
	s << argv[3];
    	s >> blockSize;
    	maxDataSize = blockSize - 35;     //Ensure enough room for block header and end line markers for records
	
	//open the old data file.
	ifstream infile;
	infile.open(oldfile);
	if (!infile) {cout << "There is no file in the storage..." << endl; return 0;}
	
	//open a new block sequence set file
	ofstream outfile;
	outfile.open("temp.txt", ofstream::trunc);
    
    	//open a new simple index file
    	ofstream simpleIndex;
    	simpleIndex.open(indexFile, ofstream::trunc);
	
	//initial necessary variable
	int blockRec = 0, totalRec = 0, blockCount = 1;
    	string data = "", lastZip = "";
	bool isFirstZip = 1;
	
	//start generating
	int curBlockSize = 0, precededBlock, succeededBlock;
	string oldline;
	int previousBlockRec = 0;
	
    	string line;
	//create temp.txt and dump.txt
    	while (getline(infile, line))
	{
        counter++;
        if(counter > 3){
            //determine the block it belongs to
            curBlockSize += line.length() + 2;
            if (curBlockSize <= maxDataSize)
            {
                stringstream l;
                l << line.length() + 2;
                //if it belongs to the current block, add to the block
                data += l.str() + line + '\n';
                blockRec++; totalRec++;
            }
		
            else
            {
                //get the last zipcode for the simple index file
                int i = 0;
                while (oldline[i] != ',') {lastZip += oldline[i]; i++;}
			
                //block information
                precededBlock = blockCount - 1;
                succeededBlock = blockCount + 1;
                curBlockSize -= (line.length() + 2);
                outfile.seekp((blockSize * blockCount) - blockSize);
                outfile << blockFlag << blockCount << ',' <<  blockRec << ','  << precededBlock << ',' << succeededBlock << ','
					<< curBlockSize << endl << data;
                
                //create the simple index file
                simpleIndex << lastZip << ',' << blockCount << endl;
			
                blockCount++;
                
                stringstream l;
                l << line.length() + 2;
                //give back the initial data
                previousBlockRec = blockRec;
                blockRec = 1; totalRec++;
                curBlockSize = line.length() + 2;
                lastZip = ""; data = l.str() + line + '\n';
            }
		
            //store the old line
            oldline = line;
        }
	}
    
    int i = 0;
    while (oldline[i] != ',') {lastZip += oldline[i]; i++;}
    
	//write the remaining data to the file
	precededBlock = blockCount - 1;
    	outfile.seekp((blockSize * blockCount) - blockSize);
	outfile << blockFlag << blockCount << ',' <<  blockRec << ','  << precededBlock << ',' << 0 << ',' 
					<< curBlockSize << endl << data;
    	simpleIndex << lastZip << ',' << blockCount << endl;
    
	int availBlock = blockCount + 1;
	
	//close all files
	outfile.close();
	infile.close();
    	simpleIndex.close();
	
	//copy to a new file with header
	ifstream feedFile;
	feedFile.open("temp.txt");
	//file header
	ofstream file;
	file.open(newfile);
	
	file << "blocked sequence set with comma separated fields, length-indicated records" << endl
		 << "1" << endl
		 << headerSize << endl
		 << blockSize << endl
		 << "ASCII" << endl
		 << maxDataSize << endl
		 << "50" << endl
		 << indexFile << endl
		 << "primaryIndex,blockNumber" << endl
		 << totalRec << endl
		 << blockCount << endl
		 << '6' << endl
		 << "Zip Code,Place,State,County,Lat,Long" << endl
		 << "Zip Code" << endl
		 << availBlock << endl
		 << '1' << endl         //Head block number
         	 << blockCount << endl  //Tail block number
		 << '1' << endl;
	
    	file.seekp(headerSize);
	//data
	int curBlock = 1;
	while (!feedFile.eof())
	{
		string line;
		getline(feedFile, line);
		if (line.length() == 0) continue;
		file << line << endl;
	}
	
	feedFile.close();
	//remove temp.txt
	remove("temp.txt");
}
