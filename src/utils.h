/*
 * utils.h
 *
 *      Author: foxtrot
 */
#include "db_types.h"
#include <stdio.h>
#include <stdlib.h>
/**
 *This file houses all of the utility functions used throughout the program
 *	It includes sorting routines, file reading functions and any general
 *	non-MPI calling code that is needed to support the program
 */
#ifndef UTILS_H_
#define UTILS_H_

#define DEBUG_DB 1

DBRow* readFromStream(FILE* infile, int lineAmount, RowList* output);

DBRow readFormattedLine(char* line);

//Used to compare dates by qsort
int compareDates(const void * a, const void * b);

int compareDatesExclusive(const void * a, const void * b);

//Used to compare Companies by qsort
int compareCompanies(const void * a, const void * b);

void qlog(char* something);

#endif /* UTILS_H_ */
