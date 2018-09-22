// IC.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

const int INFINITE_VAL = 999999;

template<class T> class Matrix
{
public:
	int N, M;
	T **matrix;

	Matrix(int N, int M) : N(N), M(M)
	{
		matrix = (T**)malloc(N * sizeof(T*));
		for (int i = 0; i < N; i++)
		{
			matrix[i] = (T*)malloc(M * sizeof(T));
			//for (int j = 0; j < M; j++)
			//	matrix[i][j] = 0;
		}
	}

	void Fill(T val)
	{
		for (int i = 0; i < N; i++)
			for (int j = 0; j < M; j++)
				matrix[i][j] = val;
	}

	~Matrix()
	{
		for (int i = 0; i < N; i++)
			free(matrix[i]);
		free(matrix);
	}

	T*& operator[](const int &index) const
	{
		return matrix[index];
	}
};

class Link
{
public:
	Link(std::string to, int cost) : to(to), cost(cost) { }
	std::string to;
	int cost;
};

class Node
{
public:
	Node() { }
	Node(int id, std::vector<Link> links) : id(id), links(links) { }
	int id;
	std::vector<Link> links = {};
};

class PathInfo
{
public:
	PathInfo() { }
	PathInfo(int from, int to, std::vector<int> hops) : from(from), to(to), hops(hops) { }
	int from;
	int to;
	std::vector<int> hops = {};
};

/*	
	auto PrintMatrix = [](int **m, int N)
	{
		for (int i = 0; i < N; i++)
		{
			for (int j = 0; j < N; j++)
			{
				if (m[i][j] != INFINITE_VAL)
					printf("%d ", m[i][j]);
				else
					printf("X ");
			}
			printf("\n");
		}
		printf("\n");
	};
	auto PrintVector = [](int *m, int N)
	{
		for (int i = 0; i < N; i++)
		{
			if (m[i] != INFINITE_VAL)
				printf("%d ", m[i]);
			else
				printf("X ");
		}
		printf("\n");
	};
*/

void Dijkstra(int from, const Matrix<int>& cost, int *distance, int N, std::vector<PathInfo>& paths)
{
	// adaptado: https://www.thecrazyprogrammer.com/2014/03/dijkstra-algorithm-for-finding-shortest-path-of-a-graph.html

	std::vector<bool> visited(N);
	std::vector<int> previous(N);
		
	for (int i = 0; i < N; i++)
	{
		if (cost[from][i] != INFINITE_VAL)
		{
			distance[i] = cost[from][i];
			previous[i] = from;
		}
		else
		{
			distance[i] = INFINITE_VAL;
			previous[i] = INFINITE_VAL;
		}

		visited[i] = false;
	}

	distance[from] = 0;
	visited[from] = true;
	int nextNode = -1;

	for (int count = 1; count < N - 1; count++)
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

		std::vector<int> tmp = { };

		int j = i;
		while (j != from && previous[j] != INFINITE_VAL)
		{
			tmp.push_back(j);
			j = previous[j];
		};

		std::reverse(tmp.begin(), tmp.end());
		paths.push_back(PathInfo(from, i, tmp));
	}
}

class Network
{
public:
	int lastNodeId = 0;
	std::map<std::string, Node> nodes = {};

	// adiciona um no na rede
	void AddNode(std::string name, std::vector<Link> links)
	{
		nodes[name] = Node(lastNodeId++, links);
	}

	// retorna o nome de um no a partir de seu id
	std::string FindNameById(int id)
	{
		for (const auto& node : nodes)
			if (node.second.id == id)
				return node.first;
		return "NOT FOUND";
	}
	
	// exibe a matriz com os nomes dos nos colocando X em valores infinitos
	void PrintNodeMatrixNotInf(const Matrix<int>& matrix)
	{
		printf("   ");
		for (int i = 0; i < matrix.N; i++)
		{
			printf("%3s ", FindNameById(i).c_str());
		}
		printf("\n");
		for (int i = 0; i < matrix.N; i++)
		{
			printf("%3s ", FindNameById(i).c_str());
			for (int j = 0; j < matrix.N; j++)
			{
				if (matrix[i][j] < INFINITE_VAL)
					printf("%-3d ", matrix[i][j]);
				else
					printf("XXX ");
			}
			printf("\n");
		}
	}

	// exibe a matriz com os nomes dos nos
	void PrintNodeMatrix(const Matrix<int>& matrix)
	{
		printf("   ");
		for (int i = 0; i < matrix.N; i++)
		{
			printf("%3s ", FindNameById(i).c_str());
		}
		printf("\n");
		for (int i = 0; i < matrix.N; i++)
		{
			printf("%3s ", FindNameById(i).c_str());
			for (int j = 0; j < matrix.N; j++)
			{
				printf("%-3d ", matrix[i][j]);
			}
			printf("\n");
		}
	}

	// exibe o valores dos pares da matriz
	void PrintPairMatrix(const Matrix<int>& matrix)
	{
		for (int i = 0; i < matrix.N; i++)
		{
			for (int j = 0; j < matrix.N; j++)
			{
				if (i == j || matrix[i][j] == 0)
					continue;
				printf("(%s,%s) ", FindNameById(i).c_str(), FindNameById(j).c_str());
				printf("%d", matrix[i][j]);
				printf("\n");
			}
		}
	}
};

int main()
{
	Network network;
	/*
	1 - 4
	|   |
	2 - 3	
	*/
	network.AddNode("n1", { Link("n2", 1), Link("n4", 1) });
	network.AddNode("n2", { Link("n1", 1), Link("n3", 1) });
	network.AddNode("n3", { Link("n2", 1), Link("n4", 1) });
	network.AddNode("n4", { Link("n1", 1), Link("n3", 1) });
	int N = network.lastNodeId;

	Matrix<int> connectionsMatrix(N, N);
	Matrix<int> costMatrix(N, N);
	Matrix<int> minimumDistanceMatrix(N, N);
	Matrix<int> utilizationMatrix(N, N);

	connectionsMatrix.Fill(0); // matriz que diz se existe conexao entre (i,j)
	costMatrix.Fill(INFINITE_VAL); // matriz que diz qual o custo para ir diretamente (de i, ate j), infinito caso nao seja possivel
	minimumDistanceMatrix.Fill(0); // matriz que possui a distancia minima para ir, passando por um ou mais nos, (de i, ate j), infinito caso nao seja possivel
	utilizationMatrix.Fill(0); // matriz que possui o numero de vezes que um caminho minimo passa por (i, j)

	for (const auto& node : network.nodes)
	{
		auto i = node.second.id;
		costMatrix[i][i] = 0;
		for (const auto& path : node.second.links)
		{
			auto j = network.nodes[path.to].id;
			connectionsMatrix[i][j] = 1;
			costMatrix[i][j] = path.cost;
		}
	}

	printf("* Matriz de conexoes:\n");
	network.PrintNodeMatrix(connectionsMatrix);
	printf("\n");
	
	printf("* Matriz de distancia minima:\n");
	std::vector<PathInfo> paths = {};
	for (int i = 0; i < N; i++)
		Dijkstra(i, costMatrix, minimumDistanceMatrix[i], N, paths);
	network.PrintNodeMatrixNotInf(minimumDistanceMatrix);
	printf("\n");
	
	printf("* Rotas:\n");
	int last = -1;
	for (auto& p : paths)
	{
		if (p.from != last)
		{
			printf("Partindo de %s\n", network.FindNameById(p.from).c_str());
		}
		if (p.hops.size() > 0)
		{
			printf(" ate %s: ", network.FindNameById(p.to).c_str());

			printf("%s->", network.FindNameById(p.from).c_str());
			for (unsigned i = 0; i < p.hops.size(); i++ )
			{
				auto& h = p.hops[i];
				printf("%s", network.FindNameById(h).c_str());
				if (i != p.hops.size() - 1)
					printf("->");
			}
		}
		else
		{
			printf(" nao pode chegar ate %s.", network.FindNameById(p.to).c_str());
		}

		printf("\n");
		last = p.from;
	}
	printf("\n");

	printf("* Numero medio de hops: ");

	int sum = 0;
	int k = 0;

	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			if (minimumDistanceMatrix[i][j] != INFINITE_VAL && minimumDistanceMatrix[i][j] > 0)
			{
				sum += minimumDistanceMatrix[i][j];
				k++;
			}
		}
	}

	float avgH = (float)sum / (float)k;
	printf("%f\n", avgH);
	printf("\n");

	printf("* Matriz de utilizacao de enlace:\n");

	for (auto& p : paths)
	{
		int from = p.from;
		for (unsigned i = 0; i < p.hops.size(); i++)
		{
			int to = p.hops[i];			
			utilizationMatrix[from][to]++;
			from = to;
		}
	}
	network.PrintNodeMatrix(utilizationMatrix);
	printf("\n");
	network.PrintPairMatrix(utilizationMatrix);	
	printf("\n");

	getchar();

	return 0;
}

