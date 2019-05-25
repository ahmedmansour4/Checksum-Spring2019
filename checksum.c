// Name: Ahmed Mansour
// NID: ah505081
// CIS3360 Spring 2019
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

// This function simpy prints out the passed string
void echoInputFile(char *inputString)
{
	char buffer;
	int len = strlen(inputString);
	int i = 0;
	printf("\n");
	for (i = 0; i < len; i++)
	{
		// After 80 characters are printed, print a newline character
		if(i % 80 == 0 && i != 0)
			printf("\n");
		printf("%c", inputString[i]);
	}
	printf("\n");
}

// This function calculates the checksum of the passed string
long unsigned int checksum(char *inputString, int checksumSize)
{
	long unsigned int sum = 0;
	int character = 0;
	int i = 0;
	int len = strlen(inputString);
	
	// If checksumSize is 8, add each character individually
	if (checksumSize == 8)
	{
		for (i = 0; i < len; i++)
		{
			sum += inputString[i];
			
			// If sum is greater than 2^8 - 1, subtract the sum by 256
			if(sum > 255)
				sum = sum - 256;
		}

	}
	// If checksumSize is 16, combine two letters together and add them
	else if (checksumSize == 16)
	{
		for (i = 0; i < len; i = i + 2)
		{
			sum += inputString[i] * 256 + inputString[i + 1];
			// If sum is greater than 2^16 - 1, subtract the sum by 65536
			if (sum > 65535)
				sum = sum - 65536;
		}
	}
	// If checksumSize is 32, combine 4 letters together and add them
	else if (checksumSize == 32)
	{
		for (i = 0; i < len; i = i + 4)
		{
			sum += inputString[i] * 16777216 + inputString[i + 1] * 65536 + inputString[i + 2] * 256 + inputString[i + 3];
			// If sum is greater than INT_MAX - 1, subtract the sum by INT_MAX
			if (sum > 4294967295)
				sum = sum - 4294967296;
		}
	}
	
	return sum;
}

// This function pads the passed string with X's depending on the checksumSize
void padX(char *inputString, int checksumSize)
{
	int xNeeded, i = 0;
	int len = strlen(inputString);
	
	// If checksumSize is 16, pad 0 or 1 X's
	if (checksumSize == 16 && len % 2 != 0)
		inputString[len] = 'X';
	
	// If checksumSize is 32, pad 0, 1, 2, or 3 X's
	if (checksumSize == 32)
	{
		xNeeded = 4 - len % 4;
		for (i = 0; i < xNeeded; i++)
			inputString[len + i] = 'X';
	}
	
}

int main(int argc, char **argv)
{
	int checksumSize;
	int len;
	char *inputString = calloc(1024, sizeof(char));
	FILE *fp;
	
	// If user does not supply enough arguments, exit the program
	if (argc < 2)
	{
		fprintf(stderr, "Correct syntax: ./a.out (inputfile.txt) (checksumSize)\n");
		return 0;
	}
	// Open the given file
	fp = fopen(argv[1], "r");
	
	// If the file doesn't exist in the directory, exit the program
	if (fp == NULL)
	{
		fprintf(stderr, "Specified file \"%s\" does not exist!\n", argv[1]);
		return 0;
	}
	
	// Store the checksum size value
	checksumSize = atoi(argv[2]);
	// If user doesn't supply a value that is allowed, exit the prgram
	if (checksumSize != 8 && checksumSize != 16 && checksumSize != 32)
	{
		fprintf(stderr, "Valid checksum sizes are 8, 16, or 32\n");
		return 0;
	}
	
	// Store the data in a string
	fread(inputString, sizeof(char), 1024, fp);
	
	// Pad the string with X's as needed
	padX(inputString, checksumSize);
	
	// Get the number of characters in the string
	len = strlen(inputString);
	
	// Print the string to the console
	echoInputFile(inputString);
	
	// Print our result of calculating the checksum
	printf("%2d bit checksum is %8lx for all %4d chars\n", checksumSize, checksum(inputString, checksumSize), len);
	
	return 0;
}