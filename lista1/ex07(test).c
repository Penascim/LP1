#include <stdio.h>

typedef struct s_list
{
        char            *time;
        int             vitoria;
        int             derrota;
        int             empate;
        struct s_list   *next
} 		t_list;

int main(void)
{
        int i;
        t_time time;

        i = 20;
        while (i > 0)
        {
		t_list	*ft_lstnew(int value)
		{
			t_list	*element;
			
			element = (t_list *)malloc(sizeof(t_list));
			if (!element)
				return (NULL);
			scanf("%s\n", &element->time);
			element->index = -1;
			element->next = NULL;
			return (element);
		}
                printf("digite o nome do time:\n");
                scanf("%s\n", &t_list->time);
                printf("digite quantas vitorias o time possui:\n");
                scanf("%d\n", &t_list->vitoria);
                printf("digite quantas derrotas o time possui:\n");
                scanf("%d\n", &t_list->derrota);
                printf("digite quantos empates o time possui:\n");
                scanf("%d\n", &t_list->empate);
                i--;
		
        }
}
