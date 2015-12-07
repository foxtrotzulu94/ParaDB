/*
 * odd.c
 *
 */
#include "odd.h"

//Our Alarm Signal Handler!
void notifyIncoming(int sig){
	//The signal came in. We need to update the table
	mustUpdateTable = 1;

	//TODO: Remove debug log
//	qlog("update");

	//More Importantly, set a random time interval between 3 and 5 seconds for the next alarm
	alarm(rand()%3 + 3);
}

//Main Function for all odd numbered processes
void pOdd(DB_Context handle){
	//TODO: Remove debug log
//	printf("%d",handle.rank);
//	qlog("Starting odd_main");

	//Setup some of the working variables
	char 	filename[50];
	FILE*	infile;
	RowList	myList;

	Query incomingQuery;
	MPI_Request op_request;
	int receivedQuery=0;
	MPI_Status op_status;


	//Initialize the variables
	//Make a new dynamic list
	RowList_init(&myList);

	//Open the file
	sprintf(filename,"data%d.txt",handle.rank-1);
	infile = fopen(filename,"r");
	if(infile==NULL){
		qlog("DID NOT FIND FILE!");
		MPI_Abort(MPI_COMM_WORLD,1);
	}

	//Seed randomness and sound the alarm!
	srand(time(NULL)+handle.rank);
	signal(SIGALRM, notifyIncoming);
	alarm(rand()%3 + 3);


	//Read some lines. The DB should start with something in its tables!
	readFromStream(infile,handle.readMax,&myList);

	//Do an initial non-blocking receive
	//This sets the op_request variable for the polling loop
	MPI_Irecv(&incomingQuery,1,handle.query,(handle.rank)-1,0,handle.all,&op_request);


	////////////
	//MAIN LOGIC
	////////////
	while(1){
		//Poll for the completion of an operation.
		do{
			MPI_Test(&op_request,&receivedQuery,&op_status);
		}while(receivedQuery==0 || mustUpdateTable==0);


		//if we have to update the table, read some lines and reset the flag
		if(mustUpdateTable){
			mustUpdateTable=0;
			readFromStream(infile,handle.readMax,&myList);
		}

		//If we received a query, handle this one and then do another non-blocking receive
		if(receivedQuery){
			if(queryDispatcher(&handle, &incomingQuery)){
				MPI_Irecv(&incomingQuery,1,handle.query,(handle.rank)-1,0,handle.all,&op_request);
			}
			else{
				//If the Query was an exit message. Cleanup and return to main!
				RowList_terminate(&myList);
				close(infile);
				break;
			}
		}
	}//End while-forever loop

}

//Deals with the incoming query
int queryDispatcher(DB_Context* context, Query* aQuery){
	//Let's take a look at the query type

	DBRow result; //TODO: Remove, this is for testing purposes.
	result.company_id=200; //Signal you're OK!

	switch(aQuery->type){

	case SALES_BY_COMPANY:
		//Route to the routine and then send the results back directly
		//TODO: COMPLETE. Right now it'll just ACK back
		findSalesForAllCompanies();

		//Just an ACK to test back
		replyToQuery(context, aQuery,&result,1);
		return 1;

	case SALES_BY_DATE:
		//Hand off the added info.
		//TODO: COMPLETE
		findSalesInDateRange(&(aQuery->conditions));

		//This is just an ACK to test back
		replyToQuery(context, aQuery,&result,1);
		return 1;

	case EXIT:
		//Begin cleaning up!
		return 0;

	default:
		//The system got a query it couldn't handle!
		//We're just going to do nothing and wait for the next one.
		return 1;
	}
}

//Sends the results of a query back to the Even Numbered Process.
//If we were doing IPC, we'd do it here
void replyToQuery(DB_Context* context, Query* aQuery, DBRow* result, int resultLength){
	//Send it down the line with a blocking send
	MPI_Send(result,resultLength,context->row,context->rank-1,0,context->all);
	//TODO: Complete!
}

//Find all sales matching the Query's Extended Info. Return a RowList of these entries
RowList findSalesInDateRange(ExtendedInfo* dates){
	//TODO: Complete!
	RowList retVal;

	return retVal;
}

//Return a list of DBRows (one per company) with the total  amounts of their sales.
DBRow* findSalesForAllCompanies(){
	//TODO: Complete!
	return NULL;
}
