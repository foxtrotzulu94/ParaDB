/*
 ============================================================================
 Name        : db.c
 Author      : Javier E. Fajardo
 Version     :
 Copyright   :
 ============================================================================
 */
#include <stdio.h>
#include <string.h>
#include "mpi.h"
#include "db_types.h"
#include "menu.h"
#include "utils.h"
#include "utils_mpi.h"
#include "odd.h"
#include "even.h"

int main(int argc, char* argv[]){

	//The context for our entire database
	DB_Context context;

	DBMS_INIT(&context,argc,argv);

	MPI_Barrier(context.all);
	//Now route to the correct processes
	if(context.rank%2==0){
		//Even processes here
		pEven(&context);
	}
	else{
		//Odd processes here
		pOdd(context);
	}


	printf("\n\n");

	DBMS_END(&context);

	return 0;
}
