/*
 * menu.c
 *
 *  Created on: Nov 21, 2015
 *      Author: foxtrot
 */

#include "menu.h"
#include <stdio.h>

//Draws the menu and returns the user input query to be propagated through the system
Query requestUserInput(){
	Query userInput;
	int input;

	while(1){
		drawMenu();
		scanf("%d",&input);
		system("clear");
		//Return as soon as they give a valid option.
		switch(input){
		case 1:
			userInput.type=SALES_BY_COMPANY;
			return userInput;
		case 2:
			userInput.type=SALES_BY_DATE;
			userInput.conditions.startDate = promptDateRange("Starting");
			userInput.conditions.endDate = promptDateRange("End");
			return userInput;
		case 3:
			userInput.type=EXIT;
			return userInput;
		default:
			break;
		}
	}
}

void printQueryInProcess(){
	printf("Processing Input....");
	fflush(stdout);
}

//Prints the resulting DBRows of a given Query
void printQueryResults(Query query, DBRow* result, int length){
	//TODO: Format printing
	printf("\nReturned %d rows\n\n",length);
	fflush(stdout);
	switch(query.type){
	case SALES_BY_COMPANY:
		printSalesByAllCompanies(result,length);
		break;
	case SALES_BY_DATE:
		printSalesByDate(result,length);
		break;
	case EXIT:
		safeWrite("\nThank you for using ParaDB... Good Bye!");
		break;
	default:
		safeWrite("Query ERROR");
		return;
	}
	printf("\nQuery OK (%d rows)\n\n",length);
	fflush(stdout);
}

//Prints the entire menu to console.
void drawMenu(){
	printf("\nParallel Database System (Powered by OpenMPI)\n\n");
	printf("\t1. Company Sales\n");
	printf("\t2. Sales By Date\n");
	printf("\t3. Exit\n\n");
	printf("Option: ");
	fflush(stdout);
	//Return control to the Request User Input menu
}

void printSalesByAllCompanies(DBRow* result, int length){
	//TODO: Change for company names when available!
	printf("Company\t\tTotal\n\n");
	fflush(stdout);
	int i=0;
	for(i=0;i<length;++i){
		printCompanySales(&(result[i]));
	}
	safeWrite("\n");
}

void printCompanySales(DBRow* result){
	//TODO: CHANGE for company name when possible!
	printf("%d\t\t%f",result->company_id,result->sales_total);
	fflush(stdout);
}

void printSalesByDate(DBRow* result, int length){
	printf("YYYY/MM/DD\t\tTotal\n\n");
	fflush(stdout);
	int i=0;
	for(i=0;i<length;++i){
		printDate(result[i].date);
		printf("\t\t%f\n",result[i].sales_total);
	}
	fflush(stdout);
}

void printRow(DBRow* row){
	printf("%u | ",row->sales_id);
	printDate(row->date);
	printf(" | %u | %f",row->company_id, row->sales_total);
	fflush(stdout);
}

Date promptDateRange(const char* promptPrefix){
	Date retVal;
	int input=0;

	//Ask for year and default to 2015
	printf("Enter %s Year: ", promptPrefix);
	fflush(stdout);
	scanf("%d",&input);
	if(input<1800 || input > 2020){
		input=2015;
	}
	retVal.year=input;

	printf("Enter %s Month: ", promptPrefix);
	fflush(stdout);
	scanf("%d",&input);
	if(input<1 || input >12){
		input =12;
	}
	retVal.month=input;

	printf("Enter %s Day: ", promptPrefix);
	fflush(stdout);
	scanf("%d",&input);
	if(input<1 || input>31){
		input = 7;
	}
	retVal.day=input;
	return retVal;
}

void printDate(Date aDate){
	printf("%d/%d/%d",aDate.year,aDate.month,aDate.day);
	fflush(stdout);
}

//Safely write and flush output stream!
void safeWrite(char* output){
	printf("%s",output);
	fflush(stdout);
}
