/*
 * utils_mpi.h
 *
 *  Created on: Dec 6, 2015
 *      Author: foxtrot
 */

#include "db_types.h"

#ifndef UTILS_MPI_H_
#define UTILS_MPI_H_

//Initialize the entire ParaDB System
void DBMS_INIT(DB_Context* handle);

//Checks the command line parameters (not including numProcesses). Call before MPI_Init
void checkArgs(int *argc, char* argv[]);

//Sets the MPI_COMM_WORLD and info related to the number of running processes and their rank
void setWorldInfo(DB_Context* handle);

//Creates the new communicator for even and odd processes
void setProcessCoworkers(DB_Context* handle);

//Close the ParaDB System safely. Takes care of freeing context references and custom datatypes
void DBMS_END(DB_Context* handle);

#endif /* UTILS_MPI_H_ */
