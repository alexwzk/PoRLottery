#include <iostream>
#include <fstream>
#include <vector>

#include <boost/lexical_cast.hpp>
#include <boost/foreach.hpp>

#include "src/merkle.h"
#include "tinyformat.h"
#include "clientversion.h"
#include "streams.h"

#define FINE 0
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
	string file_path;
	bool runout_memory = false;
	size_t num_segmts = 0, size_segmts = 0;
	MERKLE<LEAF_SIZE> *setup_treePt = NULL;
	vector<BUFFER<LEAF_SIZE> > file_segmts;
	BUFFER<LEAF_SIZE> temp_buffer;

	printf("Init Message: %s \n", "Opening a SINGLE file...");
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
		printf("Invalid input file path: %d", INVALID_FILE);
		exit(INVALID_FILE);
	}

	printf("Init Message: %s \n", "Reading from the open file...");
	//TODO concatenate the Merkle node digests (since the size of leaves is 2 large)
	while (finput) {
		finput.read((char *) temp_buffer.begin(), LEAF_SIZE);
		try {
			file_segmts.push_back(temp_buffer);
		} catch (bad_alloc &e) {
			runout_memory = true;
			break;
		}
	}

	if (!finput.eof() && !runout_memory) {
		//File size can't be divided by the LEAF_SIZE
		finput.read((char *) temp_buffer.begin(), finput.gcount());
		for (size_t i = finput.gcount(); i < LEAF_SIZE; i++) {
			*(temp_buffer.begin() + i) = 0;
		}
		file_segmts.push_back(temp_buffer);
	}
	//TODO Fix other input branches
	finput.close();

	printf(
			"Init Message: \n reach the end of file? %d \n run out of memory? %d \n %s...\n",
			finput.eof(), runout_memory, "Building the Merkle tree");
	try {
		setup_treePt = new MERKLE<LEAF_SIZE>(file_segmts);
	} catch (bad_alloc& err) {
		printf("Merkle construction error: %d", MALLOC_ERROR);
		exit(MALLOC_ERROR);
	}
	num_segmts = setup_treePt->returnNumLeaves();
	//TODO Size doesn't match
	size_segmts = setup_treePt->returnPath(0).GetSerializeSize(SER_DISK,
			CLIENT_VERSION);
	printf("Init Message: the size of file_segmets is  %lu ...\n", num_segmts);
	printf("Init Message: the length of each segment is  %lu ...\n",
			size_segmts);
	file_segmts.clear();

	printf("Init Message: %s...\n", "Writing Merkle paths to the file");
	//TODO calculate and name the file in sequence say: 1.out 2... etc
	CAutoFile foutput(fopen("/scratch1/permacoin-nopor-test/1.out", "wb"), SER_DISK,
			CLIENT_VERSION);

	vector<PATH<LEAF_SIZE> > test_path;
	test_path.reserve(num_segmts);
	for (size_t i = 0; i < num_segmts; i++) {
		setup_treePt->returnPath(i).Serialize(foutput, SER_DISK,
				CLIENT_VERSION);
		setup_treePt->returnPath(i).Serialize(cout, SER_DISK,
				CLIENT_VERSION);
	}
	printf("Init Message: %s...\n Merkle root is: %s \n", "File output done!",
			setup_treePt->returnRoot().GetHex().c_str());
	return FINE;

	return 0;
}

