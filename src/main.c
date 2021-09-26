#include <stdio.h>
#include <string.h>
#include "str_search_ptrn.h"

char *str = "sdkj fhaskljdfh lkasjdhfl kjashdf kjshad";
char *search = "lkas";

int main(void)
{

	int strln = strlen(str);
	int res = str_search_ptrn(search, str, strln);

	printf("i = %d\n", res);

	return 0;
}
