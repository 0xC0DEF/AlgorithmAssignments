#include <stdio.h>
#include <memory.h>
#include <malloc.h>

#define TOWER_COUNT 3

unsigned ring_count;

struct Tower
{
	int* rings;
	int count;
};
void push(struct Tower* tower, int n);
int pop(struct Tower* tower);
void move(struct Tower towers[TOWER_COUNT], int from, int to);
int solve(struct Tower towers[TOWER_COUNT], int from, int to, int n);
void print(struct Tower towers[TOWER_COUNT]);

int main(void)
{
	while (1)
	{
		printf("Info : When you use more than 10 rings, solving time will be long.\n");
		printf("How many rings you want to use? : ");
		scanf("%u", &ring_count);
		if (ring_count < 100)
			break;
		printf("error : please input between 0 ~ 100.\n");
	}
	printf("\n");
	struct Tower towers[TOWER_COUNT] = { 0, };
	int i;
	for (i = 0; i < TOWER_COUNT; i++)
	{
		towers[i].rings = malloc(sizeof(int)*ring_count);
		memset(towers[i].rings, 0, sizeof(int)*ring_count);
	}
	for (i = 0; i < ring_count; i++)
		push(towers + 0, ring_count - i);
	printf("Start.\n");
	print(towers);
	int n = solve(towers, 0, 2, ring_count);
	printf("Total move count : %d\n", n);
	for (i = 0; i < TOWER_COUNT; i++)
		free(towers[i].rings);
	return 0;
}

void push(struct Tower* tower, int n)
{
	tower->rings[tower->count++] = n;
}

int pop(struct Tower* tower)
{
	return tower->rings[--tower->count];
}

void move(struct Tower towers[TOWER_COUNT], int from, int to)
{
	int n;
	push(&towers[to], n = pop(&towers[from]));
	printf("Move ring%d from tower%d to tower%d.\n", n, from + 1, to + 1);
	print(towers);
}

int solve(struct Tower towers[TOWER_COUNT], int from, int to, int n)
{
	if (n <= 0)
		return 0;
	int i, tmp;
	for (i = 0; i < TOWER_COUNT; i++)
		if (i != from && i != to)
			tmp = i;
	int n1 = solve(towers, from, tmp, n - 1);
	move(towers, from, to);
	int n2 = solve(towers, tmp, to, n - 1);
	return n1 + n2 + 1;
}

void print(struct Tower towers[TOWER_COUNT])
{
	unsigned print_space_x = ring_count * 3 + 2 + 1;
	unsigned print_space_y = ring_count;
	static char** space;
	if (!space)
	{
		space = malloc(sizeof(char*)*print_space_y);
		int i;
		for (i = 0; i < print_space_y; i++)
			space[i] = malloc(sizeof(char)*print_space_x);
	}
	int i, j, k;
	for (i = 0; i < print_space_y; i++)
		memset(space[i], ' ', sizeof(char)*print_space_x);
	for (i = 0; i < print_space_y; i++)
	{
		space[i][print_space_x - 1] = '\0';
		space[i][ring_count + (1 * 0)] = '|';
		space[i][ring_count * 2 + (1 * 1)] = '|';
	}
	for (i = 0; i < TOWER_COUNT; i++)
	{
		for (j = 0; j < towers[i].count; j++)
		{
			for (k = 0; k < towers[i].rings[j]; k++)
			{
				space[print_space_y - 1 - j][ring_count*i + i + k] = '#';
			}
		}
	}
	for (i = 0; i < print_space_y; i++)
		printf("%s\n", space[i]);
	printf("\n");
}
