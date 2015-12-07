/*
 * even.h
 *
 *  Created on: Nov 21, 2015
 *      Author: foxtrot
 */

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#include "mpi.h"
#include "db_types.h"
#include "utils.h"
#include "menu.h"

/**
 *	Herein lies all the logic for the even numbered processes
 *	Most importantly, these methods forward incoming queries and organize
 *	the output received from the odd numbered processes.
 *	They also perform bucket sort operations
 *
 *	The main logic for this
 */

#ifndef EVEN_H_
#define EVEN_H_

//Main function for all even-numbered processes (including Root)
void pEven(DB_Context* handle);

//Process the incoming query. Forwards it to the odd process and returns a dynamic list of the results.
RowList processQuery(DB_Context* handle, Query* aQuery);

//Only sends the Query to the odd numbered process.
void forwardQuery(DB_Context* handle, Query* aQuery);

//Perform a full bucket sort operation such that the Root has the final values!
RowList bucketSort(DB_Context* handle, DBRow* partials, int partialsLength);




#endif /* EVEN_H_ */
