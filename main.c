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
	const char** producer_table;
	const char** password_table_s;
	int dictionary_length;
	int password_file_length;
	const char* current_password;
	const char* hashed_password;
};

struct consumer_data
{
	const char** cracked_pwd_table;
	int cracked_pwd_table_length;
	int cracked_pwd_table_index;
};

struct consumer_data consumer;

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

const char* add_number_front(const char* msg, int i,int word_len,int iterator)
{
	int size = word_len + 2 + iterator + (i / 10);
	char* buf = malloc(size * sizeof(char));
	snprintf(buf, size, "%0*d%s",iterator,i, msg); 

	return buf;
	free(buf);
}

const char* add_number_back(const char* msg, int i, int word_len,int iterator)
{
	int size = word_len + 2 + iterator + (i / 10);
	char *buf = malloc(size * sizeof(char));
	snprintf(buf, size, "%s%0*d", msg,iterator, i);
	return buf;
	free(buf);
}

const char* num_combinations(const char* msg, int j,int i, int word_len,int iterator)
{
	//litera j i letera i odpowiednio z przodu i z ty�u litery
	int size = word_len + 3 + iterator + (i / 10) + (j/10);
	char* buf = malloc(size * sizeof(char));
	snprintf(buf, size, "%0*d%s%0*d",iterator, j, msg, iterator,i); 
	return buf;
	free(buf);
}


void* PasswdCracking(void* threadarg)
{
	int taskid;
	struct thread_data* my_data;
	char buffor[33];
	const char* temp;
	const char* append;
	int iterator1 = 1;
	int iterator2 = 1;
	my_data = (struct thread_data*)threadarg;

	printf("\nCleanup time\n");

	for (int i = 0; i < my_data->dictionary_length; i++)
	{
		char* newline = strchr(&my_data->producer_table[i], '\n');
		if (newline)
			*newline = '\0';
	}

	for (int i = 0; i < my_data->password_file_length; i++)
	{
		char* newline = strchr(my_data->password_table_s[i], '\n');
		if (newline)
			*newline = '\0';
	}


	
	for (int iterator1 = 1; iterator1 < 2; iterator1++)
	{

		for (int i = 0; i < my_data->dictionary_length; i++)
		{
			for (int j = 0; j < my_data->password_file_length; j++)
			{
				char* special_sign = strchr(my_data->password_table_s[j], '#');
				if (special_sign)
				{
					goto breakpoint;
				}
		//	C:\Users\Grzesiek\source\repos\Decode\slownik.txt

				my_data->current_password = &my_data->producer_table[i];
				bytes2md5(my_data->current_password, strlen(my_data->current_password), buffor);
				my_data->hashed_password = buffor;
				//printf("\nMy current pwd: %s\n", my_data->current_password);
				// Comparing strings
				if (strcmp(my_data->hashed_password, my_data->password_table_s[j]) == 0)
				{
					snprintf(my_data->password_table_s[j], strlen(my_data->password_table_s[j]) + 1, "#%s", my_data->password_table_s[j]);
					strcpy(&consumer.cracked_pwd_table[consumer.cracked_pwd_table_index], my_data->current_password);
					consumer.cracked_pwd_table_index++;

				}


				 //Now checking combinations of numbers in front of strings
				
				for (int g = 0; g < iterator2 * 10; g++)
				{
					temp = add_number_front(my_data->current_password, g, strlen(my_data->current_password), iterator1);
					my_data->current_password = temp;
					bytes2md5(my_data->current_password, strlen(my_data->current_password), buffor);
					my_data->hashed_password = buffor;
					//printf("\nMy current pwd: %s", my_data->current_password);
					if (strcmp(my_data->hashed_password, my_data->password_table_s[j]) == 0)
					{
						snprintf(my_data->password_table_s[j], strlen(my_data->password_table_s[j]) + 1, "#%s", my_data->password_table_s[j]);
						strcpy(&consumer.cracked_pwd_table[consumer.cracked_pwd_table_index], my_data->current_password);
						consumer.cracked_pwd_table_index++;
					}
					my_data->current_password = &my_data->producer_table[i];

				}

				// Now checking combinations of numbers in the back of strings

				for (int g = 0; g < iterator2 * 10; g++)
				{
					temp = add_number_back(my_data->current_password, g, strlen(my_data->current_password), iterator1);
					my_data->current_password = temp;
					bytes2md5(my_data->current_password, strlen(my_data->current_password), buffor);
					my_data->hashed_password = buffor;
					//printf("\nMy current pwd: %s", my_data->current_password);
					if (strcmp(my_data->hashed_password, my_data->password_table_s[j]) == 0)
					{
						snprintf(my_data->password_table_s[j], strlen(my_data->password_table_s[j]) + 1, "#%s", my_data->password_table_s[j]);
						strcpy(&consumer.cracked_pwd_table[consumer.cracked_pwd_table_index], my_data->current_password);
						consumer.cracked_pwd_table_index++;
					}
					my_data->current_password = &my_data->producer_table[i];
				
				}

				// Now checking combinations of numbers in the back of strings

				for (int g = 0; g < iterator2 * 10; g++)
				{
					for (int h = 0; h < iterator2 * 10; h++)
					{
						temp = num_combinations(my_data->current_password, g, h, strlen(my_data->current_password), iterator1);
						my_data->current_password = temp;
						bytes2md5(my_data->current_password, strlen(my_data->current_password), buffor);
						my_data->hashed_password = buffor;
						//printf("\nMy current pwd: %s", my_data->current_password);
						if (strcmp(my_data->hashed_password, my_data->password_table_s[j]) == 0)
						{
							snprintf(my_data->password_table_s[j], strlen(my_data->password_table_s[j]) + 1, "#%s", my_data->password_table_s[j]);
							strcpy(&consumer.cracked_pwd_table[consumer.cracked_pwd_table_index], my_data->current_password);
							consumer.cracked_pwd_table_index++;
						}
						my_data->current_password = &my_data->producer_table[i];



					}

				}
				
			breakpoint:
				printf("");

			}
		}
		iterator2 *= 10;

	}
	pthread_exit(NULL);
}

void* Consumer(void* consumer_arg)
{
	struct consumer_data* data;
	data = (struct consumer_data*)consumer_arg;
	while (1)
	{
		for (int j = 0; j < data->cracked_pwd_table_index; j++)
		{
			char* newline = strchr(&data->cracked_pwd_table[j], '/');
			if (!newline)
			{
				printf("\nCracked password: %s\n", &data->cracked_pwd_table[j]);
				snprintf(&data->cracked_pwd_table[j], strlen(&data->cracked_pwd_table[j]) + 2, "/%s", &data->cracked_pwd_table[j]);
			}
		}
	}
	

	


	pthread_exit(NULL);
}

int main(int argc, char* argv[])
{
	FILE* fp;
	FILE* password_file;
	int rc, t,count;
	char* filename,password_file_name;
	char* ptr;
	count = 0;  
	int i = 0;
	char buf_pwd[255];
	char buf[255];
	char* temp_array[255];
	char* temp_password_array[255];

	// Jak chcesz sparametryzowa� liczb� 0 z przodu do dajesz w printf %0*d, i potem w parametrach printfa pierwszy parametr to gwiazdka


	printf("Please put in password filename: ");
	scanf("%s", &password_file_name);
	printf("Dictionary filename: %s", &password_file_name);
	
	int password_file_size = check_lines(&password_file_name);
	char** password_table;
	password_table = malloc((password_file_size) * sizeof(char*));
	
	password_file = fopen(&password_file_name, "r");
	
	for (i = 0; i < password_file_size; i++)
	{
		temp_password_array[i] = fgets(buf_pwd, 255, password_file);
		password_table[i] = malloc((33) * sizeof(char));
		strcpy(password_table[i], temp_password_array[i]);
	
	
		if (temp_password_array[i] == NULL)
		{
			break;
		}
	}

	fclose(password_file);

	char** consumer_table;
	consumer_table = malloc((password_file_size) * sizeof(char*));

	password_file = fopen(&password_file_name, "r");

	for (i = 0; i < password_file_size; i++)
	{
		consumer_table[i] = malloc((33) * sizeof(char));


		if (consumer_table[i] == NULL)
		{
			break;
		}
	}

	fclose(password_file);
	/*Wprowadzenie filepath s�ownika oraz sprawdzenie ilo�ci wyraz�w */
	
	printf("Please put in dictionary filename: ");
    scanf("%s", &filename);
	printf("Dictionary filename: %s", &filename);
	printf("\nflag4");
	
	int dictionary_size = check_lines(&filename);
	
	
	/*Alokacja pami�ci dla s�ownika*/
	
	char** sorted_array_0;
	sorted_array_0 = malloc((dictionary_size) * sizeof(char*));
	char** sorted_array_1;
	sorted_array_1 = malloc((dictionary_size) * sizeof(char*));
	char** sorted_array_2;
	sorted_array_2 = malloc((dictionary_size) * sizeof(char*));
	
	//C:\Users\Grzesiek\source\repos\Decode\slownik.txt
	
	
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
			sorted_array_ind[1]++;
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

	char*** sortedarray[3];
	sortedarray[0] = sorted_array_0;
	sortedarray[1] = sorted_array_1;
	sortedarray[2] = sorted_array_2;
	
	
	




	pthread_t producer[PRODUCER_NUM], consume;
	int* taskids[PRODUCER_NUM];
	// Przy przekazywaniu musisz mie� cons char** tablica
	const char** mytable = sortedarray[0];


	for (t = 0; t < PRODUCER_NUM; t++)
	{
		thread_data_array[t].thread_id = t;
		thread_data_array[t].password_table_s = password_table;
		thread_data_array[t].password_file_length = password_file_size;
		thread_data_array[t].producer_table = sortedarray[t];
		thread_data_array[t].dictionary_length = sorted_array_ind[t];
		printf("Creating thread %d\n", t);
		rc = pthread_create(&producer[t], NULL, PasswdCracking, (void*) &thread_data_array[t]);
		if (rc) {
			printf("ERROR; return code from pthread_create() is %d\n", rc);
			exit(-1);


		}
	}
	consumer.cracked_pwd_table = consumer_table;
	consumer.cracked_pwd_table_length = password_file_size;
	consumer.cracked_pwd_table_index=0;
	rc = pthread_create(&consume, NULL, Consumer, (void*) &consumer);
	if (rc) {
		printf("ERROR; return code from pthread_create() is %d\n", rc);
		exit(-1);
	}
	pthread_exit(NULL);
	


	return 0;
}





	

	
