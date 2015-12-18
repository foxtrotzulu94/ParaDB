# ParaDB
Parallel DBMS Project written in OpenMPI for COMP428 Fall 2015

## Overview
This is a sample on the use of MPI (Message Passing Interface) in a "large" scale project. 

ParaDB is a single table database storing info on the sales dates and amounts of certain companies. Two queries can be made on the database: 1) Sales total for all companies and 2) Sales within Date Range. 

The information that fills the DB is taken from a set of text files with approximately 50k records each. More info on how it works and how to operate it is down below.

## How It Actually Works
In a nutshell, there are an even number of processes running when the applications starts. Processes are divided into even and odd. Odd processes take care of data storage and retrieval. Even processes take care of processing the query and sending it to the root (Process 0). The root takes care of user interaction, including input/output from standard stream, and of notifying the rest of the even processes of what the query is.

Check the code for more info. Most of the framework of the DB is setup with the mpi_utils.h functions, while odd.h and even.h defines the methods being used by odd and even processes, respectively. Odd processes periodically read a certain amount of entries from the text file in order to simulate updates to the table. This is detailed more in the next section.

## Compiling and Running
You should have the `mpicc` command on your system. Install it with your traditional package manager in linux along with the OpenMPI library bindings. The process is a bit more involved for Windows

This project was made completely using [Eclipse PTP](http://www.eclipse.org/ptp/), so importing and making from there should be straightforward.

You can also run the project on a machine from command line by using:
`mpirun -np $EvenNumber db $ReadAmount`
where 
* `$EvenNumber` is an even positive integer of the nodes you want to run (Max for sample is 10)
* `$ReadAmount` is an integer larger than 0 indicating the size of the periodic input stream to the database

It is recommended, however, that you run it from command line when targetting a real cluster with the following:
`mpirun -np  $EvenNumber -npernode 2 [... other args ...] db $ReadAmount`

## Final Notes
MPI C is not easy to grasp at first. The [API Documentation](https://www.open-mpi.org/doc/v1.8/) is **huge** and it's very easy to make mistakes when calling MPI Functions that will result in either segmentation faults or more specific MPI Errors. Even this solution is not perfect, but it's a good working example of all the basic functionality MPI has to offer (Custom communicators, derived data types, point-to-point communications, collective operations, etc).

I hope you find it useful!
