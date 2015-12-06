/*
 * menu.h
 *
 *  Created on: Nov 21, 2015
 *      Author: foxtrot
 */
#include "db_types.h"
#include <stdio.h>
/**
 *This file contains all functions relevant in drawing to the console
 *	While the inputs and printing features were tested independently
 *	of MPI, it is important that these are only used in the ROOT node
 */
#ifndef MENU_H_
#define MENU_H_

//Draws the menu and returns the user input query to be propagated through the system
Query requestUserInput();

//Shows that the query is being processed
void printQueryInProcess();

//Prints the resulting DBRows of a given Query
void printQueryResults(Query query, DBRow* result, int length);

//Prints the entire menu to console.
void drawMenu();

void printSalesByCompany(DBRow* result, int length);

void printSalesByDate(DBRow* result, int length);

Date promptDateRange(const char* promptPrefix);

void printDate(Date aDate);

#endif /* MENU_H_ */
