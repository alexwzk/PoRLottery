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
public:
	std::string pubkey;
	std::string seed;
	std::vector<PATH*> mkproofs; //uses allmkproofs' pointers in USER class

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
};

#endif /* PERMACN_TICKET_H_ */
