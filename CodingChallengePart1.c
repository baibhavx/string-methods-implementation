#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Function Prototypes
char* allocateStringMemory(int);
char** allocateWordsMemory(int);
int strlength(char*);
int wordsCount(char, char*);
int getLengthOfSentence(char**, int);
char* slice(int, int, char*);
char** split(char, char*);
char* join(char, char**, int);
char** getReverseWords(char**, int);
void bubbleSort(char*);
void swap(int, int, char*);
int partition(char*, int, int);
void quickSort(char*, int, int);
void freeAllWords(char**, int);

int main() {
  char* input = allocateStringMemory(255);
  char* sentence = 0;

  printf("Enter Sentence: ");
  fgets(input, 255, stdin);

  sentence = slice(0, (strlength(input) - 2), input); //Dispose newline char
  printf("\nFreeing Input %p\n", input);
  free(input);

  //Print Input as entered
  printf("\nInput as entered: \n\t\t%s\n", sentence);

  char** words = split(' ', sentence);
  int wordsLength = wordsCount(' ', sentence);

  char** reversedWords = getReverseWords(words, wordsLength);
  char* inputReversed = join(' ', reversedWords, wordsLength);

  // Print input with words reversed
  printf("\nInput with words reversed: \n\t\t%s\n", inputReversed);

  printf("\nNow Sorting words...\n");
  char** sortedWords = split(' ', sentence);

  for(int i = 0; i < wordsLength; i++) {
    quickSort(sortedWords[i], 0, strlength(sortedWords[i]) - 1);
  }

  char* sortedSentence = join(' ', sortedWords, wordsLength);

  //Print sorted version of individual words
  printf("\nSorted Words : \n\t\t%s\n", sortedSentence);

  printf("\nNow Freeing All Allocated Memory...\n");

  //Free char*'s inside char**'s
  freeAllWords(words, wordsLength);
  freeAllWords(reversedWords, wordsLength);
  freeAllWords(sortedWords, wordsLength);

  //Free char*'s in main
  free(sentence);
  free(inputReversed);
  free(sortedSentence);

  //Free char**'s
  free(words);
  free(reversedWords);
  free(sortedWords);

  printf("\n\n\n");
  return 0;
}

//Function Definitions

char* allocateStringMemory(int size) {
  return (char*)malloc(size * sizeof(char));
}

char** allocateWordsMemory(int size) {
  return (char**)malloc(size * sizeof(char*));
}

int strlength(char* string) { // My implementation of strlen()
  int count;
  for(count = 0; string[count] != '\0'; count++);
  return count;
}

int wordsCount(char delimeter, char* string) {
  int count = 1;
  for(int i = 0; i < strlength(string); i++) {
    if(string[i] == delimeter) {
      count++;
    }
  }
  return count;
}

int getLengthOfSentence(char** words, int size) {
  int length = 0;
  for(int i = 0; i < size; i++) {
    length += strlength(words[i]);
  }
  return length + (size - 1); // add (size - 1) to account for spaces
}

char* slice(int startIdx, int endIdx, char* string) {
  int i = 0, j = 0;
  // Allocate one more for '\0'
  char* newWord = allocateStringMemory((endIdx - startIdx) + 2);
  for(i = startIdx; i <= endIdx; i++) {
    newWord[j] = string[i];
    j++;
  }
  newWord[j] = '\0';
  return newWord;
}

char** split(char delimeter, char* string) {
  int i = 0, j = 0, startIdx = 0, endIdx = 0;
  int length = strlength(string);
  char** words = 0;
  char* newWord = 0;
  int numOfWords = wordsCount(delimeter, string);

  if(numOfWords == 1) { // Allocate 1 space and construct array
    words = allocateWordsMemory(1);
    words[0] = slice(0, length - 1, string);
  } else { // Allocate space for individual words
    words = allocateWordsMemory(numOfWords);
    // Add newWord to words
    for(i = 0; i < length; i++) {
      if(string[i] == delimeter) {
        endIdx = i - 1;
        newWord = slice(startIdx, endIdx, string);
        words[j] = newWord;
        j++;
        // update startIdx
        startIdx = i + 1;
      }
    }
    // Extract last word, utilize value of j
    words[j] = slice((endIdx + 2), strlength(string) - 1, string);
  }

  return words;
}

char* join(char delimeter, char** words, int numOfWords) {
  int i = 0, j = 0, k = 0;
  char* currWord = 0;
  int sentenceLength = getLengthOfSentence(words, numOfWords);
  char* jointSentence = allocateStringMemory(sentenceLength + 1);

  for(i = 0; i < numOfWords; i++) {
    currWord = words[i];
    for(j = 0; j < strlength(currWord); j++) {
      jointSentence[k] = currWord[j];
      k++;
    }
    if(i != (numOfWords - 1)) {
      jointSentence[k] = delimeter;
      k++;
    }
  }
  return jointSentence;
}

char** getReverseWords(char** words, int length) {
  int i = 0;
  char* corrWord = 0;
  char** reversedWords = allocateWordsMemory(length);

  for(i = 0; i < length; i++) {
    corrWord = words[(length - 1) - i];
    reversedWords[i] = slice(0, strlength(corrWord) - 1, corrWord);
  }
  return reversedWords;
}

//Bubble sort is NOT used in this program
void bubbleSort(char* string) {
  int sorted = 0;
  char temp;
  int j = 0;

  while(sorted == 0) {
    sorted = 1;
    for(int i = 0; i < strlength(string) - 1; i++) {
      j = i + 1;
      if(tolower(string[j]) < tolower(string[i])) {
        temp = string[j];
        string[j] = string[i];
        string[i] = temp;
        sorted = 0;
      }
    }
  }
}

//Quick Sort Helper Function
void swap(int i, int j, char* string) {
  char temp = string[i];
  string[i] = string[j];
  string[j] = temp;
}

//Quick Sort Helper Function
int partition(char* string, int start, int end) {
  int pivot = string[end];
  int i = start - 1;

  for(int j = start; j <= end - 1; j++) {
    if(tolower(string[j]) <= tolower(pivot)) {
      i++;
      swap(i, j, string);
    }
  }
  swap(i + 1, end, string);
  return i + 1;
}

void quickSort(char* string, int start, int end) {
  if(start < end) {
    int pIdx = partition(string, start, end);

    quickSort(string, start, pIdx - 1);
    quickSort(string, pIdx + 1, end);
  }
}

void freeAllWords(char** words, int length) {
  for(int i = 0; i < length; i++) {
  char* word = words[i];
  printf("\nFreeing string %p\n", word);
  free(word);
  }
}
