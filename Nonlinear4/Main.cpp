// Nonlinear Data structures
// EECS 2510 : Lab 4
// David Carek

#include "stdafx.h"
#include "AVL.h"
#include <fstream>
#include <iostream>
#include <chrono>

using namespace std;

int main()
{
	string inputFilePath = "C:\\Users\\DMCar\\Desktop\\Shakespeare.txt";

	// set of delimiters for reading in the file
	char delimiters[11] = { 9 , 10 , 13 , 32 , '.' , ',' , '!' , ';' , ':' , '(' , ')' };

	AVL avl("C:\\Users\\DMCar\\Desktop\\test.txt");
	
	std::ifstream inputStream;
	inputStream.open(inputFilePath, std::ios::binary); // binary flag is set to read the file one byte at a time

												  // if we couldn't open the file, let the user know and return
	if (inputStream.fail())
	{
		std::cout << "Could not open file" << std::endl;
		return 0;
	}

	bool isDelimiter = false;
	char nextChar;
	inputStream.get(nextChar);

	char input[50]{ 0 };
	int index = 0;

	chrono::time_point<chrono::system_clock> start, end;
	start = chrono::system_clock::now();

	// keep getting bytes until we have reached the end of the file
	while (!inputStream.eof())
	{
		// loop through the delimiters to check if the character read is one of them
		for each (char delimiter in delimiters)
		{
			// if the character is a delimiter we check to see if the word is empty
			// if the word is not empty it is sent to the trees insert function
			if (nextChar == delimiter)
			{
				if (input[0])
				{
					//cout << input << endl;
					avl.insert(input);
				}
					

				memset(input, 0, sizeof(input));
				index = -1;
				isDelimiter = true;
			}
		}

		// if the character was not a delimiter we need to append it to next word
		if (!isDelimiter)
			input[index] = nextChar;

		index++;

		isDelimiter = false; // reset is delimiter

		if (inputStream.eof())
		{
			int i = 1;
			i++;
		}

		inputStream.get(nextChar); // try to read the next character
	}

	if (input[0] != 0)
	{
		avl.insert(input);
	}

	end = std::chrono::system_clock::now();
	avl.setInsertTime(end - start);

	avl.printStats();

	// then we wait for the user to finish viewing the data
	cout << "Processing finished. Press ENTER to exit" << endl;
	char waitChar;
	cin.get(waitChar);

    return 0;
}

