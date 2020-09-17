#include <stdio.h>
#include <stdlib.h>

// Create an object which is stored in an array
struct wordDetails
{
    char string[8]; // the string
    int value;  // the value of the word
};

/**
 * Generates a number of random lower case letters.
 * The letters will be stored in the given array.
 */
void generate_letters(int number, char *destination){
    char vowels[] = {'a', 'e', 'i', 'o', 'u'};
    int i;
    unsigned int rand_seed;
    printf("Number < 1000:\n");
    scanf("%d", &rand_seed);
    getchar();
    srandom(rand_seed);
    // initialise the PRNG
    // Half of the letters should be vowels
    for (i=0;i<number/2;i++, destination++) *destination = vowels[random()%5];
    for (;i<number;i++, destination++) *destination = random() % 26 + 'a';
}

/**
 * Tries to read the file in * Each line has one value.
 *
 * Returns the number of lines read or -1 in case of file open error.
 */

int read_array_from_file(const char* filepath, int *array, int array_length){
    FILE *file;
    int i;
    if ((file = fopen(filepath, "r")) == NULL) return -1;
    for (i=0;i<array_length;i++){
        if (fscanf(file, "%d", &array[i]) == EOF) break;
    }
return i; }
/*
*   This Counts how many times a letter appears in an array
*   This returns the new length of the array (dereferencing the duplicated letters)
*/

int countArray(char *array,int number, int *countDestination){
    int i, j, k;
    int count = 1;
    int arraySize = number;

    for(i = 0; i < number; i++){
        for(j = i + 1; j < arraySize; j++){

            if(array[i] == array[j]){
                if((j + 1) <= number){
                    for(k = j; k < number; k++){
                        if(k + 1 < number){
                            array[k] = array[k + 1];
                        }
                    }
                    arraySize--;
                }
                count ++;
            }  
        }
        countDestination[i] = count;
        count = 1;
    }

    return arraySize;
}
/*
* Displays All the letters
*/
void displayLetters(char *letters, int *letterCount, int number){
    int i;
    for(i = 0; i < number; i++){
        printf("%c --> %d\n", letters[i], letterCount[i]);
    }
}
/*
* Checks if the letter is in the list and checks if its already used
*/

int checkLetter(char *letters, char letter, int size, int *letterCount){
    int i;
    int res = 0;

    for(i = 0; i < size; i++){
        if(letter == letters[i] && letterCount[i] > 0){
            res = 1;
            letterCount[i]--;
        }else if(letterCount[i] == 0 && letter == letters[i]){
            printf("Already Used letter %c\n", letter);
        }
    }

    return res;
}

/*
* Reads in a word and checks each character
* Returns if the word was valid
*/
int readWord(char *wordsCollection, int *letterCount, char *letters, int size){
    char input;
    int i;
    int count = 0;
    char string[8];
    int isValid = 1;

    printf("Enter Word:\n");
    while(input != '\n'){
        input = getchar();

        if(count <= 7){
            if(input < 'a' && input >= 'A'){
                input += 32;
            }
            if(input >= 'a' && input <= 'z'){
                int check = 0;
                check = checkLetter(letters, input, size, letterCount);
                if(check == 0){
                    isValid = 0;
                }
                string[count] = input;
            }else if(count == '\n'){
                break;
            }
        }else{
            printf("Too Long!\n");
            while(input != '\n'){
                input = getchar();
            }
            break;
        }
        count++;
    }
    string[count - 1] = '\0';

    if(isValid == 1){
        for(i = 0; i < count; i++){
            wordsCollection[i] = string[i];
        }
    }else{
        printf("Word:%s Is Not Valid, Invalid Letter(s)!\n", string);
    }

    return isValid;
}

/*
* Calculates the total points of a word
* Returns the points after being accumalated
*/

int calculatePoints(char *words, int *points){
    int acc = 0;
    int i = 0;

    while(words[i] != '\0'){
        int letter = words[i] - 'a';
        acc += points[letter];
        i++;
    }

    return acc;
}

/*
*   Checks if The used Wishes to continue or not
*   Returns the option
*/
int askPermission(){
    char option;
    char select;

    printf("Continue?: (y/ n)\n");
    char input = getchar();
    select = input;

    while(input != '\n'){
        input = getchar();
    }

    switch(select){
        case 'y':
        case 'Y':
            option = 1;
            break;
        case 'n':
        case 'N':
            option = 0;
            break;
        default:
        printf("Please Enter Y/n\n");
            option = askPermission();
            break;
    }
    return option;

}

/* 
* Calculates the total letters used for each word
* Returns the amount of letters in that string
*/

int totalLetters(char *word){
    char letter;
    int i = 0;

    while(word[i] != '\0'){
        i++;
    }

    return i;
}

int main(void){
    // variables
    int total = 0;
    int number = 7;
    char filepath[] = "letter_values";
    int i;
    int check = 1;
    int wordCount = 0;

    // arrays
    int pointsOfLetters[26];

    char *letters = malloc(sizeof(char) * number);
    int countOfArray[number];

    struct wordDetails wordCollection[20];
    

    // reading + generating
    read_array_from_file(filepath, pointsOfLetters, 26);
    generate_letters(number, letters);

    int arraySize = countArray(letters, number, countOfArray);

    displayLetters(letters, countOfArray, arraySize);

    while(check){
        printf("------------------\n");
        int duplicateArray[arraySize]; // of countOfArray
        for(i = 0; i < arraySize; i++){
            duplicateArray[i] = countOfArray[i];
        }
        
        int valid = readWord(wordCollection[wordCount].string, duplicateArray, letters, arraySize);

        if(valid == 1){
            int wordPoints = calculatePoints(wordCollection[wordCount].string, pointsOfLetters);
            total += wordPoints;
            wordCollection[wordCount].value = wordPoints;

            printf("You Used: %s Worth: %d Total: %d\n", wordCollection[wordCount].string, wordCollection[wordCount].value, total);
            wordCount++;
        }
        check = askPermission();
        valid = 0;
    }

    // ------Final Message------
    int totalLettersUsed = 0;
    printf("Your Words Are:\n");
    for(i = 0; i < wordCount; i++){
        int x = totalLetters(wordCollection[i].string);
        totalLettersUsed += x;
        printf("\t%s --> %d : Length: %d\n", wordCollection[i].string, wordCollection[i].value, x);
    }
    printf("Total: %d\n", total);
    printf("Total Letters Used: %d\n", totalLettersUsed);
    return 0;
}