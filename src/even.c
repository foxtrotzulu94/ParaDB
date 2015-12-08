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

	//Initialize your dynamic lists!
	RowList_init(&queryResults);
	RowList_init(&querySorted);

	while(1){
		//Root shows menu and asks input
		if(handle->rank==0){
			inputQuery = requestUserInput();
			if(inputQuery.type==EXIT){
				safeWrite("\nThank you for using ParaDB... Good Bye!");
			}
			else{
				printQueryInProcess();
			}
		}

		//We need to add a barrier to synchronize the rest of the processes (Do we?)
		MPI_Barrier(handle->coworkers); //Might not be necessary

		forwardQuery(handle,&inputQuery); //send it out
		if(inputQuery.type==EXIT){ break; } //Leave if needed...
		queryResults = waitForQueryReply(handle,&inputQuery); //... Or get it back

		querySorted = processQuery(handle,&inputQuery,queryResults); //TODO: enter this method and DO BUCKET SORT!

		//Root delivers!
		if(handle->rank==0){
			printQueryResults(inputQuery,querySorted.rows,querySorted.size);
		}


		//Recycle the used types
		//TODO: Think this one through a bit better. BucketSort will likely return new lists that need to be recycled
//		RowList_terminate(&queryResults);
//
//		if(handle->rank==0){
//
//			RowList_terminate(&querySorted);
//		}
		//TODO: Remove these leaky statements!
//		RowList_recycle(&queryResults);
//		RowList_recycle(&querySorted);
	}

	//cleanup the lists before returning to master!
	if(queryResults.size)
		RowList_terminate(&queryResults);
	if(querySorted.size && querySorted.rows!=queryResults.rows)
		RowList_terminate(&querySorted);
}

//Process the incoming query. Forwards it to the odd process and returns a dynamic list of the results.
RowList waitForQueryReply(DB_Context* handle, Query* aQuery){
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
	printf("even received %d\n",responseSize);

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

RowList processQuery(DB_Context* handle,Query* aQuery, RowList partials){
	//For a sales by date query, we need to bucket sort
	if(aQuery->type==SALES_BY_DATE){
		//TODO: IMPLEMENT BUCKET SORT AND UNCOMMENT
		safeWrite("PROCESSING DATES!");
		return bucketSort(handle,partials.rows,partials.size);
	}

	//This is simpler if we just reduce here.
	if(aQuery->type==SALES_BY_COMPANY){
		//here we'd want to simply reduce the results at the root
		//Just do successive MPI_Reduce operations to fill in the return array
		int maxRounds = partials.size; //corresponds to Max Company ID by now.
		DBRow* reduced;
		int i=0;

		if(handle->rank==0){
			reduced = calloc(maxRounds,sizeof(DBRow));
			//Make an exact copy of the significant info
			for(i=0;i<maxRounds;++i){
				reduced[i].company_id=i;
			}
		}

		//This for loop simply sums the results retrieved from all coworker nodes.
		for(i=0;i<maxRounds;++i){
			MPI_Reduce(&(partials.rows[i].sales_total),&(reduced[i].sales_total),1,MPI_FLOAT,MPI_SUM,0,handle->coworkers);
		}

		if(handle->rank==0){
			//If root, return a new container
			RowList container;
			RowList_fit(&container,reduced,maxRounds);
			return container;
		}
		else{
			//Other processes will get back what they came with
			return partials;
		}

	}
}

//Perform a full bucket sort operation such that the Root has the final values!
RowList bucketSort(DB_Context* handle, DBRow* partials, int partialsLength){
	RowList retVal;
	int evenProcessCount;
	int expectedBufferSize=0;
	int* expectedSize;
	int* offsets;
	DBRow* finals;

	//Get our coworkers size and make an array of said size
	MPI_Comm_size(handle->coworkers,&evenProcessCount);
	expectedSize = calloc(evenProcessCount,sizeof(int));

	//First, initially sort all of the elements
	qsort(partials,partialsLength,sizeof(DBRow),compareDates);

	//Now make the buckets.







	//TODO: COMPLETE! for now it's just a gather to the root

	//Get the amounts being sent by each process
	MPI_Gather(&partialsLength,1,MPI_INT,expectedSize,evenProcessCount,MPI_INT,0,handle->coworkers);

	if(handle->rank==0){
		int i=0;
		offsets = calloc(evenProcessCount,sizeof(int));
		for(i=0;i<evenProcessCount;++i){
			//Get the running sum
			expectedBufferSize += expectedSize[i];

			if(i<evenProcessCount-1){ //And also calculate the displacements.
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

void cleanupAfterQuery(DB_Context* handle, Query* aQuery, RowList partials, RowList finals){
	if(handle->rank==0){


	}
	else{
		RowList_terminate(&partials);
	}
}
