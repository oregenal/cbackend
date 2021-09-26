#include "accept_key.h"
#include "sha1.h"
#include "base64.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>

void accept_key_generator(const char *request, char *response) {
	int res;
	char *guid = "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";
	int mesarrlen = (24 + strlen(guid));
	char mesarr[mesarrlen];
	unsigned char mesdig[SHA1HashSize];
	SHA1Context context;

	for(int i = 0; i < mesarrlen; ++i) {
		if(i < 24) {
			mesarr[i] = request[i];
		} else {
			mesarr[i] = guid[i - 24];
		}
	}

	write(1, mesarr, mesarrlen);
	printf("\n");

	res = SHA1Reset(&context);
	if(res == -1) {
		printf("SHA1Reset fail\n");
	}

	res = SHA1Input(&context, (unsigned char *)mesarr, mesarrlen);

	res = SHA1Result(&context, mesdig);
	if(res) {
		printf("Result fail\n");
	}

	base64_encode((char *)mesdig, SHA1HashSize, response, MESSAGE_SIZE);
}
