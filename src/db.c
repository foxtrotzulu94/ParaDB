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
	//TODO: Cleanup for final solution

	//The context for our entire database
	DB_Context context;
	printf("DON'T WORRY ABOUT THE MAKE FILE!");
	printf("Initializing");
	DBMS_INIT(&context,argc,argv); //PROBLEM IN HERE!
//	printf("finished");
//
//	printf("context rank %d\n",context.rank);
//
	MPI_Barrier(context.all);
	if(context.rank%2==0){
		printf("I'm even, I deal with you! %d\n",context.rank);
		pEven(&context);
	}
	else{
		printf("I'm out of here! %d\n",context.rank);
		pOdd(context); //Working fine
	}

//	DBRow* myrows = calloc(10,sizeof(DBRow));
//	Date def;
//	def.day=29;
//	def.month=1;
//	def.year=1994;
//
//
//	int i=0;
//	int rand=0;
//	for(i=0;i<10;++i){
//		scanf("%d",&rand);
//		myrows[i].company_id = rand;
//		myrows[i].date=def;
//		myrows[i].date.month=i;
//	}
//
//	qsort(myrows,10,sizeof(DBRow),compareCompanies);
//
//	for(i=0;i<10;++i){
//		printRow(&myrows[i]);
//		printf("\n");
//	}

	printf("\n\n");

	DBMS_END(&context);

	return 0;
}
