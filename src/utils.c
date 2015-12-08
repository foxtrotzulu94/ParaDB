/*
 * utils.c
 *
 *      Author: foxtrot
 */


#include "utils.h"
#include "menu.h" //TODO: REMOVE!

DBRow* readFromStream(FILE* infile, int lineAmount, RowList* output, int *id){
	//Temporary variables for our reading
	char *aLine=NULL;

	size_t len=0;

	int i=0;
	//Run this loop until we get to the lineAmount or EOF (getline returning 0)
	for(i=0; i<lineAmount && getline(&aLine, &len,infile);++i){
		//We can append directly into RowList since it'll dynamically grow.
		DBRow readRow = readFormattedLine(aLine);
		RowList_push_back(output, readRow);

		//Keep track of the largest company size
		if( (*id) < readRow.company_id){
			(*id) = readRow.company_id;
		}
	}

	//Free the used memory
	free(aLine);

	//Might eventually change method signature to stop returning
	return NULL;
}

DBRow readFormattedLine(char* line){
	DBRow retVal;
	char* raw;
	char* date;
	//A well formatted line will have exactly 5 fields we have to parse

	//First field: sales_id
	raw = strtok(line,"|");
	retVal.sales_id = atoi(raw);

	//Second field: the date
	//For now, store the entire string since we'll need to call strtok again later
	date = strtok(NULL,"|");

	//Third field: company_id
	raw = strtok(NULL,"|");
	retVal.company_id = atoi(raw);

	//Fourth field: company_name
	raw=strtok(NULL,"|");
	strncpy(&retVal.company_name,raw,49);
	retVal.company_name[49]='\0'; //Gotta add that null char!

	//Last field: sales_amount
	raw = strtok(NULL,"|");
	retVal.sales_total = atof(raw);


	//Now let's finish our work with that date field. The string has variable length,
	//	so we rely on the standard delimiters
	raw = strtok(date,"/");
	retVal.date.year = atoi(raw);
	raw=strtok(NULL,"/");
	retVal.date.month = atoi(raw);
	raw=strtok(NULL,"/");
	retVal.date.day = atoi(raw);

	//The object should be done by this point
	return retVal;
}

//Used to compare dates by qsort
int compareDates(const void * a, const void * b){
	int retVal=0;	//if a<b -> -1, a==b -> 0, a>b -> 1
	Date *aDate,*bDate;
	DBRow *aRow,*bRow;

	//Read as DBRows
	aRow = (DBRow*)a;
	bRow = (DBRow*)b;

	//Read them as Date structs
	aDate = (Date*)&(aRow->date);
	bDate = (Date*)&(bRow->date);

	if(aDate->year < bDate->year){
		retVal=-1;
	}
	else if(aDate->year > bDate->year){
		retVal = 1;
	}
	else{ //Years must be the same
		if(aDate->month < bDate->month){
			retVal=-1;
		}
		else if(aDate->month > bDate->month){
			retVal = 1;
		}
		else{ //Months must be the same
			if(aDate->day < bDate->day){
				retVal=-1;
			}
			else if(aDate->day > bDate->day){
				retVal = 1;
			}
			else{
				return 0;
			}
		}
	}

	return retVal;
}

int compareDatesExclusive(const void * a, const void * b){
	Date* aDate,*bDate;
	int retVal=0;
	//Read them as Date structs
	aDate = (Date*)a;
	bDate = (Date*)b;

	if(aDate->year < bDate->year){
		retVal=-1;
	}
	else if(aDate->year > bDate->year){
		retVal = 1;
	}
	else{ //Years must be the same
		if(aDate->month < bDate->month){
			retVal=-1;
		}
		else if(aDate->month > bDate->month){
			retVal = 1;
		}
		else{ //Months must be the same
			if(aDate->day < bDate->day){
				retVal=-1;
			}
			else if(aDate->day > bDate->day){
				retVal = 1;
			}
			else{
				return 0;
			}
		}
	}

	return retVal;
}

//Used to compare Companies by qsort
int compareCompanies(const void * a, const void * b){
	DBRow *aRow,*bRow;

	//Read as DBRows
	aRow = (DBRow*)a;
	bRow = (DBRow*)b;

	return (aRow->company_id)-(bRow->company_id);
}

//Returns milliseconds from Epoch for given date
long long convertDateToEpoch(Date* date){
	long long  retVal;
	struct tm cDate = {0};
	time_t epochTime = {0};

	cDate.tm_year = (date->year)-1900;
	cDate.tm_mon = (date->month)-1;
	cDate.tm_mday = date->day;
	printf("converting from %s\n",asctime(&cDate));
	epochTime = mktime(&cDate);

	retVal = (long long) epochTime;
	return retVal;
}

//Returns a Date struct for a particular millisecond representation of Epoch
Date convertEpochToDate(long long epochTime){
	Date retVal;
	struct tm *datePtr;
	time_t cTime = (time_t)epochTime;

	datePtr = localtime(&cTime);
	retVal.year = datePtr->tm_year + 1900;
	retVal.month = datePtr->tm_mon + 1;
	retVal.day = datePtr->tm_mday;

	return retVal;
}

//Return the parameters to do an all to all
void getAllToAllParameters(long long start, long long end, int divisions, DBRow* list, int listLength, int* outAmounts, int* outOffsets){
	//Adapted from our assignment 3
	//We've once more assumed that outAmounts and outOffsets are of "divisions" length
	long long timeLimits = (end-start)/divisions;
	printf("time limits %d\n, input length %d\n",timeLimits,listLength);

	Date* bucketRanges = calloc(divisions,sizeof(Date));
	int currentOffset=0;
	int i=0, index=0;

	//We first make a map of the ranges
	for(i=0;i<divisions;++i){
		bucketRanges[i]=  convertEpochToDate(start+(i+1)*timeLimits);
		printDate(bucketRanges[i]);
		printf("->Date in Param\n");
		fflush(stdout);
	}

	//Now we count how much we should assign to each division, given that DBRow* list
	for(i=0; i<divisions; ++i){
		int count=0;
		outOffsets[i] = currentOffset;

		//While within the bucket range (date<bucketRange[i]) and the list bound, count
		while(compareDatesExclusive(&(list[index].date),&bucketRanges[i])==-1 && index<listLength){
			count++;
			index++;

		}
		outAmounts[i]=count;
		currentOffset+=count;
	}


	free(bucketRanges);
}

void qlog(char* something){
#if DEBUG_DB
	printf("%s\n",something);
	fflush(stdout);
#endif
}
