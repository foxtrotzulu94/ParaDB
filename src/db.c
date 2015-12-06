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

int main(int argc, char* argv[]){
	//TODO: Cleanup for final solution

	RowList myList;
	DBRow aRow;
	aRow.company_name="TESTY";
	RowList_init(&myList);
	int i=0;
	for(i=0;i<5;++i){
		RowList_push_back(&myList,aRow);
	}
	printf("%s man!\n",myList.rows[4].company_name);

	Date myDate = promptDateRange("Some");
	printDate(myDate);

	RowList_terminate(&myList);
	
	Query myQuery = requestUserInput();
	while(myQuery.type!=EXIT){
		printf("Query Type %d\n",myQuery.type);
		myQuery = requestUserInput();
	}
	printf("\n\n");

	return 0;
}
