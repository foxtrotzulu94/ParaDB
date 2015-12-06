/*
 * db_types.c
 *
 *  Created on: Dec 5, 2015
 *      Author: foxtrot
 */

#include "db_types.h"

void RowList_init(RowList* aList){
	aList->capacity=10;
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
	this->capacity=10;
	this->size=0;
}

void RowList_recycle(RowList *this){
	RowList_terminate(this);
	RowList_init(this);
}

//Setup all MPI Datatypes as named in arguments
void setupDBTypes(MPI_Datatype* date, MPI_Datatype* row, MPI_Datatype* query){
	configureDateType(date);
	configureQueryType(date,query);
	configureRowType(date,row);
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
	//TODO: Serialize and commit
}

//Configure and serialize the Database Query message
void configureQueryType(MPI_Datatype* date, MPI_Datatype* query){
	//TODO: Serialize and commit
}
