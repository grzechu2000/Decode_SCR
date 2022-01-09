#include <openssl/evp.h>
#include <string.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>

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

const char* add_number_front(const char* msg, int i,int word_len)
{
	int size = word_len + 2 + (i / 10);
	char* buf = malloc(size * sizeof(char));

	printf("size is: %d\n\n", size);
	printf("wordlen is: %d\n", word_len);
	snprintf(buf, size, "%d%s",i, msg);
	printf("%s\n", buf); 

	return buf;
	free(buf);
}

const char* add_number_back(const char* msg, int i, int word_len)
{
	int size = word_len + 2 + (i / 10);
	char *buf = malloc(size * sizeof(char));

	printf("size is: %d\n\n", size);
	printf("wordlen is: %d\n", word_len);
	snprintf(buf, size, "%s%d", msg, i);
	printf("%s\n", buf);

	return buf;
	free(buf);
}

const char* num_combinations(const char* msg, int j,int i, int word_len)
{
	//litera j i letera i odpowiednio z przodu i z ty³u litery
	int size = word_len + 3 + (i / 10) + (j/10);
	char* buf = malloc(size * sizeof(char));

	printf("size is: %d\n\n", size);
	printf("wordlen is: %d\n", word_len);
	snprintf(buf, size, "%d%s%d",j, msg, i); 
	printf("%s\n", buf); 

	return buf;
	free(buf);
}


//void* PrintPasswd(void* threadarg)
//{
//	int taskid;
//	struct thread_data* my_data;
//	char buffor[33];
//
//	my_data = (struct thread_data*)threadarg;
//	bytes2md5(my_data->dictionary_word, my_data->len, buffor);
//	printf("\nThread id: %d\nProducer class: %d\n My password is : %s \n", my_data->thread_id,m_szynka_y_data->producer ,buffor);
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

	char word[] = "womankiller";
	const char* tmp;
	int word_len = strlen(word);
	printf("Length of word is : %d\n", word_len);


	tmp = num_combinations(word,19,14,word_len);

	printf("\ntmp is : %s", tmp);





	/*Wprowadzenie filepath s³ownika oraz sprawdzenie iloœci wyrazów */

	/*printf("Please put in dictionary filename: ");
    scanf("%s", &filename);
	printf("Dictionary filename: %s", &filename);
	printf("\nflag4");*/

	//int dictionary_size = check_lines(&filename);

	//
	///*Alokacja pamiêci dla s³ownika*/

	//char* temp_array[255];
	//char** sorted_array_0;
	//sorted_array_0 = malloc((dictionary_size) * sizeof(char*));
	//char** sorted_array_1;
	//sorted_array_1 = malloc((dictionary_size) * sizeof(char*));
	//char** sorted_array_2;
	//sorted_array_2 = malloc((dictionary_size) * sizeof(char*));

	////C:\Users\Grzesiek\source\repos\Decode\slownik.txt

	////Dzia³a alokacja odwo³anie do wyrazów ze s³ownika robisz &words[i] lub words[i], sprwadŸ se

	//int sorted_array_ind[3] = { 0, 0, 0 };

	//fp = fopen(&filename, "r");

	//for (i = 0; i < dictionary_size; i++)
	//{
	//	temp_array[i] = fgets(buf, 255, fp);
	//	switch (check_letter(temp_array[i]))
	//	{
	//	case 0:
	//	{
	//		strcpy(&sorted_array_0[sorted_array_ind[0]],temp_array[i]);
	//		sorted_array_ind[0]++;
	//		break;
	//	}

	//	case 1:
	//	{
	//		
	//		strcpy(&sorted_array_1[sorted_array_ind[1]], temp_array[i]);
	//		sorted_array_ind[1]++;
	//		break;
	//	}

	//	case 2:
	//	{
	//		
	//		strcpy(&sorted_array_2[sorted_array_ind[2]], temp_array[i]);
	//		sorted_array_ind[2]++;
	//		break;
	//	}


	//	default:
	//		break;
	//	}
	//	if (temp_array[i] == NULL)
	//	{
	//		break;
	//	}
	//}

	//fclose(fp);

	//// Dzia³a lmaooooo, po prostu debil ze mnie
	//char*** sortedarray[3];
	//sortedarray[0] = sorted_array_0;
	//sortedarray[1] = sorted_array_1;
	//sortedarray[2] = sorted_array_2;

	//

	///*for (int i = 0; i < sorted_array_ind[2]; i++)
	//{ Tak siê wypisuje wartoœci

	//	printf("sortedarray2 : %s", &sortedarray[2][i]);
	//}*/



	/*pthread_t producer[PRODUCER_NUM], consume;
	int* taskids[PRODUCER_NUM];
	
	for (t = 0; t < PRODUCER_NUM; t++)
	{
		thread_data_array[t].thread_id = t;
		thread_data_array[t].producer_table = sortedarray[t];
		thread_data_array[t].dictionary_length = sorted_array_ind[t];
		printf("Creating thread %d\n", t);
		rc = pthread_create(&producer[t], NULL, PrintPasswd, (void*)
			&thread_data_array[t]);
		if (rc) {
			printf("ERROR; return code from pthread_create() is %d\n", rc);
			exit(-1);*/


	//	}
	//}

	//pthread_exit(NULL);



	return 0;
}





	

	
