/*
 * odd.c
 *
 */
#include "odd.h"
#include "menu.h"
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
	RowList	memoryTable;

	Query incomingQuery;
	MPI_Request op_request;
	int receivedQuery=0;
	MPI_Status op_status;


	//Initialize the variables
	//Handle the global ones first
	mustUpdateTable = 0;
	maxCompanyID = 0;

	//Make a new dynamic list
	RowList_init(&memoryTable);

	//Open the file
	sprintf(filename,"data%d.txt",handle.rank/2);
	printf("opening %s\n",filename);
	fflush(stdout);
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
	readFromStream(infile,handle.readMax,&memoryTable,&maxCompanyID);
	printf("max company id: %d\n",maxCompanyID);
	fflush(stdout);

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
			readFromStream(infile,handle.readMax,&memoryTable,&maxCompanyID);
		}

		//If we received a query, handle this one and then do another non-blocking receive
		if(receivedQuery){
			qlog("dispatching query");
			//Say we have the query
			if(queryDispatcher(&handle, &incomingQuery, &memoryTable)){
				MPI_Irecv(&incomingQuery,1,handle.query,(handle.rank)-1,0,handle.all,&op_request);
			}
			else{
				//If the Query was an exit message. Cleanup and return to main!
				RowList_terminate(&memoryTable);
				close(infile);
				break;
			}
		}
	}//End while-forever loop

}

//Deals with the incoming query
int queryDispatcher(DB_Context* context, Query* aQuery, RowList* table){
	//Let's take a look at the query type

	DBRow result; //TODO: Remove, this is for testing purposes.
	result.company_id=200; //Signal you're OK!
	result.date.year=1994;
	result.date.month=	9;
	result.date.day=29;

	qlog("in query");


	RowList matchingRows;

	switch(aQuery->type){

	case SALES_BY_COMPANY:
		//Route to the routine and then send the results back directly
		//TODO: COMPLETE. Right now it'll just ACK back
		matchingRows = findSalesForAllCompanies(table);
		qlog("found companies");
		//Just an ACK to test back
		replyToQuery(context, aQuery,matchingRows.rows,matchingRows.size);
		return 1;

	case SALES_BY_DATE:
		//Find and send all the rows that match the dates.
		matchingRows = findSalesInDateRange(&(aQuery->conditions),table);
		qlog("found sales by dates");
		replyToQuery(context, aQuery,matchingRows.rows,matchingRows.size);
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
	printf("sent %d elements\n",resultLength);
	qlog("Reply sent");

	//TODO: Complete! //I think this is done by now :/
}

//Find all sales matching the Query's Extended Info. Return a RowList of these entries
RowList findSalesInDateRange(ExtendedInfo* dates, RowList* table){
	//TODO: Complete!
	RowList retVal;
	RowList_init(&retVal);
	printf("table has %d rows\n",table->size);
	printDate(dates->startDate);
	printf("\n");
	printDate(dates->endDate);
	printf("\n");
	fflush(stdout);
	int i=0;
	for(i=0;i<table->size;++i){
		//Use the utils compare function for this
		//Date has to be larger than or equal to startDate and less than or equal to endDate
		//This translates to startCompare!=-1 && endCompare!=1
		if(compareDatesExclusive(&(table->rows[i].date),&(dates->startDate))!=-1 && compareDatesExclusive(&(table->rows[i].date),&(dates->endDate))!=1){
			//Entry is good, Append!
			RowList_push_back(&retVal,table->rows[i]);
		}
	}
	printf("reply has %d rows\n",retVal.size);
		fflush(stdout);

	return retVal;
}

//Return a list of DBRows (one per company) with the total  amounts of their sales.
RowList findSalesForAllCompanies(RowList* table){
	//What we're trying to do here is a makeshift dictionary. We expect that
	RowList container;
	DBRow* mainRows=calloc(maxCompanyID,sizeof(DBRow));
	int i=0;

	printf("table has %d rows\n",table->size);
	printf("max company id is %d\n",maxCompanyID);

	//First we'd want to make sure everyone starts off with 0.0
	for(i=0;i<maxCompanyID;++i){
		mainRows[i].sales_total=0;
		mainRows[i].company_id=i+1;
	}

	//Now we want to fill in the cells with the correct info
	int companyID=1; //Note we are GUARANTEED companyID=0 does not exist
	for(i=0;i<table->size;++i){
		companyID = table->rows[i].company_id;
		mainRows[companyID-1].sales_total+=table->rows[i].sales_total;
		//TODO: Assign company name if needed as well
		//We won't assign date or other variables since they are not needed...
	}
	fflush(stdout);

	RowList_fit(&container,mainRows,maxCompanyID);
	return container;
}
