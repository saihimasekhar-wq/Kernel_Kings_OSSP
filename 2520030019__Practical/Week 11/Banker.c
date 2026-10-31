#include <stdio.h>
#define MAX 10
int n, m;
void read_matrix(int arr[MAX][MAX])
{
    int i, j;
    for (i = 0; i < n; i++)
    {
        printf("Process P%d: ", i);
        for (j = 0; j < m; j++)
        {
            scanf("%d", &arr[i][j]);
        }
    }
}
void display_matrix(int arr[MAX][MAX])
{
    int i, j;
    for (i = 0; i < n; i++)
    {
        printf("P%d\t", i);
        for (j = 0; j < m; j++)
        {
            printf("%d\t", arr[i][j]);
        }
        printf("\n");
    }
}
int main()
{
    int available[MAX];
    int allocated[MAX][MAX];
    int maximum[MAX][MAX];
    int i;
    printf("Enter the number of processes: ");
    scanf("%d", &n);
    printf("Enter the number of resources: ");
    scanf("%d", &m);
    printf("\nEnter available resources:\n");
    for (i = 0; i < m; i++)
    {
        printf("Resource R%d: ", i);
        scanf("%d", &available[i]);
    }
    printf("\nEnter allocated resources for each process:\n");
    read_matrix(allocated);
    printf("\nEnter maximum resources needed for each process:\n");
    read_matrix(maximum);
    printf("\n========================================\n");
    printf("          ENTERED VALUES\n");
    printf("========================================\n");
    printf("\nAvailable Resources:\n");
    for (i = 0; i < m; i++)
    {
        printf("R%d\t", i);
    }
    printf("\n");
    for (i = 0; i < m; i++)
    {
        printf("%d\t", available[i]);
    }
    printf("\n");
    printf("\nAllocation Matrix:\n");
    printf("\t");
    for (i = 0; i < m; i++)
    {
        printf("R%d\t", i);
    }
    printf("\n");
    display_matrix(allocated);
    printf("\nMaximum Matrix:\n");
    printf("\t");
    for (i = 0; i < m; i++)
    {
        printf("R%d\t", i);
    }
    printf("\n");
    display_matrix(maximum);
    return 0;
}
