#include <stdio.h>
#include <stdlib.h>
#include <string.h>

main(int argc, char **argv)
{

if (argc < 2) { //no arguments were passed
printf("Usage: ./csdecrypt [key] [loop count] [file]\n");
exit(0);
}

unsigned int i=0;
unsigned int j=0;
unsigned int temp=0;
unsigned int input=0;
unsigned int t=0;
unsigned int K=0;
unsigned int keyLength=strlen(argv[1]);
unsigned char S[256]; //S-box
unsigned char key[256];
unsigned char iv[256];
unsigned int loopCount = atoi(argv[2]);

// Copy key into key buffer.
if(strlen(argv[1]) == 0 ){
printf("Usage: ./csdecrypt [key] [loop count] [filename]\n");
printf("You must enter a key.\n");
exit(0);
}


strncpy(key, argv[1], 256);


//Read IV from file.
	FILE *f;
	//Read 10 byte IV from file.
	f = fopen(argv[3], "rb");
	fread(iv, 1, 10, f);
	fclose(f);

// Ciphersaber key is (user supplied key)+(IV).
// Create a new array that concatenates the IV to the key.
unsigned int keyIVLength=keyLength+10;
unsigned char keyIV[keyIVLength];
for(i=0; i<keyLength; i++){
keyIV[i]=key[i];
}
for(i=keyLength; i<(keyIVLength); i++){
keyIV[i]=iv[i-keyLength];
}

//initialize S-Box
i=0;
j=0;
temp=0;
	//Fill S-box with 0, 1, 2, ..., 253, 254, 255.
	for(i=0; i<256; i++){
	S[i]=i;
	}
unsigned int loopIter=0;
for(loopIter=0; loopIter<loopCount; loopIter++){
	for(i=0; i<256; i++){
	j = (j + S[i] + keyIV[i % keyIVLength]) % 256;
	temp = S[i];
	S[i] = S[j];
	S[j] = temp;
	}
}
i=0;
j=0;
// Print header
//printf("Length of key: %d\n", strlen(key));
//printf("Length of IV: %d\n", strlen(iv));
//printf("Length of key+IV: %d\n", strlen(keyIV));

FILE *fg = fopen(argv[3], "rb");
fseek(fg, 10, SEEK_SET); //The IV is the first 10 bytes. Skip past this and start reading the message.

// Decrypt file.
i=0;
j=0;
temp=0;
while ((input = fgetc(fg)) !=EOF ) {
        i=(i+1)%256;
        j=(j+S[i])%256;

        //Swap S[i] and S[j].
        temp=S[i];
        S[i]=S[j];
        S[j]=temp;

        t=(S[i]+S[j])%256;
        K=S[t];
        printf( "%c", (K^input)); //Print decrypted message.
}
//printf("\n");
fclose(fg);
}


