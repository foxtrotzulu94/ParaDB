/*
 * utils.h
 *
 *      Author: foxtrot
 */
#include "db_types.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
/**
 *This file houses all of the utility functions used throughout the program
 *	It includes sorting routines, file reading functions and any general
 *	non-MPI calling code that is needed to support the program
 */
#ifndef UTILS_H_
#define UTILS_H_

#define DEBUG_DB 1

DBRow* readFromStream(FILE* infile, int lineAmount, RowList* output, int* id);

DBRow readFormattedLine(char* line);

//Used to compare dates by qsort
int compareDates(const void * a, const void * b);

int compareDatesExclusive(const void * a, const void * b);

//Used to compare Companies by qsort
int compareCompanies(const void * a, const void * b);

//Returns milliseconds from Epoch for given date
long long convertDateToEpoch(Date* date);

//Returns a Date struct for a particular millisecond representation of Epoch
Date convertEpochToDate(long long epochTime);

//Return the parameters to do an all to all
void getAllToAllParameters(long long start, long long end, int divisions, DBRow* list, int listLenght, int* outAmounts, int* outOffsets);

RowList sumAllSalesForDate(DBRow* list, int listLength);

int dateIsValid(const Date* aDate);

void qlog(char* something);

#endif /* UTILS_H_ */
