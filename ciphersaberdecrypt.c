#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void initializeSBox();
int getIV();
unsigned char genRandomByte();

main(int argc, char **argv)
{

if (argc < 2) { //no arguments were passed
printf("Usage: ./ciphersaberdecrypt [key] [message]\n");
exit(0);
}

unsigned int keyLength = strlen(argv[1]);
unsigned char S[256]; //s-box
unsigned char key[keyLength];
unsigned char iv[10];

// Copy key into key buffer. Truncate after 16 bytes (128 bits).
if(strlen(argv[1]) == 0 ){
printf("Usage: ./ciphersaber [key] [message]\n");
printf("You must enter a key.\n");
exit(0);
}
strncpy(key, argv[1], keyLength);

//Read IV from file.
getIV(iv, 2);

unsigned int i=0;
unsigned char keyIV[keyLength+10];
for(i=0; i<keyLength; i++){
keyIV[i]=key[i];
}
for(i=keyLength; i<(keyLength+10); i++){
keyIV[i]=iv[i-keyLength];

}

//initialize S-Box
initializeSBox( S, key, strlen(keyIV) );

/////////////////////////
// Load message file.
FILE *f = fopen("textfile.txt", "rb");
unsigned int input=0;
i=0;
unsigned int j=0;
unsigned int temp=0;
unsigned int t=0;
unsigned int K=0;
unsigned char output=0;
fseek(f, 10, SEEK_SET);

// Decrypt file.
for ((input = fgetc(f)) !=EOF ) {
i=(i+1)%256;
j=(j+S[i])%256;
temp=S[i];
S[i]=S[j];
S[j]=temp;
t=(S[i]+S[j])%256;
K=S[t];
printf( "%c", (K^input));
}

fclose(f);
}
// Generate random byte
unsigned char genRandomByte( unsigned char sBox[], unsigned int i, unsigned int j){
unsigned int temp = 0;
unsigned int K = 0;
i = (i+1)%256;
j = (j+sBox[i])%256;
temp = sBox[i];
sBox[i] = sBox[j];
sBox[j]=temp;
K = sBox[sBox[i] + sBox[j]]%256;
return K;

}

void initializeSBox( unsigned char sBox[], unsigned char key[] , unsigned int keyLength ){
	unsigned int i=0;
	unsigned int j=0;
	unsigned int temp=0;
	for(i=0; i<256; i++){
	sBox[i]=i;
	}

	for(i=0; i<256; i++){
	j = (j+sBox[i] + key[i%keyLength])%256;
	temp = sBox[i];
	sBox[i]=sBox[j];
	sBox[j]=temp;
	}
return;
}

int getIV(char input[], int encrypt){
	char data[10];
	FILE* fp;
	//Read 10 byte IV from file.
	fp = fopen("textfile.txt", "rb");
	fread(input, 1, 10, fp);
	fclose(fp);
return;
}
