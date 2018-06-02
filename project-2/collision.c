/*
 * Compilation: gcc –o collision collision.c –lcrypto –ldl –std=c99
 * Excution: ./collision md5
 * 
 * 
 */

#include <stdio.h>
#include "openssl/evp.h"
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>

//This section declares the functions used in the main program

// generates random alphanumeric string of length len
void gen_random (char *s, const int len)
{
	int charlen = len; // Local variable of string len
	
	/* Define an array with all the provided characters */
	char chararr[] = "0123456789!\"#$%&'()*+-/. ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
	
	/* Generate random string using the characters of length 'len'*/
	while (charlen-- > 0) // Keep going till last char
	{
		unsigned index = (double)rand() / RAND_MAX * (sizeof chararr - 1); // Generate a random char
		*s++ = chararr[index]; // Add random char to the next position in array
	}
	*s = '\0'; // Set final char to a null terminator*/
}

// compares first len bytes of two hash values hash1 and hash2
int match_digest (char *hash1, char *hash2, int len)
{
	/* Judge whether 'hash1' and 'hash2' are same or not. If match found, return '1' else '0'. Use the variables passed as parameters in the function*/ 
	int match = 0;
	for(int i = 0; i < len; i++)
	{
		// Check for matching character till end of array unless not matched	
		int count = 0;
		while(count < len)
		{		
			// If matched, increase count
			if(hash1[count] == hash2[count])
			{
				match = 1;
				count++;
			}
			else
			{
				// Reset counter when there are no match
				// Break out of current key when does not match
				match = 0;
				count = 0;
				break;
			} 
		}
	}

	if (match)
		return 1; // Match found
	return 0; // Match not found
}

// generates the digest for the message mess using hash algorithm digest_method and
// saves the first len bytes in mess_hash
void generate_digest(char *mess, char *mess_hash, int len, char *digest_method)
{
	EVP_MD_CTX mdctx;
	const EVP_MD *md;
	
	unsigned char md_value [EVP_MAX_MD_SIZE];
	int md_len, i;
	OpenSSL_add_all_digests();
	
	md = EVP_get_digestbyname(digest_method);

	if(!md)
	{
		printf("Unkown message digest %s\n", digest_method);
		exit(1);
	}

	EVP_MD_CTX_init(&mdctx);
	EVP_DigestInit_ex(&mdctx ,md, NULL);
	EVP_DigestUpdate(&mdctx, mess, strlen(mess));
	EVP_DigestFinal_ex(&mdctx, md_value, &md_len);
	EVP_MD_CTX_cleanup(&mdctx);

	for(i = 0; i < len; i++)
	{
		mess_hash[i] = md_value[i];
	}
}

void save_to_repo(unsigned char repo[] [3], char repo_msg[] [7], int *repo_index, char *msg, unsigned char * hash)
{
	// increment repository index
	*repo_index = (*repo_index) + 1;
	
	// save hash
	repo[*repo_index][0] = hash[0];
	repo[*repo_index][1] = hash[1];
	repo[*repo_index][2] = hash[2];

	// save message
	strcpy(repo_msg[*repo_index], msg);
}

// returns -1 for no match
// returns index if match

int match_repo (unsigned char repo[] [3], const int repo_index , unsigned char *hash)
{
	int i;
	
	for(i = 0; i <= repo_index; i++)
	{
		if((repo[i][0] == hash[0]) && (repo[i][1] == hash[1]) && (repo[i][2] == hash[2]))
		{
			return i;
		}
	}
	return -1;
}

int main(int argc, char *argv[])
{
	int i, match, c, length, j;
	char ch;
	int curr_length = 0, inc_len=1;

	// hash algorithm
	char *hash_algo = argv[1];

	char *mess = NULL;
	unsigned char mess_hash[3];

	char *rand_str = NULL;
	unsigned char rand_hash[3];

	//Approach used in program
	//initial text - initial hash
	//generate text - hash compare with initial hash and hash repository -> true exit
	//false -> save text and hash

	// generated 24 bit hash values will be added to this repository
	unsigned char repo[10000][3];

	// index to keep track of number of hash values in repository
	int repo_index = -1;

	// message correposding to each hash value in repo is kept here
	char repo_msg[10000][7];

	srand(time(NULL));

	printf("\nBreak collision free property\n");
	printf("\nFind M1 and M2 s.t. 24 bits H(M1) = 24 bits H(M2) \n");

	//generate random string of random length
	length = rand() %20 + 1; // length varies 1-20
	mess = malloc(length + 1);
	gen_random (mess, length);//Stores the generated string in mess

	// calculate digest for mess in mess hash
	generate_digest(mess, mess_hash, 3, hash_algo );// a 24 bit (3 byte)hash is generated here and stored in variable mess_hash using a particular hash algrithm hash_algo

	printf("\n");

	rand_str = malloc(7);

	for(j = 0; j < 10000; j++)
	{

		// generate random string
		gen_random(rand_str, 6);

		// generate digest for random string
		generate_digest(rand_str, rand_hash, 3, hash_algo);

		//compare first 3 bytes of the hash values
		match = match_digest(rand_hash, mess_hash, 3);

		if(match == 1) //check the match found. 
		{
			//TODO: TEST
			printf("IN NONE REPO!\n");
			/* When match found print the two strings i.e. the initial string and the randomly generated string which have the same hash values*/
			printf("Initial string: %s\n", mess);
			printf("Random generated string: %s\n", rand_str);

			/* Print the hash value of the randomly generated string in hexadecimal format */
      		printf("Random generated string hash value: ");
			for(i = 0; i < length; i++)
			{
			  printf("%x", rand_hash[i]);
			}
			printf("\n");
			
			/* Print the number of iterations used to find the match. */
			printf("Iteration: %d", j);
			
			break;
		}
		match = -1, // match_repo may return actual index
		match = match_repo (repo, repo_index, rand_hash);// This statement compares the random String's hash with the repository storing all the generated strings' hash 

//If the random string's hash matches with the ones in the repository, match becomes positive
		if(match >= 0)
		{
			printf("IN REPOSITORY\n");
			/* When match found from the repositary, print the two strings i.e. the random string and the repositary string which have the same hash values*/
			printf("Random generated string: %s\n", rand_str);
			//printf("Repository string: %s\n", rand_str);
			printf("Repository string: ");
			for(int l = 0; l < 7; l++)
			  printf("%c", repo_msg[match][l]);
      		printf("\n");

			//printf("REPO INDEX: %d\n", repo_index);
			//printf("MATCH number: %d\n", match);

			/* Print the hash value of the randomly generated string in hexadecimal format */
			printf("Random generated string hash value: ");
			for(int k = 0; k < length; k++)
			{
			  printf("%x", rand_hash[k]);
			}
			printf("\n");
			
			//TODO: TEST
			printf("REPO String Hash Value (hex): ");
			for(int k = 0; k < length; k++)
			{
			  printf("%x", repo[match][k]);
			}
			printf("\n");

			/* Print the number of iterations used to find the match. */
			printf("Iteration: %d", j);
			
			break;
		}
		else
		{
		save_to_repo(repo, repo_msg, &repo_index, rand_str, rand_hash);
		}
	}
	if(rand_str != NULL)
	{
		free(rand_str);
	}
	if(mess != NULL)
	{
		free(mess);
	}
	printf("\n\n");
}
