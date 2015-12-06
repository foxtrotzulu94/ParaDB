/*
 * utils_mpi.c
 *
 *  Created on: Dec 6, 2015
 *      Author: foxtrot
 */
#include "utils.h"
#include "utils_mpi.h"

//Initialize the entire ParaDB System
void DBMS_INIT(DB_Context* handle, int argc, char* argv[]){
	//First, check and set the args
	handle->readMax = checkArgs(argc,argv);

	//Now get info about our world
	MPI_Init(&argc,&argv);
	setWorldInfo(handle);

	//Set the new communicators
	setProcessCoworkers(handle);

	//Set the Datatypes
	setupDBTypes(&(handle->date),&(handle->row),&(handle->query),&(handle->ext_info));

	//finished
}

//Checks the command line parameters (not including numProcesses). Call before MPI_Init
int checkArgs(int argc, char* argv[]){
	//Check that only two arguments were provided. This should be the name of the program and "readmax"
	if(argc!=2){
		abort();
	}

	//We are interested in the second argument, "readMax"
	int retVal =atoi(argv[1]);
	if(retVal<=0){
		//Stop executing the program if the parameter is 0 or less
		abort();
	}

	return retVal;
}

//Sets the MPI_COMM_WORLD and info related to the number of running processes and their rank
void setWorldInfo(DB_Context* handle){
	handle->all = MPI_COMM_WORLD;
	MPI_Comm_rank(MPI_COMM_WORLD, &(handle->rank));
	MPI_Comm_size(MPI_COMM_WORLD, &(handle->processes));

	//Now we have to check that we are truly running in parallel. Hopefully, we are.
	//We'd need to abort if we're not
	if(handle->processes < 2){
		if(handle->rank==0){
			printf("Cannot run with less than 2 processes. ABORTING!");
		}
		MPI_Abort(MPI_COMM_WORLD,1);
		abort();
	}
}

//Creates the new communicator for even and odd processes
void setProcessCoworkers(DB_Context* handle){
	//We'll be splitting MPI_COMM_WORLD in even and odd.
	//Each process knows its rank by now, so all we need to do is modulo
	MPI_Comm_split(MPI_COMM_WORLD,handle->rank %2,1,&(handle->coworkers));
}

//Close the ParaDB System safely. Takes care of freeing context references and custom datatypes
void DBMS_END(DB_Context* handle){
	//We have to free all of our custom types
	MPI_Type_free(&(handle->row));
	MPI_Type_free(&(handle->query));
	MPI_Type_free(&(handle->ext_info));
	MPI_Type_free(&(handle->date));

	//Also free up the custom communicator
	MPI_Comm_free(&(handle->coworkers));

	//And finalize!
	MPI_Finalize();
}
