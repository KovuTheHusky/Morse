/*
 * main.cpp
 *
 *  Created on: Oct 8, 2010
 *      Author: Kevin Breslin
 */

#include <iostream.h>;

const unsigned short MAXIMUM_LENGTH = 1024; // How long can the input string be
unsigned char* chars = (unsigned char*)malloc(20); // We're compressing this into 20 bytes

char* getMorseCode(char index) {
	index -= 'a'; // Zero based index, please
	unsigned int firstBit = index * 6; // Find what bit the character starts at
	unsigned int firstByte = firstBit / 8; // Now find the first byte you have to get
	unsigned short data = (chars[firstByte] << 8) | chars[firstByte + 1]; // Get the two bytes the character could stretch across
	data = data >> 10 - (firstBit - firstByte * 8); // Shift the six bits to the rightmost portion
	unsigned short length = (data >> 4 & 0x3) + 1; // Mask the two-bit length portion
	unsigned short morse = data & 0xF; // Mask the two-bit Morse code portion
	char* ret = (char*)malloc(sizeof(char) * 8); // Maximum length is 4 symbols, 3 spaces, and 1 terminating character
	short cur = 0;
	for (int i = 0; i < length; ++i) {
		if (i > 0)
			ret[cur++] = ' ';

		if ((morse >> 4 - i - 1) & 1 == 1)
			ret[cur++] = '-';
		else
			ret[cur++] = '.';
	}
	ret[cur] = '\0'; // We need to know where our data ends or we'll get garbage
	return ret;
}

int main() {
	// Assemble the bits for our compact data structure
	char* bits = (char*)"0101001110001110101010000000001100101011001100000100001101111010101101000111000110001011101101101111011001001000000010001000101100011001101110011110111111000000"; // Last four bits are garbage
	for (int i = 0; i < 20 * 8; i += 8) { // For every byte, make an equal value char and put it in our data structure
		chars[i / 8] = (bits[i] - '0') * 128 + (bits[i + 1] - '0') * 64 + (bits[i + 2] - '0') * 32 + (bits[i + 3] - '0') * 16 + (bits[i + 4] - '0') * 8 + (bits[i + 5] - '0') * 4 + (bits[i + 6] - '0') * 2 + (bits[i + 7] - '0') * 1;
	}
	//free(bits);
	bits = NULL; // We're not going to use this anymore, just needed it to fill our 20-byte structure

	// Lets ask for a string
	cout << "Please enter a string (non-alphabetic characters will be ignored, enter \"/quit\" to quit):\n";
	char* input = (char*)malloc(sizeof(char) * MAXIMUM_LENGTH);
	while (true) {
		cin.getline(input, MAXIMUM_LENGTH);

		// Does the user want to quit
		if (strcmp(input, "/quit") == 0)
			break;

		// Now convert the string to morse code
		bool inWord = false; // Only put spaces between characters in a word, keep track of that
		for (unsigned int i = 0; i < strlen(input); ++i) {
			char c = tolower(input[i]); // Morse code is case insensitive

			if (c == ' ') { // Spaces are special cases
					cout << "       ";
					inWord = false; // We're outside of a word now, don't put spaces
			}

			if (c >= 'a' && c <= 'z') { // If it's alphabetic, find the morse code
				if (inWord)
					cout << "   ";
				inWord = true; // We're inside a word now, put spaces
				char* code = getMorseCode(c);
				cout << code;
				free(code); // No memory leaks, please
				code = NULL;
			}
		}
		cout << "\n"; // New line for input in the next run of the loop
	}
	free(input);
	input = NULL;
}
