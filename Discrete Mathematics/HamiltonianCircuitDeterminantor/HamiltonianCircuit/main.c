#include <stdio.h>

#define VERT_MAX 100
#define EDGE_MAX (VERT_MAX*VERT_MAX)

unsigned vn, en;
_Bool vertices[VERT_MAX];
_Bool edges[VERT_MAX][VERT_MAX];

_Bool IsVertexExist(int vertex)
{
	return vertices[vertex];
}

void AddVertex(int vertex)
{
	vertices[vertex] = 1;
}

void RemoveVertex(int vertex)
{
	vertices[vertex] = 0;
}

_Bool IsEdgeExist(int v1, int v2)
{
	return edges[v1][v2];
}

void AddEdge(int v1, int v2)
{
	edges[v1][v2] = edges[v2][v1] = 1;
}

void RemoveEdge(int v1, int v2)
{
	edges[v1][v2] = edges[v2][v1] = 0;
}

_Bool IsOnlyOneVertexLeft()
{
	int vertex_count = 0;
	int i;
	for (i = 0; i < vn; i++)
		vertex_count += IsVertexExist(i);
	return vertex_count == 1;
}

int GetConnectedVertices(int vertex, int *vertices_buffer)
{
	int buffer_len = 0;
	int v;
	for (v = 0; v < vn; v++)
	{
		if (IsEdgeExist(vertex, v))
			vertices_buffer[buffer_len++] = v;
	}
	return buffer_len;
}

_Bool IsHamiltonianPathExist(int from, int to)
{
	printf("도시%d 방문\n", from);

	if (from == to)
	{
		if (IsOnlyOneVertexLeft())
		{
			printf("해밀턴 회로가 존재합니다\n");
			printf("회로 : %d", from);
			return 1;
		}
		else //가지치기
		{
			printf("도시%d 방문 취소(backtrack)\n", from);
			return 0;
		}
	}

	int connected_vertices[VERT_MAX];
	int connected_vertices_count = GetConnectedVertices(from, connected_vertices);
	int i;

	for (i = 0; i < connected_vertices_count; i++)
		RemoveEdge(from, connected_vertices[i]);
	RemoveVertex(from);

	for (i = 0; i < connected_vertices_count; i++)
	{
		if (IsHamiltonianPathExist(connected_vertices[i], to))
		{
			printf(" %d", from);
			return 1;
		}
	}

	AddVertex(from);
	for (i = 0; i < connected_vertices_count; i++)
		AddEdge(from, connected_vertices[i]);

	printf("도시%d 방문 취소(backtrack)\n", from);
	return 0;
}

_Bool IsHamiltonianCircuitExist(int from)
{
	printf("도시%d 방문\n", from);

	if (IsOnlyOneVertexLeft())
	{
		printf("해밀턴 회로가 존재합니다\n");
		printf("회로 : %d", from);
		return 1;
	}

	int connected_vertices[VERT_MAX];
	int connected_vertices_count = GetConnectedVertices(from, connected_vertices);
	int i;
	for (i = 0; i < connected_vertices_count; i++)
	{
		if (IsHamiltonianPathExist(connected_vertices[i], from))
		{
			printf(" %d", from);
			return 1;

		}
	}

	printf("도시%d 방문 취소(backtrack)\n", from);
	return 0;
}

int main(void)
{
	int i;

	while (1)
	{
		printf("정점의 개수를 입력하세요 : ");
		scanf("%u", &vn);
		if (vn <= VERT_MAX)
			break;
		printf("정점의 개수는 %d개 이하여야 합니다\n", VERT_MAX);
	}
	for (i = 0; i < vn; i++)
		AddVertex(i);

	while (1)
	{
		printf("간선의 개수를 입력하세요 : ");
		scanf("%u", &en);
		if (en <= EDGE_MAX)
			break;
		printf("간선의 개수는 %d개 이하여야 합니다\n", EDGE_MAX);
	}
	for (i = 0; i < en; i++)
	{
		while (1)
		{
			printf("간선%d데이터(정점 2개)를 입력하세요 : ", i);
			unsigned v1, v2;
			scanf("%u %u", &v1, &v2);
			if (v1 < vn && v2 < vn)
			{
				AddEdge(v1, v2);
				break;
			}
			printf("정점의 이름은 정점의 개수(=%d) 미만이어야 합니다\n", VERT_MAX);
		}
	}

	if (IsHamiltonianCircuitExist(0) == 0)
		printf("해밀턴 회로가 존재하지 않습니다");
	printf("\n");

	return 0;
}