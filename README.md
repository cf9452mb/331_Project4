# 331_Project4

1. Purpose
- The generator program helps convert the comma separated file to a block sequence set file to use in the mainIndex program
    which then converts that into a blocked/index file.
- The mainIndex program can be used to search for the information of a specific zip code given by the user
- The mainIndex program can also be used to get the 4 extreme latitude/longitude locations for each state
- The mainIndex program can be used to add or remove data from the blocked sequence set file
- The mainIndex program can be used to show the logical or physical ordering of the blocks

2. File location
- All necessary CSV, C++ files, header files, and .txt files are compressed and stored in Project 4 - Team 1 - files.zip.
- Compile the file generator.cpp to convert the .csv file.
- Compile the file mainIndex.cpp to run program.

3. Execution
- To compile the generator program, use g++ generator.cpp -std=c++11
	then execute the generator by: ./a.out *csv-file-name* *blocked-sequence-set-file-name* *max-block-size*
    (Ex. ./a.out us_postal_codes.csv index.txt 512      Creates a blocked sequence set file named index.txt that is created from the us_postal_codes.csv file with a max block size of 512 bytes.)
- To compile the main program, use: g++ mainIndex.cpp -std=c++11
- After successfully compiling the main program, you can use ./a.out followed by the blocked sequence set file and a flag to execute the program and use the public methods in the BPTree class.
	* The file must be converted before by using generator.cpp
    * After running the program once, the blocked sequence file will be converted to a blocked/index file.
- Each flag indicates a different task (A different public method to use in the BPTree class):
    -t  : output the current B+ Tree containing the highest keys in each block
        (Ex. ./a.out index.txt -t   will generate the current B+ Tree)
	-s	: show a table of the 4 extreme latitude/longitude locations of each state.
        (Example shown below)
	-z00000	: find the information of a given zip code, replace 00000 by the desired zip code (no leading zero).
        (Example shown below)
	-a00000	additional-record-file: add data to the file. Here to simplify, records can be chosen from additiona-record-file
        to add. (Ex. If index.txt is the blocked/index file and toadd.txt is the additonal file of length-indicated records, then ./a.out index.txt -a1 toadd.txt    will add the record on the first line of toadd.txt)
	-d00000 : delete data from a file, replace 00000 by the zip code you want to delete (no leading zero). (Ex. If index.txt is the blocked/index file, then ./a.out index.txt -d56303    will delete the record with zip code 56303 if it exists)
    
    
- When adding or deleting, the change can be easily seen by displaying the B+ Tree conents using the -t flag

- NOTE* The Index Block size is set to 4 to easier output the B+ Tree on the screen. To modify the Index Block size to be the same as the size
    of the Record Blocks, go to line 25 in BPTree.cpp and change: int MAX = 4; -->  int MAX;    ALSO: uncomment line 278 in the same file.

- To show how to output the B+ Tree, we have used a smaller data set (short.csv) with a MAX IndexNode size of 4 since it easily fits on screen.
  After compiling the generator program and running it on short.csv (./a.out short.csv short.txt 512), we compile the mainIndex program and
  execute the command ./a.out short.txt -t
    Output is:
    1037 1084 1139
    1009 1027 1037
    1059 1072 1084
    1096 1108 1139
    1223 1230
    
    This correlates to the tree structure as follows:
    
                        _________1037 1084 1139_________
                       /            /      \            \
                      /            /        \            \
                     /            /          \            \
                    /            /            \            \
                   /            /              \            \
      1009 1027 1037    1059 1072 1084   1096 1108 1139    1223 1230
        
        
- The file storing the data for the next 2 examples: us_postal_codes.csv
