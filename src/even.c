/*
 * even.c
 *
 */

#include "even.h"

//Main function for all even-numbered processes (including Root)
void pEven(DB_Context* handle){
	Query inputQuery;
	RowList queryResults;
	RowList querySorted;


	while(1){
		//Root shows menu and asks input
		if(handle->rank==0){
			inputQuery = requestUserInput();
		}

		//We need to add a barrier to synchronize the rest of the processes (Do we?)
		MPI_Barrier(handle->coworkers); //Might not be necessary

		forwardQuery(handle,&inputQuery); //send it out
		if(inputQuery.type==EXIT){ break; } //Leave if needed...
		//TODO: Test the stuff below. We still need to check the reply of the odd processes
//		queryResults = processQuery(handle,&inputQuery); //... Or get it back
//
//		querySorted = bucketSort(handle,queryResults.rows,queryResults.size); //Sort it out with the rest
//
//		//Root delivers!
//		if(handle->rank==0){
//			printQueryResults(inputQuery,querySorted.rows,querySorted.size);
//		}
//
//
//		//Recycle the used types
//		RowList_recycle(&queryResults);
//		RowList_recycle(&querySorted);

	}

}

//Process the incoming query. Forwards it to the odd process and returns a dynamic list of the results.
RowList processQuery(DB_Context* handle, Query* aQuery){
	RowList retVal;
	DBRow* response;
	int responseSize;

	MPI_Status op_status;

	//Get the result back!
	//Probe first, get_count and calloc the buffer!
	MPI_Probe((handle->rank)+1,0,handle->all,&op_status);
	MPI_Get_count(&op_status,handle->row,&responseSize);
	response = calloc(responseSize,sizeof(DBRow));

	//Now we're ready for that message
	MPI_Recv(response,responseSize,handle->row,(handle->rank)+1,0,handle->all,&op_status);

	//Make it into a high level format
	RowList_fit(&retVal,response,responseSize);

	return retVal;
}

//Only sends the Query through the system in the correct order.
void forwardQuery(DB_Context* handle, Query* aQuery){
	//Forward to other even nodes
	MPI_Bcast(aQuery,1,handle->query,0,handle->coworkers);

	//From there, give off the odd processes
	MPI_Send(aQuery,1,handle->query,(handle->rank)+1,0,handle->all);
}

//Perform a full bucket sort operation such that the Root has the final values!
RowList bucketSort(DB_Context* handle, DBRow* partials, int partialsLength){
	RowList retVal;
	int evenProcessCount;
	int expectedBufferSize=0;
	int* expectedSize;
	int* offsets;
	DBRow* finals;

	MPI_Comm_size(handle->coworkers,&evenProcessCount);
	expectedSize = calloc(evenProcessCount,sizeof(int));

	//TODO: COMPLETE! for now it's just a gather to the root

	//Get the amounts being sent by each process
	MPI_Gather(&partialsLength,1,MPI_INT,expectedSize,evenProcessCount,MPI_INT,0,handle->coworkers);

	if(handle->rank==0){
		int i=0;
		offsets = calloc(evenProcessCount,sizeof(int));
		for(i=0;i<evenProcessCount;++i){
			//Get the running sum
			expectedBufferSize += expectedSize[i];
			if(i<evenProcessCount-1){ //And also get the displacements.
				offsets[i+1]=expectedSize[i]+offsets[i]-1; //TODO: Double check if that -1 causes any trouble or not :/
			}
		}
		finals = calloc(expectedBufferSize,sizeof(DBRow));
	}

	//Get them all at root!
	MPI_Gatherv(partials,partialsLength,handle->row,finals,expectedSize,offsets,handle->row,0,handle->all);


	if(handle->rank==0){
		RowList_fit(&retVal,finals,expectedBufferSize);
	}
	else{
		RowList_fit(&retVal,partials,partialsLength);
	}

	return retVal;
}
