#include <iostream>
#include <fstream>
#include <vector>

#include <boost/lexical_cast.hpp>
#include <boost/foreach.hpp>

#include "src/merkle.h"
#include "tinyformat.h"

#define INVALID_INPUT -1
#define INVALID_FILE -2
#define MALLOC_ERROR -3

#define LEAF_SIZE 1000000 //1 MB
#define NUM_ELEMENTS_PF 1000

using namespace std;

int main(int argc, const char *argv[]) {

	using boost::lexical_cast;
	using boost::bad_lexical_cast;

	ifstream finput;
	ofstream foutput;
	string file_path;
	bool runout_memory = false;
	size_t num_segmts = 0;
	MERKLE<LEAF_SIZE> *setup_treePt = NULL;
	vector< BUFFER<LEAF_SIZE> > file_segmts;
	BUFFER<LEAF_SIZE> temp_buffer;

	//Open File
	//TODO input validation check
	/*
	 size_t  num_files;
	 vector<strings> file_paths;
	 try{
	 num_files = lexical_cast<unsigned int>(argv[1]);
	 }catch(bad_lexical_cast &){
	 return INVALID_INPUT;
	 }
	 for (size_t i = 0; i < num_files; i++){
	 file_paths.push_back(argv[2+i]);
	 }*/
	file_path = argv[1];
	try {
		finput.open(file_path.c_str(), ifstream::binary);
	} catch (ifstream::failure& err) {
		strprintf("Invalid input file path: %d",INVALID_FILE);
		exit(INVALID_FILE);
	}

	//Process the file with seek()
	//TODO concatenate the Merkle node digests (since the size of leaves is 2 large)
	while (finput) {
		finput.read(temp_buffer.charData(), LEAF_SIZE);
		try{
			file_segmts.push_back(temp_buffer);
		}catch(bad_alloc &e){
			runout_memory = true;
			break;
		}
	}

	if (!finput.eof() && !runout_memory) {
		//File size can't be divided by the LEAF_SIZE
		finput.read(temp_buffer.charData(), finput.gcount());
		for (size_t i = finput.gcount(); i < LEAF_SIZE; i++) {
			*(temp_buffer.charData() + i) = 0;
		}
		file_segmts.push_back(temp_buffer);
	}
	//TODO Fix other situations
	num_segmts = file_segmts.size();
	finput.close();

	strprintf("Init Message: %s","Building the Merkle tree...");
	try {
		setup_treePt = new MERKLE<LEAF_SIZE>(file_segmts);
	} catch (bad_alloc& err) {
		strprintf("Merkle construction error: %d",MALLOC_ERROR);
		exit(MALLOC_ERROR);
	}
	file_segmts.clear();

	for(size_t i = 0; i < num_segmts; i++){
		//TODO Write paths to the file
	}


	return 0;
}

