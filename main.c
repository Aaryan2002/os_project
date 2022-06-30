#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
// sorting function used in mergeSort
void merge(int arr[], int l, int m, int r)
{
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;

    int L[n1], R[n2];

    for (i = 0; i < n1; i++)
    {
        L[i] = arr[l + i];
    }
    for (j = 0; j < n2; j++)
    {
        R[j] = arr[m + 1 + j];
    }

    i = 0;
    j = 0;
    k = l;
    while (i < n1 && j < n2)
    {
        if (L[i] <= R[j])
        {
            arr[k] = L[i];
            i++;
        }
        else
        {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1)
    {
        arr[k] = L[i];
        i++;
        k++;
    }

    while (j < n2)
    {
        arr[k] = R[j];
        j++;
        k++;
    }
}
// Sorting algortithm 
void mergeSort(int arr[], int l, int r)
{
    if (l < r)
    {
        int m = l + (r - l) / 2;

        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);

        merge(arr, l, m, r);
    }
}
// main function
int main(int argc, char *argv[])
{
    //Checking whether correct number of command line arguments are passed
    if (argc != 5)
    {
        fprintf(stderr, "Incorrect command line arguments.\n");
        return 1;
    }

    // We use 4 pipes, 2 pipes for each child processes to create a bidirection data flow.
    // First pipe to send input array from parent to child 1
    // Second pipe to send sorted from child 1 to parent
    // third pipe to send input array from parent to child 2
    // fourth pipe to send sorted from child 2 to parent

    int p1[2], p2[2], p3[2], p4[2]; // 4 pipes
    pid_t c1, c2 = 10; // defining two variable to store the child processes's pid
    int n1 = atoi(argv[3]);// Getting the number of elements in file1 from the command line arguments
    int n2 = atoi(argv[4]);// Getting the number of elements in file1 from the command line arguments
    int array1[n1];
    int array2[n2];

    // Checking whether any of the four pipes have failed.
    if (pipe(p1) == -1)
    {
        fprintf(stderr, "Pipe Failed");
        return 1;
    }
    if (pipe(p2) == -1)
    {
        fprintf(stderr, "Pipe Failed");
        return 1;
    }
    if (pipe(p3) == -1)
    {
        fprintf(stderr, "Pipe Failed");
        return 1;
    }
    if (pipe(p4) == -1)
    {
        fprintf(stderr, "Pipe Failed");
        return 1;
    }

    // making two child processes using fork.
    c1 = fork();
    if (c1 != 0)
    {
        c2 = fork();
    }
    // checking if fork has failed
    if (c1 < 0 || c2 < 0)
    {
        fprintf(stderr, "fork Failed");
        return 1;
    }

    // Parent process
    else if (c1 > 0 && c2 > 0)
    {
        char buffer1[100]; 
        char buffer2[100];
        // opening and reading data from file 1
        int fd1 = open(argv[1], O_RDONLY);
        
        // storing data from file1 in an character array named buffer1
        read(fd1, buffer1, 100);
        close(fd1);
        // converting char array buffer1 to integer array named array1
        for (int i = 0, j = 0, number = 0; buffer1[i] != ';'; i++)
        {
            if (buffer1[i] != ',')
            {
                number = number * 10 + (buffer1[i] - '0');
            }
            else
            {
                array1[j] = number;
                j++;
                number = 0;
            }
        }
        close(p1[0]); // Close reading end of first pipe
        // Write input array and close writing end of first pipe
        write(p1[1], array1, sizeof(array1));
        close(p1[1]);
        
        // opening and reading data from file 2
        int fd2 = open(argv[2], O_RDONLY);
        // storing data from file2 in an character array named buffer2
        read(fd2, buffer2, 100);
        close(fd2);
	
	// converting char array buffer2 to integer array named array2
        for (int i = 0, j = 0, number = 0; buffer2[i] != ';'; i++)
        {
            if (buffer2[i] != ',')
            {
                number = number * 10 + (buffer2[i] - '0');
            }
            else
            {
                array2[j] = number;
                j++;
                number = 0;
            }
        }
        close(p3[0]); // Close reading end of third pipe

        // Write input array and close writing end of third pipe.
        write(p3[1], array2, sizeof(array2));
        close(p3[1]);

        int status;
        // Wait for child process 1 to send an array
        waitpid(c1,&status, 0);

        close(p2[1]); // Close writing end of second pipe

        // Reading sorted array1 from child 1.
        // close reading end.
        
        read(p2[0], array1, sizeof(array1));
        close(p2[0]);

        

        // Wait for child process 2 to send a array
        waitpid(c2,&status, 0);

        close(p4[1]); // Close writing end of fourth pipe

        // Read sorted array2 from child process 2
        // close reading end.
        read(p4[0], array2, sizeof(array2));
        
        close(p4[0]);
        
        
        // after receiving the sorted arrays, array1 and array2, we again print them in the ascending\sorted order .
        int i =0;
        int j = 0;
        while (i < n1 && j < n2)
        {
            // Check if current element of first
            // array is smaller than current element
            // of second array. If yes, store first
            // array element and increment first array
            // index. Otherwise do same with second array
            if (array1[i] < array2[j])
                printf(" %d ",array1[i++]);
            else
                printf(" %d ",array2[j++]);
        }

        // Store remaining elements of first array
        while (i < n1)
            printf(" %d ",array1[i++]);

        // Store remaining elements of second array
        while (j < n2)
            printf(" %d ",array2[j++]);
    }
    //parent process ends
    
    
    // child process 1
    else if (c1 == 0)
    {
        close(p1[1]); // Close writing end of first pipe

        // Read an array using first pipe
        read(p1[0], array1, sizeof(array1));

        // Sort array 1
        mergeSort(array1,0,n1-1);

        // Close both reading ends
        close(p1[0]);
        close(p2[0]);

        // Write sorted array and close writing end
        write(p2[1], array1, sizeof(array1));
        close(p2[1]);

        exit(0);
    }
    // child process 2
    else if (c2 == 0)
    {
        close(p3[1]); // Close writing end of third pipe

        // Read an array using third pipe
        read(p3[0], array2, sizeof(array2));

         // Sort array 2
        mergeSort(array2,0,n2-1);

        // Close both reading ends
        close(p3[0]);
        close(p4[0]);

        // Write sorted array and close writing end
        write(p4[1], array2, sizeof(array2));
        close(p4[1]);

        exit(0);
    }
}
