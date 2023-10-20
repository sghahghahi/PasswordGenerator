#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>

/* Helper function to convert string to lower case */
void stringToLower(char* str)
{
	while (*str != '\0') {
		*str = tolower(*str);
		str++;
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
	const int MAX_USERNAME_LENGTH = 100;
	const int MAX_PASSWORD_LENGTH = 100;

	char lowercase_username[MAX_USERNAME_LENGTH];
	char lowercase_password[MAX_PASSWORD_LENGTH];

	strcpy(lowercase_username, username);
	strcpy(lowercase_password, password);

	stringToLower(lowercase_username);
	stringToLower(lowercase_password);

	// For every char in password, iterate through entire username to see if it's a match
	for (int i = 0; i < strlen(lowercase_password); i++) {
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
bool isStrongPassword(const char * username, const char * password) {
	const int MIN_PASSWORD_LENGTH = 8;

	if (strlen(password) < MIN_PASSWORD_LENGTH) return false;

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

bool isStrongDefaultPassword(const char *username, const char *password) {
	const int MIN_PASSWORD_LENGTH = 8, MAX_PASSWORD_LENGTH = 15;

	if (strlen(password) < MIN_PASSWORD_LENGTH || strlen(password) > MAX_PASSWORD_LENGTH) return false;

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

/* Helper function to generate default passwords */
void generatePassword(char* default_password, char* valid_characters, int number_of_valid_characters)
{
	const int MIN_PASSWORD_LENGTH = 8, MAX_PASSWORD_LENGTH = 15;
	const int PASSWORD_LENGTH = (rand() % (MAX_PASSWORD_LENGTH - MIN_PASSWORD_LENGTH + 1)) + MIN_PASSWORD_LENGTH;

	char random_password[PASSWORD_LENGTH + 1];

	for (int i = 0; i < PASSWORD_LENGTH; i++) {
		int n = rand() % number_of_valid_characters;
		random_password[i] = valid_characters[n];
	}
	random_password[PASSWORD_LENGTH] = '\0';

	strcpy(default_password, random_password);
}

/**
 * Generated password must be >= 8 && <= 15 in length
 * Doesn't have to satisfy the 4 consecutive characters requirement
 * Allowed chars are lowercase letters, uppercase letters, and numbers
 * Call rand() % (15 - 8 + 1) + 8 to get length of password -> change raw numbers to variables lower_bound and upper_bound
 * Create array of length (# of lowercase letters + # of uppercase letters + # of digits). These will be our available characters to create a password with
 * Call rand() % length, get character at index of characters from number returned from rand() % length
*/
void generateDefaultPassword(char * default_password, const char * username) {
	const int NUMBER_OF_LOWERCASE_CHARS = 26, NUMBER_OF_UPPERCASE_CHARS = 26, NUMBER_OF_DIGITS = 10;
	const int NUMBER_OF_VALID_CHARACTERS = NUMBER_OF_LOWERCASE_CHARS + NUMBER_OF_UPPERCASE_CHARS + NUMBER_OF_DIGITS;
	char valid_characters[NUMBER_OF_VALID_CHARACTERS + 1];
	int i = 0;

	/* Populate array with valid characters */
	for (char ch = 'a'; ch <= 'z'; ch++) {
		valid_characters[i++] = ch;
	}
	for (char ch = 'A'; ch <= 'Z'; ch++) {
		valid_characters[i++] = ch;
	}
	for (char ch = '0'; ch <= '9'; ch++) {
		valid_characters[i++] = ch;
	}
	valid_characters[i] = '\0';

	while (!isStrongDefaultPassword(username, default_password)) {
		generatePassword(default_password, valid_characters, NUMBER_OF_VALID_CHARACTERS);
	}
}

int main(void)
{
	srand(time(0));

	char username[100];
	char password[100];

	printf("Enter a username: ");
	scanf(" %99[^\n]", username);

	while (1) {
		printf("Enter a password: ");
		scanf(" %99[^\n]", password);

		if (isStrongPassword(username, password)) {
			printf("Strong password!\n");
			break;
		} else {
			printf("Your password is weak. Try again!\n");
		}
	}

	printf("\n");

	char default_password[16] = "test";

	printf("Generating a default password...\n");
	generateDefaultPassword(default_password, username);
	printf("Generated default password: %s\n", default_password);

	return 0;
}
