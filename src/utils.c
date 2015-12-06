/*
 * utils.c
 *
 *      Author: foxtrot
 */


#include "utils.h"

DBRow* readFromStream(FILE* infile, int lineAmount, RowList* output){
	//Temporary variables for our reading
	char *aLine=NULL;

	size_t len=0;


	int i=0;
	//Run this loop until we get to the lineAmount or EOF (getline returning 0)
	for(i=0; i<lineAmount && getline(&aLine, &len,infile);++i){
		//We can append directly into RowList since it'll dynamically grow.
		RowList_push_back(output, readFormattedLine(aLine));
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
	//TODO: do strcpy on whatever string gets returned by strtok and assign the pointer to company_name.
	retVal.company_name = strtok(NULL,"|");

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
