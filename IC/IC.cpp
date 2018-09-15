// IC.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

const int INFINITE_VAL = 9999999;

class Connection
{
public:
	Connection(std::string to, int cost) : to(to), cost(cost) { }
	std::string to;
	int cost;
};

class Node
{
public:
	Node() { }
	Node(int id, std::vector<Connection> connections) : id(id), connections(connections) { }
	int id;
	std::vector<Connection> connections;
};

class PathInfo
{
public:
	PathInfo() { }
	PathInfo(int from, std::vector<int> hops) : from(from), hops(hops) { }
	int from;
	std::vector<int> hops;
};

void FreeMatrix(int **m, int N, int M)
{
	for (int i = 0; i < N; i++)
		free(m[i]);	
	free(m);
}
int **CreateMatrix(int N, int M)
{
	int **m = (int**)malloc(N * sizeof(int*));
	for (int i = 0; i < N; i++)
	{
		m[i] = (int*)malloc(M * sizeof(int));
		for (int j = 0; j < M; j++)
			m[i][j] = 0;
	}
	return m;
}

void Dijkstra(int from, int **matrix, int *distance, int N, std::vector<PathInfo>& paths)
{
	// adaptado: https://www.thecrazyprogrammer.com/2014/03/dijkstra-algorithm-for-finding-shortest-path-of-a-graph.html

	bool *visited = (bool*)malloc(N * sizeof(bool));
	int *previous = (int*)malloc(N * sizeof(int));
	int **cost = CreateMatrix(N, N);
	
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			if (matrix[i][j] > 0)
				cost[i][j] = matrix[i][j];
			else
				cost[i][j] = INFINITE_VAL;
		}

		visited[i] = false;
		previous[i] = from;
		distance[i] = cost[from][i];
	}

	distance[from] = 0;
	visited[from] = true;
	int nextNode = -1;

	for (int n = 1; n < N - 1; n++)
	{
		int minDistance = INFINITE_VAL;

		for (int i = 0; i < N; i++)
		{
			if (visited[i])
				continue;

			if (distance[i] < minDistance)
			{
				minDistance = distance[i];
				nextNode = i;
			}
		}

		visited[nextNode] = true;
		for (int i = 0; i < N; i++)
		{
			if (visited[i])
				continue;

			if (minDistance + cost[nextNode][i] < distance[i])
			{
				distance[i] = minDistance + cost[nextNode][i];
				previous[i] = nextNode;
			}
		}
	}

	for (int i = 0; i < N; i++)
	{
		if (i == from)
			continue;

		std::vector<int> tmp;
		tmp.clear();
		int j = i;
		tmp.push_back(j);
		do
		{
			j = previous[j];
			tmp.push_back(j);
		} while (j != from);

		std::reverse(tmp.begin(), tmp.end());
		paths.push_back(PathInfo(from, tmp));
	}

	free(visited);
	free(previous);
	FreeMatrix(cost, N, N);
}

int main()
{
	std::map<std::string, Node> nodes;

	/*
	1 - 4
	|   |
	2 - 3	
	*/

	const int cost = 1;
	unsigned N = 0;
	nodes["1"] = Node(N++, { Connection("2", cost), Connection("4", cost) });
	nodes["2"] = Node(N++, { Connection("1", cost), Connection("3", cost) });
	nodes["3"] = Node(N++, { Connection("2", cost), Connection("4", cost) });
	nodes["4"] = Node(N++, { Connection("1", cost), Connection("3", cost) });

	auto FindNameById = [&](int id) -> std::string
	{
		for (const auto& node : nodes)
			if (node.second.id == id)
				return node.first;
		return "NOT FOUND";
	};

	auto PrintNodeMatrix = [&](int **matrix, int N) -> void
	{
		printf("   ");
		for (int i = 0; i < N; i++)
		{
			printf("%s ", FindNameById(i).c_str());
		}
		printf("\n");
		printf("\n");
		for (int i = 0; i < N; i++)
		{
			printf("%s  ", FindNameById(i).c_str());
			for (int j = 0; j < N; j++)
			{
				printf("%d ", matrix[i][j]);
			}
			printf("\n");
		}
	};

	auto PrintNodeMatrixNotInf = [&](int **matrix, int N) -> void
	{
		printf("   ");
		for (int i = 0; i < N; i++)
		{
			printf("%s ", FindNameById(i).c_str());
		}
		printf("\n");
		printf("\n");
		for (int i = 0; i < N; i++)
		{
			printf("%s  ", FindNameById(i).c_str());
			for (int j = 0; j < N; j++)
			{
				if (matrix[i][j] < INFINITE_VAL)
					printf("%d ", matrix[i][j]);
				else
					printf("X ");
			}
			printf("\n");
		}
	};
	
	int **connectionsMatrix = CreateMatrix(N, N);
	int **minimumDistanceMatrix = CreateMatrix(N, N);

	for (const auto& node : nodes)
	{
		auto i = node.second.id;
		for (const auto& path : node.second.connections)
		{
			auto j = nodes[path.to].id;
			connectionsMatrix[i][j] = path.cost;
		}
	}

	printf("* Matrix de conexoes:\n");
	PrintNodeMatrix(connectionsMatrix, N);
	
	printf("* Matrix de caminho minimo:\n");
	std::vector<PathInfo> paths;
	paths.clear();
	for (int i = 0; i < N; i++)
		Dijkstra(i, connectionsMatrix, minimumDistanceMatrix[i], N, paths);
	PrintNodeMatrixNotInf(minimumDistanceMatrix, N);
	
	printf("* Rotas:\n");
	int last = -1;
	for (auto& p : paths)
	{
		if (p.from != last)
		{
			printf("Partindo de %s:\n", FindNameById(p.from).c_str());
		}				
		for (auto& h : p.hops)
		{
			printf("%s->", FindNameById(h).c_str());			
		}
		printf("|\n");
		last = p.from;
	}

	printf("* Numero medio de hops: ");

	int sum = 0;

	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			sum += minimumDistanceMatrix[i][j];
		}
	}

	float H = (float)sum / (float)(N*(N - 1));
	printf("%f\n", H);

	FreeMatrix(connectionsMatrix, N, N);
	FreeMatrix(minimumDistanceMatrix, N, N);
	getchar();

	return 0;
}

