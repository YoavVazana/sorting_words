#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

/*Define the maximum length for file names*/
#define MAX_FILE_LENGTH 100



/*Define a structure to store information about a word*/
typedef struct {
	
	int *line_numbers;/*Array to store line numbers where the word appears*/

	char *word;/*Pointer to store the actual word*/

	int counter;/*Counter to keep track of the number of occurrences of the word*/

} word_data;



/*Define a structure to store a list of word_data structures*/
typedef struct {

	word_data *words;/*Array to store word_data structures*/

	int count;/*Counter to keep track of the number of unique words*/

} word_list;



/*Function to convert the first character of a word to lowercase*/
void convert_uppercase(char *word) {

	if(word[0] >= 'A' && word[0] <= 'Z') {

		word[0] += 32;

	}
}



/*Function to add a word to the word_list structure*/
void add_word(word_list *arr_words, int line, char *current_word,int size) {

	int i,j;

	size_t length = (size_t)size;

	/*Convert the first character of the word to lowercase*/
	convert_uppercase(current_word);

	/*Checking if the word has appeared before*/
	for(i = 0; i < arr_words->count; i++) {

		if(strcmp(arr_words->words[i].word, current_word) == 0){

		/*Check if the line number already exists for the current word*/
			for(j = 0; j < arr_words->words[i].counter; j++){

				if(arr_words->words[i].line_numbers[j] == line){

					return;
				}
			}

		/*If the line number does not exist, add it to the array*/
			arr_words->words[i].counter++;

			arr_words->words[i].line_numbers = realloc(arr_words->words[i].line_numbers, arr_words->words[i].counter * sizeof(int));

			arr_words->words[i].line_numbers[arr_words->words[i].counter - 1] = line;
		return;

		}
	}


	/*If the word does not exist in the word_list, add a new entry*/
	arr_words->count++;

	arr_words->words = realloc(arr_words->words, arr_words->count * sizeof(word_data));


	/*Allocate memory for the new word and copy it*/
	arr_words->words[arr_words->count - 1].word = malloc(length + 1);

	strncpy(arr_words->words[arr_words->count-1].word, current_word,length);

	 arr_words->words[arr_words->count - 1].word[length] = '\0';

	/*Initialize the counter for the new word*/
	arr_words->words[arr_words->count - 1].counter = 1;

	/*Allocate memory for the line number array and add the current line*/
	arr_words->words[arr_words->count -1].line_numbers = malloc(sizeof(int));

	arr_words->words[arr_words->count - 1].line_numbers[0] = line;
	
}


/*Checking if a character is an English letter*/
int check_char(char ch){

	if((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z'))
		return 1;

	return 0;
}


	
/*Reads the content of a file, extracts words along with their line numbers*/				
void read_file(char *file_name, word_list *arr_words) {

	FILE *file = fopen(file_name,"r");

	char curr_char,prev_char;

	char *word = malloc(sizeof(char));

	int word_index = 0;

	int word_size = 1;

	int curr_line = 1;

	int size = 0;

	int is_new_line = 0;


	/*Checking whether the file is correct before starting reading*/
	if(file == NULL) {

		printf("Unable to open file.\n");
		
		exit(EXIT_FAILURE);
	}

	
	/*Start reading by character*/
	while((curr_char = fgetc(file)) != EOF) {
		
		if(curr_char == '\n') {

				curr_line++;

			}

		/*When we reach the end of a word, we will send it to the add_word function and reset the values ​​for the next word*/
		if((ispunct(curr_char) || curr_char == ' ' || curr_char == '\n' || !check_char(curr_char)) && check_char(prev_char)) {

			if(curr_char == '\n' && check_char(prev_char)) {

				curr_line--;
				is_new_line = 1;

			}
			

			word[word_index] = '\0';

			if(word_index > 0) {

				add_word(arr_words, curr_line, word, size);

				free(word);

				word = malloc((word_size + 1) * sizeof(char));
                		
				size = 0;

				word_index = 0;

				word_size = 0;
			}

			if(is_new_line){
				curr_line++;
				is_new_line = 0;
			}


		/*If the current character is a letter we will add a space in the word and position the character*/
		} else if(check_char(curr_char)){

			if(word_index > 0) {

				word_size += 2 * word_index;

				word = realloc(word, word_size * sizeof(char));
				
			}

			size++;

			word[word_index++] = curr_char;


		}
		prev_char = curr_char;

	}

	
	fclose(file);

	free(word);
}



/*Sorting the array of words*/
void stru_sort(word_list *arr_words) {
	int i,j,k,min_len;
	word_data temp;
	
	for(i = 0; i < arr_words->count - 1; i++){
		for(j = 0; j < arr_words->count - i - 1; j++) {
			
			min_len = strlen(arr_words->words[j].word) < strlen(arr_words->words[j+1].word) ? strlen(arr_words->words[j].word) : strlen(arr_words->words[j+1].word);

			for(k = 0; k < min_len; k++){
				if(arr_words->words[j].word[k] > arr_words->words[j+1].word[k]){
					temp = arr_words->words[j];
					arr_words->words[j] = arr_words->words[j + 1];
					arr_words->words[j + 1] = temp;
					break;
				}else if(arr_words->words[j].word[k] < arr_words->words[j + 1].word[k]){
					break;
				}
			}
		}
	}
	
}



/*Printing the words and lines that the son appeared*/
void print_index(word_list *arr_words) {

	int i,j,first;

	stru_sort(arr_words);

	for(i = 0; i < arr_words->count; i++){

		printf("%s appears in lines ",arr_words->words[i].word);
			first = 0;
		for(j = 0; j < arr_words->words[i].counter; j++){
			if(!first){
				printf("%d", arr_words->words[i].line_numbers[j]);
				first = 1;
			} else {
				printf(",%d", arr_words->words[i].line_numbers[j]);
			}
		}

	printf("\n");

	}
}

	



/*The main program from which we will take the file and the user and transfer it to processing*/
int main() 
{
	char file_name[MAX_FILE_LENGTH];

	word_list arr_words = {NULL, 0};

	int i;

	printf("Enter the file name: ");

	scanf("%s", file_name); 
	
	read_file(file_name, &arr_words);

	print_index(&arr_words);

	for(i = 0; i < arr_words.count; i++) {

		free(arr_words.words[i].line_numbers);

		free(arr_words.words[i].word);
	}

	free(arr_words.words);
 
	return 0;
	
}
	



