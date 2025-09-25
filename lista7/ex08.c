#include <stdio.h> 
 
typedef struct  s_list 
{ 
        int     x; 
        int     y; 
 
}               t_list; 
 
void            fill_dot(t_list ponto) 
{ 
        printf("ponto x:\n"); 
        scanf("%d", ponto->x); 
        printf("ponto y:\n"); 
        scanf("%d", ponto->y); 
                 
} 
void            fill_vector(t_list *vet[]) 
{ 
 
} 
 
int             main(void) 
{ 
        t_list  vet[10]; 
        int     i; 
}
