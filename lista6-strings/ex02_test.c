#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void    concatena(char *o1, char *o2)
{
        int     len_d;
        int     i;
        int     j;

        i = 0;
        j = 0;
        len_d = ( strlen(o1) +  strlen(o2) );
        char *d = malloc(sizeof(char) * (len_d + 1));
        while(o1[i] != '\0')
        {
                d[i] = o1[i];
                i++;
        }
        while(o2[j] != '\0')
        {
                d[i] = o2[j];
                i++;
                j++;
        }
        d[i] = '\0';
}

int main(void)
{   
        free();
}
