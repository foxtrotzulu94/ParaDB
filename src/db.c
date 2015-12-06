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

int main(int argc, char* argv[]){
	//TODO: Cleanup for final solution

	RowList myList;
	DBRow aRow;
	FILE* myFile;
	myFile = fopen("data0.txt","r");
//	aRow.company_name="TESTY";
//	aRow.date = promptDateRange("some");
//	aRow.company_id=0;
//	aRow.sales_id=1;
//	aRow.sales_total=37.52;
//	aRow = readFormattedLine()

	RowList_init(&myList);
	readFromStream(myFile,50000,&myList);

//	int i=0;
//	for(i=0;i<5;++i){
//		RowList_push_back(&myList,aRow);
//	}
//	printf("%s man!\n",myList.rows[4].company_name);
//
//	Date myDate = promptDateRange("Some");
//	printDate(myDate);
//
//	RowList_terminate(&myList);
	printRow(&(myList.rows[49999]));
//	Query myQuery = requestUserInput();
//	while(myQuery.type!=EXIT){
//		printf("Query Type %d\n",myQuery.type);
//		myQuery = requestUserInput();
//	}
	printf("\nRow list stats: capacity=%d size=%d",myList.capacity,myList.size);
	RowList_terminate(&myList);
	fclose(myFile);
	printf("\n\n");

	return 0;
}
