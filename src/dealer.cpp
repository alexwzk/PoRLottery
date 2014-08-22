#include "dealer.h"

DEALER::DEALER(std::string filenamePt) {
	using namespace std;
	ifstream inputs;
	try {
		inputs.open(filenamePt);
	} catch (ifstream::failure& err) {
		cerr << err.what() << " @ Opening file at Dealer 1st constructor."
				<< endl;
	}
	//Get the start and end of inputs
	//and repeatedly feed data to zk_leaves
	istream_iterator<zk_leaf> start(inputs), end;
	vector<zk_leaf> file_segmts(start, end);

	//Coutest size
	cout << "The file will be divided into " << file_segmts.size()
			<< " segments" << endl;
	//Coutest contents
	/*cout << "File contest read in: " << endl;
	 copy(file_segmts.begin(), file_segmts.end(),
	 std::ostream_iterator<zk_leaf>(std::cout, "\n"));*/

	mktreePt = new MERKLE(file_segmts);

}
int DEALER::createSubset() {
	if (!can_update) {
		return INVALID_ERR;
	}
	digest hashvalue;
	subset_pk.clear();
	// Hash(pk||i) for i: 0 -> l
	for (size_t i = 0; i < num_subset; i++) {
		pubkey.push_back(i); //TODO: size_t truncate into char, is it save?
		//Coutest pubkey
		std::cout << "The pk||i : " << pubkey << std::endl;
		SHA1((unsigned char *) pubkey.data(), pubkey.size(), hashvalue);
		//pick part of the hash value and mod by n
		//Coutest extractHash
		std::cout << "extract from hash : " << COMMON::extractHash(hashvalue)
				<< " then mod by " << num_all << " turns out "
				<< COMMON::extractHash(hashvalue) % num_all << std::endl;
		subset_pk.insert(COMMON::extractHash(hashvalue) % num_all); //TODO: Duplicates?
		pubkey.pop_back();
	}
	//Coutest subset_pk
	std::cout << "The size of subset_{pk}: " << subset_pk.size() << std::endl;
	for (auto i : subset_pk) {
		std::cout << i << " ";
	}
	std::cout << std::endl;

	can_update = false;
	return 0;
}

std::set<PATH> DEALER::createSource(std::string usr_pubkey) {
	if (pubkey.compare(usr_pubkey) != 0) {
		pubkey = usr_pubkey;
		can_update = true;
		createSubset();
		path_set.clear();
		for (auto it : subset_pk) {
			path_set.insert(mktreePt->buildPath(it));
		}
	}
	return path_set;
}
