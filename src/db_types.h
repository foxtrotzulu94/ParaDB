/*
 * db_types.h
 *
 *      Author: foxtrot
 */

#include "mpi.h"

/**
 *This file contains all of the definitions and supporting functions
 *	of the programs datatypes. The functions defined are used to initialize
 *	the derived types with MPI and require a static handle to these.
 */

#ifndef DB_TYPES_H_
#define DB_TYPES_H_

struct Date;
struct DBRow;
struct QueryType;
struct ExtendedInfo;
struct Query;
struct RowList;

//Reused from Assignment 3.
//This is a less expensive struct than "struct tm"
typedef struct{
	unsigned short year;
	unsigned short month;
	unsigned short day;
} Date;

//A row in the DB Table (Only one table type!)
typedef struct{
	unsigned int sales_id; //Starts at 1
	Date date;
	unsigned int company_id; //Starts at 1
	char* company_name;
	float sales_total;
} DBRow;

//Defines the types of query supported by the system
typedef enum{
	EXIT =-1,
	SALES_BY_DATE,
	SALES_BY_COMPANY
}QueryType; //Can be treated as an MPI_INT

//Additional info on the query. For now it will just be Date ranges
typedef struct{
	Date startDate;
	Date endDate;
}ExtendedInfo;

//Defines a user-made query message. Can be sent throughout the system
typedef struct{
	QueryType type;
	ExtendedInfo conditions;
}Query;

//Dynamic array for DB Rows. It is NOT meant to be sent directly through MPI
typedef struct{
	DBRow* rows;
	int size;
	int capacity;
}RowList;

//Initialize the Dynamic List
void RowList_init(RowList* aList);

//Add a new element and grow the dynamic list if needed
void RowList_push_back(RowList* this, DBRow row);

//Delete all dynamic elements in the list.
void RowList_terminate(RowList* this);

//Terminate and Reinit the list for reusing the same pointer later.
void RowList_recycle(RowList* this);



//Some helper functions needed to set them up in MPI

//Setup all MPI Datatypes as named in arguments
void setupDBTypes(MPI_Datatype* date, MPI_Datatype* row, MPI_Datatype* query);

//Configure and serialize the Date datatype
void configureDateType(MPI_Datatype* date);

//Configure and serialize the Database Row datatype
void configureRowType(MPI_Datatype* date, MPI_Datatype* row);

//Configure and serialize the Database Query message
void configureQueryType(MPI_Datatype* date, MPI_Datatype* query);

#endif /* DB_TYPES_H_ */

