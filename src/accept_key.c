#include "accept_key.h"
#include "base64.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <openssl/evp.h>

void accept_key_generator(const char *request, char *response) {
	char *guid = "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";
	int mesarrlen = (24 + strlen(guid));
	char mesarr[mesarrlen];
	unsigned char mesdig[EVP_MAX_MD_SIZE];
	unsigned int mesidg_len = sizeof(mesdig);

	for(int i = 0; i < mesarrlen; ++i) {
		if(i < 24) {
			mesarr[i] = request[i];
		} else {
			mesarr[i] = guid[i - 24];
		}
	}

	EVP_MD_CTX *ctx;

	ctx = EVP_MD_CTX_new();
	EVP_DigestInit_ex(ctx, EVP_sha1(), NULL);
	EVP_DigestUpdate(ctx, mesarr, mesarrlen);
	EVP_DigestFinal_ex(ctx, mesdig, &mesidg_len);

	base64_encode((char *)mesdig, mesidg_len, response, MESSAGE_SIZE);

	EVP_MD_CTX_free(ctx);
}
