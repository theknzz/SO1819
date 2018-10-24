#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "medit-defaults.h"

int main(int argc, char *argv) {

	char str[11], *pal[3];
	int n, i;

	fgets(str,12,stdin);
	puts(str);
	str[strlen(str)-1] = '\0';

	n = 0;
	pal[n] = strtok(str, " ");
	n++;
	while( (pal[n] = strtok( NULL, " ") ) != NULL)
		n++;

	for(i=0;i<n;i++)
		printf("%s\n",pal[i]);
	exit(0);
}
