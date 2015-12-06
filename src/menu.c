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
}

//Prints the resulting DBRows of a given Query
void printQueryResults(Query query, DBRow* result, int length){
	//TODO: Format printing
}

//Prints the entire menu to console.
void drawMenu(){
	printf("\nParallel Database System (Powered by OpenMPI)\n\n");
	printf("\t1. Company Sales\n");
	printf("\t2. Sales By Date\n");
	printf("\t3. Exit\n\n");
	printf("Option: ");
	//Return control to the Request User Input menu
}

void printSalesByCompany(DBRow* result, int length){
	//TODO: Format printing
}

void printSalesByDate(DBRow* result, int length){
	//TODO: Format printing
}

void printRow(DBRow* row){
	printf("%u | ",row->sales_id);
	printDate(row->date);
	printf(" | %u | %f",row->company_id, row->sales_total);
}

Date promptDateRange(const char* promptPrefix){
	Date retVal;
	int input=0;

	//Ask for year and default to 2015
	printf("Enter %s Year: ", promptPrefix);
	scanf("%d",&input);
	if(input<1900 || input > 2020){
		input=2015;
	}
	retVal.year=input;

	printf("Enter %s Month: ", promptPrefix);
	scanf("%d",&input);
	if(input<1 || input >12){
		input =12;
	}
	retVal.month=input;

	printf("Enter %s Day: ", promptPrefix);
	scanf("%d",&input);
	if(input<1 || input>31){
		input = 7;
	}
	retVal.day=input;
	return retVal;
}

void printDate(Date aDate){
	printf("%d/%d/%d",aDate.year,aDate.month,aDate.day);
}
