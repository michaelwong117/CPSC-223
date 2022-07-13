#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

int *make_range(int m, int n)
{
    int *a = malloc((n - m) * sizeof(int));
    int *b = a;
    int len = n-m;
    int i = 0;
    while (m < n)
    {
        //a[i] = m;
        *a = m;
        // printf("%d ", *a);
        a++;
        m++;
        // i++;
    }
    printf("\n");
    for (int i = 0; i < len; i++)
    {
        printf("%d\n", b[i]);
    }
    return a;
}


int main() {

    // int *a = malloc(sizeof(int) * 3);
    // printf("%d %d %d\n", *(a), *(a+1), *(a+2));

    // int a[] = {3, 15, 7, 0, -99};
    // int *b = &a[3];
    // int *b = a + 3;
    // printf("%d\n", *b);
    // a[0] = 0;
    // a[1] = 0;
    // a[2] = 0;


    char a[] = {'D', 'E', 'N', '\0'};
    char *b = a;
    b = a + 2;
    // printf("%c\n", *b);

    make_range(4, 8);
    int *p;
    int c = 100;
    p = &c;

    printf("%d\n", *p++);


}
