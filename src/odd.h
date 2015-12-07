/*
 * odd.h
 *
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

/**
 * Contains all logic that the odd numbered processes deal with
 * specifically, that is the query dispatcher, the signal handler and
 * the functions used to fulfill the queries successfully.
 * File handling relies on the utils.h functions. File stream is only
 * opened and closed here
 * The "pOdd" function hosts the high level logic of the process.
 *
 */

#ifndef ODD_H_
#define ODD_H_

//Global Variables... Only one is needed (luckily)
//Acts as a bool type. "1" is True, "0" is false
int mustUpdateTable;

//Our Alarm Signal Handler!
void notifyIncoming(int sig);

//Main Function for all odd numbered processes
void pOdd(DB_Context handle);

//Deals with the incoming query. Returns 0 if there's no need to listen for a new Query.
int queryDispatcher(DB_Context* context,Query* aQuery);

//Sends the results of a query back to the Even Numbered Process.
//If we were doing IPC, we'd do it here
void replyToQuery(DB_Context* context,Query* aQuery, DBRow* result, int resultLength);

//Find all sales matching the Query's Extended Info. Return a RowList of these entries
RowList findSalesInDateRange(ExtendedInfo* dates);

//Return a list of DBRows (one per company) with the total  amounts of their sales.
DBRow* findSalesForAllCompanies();

#endif /* ODD_H_ */
