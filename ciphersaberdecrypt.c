#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void decryptText();
void initializeSBox();
int getIV();

main(int argc, char **argv)
{

if (argc < 2) { //no arguments were passed
printf("Usage: ./ciphersaberdecrypt [key] [message]\n");
exit(0);
}

unsigned int i=0;
unsigned int keyLength = strlen(argv[1]);
unsigned char S[256]; //S-box
unsigned char key[keyLength];
unsigned char iv[10];

// Copy key into key buffer.
if(strlen(argv[1]) == 0 ){
printf("Usage: ./ciphersaber [key] [message]\n");
printf("You must enter a key.\n");
exit(0);
}
strncpy(key, argv[1], keyLength);

//Read IV from file.
getIV(iv);

// Ciphersaber key is (user supplied key)+(IV).
// Create a new array that concatenates the IV to the key.
unsigned char keyIV[keyLength+10];
for(i=0; i<keyLength; i++){
keyIV[i]=key[i];
}
for(i=keyLength; i<(keyLength+10); i++){
keyIV[i]=iv[i-keyLength];
}

//initialize S-Box
initializeSBox( S, keyIV, strlen(keyIV) );

decryptText( S );

}

void decryptText(unsigned char sBox[] ){
// Load message file.
FILE *f = fopen("textfile.txt", "rb");
unsigned int input=0;
unsigned int i=0;
unsigned int j=0;
unsigned int temp=0;
unsigned int t=0;
unsigned int K=0;
fseek(f, 10, SEEK_SET); //The IV is the first 10 bytes. Skip past this and start reading the message.

// Decrypt file.
while ((input = fgetc(f)) !=EOF ) {
        i=(i+1)%256;
        j=(j+sBox[i])%256;

        //Swap S[i] and S[j].
        temp=sBox[i];
        sBox[i]=sBox[j];
        sBox[j]=temp;

        t=(sBox[i]+sBox[j])%256;
        K=sBox[t];

        printf( "%c", (K^input)); //Print decrypted message.
}

fclose(f);

return;
}

void initializeSBox( unsigned char sBox[], unsigned char key[] , unsigned int keyLength ){
	unsigned int i=0;
	unsigned int j=0;
	unsigned int temp=0;

	//Fill S-box with 0, 1, 2, ..., 253, 254, 255.
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

int getIV(unsigned char input[]){
	FILE *f;
	//Read 10 byte IV from file.
	f = fopen("textfile.txt", "rb");
	fread(input, 1, 10, f);
	fclose(f);
return;
}
