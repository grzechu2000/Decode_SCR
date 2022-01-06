#include <openssl/evp.h>
#include <string.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define PRODUCER_NUM 3


struct thread_data
{

	int thread_id;
	const char* cracked_paswd;
	const char* producer_table;
	int dictionary_length;
};

struct thread_data thread_data_array[PRODUCER_NUM];

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


//void* PrintPasswd(void* threadarg)
//{
//	int taskid;
//	struct thread_data* my_data;
//	char buffor[33];
//
//	my_data = (struct thread_data*)threadarg;
//	bytes2md5(my_data->dictionary_word, my_data->len, buffor);
//	printf("\nThread id: %d\nProducer class: %d\n My password is : %s \n", my_data->thread_id,my_data->producer ,buffor);
//	my_data->cracked_paswd = buffor;
//	printf("\n\nMy hashed password is: %s\n\n", my_data->cracked_paswd);
//	pthread_exit(NULL);
//}

int main(int argc, char* argv[])
{
	FILE* fp,paswd_file;
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

	char* temp_array[255];
	char** sorted_array_0;
	sorted_array_0 = malloc((dictionary_size) * sizeof(char*));
	char** sorted_array_1;
	sorted_array_1 = malloc((dictionary_size) * sizeof(char*));
	char** sorted_array_2;
	sorted_array_2 = malloc((dictionary_size) * sizeof(char*));

	//C:\Users\Grzesiek\source\repos\Decode\slownik.txt

	//Dzia³a alokacja odwo³anie do wyrazów ze s³ownika robisz &words[i] lub words[i], sprwadŸ se

	int sorted_array_ind[3] = { 0, 0, 0 };

	fp = fopen(&filename, "r");

	for (i = 0; i < dictionary_size; i++)
	{
		temp_array[i] = fgets(buf, 255, fp);
		switch (check_letter(temp_array[i]))
		{
		case 0:
		{
			strcpy(&sorted_array_0[sorted_array_ind[0]],temp_array[i]);
			sorted_array_ind[0]++;
			break;
		}

		case 1:
		{
			
			strcpy(&sorted_array_1[sorted_array_ind[1]], temp_array[i]);
			sorted_array_ind[1];
			break;
		}

		case 2:
		{
			
			strcpy(&sorted_array_2[sorted_array_ind[2]], temp_array[i]);
			sorted_array_ind[2]++;
			break;
		}


		default:
			break;
		}
		if (temp_array[i] == NULL)
		{
			break;
		}
	}

	fclose(fp);






	pthread_t producer[PRODUCER_NUM], consume;
	int* taskids[PRODUCER_NUM];
	
	/*for (t = 0; t < PRODUCER_NUM; t++)
	{
		thread_data_array[t].thread_id = t;
		thread_data_array[t].dictionary_length = sorted_array_ind[t];
		printf("Creating thread %d\n", t);
		rc = pthread_create(&producer[t], NULL, PrintPasswd, (void*)
			&thread_data_array[t]);
		if (rc) {
			printf("ERROR; return code from pthread_create() is %d\n", rc);
			exit(-1);


		}
	}
	*/






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





	

	
