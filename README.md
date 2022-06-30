# OS PROJECT 

INTRODUCTION
------------

This program was created for the final project of CS F372 Operating System.
It create three processes one parent (P1) and 2 child processes (P2, P3).
The process P1 receives four command-line arguments: two input files containing integer values
and two integers N1 and N2 are the number of integers in the respective files.
P1 sends one array to P2 and another to P3. Both processes P2 and P3 sort the given array and
return the sorted arrays to the parent P1. Then P1 merges both sorted arrays and prints the output
sorted sequence of integers (of size(N1+N2)).

REQUIREMENTS
------------

This program requires the following :

 * Linux/Unix operating system
 * C compiler
 
INSTRUCTIONS
------------

This program requires the following :
* Create 2 files containing the integers in the following format:

   - the data is in a list of numbers where each number  ends with a ',' comma.

   - For eg:
     12,213,221,112,22,

 * Next we need to compile the program using the shell command gcc.
   - gcc main.c
 * Once compiled , we can the run the program with the following 4 command line arguments.
   - ./a.out fileName1 fileName2 n1 n2
   
   Here fileName1 and fileName2 are the names of the file containing the integers. n1 and n2 are the number of integers in the given files respectively.
 * Sample Input and Output 
   - Sample Input
    
     File1: 23,34,4,75,3,54,67,
     
     File2: 12,35,56,44,
     
     Command line: 
     
                   gcc main.c
                   ./a.out file1 file2 7 5
   - Sample Output 
  
     Parent process started 
     
     Both child processes have been started successfully 
   
     Child 1 finished successfully 
   
     Child 2 finished successfully 
     The required sorted array of integers-:
     3 4 12 23 34 35 44 53 54 56 67 75
   
AUTHORS
------------
* Aaryan Agarwal 2019B5A30713P
* Vedant Diwakar 2019B5A30713P
* Dhruv Dixit 2019B5A30713P
