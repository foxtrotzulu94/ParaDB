/*
 * utils.h
 *
 *      Author: foxtrot
 */
#include "db_types.h"
#include <stdio.h>
/**
 *This file houses all of the utility functions used throughout the program
 *	It includes sorting routines, file reading functions and any general
 *	non-MPI calling code that is needed to support the program
 */
#ifndef UTILS_H_
#define UTILS_H_

DBRow* readFromStream(int lineAmount, RowList* output);

DBRow readFormattedLine(char* line);



#endif /* UTILS_H_ */
