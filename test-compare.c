#include "stdio.h"
#include "string.h"
#include "stdbool.h"

int main (int argc, const char *argv[])
{
  unsigned char ciphertxt[] = {0x1,0x2,0x3,0x4,0x5};
  unsigned char outbuf[] = {0x1,0x2,0x3,0x4,0x5};
  
  bool match;
  
	for(int q = 0; q < sizeof(ciphertxt); q++)
	{
		/* Judge whether the cipher text generated by this key is match for the provoded one */
		/* As the whole ciphertext cannot be matched at once, use this loop to match it bit by bit */		
		if(ciphertxt[q] == outbuf[q])
		  match = true;
		else
		  match = false;
	}
  
  if (match)
    printf("Same\n");
  else
    printf("Different\n");
    
  printf("string %s \n", ciphertxt);
  
  return 0;
}
