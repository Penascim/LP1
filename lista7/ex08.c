#include <stdio.h> 
#include <math.h>

typedef struct  s_list 
{ 
        int     x; 
        int     y; 
 
}               t_list; 
 
void    fill_dot(t_list ponto)  
{ 
        printf("ponto x:\n");
        scanf("%d", &ponto->x);
        printf("ponto y:\n");
        scanf("%d", &ponto->y);
             
} 

void    fill_vector(t_list vet[], int n)
{
        int     i;

        i = 0;
        while (i < n)
                fill_dot(&vet[i++]);
} 
 
double  euclides(t_list ponto)
{
        return sqrt( (ponto.x * ponto.x) + (ponto.y * ponto.y) );
}

t_list  check_distance(t_list *vet[])
{
}

int     main(void)
{
        t_list vet[10];
        fill_vector(vet[10], 10);
}

