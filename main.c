#include <openssl/evp.h>
#include <string.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define NUM_THREADS	3


struct thread_data
{

	int thread_id;
	int producer;
	const char* cracked_paswd;
	const char* dictionary_word;
	int  len;
};

struct thread_data thread_data_array[NUM_THREADS];

int check_letter(const char* msg)
{
	// Checking for password dictionary criteria to pass to producer
	printf("\nChecking message:%s\n", msg);
	if (islower(msg[0]))
	{
		return 0;
	}
	else if (isupper(msg[0]))
	{
		if (islower(msg[1]))
		{
			return 1;
		}

		else if (isupper(msg[1]))
		{
			return 2;
		}

	}
	else
	{
		fprintf(stderr, "Error faulty string");
		return 3;
	}
}
int check_lines(char* filename)
{
	FILE* fp;
	int lines = 0;
	char chr;
	fp = fopen(filename, "r");
	while (!feof(fp))
	{
		chr = fgetc(fp);
		if (chr == '\n' || chr == EOF)
		{
			++lines;
		}
	}
	fclose(fp);
	
	printf("\nLines counted: %d\n\n", lines);
	return lines;
}

void bytes2md5(const char* data, int len, char* md5buf) {
	EVP_MD_CTX* mdctx = EVP_MD_CTX_new();
	const EVP_MD* md = EVP_md5();
	unsigned char md_value[EVP_MAX_MD_SIZE];
	unsigned int md_len, i;
	EVP_DigestInit_ex(mdctx, md, NULL);
	EVP_DigestUpdate(mdctx, data, len);
	EVP_DigestFinal_ex(mdctx, md_value, &md_len);
	EVP_MD_CTX_free(mdctx);
	for (i = 0; i < md_len; i++) {
		snprintf(&(md5buf[i * 2]), 16 * 2, "%02x", md_value[i]);
	}
}


void* PrintPasswd(void* threadarg)
{
	int taskid;
	struct thread_data* my_data;
	char buffor[33];

	my_data = (struct thread_data*)threadarg;
	bytes2md5(my_data->dictionary_word, my_data->len, buffor);
	printf("\nThread id: %d\nProducer class: %d\n My password is : %s \n", my_data->thread_id,my_data->producer ,buffor);
	my_data->cracked_paswd = buffor;
	printf("\n\nMy hashed password is: %s\n\n", my_data->cracked_paswd);
	pthread_exit(NULL);
}

int main(int argc, char* argv[])
{
	FILE* fp,paswd_file;
	pthread_t producer[NUM_THREADS], consume;
	int* taskids[NUM_THREADS];
	int rc, t,count;
	char* filename;
	char* ptr;
	count = 0;  
	int i = 0;
	char buf[255];

	/*Wprowadzenie filepath s³ownika oraz sprawdzenie iloœci wyrazów */

	printf("Please put in dictionary filename: ");
    scanf("%s", &filename);
	printf("Dictionary filename: %s", &filename);
	printf("\nflag4");

	int dictionary_size = check_lines(&filename);

	/*Alokacja pamiêci dla s³ownika*/

	char** words;
	words = malloc((dictionary_size) * sizeof(char*));

	if (words == NULL)
	{
		/* Nie uda³o siê zaalokowaæ pamiêci */

		printf("Error: out of memory.\n");
		return;
	}

	//Dzia³a alokacja odwo³anie do wyrazów ze s³ownika robisz &words[i] lub words[i], sprwadŸ se

	char* temp_array[255];
	fp = fopen(&filename, "r");

	for (i = 0; i < dictionary_size; i++)
	{
		temp_array[i] = fgets(buf, 255, fp);
		strcpy(&words[i], temp_array[i]);
		if (temp_array[i] == NULL)
		{
			break;
		}
	}
	fclose(fp);
	/*Sprawdzanie kryteria wyrazów s³ownika i przekazania ich do producentów*/



	for (t = 0; t < dictionary_size; t++)
	{
		int is_lower = check_letter(&words[t]);
		printf("Value of functions is:%d", is_lower);
		thread_data_array[t].thread_id = t;
		thread_data_array[t].len = strlen(&words[t]);
		thread_data_array[t].dictionary_word = &words[t];
		printf("Creating thread %d\n", t);
		rc = pthread_create(&producer[t], NULL, PrintPasswd, (void*)
			&thread_data_array[t]);
		if (rc) {
			printf("ERROR; return code from pthread_create() is %d\n", rc);
			exit(-1);


	}




//C:\Users\Grzesiek\source\repos\Decode\slownik.txt


	/*
	for (t = 0; t < NUM_THREADS; t++) {
		sum = sum + t;
		thread_data_array[t].thread_id = t;
		thread_data_array[t].len = strlen(messages[t]);
		thread_data_array[t].data = messages[t];
		printf("Creating thread %d\n", t);
		rc = pthread_create(&threads[t], NULL, PrintPasswd, (void*)
			&thread_data_array[t]);
		if (rc) {
			printf("ERROR; return code from pthread_create() is %d\n", rc);
			exit(-1);
		}
	}
	*/


	//pthread_exit(NULL);



	return 0;
}





	

	
