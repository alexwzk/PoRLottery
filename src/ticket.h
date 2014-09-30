/*
 * ticket.h
 *
 *  Created on: Sep 27, 2014
 *      Author: zkwen
 */

#ifndef PERMACN_TICKET_H_
#define PERMACN_TICKET_H_

#include "path.h"

#define PUZID 0
#define ALLPROFS 1
#define PUBKEY 2
#define SEED 3
#define TICPROFS 4

class TICKET {
private:
	/**
	 * Transfer path data to buffer
	 * INPUT
	 * AFFECT legnth_count(parameter)
	 */
	int pathsToBuffer(std::vector<PATH*> paths, uchar* buffer, size_t &length_count);

public:
	digest pubkey;
	std::string seed;
	std::vector<PATH*> mkproofs; //uses allmkproofs' pointers in USER class
	std::vector<PATH*> fps_signs;

	/**
	 * Default constructor and deconstructor
	 */
	TICKET(){}
	~TICKET(){}

	/**
	 * Computes the hashvalue of a ticket
	 * OUTPUT digest of this ticket
	 */
	uchar* hashOfTicket();

	/**
	 * Writes ticket data to buffer
	 * !!!! Exclude the hash of a ticket
	 */
	friend std::ostream& operator<<(std::ostream &out, TICKET &tic);

	/**
	 * Reads ticket data from buffer or files
	 */
	friend std::istream& operator>>(std::istream &in, TICKET & tic);
};

#endif /* PERMACN_TICKET_H_ */
