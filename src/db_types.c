/*
 * db_types.c
 *
 *  Created on: Dec 5, 2015
 *      Author: foxtrot
 */
#include <string.h>
#include <stdlib.h>
#include "db_types.h"

void RowList_init(RowList* aList){
	aList->capacity=8; //We want to keep this in powers of two
	aList->size=0;
	aList->rows = calloc(aList->capacity,sizeof(DBRow));
}

void RowList_push_back(RowList* this, DBRow row){
	//Dynamically allocate more space without thinking about it
	if(this->size==this->capacity){
		int previousCapacity = this->capacity;
		this->capacity= (this->capacity)*2;
		this->rows = realloc(this->rows, sizeof(DBRow)* this->capacity);

		//Reset the new part of the array to all zeros.
		memset(&(this->rows[previousCapacity]),0,previousCapacity);
	}
	this->rows[this->size] = row;
	this->size+=1;
}

void RowList_terminate(RowList* this){
	free(this->rows);
	this->capacity=8;
	this->size=0;
}

void RowList_recycle(RowList *this){
	RowList_terminate(this);
	RowList_init(this);
}

//Setup all MPI Datatypes as named in arguments
void setupDBTypes(MPI_Datatype* date, MPI_Datatype* row, MPI_Datatype* query, MPI_Datatype* ext_info){
	configureDateType(date);
	configureQueryType(date,query);
	configureRowType(date,row);
	configureQueryExtendedInfo(date,ext_info);
}

//Configure and serialize the Date datatype
void configureDateType(MPI_Datatype* date){
	//Recycled from Assignment 3
	const int numElements=3;
	int numBlocks[3]={1,1,1};
	MPI_Aint displacements[3]={offsetof(Date, year),offsetof(Date, month),offsetof(Date, day)};
	MPI_Datatype usedTypes[3]={MPI_UNSIGNED_SHORT,MPI_UNSIGNED_SHORT,MPI_UNSIGNED_SHORT};
	MPI_Type_create_struct(numElements,numBlocks,displacements,usedTypes,date);
	MPI_Type_commit(date);
}

//Configure and serialize the Database Row datatype
void configureRowType(MPI_Datatype* date, MPI_Datatype* row){
	//By now, date should've been initialized
	//TODO: Fix the Company name problem (see the comment in the typedef DBRows at db_types.h)
	//For now, what were doing is leaving the Company ID as a sort of Foreign Key into a separate table.
	const int numElements=4;
	int numBlocks[4]={1,1,1,1};
	MPI_Aint displacements[4]={offsetof(DBRow,sales_id),offsetof(DBRow,date),offsetof(DBRow,company_id),offsetof(DBRow,sales_total)};
	MPI_Datatype usedTypes[4]={MPI_UNSIGNED,*date,MPI_UNSIGNED,MPI_FLOAT};
	MPI_Type_create_struct(numElements,numBlocks,displacements,usedTypes,row);
	MPI_Type_commit(row);
}

//Configure and serialize the Database Query message
void configureQueryType(MPI_Datatype* ext_info,MPI_Datatype* query){
	const int numElements=2;
	int numBlocks[2]={1,1};
	MPI_Aint displacements[2]={offsetof(Query,type),offsetof(Query,conditions)};
	MPI_Datatype usedTypes[3]={MPI_INT,*ext_info};
	MPI_Type_create_struct(numElements,numBlocks,displacements,usedTypes,query);
	MPI_Type_commit(query);
}

//Configure the Query Extended Info struct
void configureQueryExtendedInfo(MPI_Datatype* date, MPI_Datatype* extended_info){
	const int numElements=2;
	int numBlocks[2]={1,1};
	MPI_Aint displacements[2]={offsetof(ExtendedInfo,startDate),offsetof(ExtendedInfo,endDate)};
	MPI_Datatype usedTypes[3]={*date,*date};
	MPI_Type_create_struct(numElements,numBlocks,displacements,usedTypes,extended_info);
	MPI_Type_commit(extended_info);
}
