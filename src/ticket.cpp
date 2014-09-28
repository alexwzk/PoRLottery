/*
 * ticket.cpp
 *
 *  Created on: Sep 27, 2014
 *      Author: zkwen
 */

#include "ticket.h"

uchar* TICKET::hashOfTicket() {
	uchar* hashvaluePt = new digest;
	size_t length_count = 0;
	length_count += this->pubkey.size();
	length_count += this->seed.size();
	size_t size_path = LEAF_SIZE
			+ mkproofs.at(HEAD)->returnSiblings().size() * HASH_SIZE; // each sibling should have some # of siblings
	length_count += size_path * mkproofs.size();
	uchar* buffer = new uchar[length_count];

	length_count = 0;
	memcpy(buffer + length_count, this->pubkey.data(), this->pubkey.size());
	length_count += this->pubkey.size();
	//Coutest
//	COMMON::printHex(buffer, length_count);
	memcpy(buffer + length_count, this->seed.data(), this->seed.size());
	length_count += this->seed.size();
	//Coutest
//	COMMON::printHex(buffer, length_count);
	for (PATH* mkproofPt : mkproofs) {
		memcpy(buffer + length_count, mkproofPt->releaseLeafPt(), LEAF_SIZE);
		length_count += LEAF_SIZE;
		//Coutest
//		COMMON::printHex(buffer, length_count);
		for (uchar* nodePt : mkproofPt->returnSiblings()) {
			memcpy(buffer + length_count, nodePt, HASH_SIZE);
			length_count += HASH_SIZE;
			//Coutest
//			COMMON::printHex(buffer, length_count);
		}
	}
	SHA1(buffer, length_count, hashvaluePt);
	return hashvaluePt;
}
