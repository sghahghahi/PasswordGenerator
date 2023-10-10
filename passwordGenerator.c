#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>

// Helper function to convert string to lower case
void stringToLower(char* s)
{
	while (*s != '\0') {
		*s = tolower(*s);
		s++;
	}
}

/**
 * Helper function to determine if username is in password
 * Need to check if password contains username (not case-sensitive)
 * Can check if username is substring of password
 * Convert both username and password to lower case and check for substring (username in password)
*/
bool usernameInPassword(const char* username, const char* password)
{

	char lowercase_username[16];
	char lowercase_password[16];
	strcpy(lowercase_username, username);
	strcpy(lowercase_password, password);
	stringToLower(lowercase_username);
	stringToLower(lowercase_password);

	for (int i = 0; *(lowercase_password + i) != '\0'; i++) {
		int j;
		for (j = 0; j < strlen(lowercase_username); j++) {
			if (lowercase_password[i + j] != lowercase_username[j]) {
				break;
			}
		}
		if (j == strlen(lowercase_username)) {
			return true;
		}
	}

	return false;
}

/**
 * Only accept letters and numbers
 * Loop through password, check if has 1 upper case, 1 lower case, 1 digit, and 4 consecutive letters
 * If any of those four fail, weak password -> return false
 * Flags for lower case, upper case, digit, and consecutive letters
 * If any of those flags are false, weak password -> return false
*/
bool isStrongPassword(const char *username, const char *password)
{
	if (strlen(password) < 8) return false;


	int consecutive_chars = 0;
	bool lower_flag = false, upper_flag = false, digit_flag = false, consecutive_flag = false;

	for (int i = 0; i < strlen(password); i++) {
		char ch = *(password + i);
		if (islower(ch)) {
			lower_flag = true;
			consecutive_chars++;
		} else if (isupper(ch)) {
			upper_flag = true;
			consecutive_chars++;
		} else if (isdigit(ch)) {
			digit_flag = true;
			consecutive_chars = 0;
		} else { // If character other than letter or number detected
			return false;
		}
		if (consecutive_chars == 4) consecutive_flag = true;
	}

	bool user_in_pwd = usernameInPassword(username, password);

	if (!lower_flag || !upper_flag || !digit_flag || !consecutive_flag || user_in_pwd) return false;

	return true;
}

bool isStrongDefaultPassword(const char *username, const char *password)
{
	if (strlen(password) < 8 || strlen(password) > 15) return false;
	
	bool lower_flag = false, upper_flag = false, digit_flag = false;
	for (int i = 0; i < strlen(password); i++) {
		char ch = *(password + i);
		if (islower(ch)) {
			lower_flag = true;
		} else if (isupper(ch)) {
			upper_flag = true;
		} else if (isdigit(ch)) {
			digit_flag = true;
		} else {
			return false;
		}
	}
	bool user_in_pwd = usernameInPassword(username, password);

	if (!lower_flag || !upper_flag || !digit_flag || user_in_pwd) return false;

	return true;
}

// Helper function to generate default passwords
void generatePassword(char* default_password, char* valid_characters, int number_of_valid_characters)
{
	const int lower_bound = 8, upper_bound = 15;
	const int password_length = (rand() % (upper_bound - lower_bound + 1)) + lower_bound;
	char random_password[password_length + 1];

	for (int i = 0; i <= password_length; i++) {
		int n = rand() % number_of_valid_characters;
		char ch = valid_characters[n];
		random_password[i] = ch;
	}
	random_password[password_length] = '\0';

	strcpy(default_password, random_password);
}

/**
 * Generated password must be >= 8 && <= 15 in length
 * Doesn't have to satisfy the 4 consecutive characters requirement
 * Allowed chars are lowercase letters, uppercase letters, and numbers
 * Call rand() % (15 - 8 + 1) + 8 to get length of password -> change raw numbers to variables lower_bound and upper_bound
 * Create array of length (# of lowercase letters + # of uppercase letters + # of numbers). This will be our available characters to create a password with
 * Call rand() % length, get character at index of characters from number returned from rand() % length
*/
void generateDefaultPassword(char *default_password, const char *username)
{

	const int number_of_lowercase_chars = 26, number_of_uppercase_chars = 26, number_of_numbers = 10;
	const int number_of_valid_characters = number_of_lowercase_chars + number_of_uppercase_chars + number_of_numbers;
	char valid_characters[number_of_valid_characters];
	int index = 0;

	// Populate array of valid characters
	for (char ch = 'a'; ch <= 'z'; ch++) {
		valid_characters[index++] = ch;
	}
	for (char ch = 'A'; ch <= 'Z'; ch++) {
		valid_characters[index++] = ch;
	}
	for (char ch = '0'; ch <= '9'; ch++) {
		valid_characters[index++] = ch;
	}

	printf("Generating a default password...\n");
	while (!isStrongDefaultPassword(username, default_password)) {
		generatePassword(default_password, valid_characters, number_of_valid_characters);
	}
	printf("Generated a default password: %s\n", default_password);
}

int main(void)
{
	srand(time(0));

	char username[16];
	char password[16];

	printf("Enter a username: ");
	scanf("%s", username);

	bool strong = false;
	while (1) {
		printf("Enter a password: ");
		scanf("%s", password);

		if (isStrongPassword(username, password)) {
			printf("Strong password!\n");
			break;
		} else {
			printf("Your password is weak. Try again!\n");
		}
	}

	printf("\n");

	char default_password[16] = "test";
	generateDefaultPassword(default_password, username);

	return 0;
}
