#include "stdafx.h"
#include "Network.h"
#include "NetworkConfigParser.h"
#include "NetworkInfo.h"
#include "StaticSimulation.h"

#define PRINT_SEPARATOR() printf("*******************************************************\n")
FILE *redirectFile = NULL;

void ShowResults(Network& network, const StaticSimulation& staticSim);
void RedirectStdoutToFile(std::string file); // redireciona printf para um arquivo
void RestoreStdout();

int main()
{
	//Network network;
	//network.name = "";
	//NetworkConfigParser::Parse("config.txt", &network);
	
	auto TEST0 = []()->Network
	{
		// 1
		// | \
		// 2 - 3
		Network network;
		network.name = "TEST0";
		network.AddNode("n1", { Link("n2", 1, false), Link("n3", 1, false) });
		network.AddNode("n2", { Link("n1", 1, false), Link("n3", 1, false) });
		network.AddNode("n3", { Link("n2", 1, false), Link("n1", 1, false) });
		return network;
	};

	auto TEST1 = []()->Network
	{
		// 1
		// | \
		// 2 - 3 - 4 - 5 - 6 - 7
		Network network;
		network.name = "TEST1";
		network.AddNode("n1", { Link("n2", 1, false), Link("n3", 1, false) });
		network.AddNode("n2", { Link("n1", 1, false), Link("n3", 1, false) });
		network.AddNode("n3", { Link("n2", 1, false), Link("n1", 1, false), Link("n4", 1, false) });
		network.AddNode("n4", { Link("n3", 1, false), Link("n5", 1, false) });
		network.AddNode("n5", { Link("n4", 1, false), Link("n6", 1, false) });
		network.AddNode("n6", { Link("n5", 1, false), Link("n7", 1, false) });
		network.AddNode("n7", { Link("n6", 1, false) });
		return network;
	};

	auto TEST2 = []()->Network
	{
		// 1 - 4
		// |   |
		// 2 - 3
		Network network;
		network.name = "TEST2";
		network.AddNode("n1", { Link("n2", 1, false), Link("n4", 1, false) });
		network.AddNode("n2", { Link("n1", 1, false), Link("n3", 1, false) });
		network.AddNode("n3", { Link("n2", 1, false), Link("n4", 1, false) });
		network.AddNode("n4", { Link("n1", 1, false), Link("n3", 1, false) });
		return network;
	};

	auto TEST3 = []()->Network
	{
		//  1[c]-[c]4
		// [c]      |
		//  |       |
		// [c]      |
		//  2-------3
		Network network;
		network.name = "TEST3";
		network.AddNode("n1", { Link("n2", 1, true), Link("n4", 1, true) });
		network.AddNode("n2", { Link("n1", 1, true), Link("n3", 1, false) });
		network.AddNode("n3", { Link("n2", 1, false), Link("n4", 1, false) });
		network.AddNode("n4", { Link("n1", 1, true), Link("n3", 1, false) });
		return network;
	};

	auto TEST4 = []()->Network
	{
		// 1 - 2[conversor] - 3 - 4
		Network network;
		network.name = "TEST4";
		network.AddNode("n1", { Link("n2", 1, false) });
		network.AddNode("n2", { Link("n3", 1, true), Link("n1", 1, false) });
		network.AddNode("n3", { Link("n4", 1, false), Link("n2", 1, false) });
		network.AddNode("n4", { Link("n3", 1, false) });
		return network;
	};

	auto TEST5 = []()->Network
	{
		// 1[conversor] - 2[conversor] - 3[conversor] - 4
		Network network;
		network.name = "TEST5";
		network.AddNode("n1", { Link("n2", 1, true) });
		network.AddNode("n2", { Link("n3", 1, true), Link("n1", 1, false) });
		network.AddNode("n3", { Link("n4", 1, true), Link("n2", 1, false) });
		network.AddNode("n4", { Link("n3", 1, false) });
		return network;
	};

	auto TEST6 = []()->Network
	{
		// 1 - 2 - 3 - 4
		Network network;
		network.name = "TEST6";
		network.AddNode("n1", { Link("n2", 1, false) });
		network.AddNode("n2", { Link("n3", 1, false), Link("n1", 1, false) });
		network.AddNode("n3", { Link("n4", 1, false), Link("n2", 1, false) });
		network.AddNode("n4", { Link("n3", 1, false) });
		return network;
	};

	auto TEST7 = []()->Network
	{
		// 1 - 4 - 5 - 6
		// |   |
		// 2 - 3
		Network network;
		network.name = "TEST7";
		network.AddNode("n1", { Link("n2", 1, false), Link("n4", 1, false) });
		network.AddNode("n2", { Link("n1", 1, false), Link("n3", 1, false) });
		network.AddNode("n3", { Link("n2", 1, false), Link("n4", 1, false) });
		network.AddNode("n4", { Link("n1", 1, false), Link("n3", 1, false) });
		network.AddNode("n5", { Link("n4", 1, false), Link("n6", 1, false) });
		network.AddNode("n6", { Link("n5", 1, false) });
		return network;
	};

	auto RNP = []()->Network
	{
		// http://www.av.it.pt/anp/on/refnet2.html
		// RNP - http://www.av.it.pt/anp/on/figuras/rnp.jpg
		Network network;
		network.name = "RNP";
		network.AddNode("Fortaleza", { Link("BeloHorizonte", 1, false), Link("Recife", 1, false), });
		network.AddNode("Recife", { Link("Fortaleza", 1, false), Link("Salvador", 1, false), });
		network.AddNode("Salvador", { Link("Recife", 1, false), Link("RioDeJaneiro", 1, false), });
		network.AddNode("Brasilia", { Link("BeloHorizonte", 1, false), Link("SaoPaulo", 1, false), Link("SemNome", 1, false), });
		network.AddNode("BeloHorizonte", { Link("Fortaleza", 1, false),Link("Brasilia", 1, false), Link("RioDeJaneiro", 1, false), });
		network.AddNode("RioDeJaneiro", { Link("Salvador", 1, false), Link("BeloHorizonte", 1, false), Link("SaoPaulo", 1, false), });
		network.AddNode("SaoPaulo", { Link("Brasilia", 1, false), Link("RioDeJaneiro", 1, false), Link("Curitiba", 1, false), });
		network.AddNode("Curitiba", { Link("SaoPaulo", 1, false), Link("Florianopolis", 1, false), });
		network.AddNode("Florianopolis", { Link("Curitiba", 1, false), Link("SemNome", 1, false), });
		network.AddNode("SemNome", { Link("Florianopolis", 1, false), Link("Brasilia", 1, false), });
		return network;
	};

	auto NFSNET = []()->Network
	{
		// http://www.av.it.pt/anp/on/refnet2.html
		// NFSNET - http://www.av.it.pt/anp/on/figuras/nfsnet.jpg
		Network network;
		network.name = "NFSNET";
		network.AddNode("Seattler", { Link("PaloAlto", 1, false), Link("SanDiego", 1, false),  Link("Champaign", 1, false), });
		network.AddNode("PaloAlto", { Link("Seattler", 1, false), Link("SaltLKcity", 1, false),  Link("SanDiego", 1, false), });
		network.AddNode("SanDiego", { Link("PaloAlto", 1, false), Link("Seattler", 1, false),  Link("Houston", 1, false), });
		network.AddNode("SaltLKcity", { Link("PaloAlto", 1, false), Link("Boulder", 1, false),  Link("AnnArbor", 1, false), });
		network.AddNode("Boulder", { Link("SaltLKcity", 1, false), Link("Lincoln", 1, false),  Link("Houston", 1, false), });
		network.AddNode("Lincoln", { Link("Boulder", 1, false), Link("Champaign", 1, false), });
		network.AddNode("Houston", { Link("SanDiego", 1, false), Link("Boulder", 1, false),  Link("Atalanta", 1, false), Link("CollegePk", 1, false), });
		network.AddNode("Champaign", { Link("Lincoln", 1, false), Link("Seattler", 1, false),  Link("Pittsburgh", 1, false), });
		network.AddNode("AnnArbor", { Link("SaltLKcity", 1, false), Link("Princeton", 1, false),  Link("Ithaca", 1, false), });
		network.AddNode("Atalanta", { Link("Houston", 1, false), Link("Pittsburgh", 1, false), });
		network.AddNode("Pittsburgh", { Link("Atalanta", 1, false), Link("Champaign", 1, false),  Link("Ithaca", 1, false), Link("Princeton", 1, false), });
		network.AddNode("Ithaca", { Link("AnnArbor", 1, false), Link("Pittsburgh", 1, false),  Link("CollegePk", 1, false), });
		network.AddNode("CollegePk", { Link("Ithaca", 1, false), Link("Princeton", 1, false),  Link("Houston", 1, false), });
		network.AddNode("Princeton", { Link("CollegePk", 1, false), Link("Pittsburgh", 1, false),  Link("AnnArbor", 1, false), });
		return network;
	};

	auto GERMANY = []()->Network
	{
		// http://www.av.it.pt/anp/on/refnet2.html
		// GERMANY - http://www.av.it.pt/anp/on/figuras/germany.jpg
		Network network;
		network.name = "GERMANY";
		network.AddNode("Norden", { Link("Bremen", 1, false), Link("Dortmund", 1, false), });
		network.AddNode("Essen", { Link("Dortmund", 1, false), Link("Dusseldorf", 1, false), });
		network.AddNode("Dusseldorf", { Link("Essen", 1, false), Link("Koln", 1, false), });
		network.AddNode("Koln", { Link("Dusseldorf", 1, false), Link("Dortmund", 1, false), });
		network.AddNode("Dortmund", { Link("Essen", 1, false), Link("Koln", 1, false), Link("Norden", 1, false), Link("Hannover", 1, false), });
		network.AddNode("Bremen", { Link("Norden", 1, false), Link("Hamburg", 1, false), Link("Hannover", 1, false), });
		network.AddNode("Frankfurt", { Link("Koln", 1, false), Link("Hannover", 1, false), Link("Mannheim", 1, false), Link("Leipzig", 1, false), Link("Numberg", 1, false), });
		network.AddNode("Mannheim", { Link("Frankfurt", 1, false), Link("Karlruhe", 1, false), });
		network.AddNode("Karlruhe", { Link("Stuttgart", 1, false), Link("Mannheim", 1, false), });
		network.AddNode("Stuttgart", { Link("Karlruhe", 1, false), Link("Numberg", 1, false), Link("Ulm", 1, false), });
		network.AddNode("Hannover", { Link("Hamburg", 1, false), Link("Bremen", 1, false), Link("Dortmund", 1, false), Link("Frankfurt", 1, false), Link("Leipzig", 1, false), Link("Berlin", 1, false), });
		network.AddNode("Hamburg", { Link("Bremen", 1, false), Link("Hannover", 1, false), Link("Berlin", 1, false), });
		network.AddNode("Ulm", { Link("Stuttgart", 1, false), Link("Munich", 1, false), });
		network.AddNode("Munich", { Link("Ulm", 1, false), Link("Numberg", 1, false), });
		network.AddNode("Numberg", { Link("Munich", 1, false), Link("Leipzig", 1, false), Link("Frankfurt", 1, false), Link("Stuttgart", 1, false), });
		network.AddNode("Leipzig", { Link("Berlin", 1, false), Link("Hannover", 1, false), Link("Frankfurt", 1, false), Link("Numberg", 1, false), });
		network.AddNode("Berlin", { Link("Hamburg", 1, false), Link("Hannover", 1, false), Link("Leipzig", 1, false), });
		return network;
	};
	
	auto EON = []()->Network
	{
		// http://www.av.it.pt/anp/on/refnet2.html
		// EON - http://www.av.it.pt/anp/on/figuras/eon.jpg
		Network network;
		network.name = "EON";
		network.AddNode("Dublin", { Link("London", 1, false), Link("Paris", 1, false), });
		network.AddNode("Lisbon", { Link("London", 1, false), Link("Madrid", 1, false), });
		network.AddNode("London", { Link("Lisbon", 1, false), Link("Dublin", 1, false), Link("Oslo", 1, false), Link("Amesterdam", 1, false), Link("Berlim", 1, false), Link("SemNome", 1, false), Link("Paris", 1, false), });
		network.AddNode("Madrid", { Link("Lisbon", 1, false), Link("Paris", 1, false), });
		network.AddNode("Paris", { Link("Madrid", 1, false), Link("Dublin", 1, false), Link("London", 1, false), Link("SemNome", 1, false), Link("Zurich", 1, false), Link("Milan", 1, false), });
		network.AddNode("Amesterdam", { Link("London", 1, false), Link("SemNome", 1, false), Link("Prague", 1, false), Link("Berlim", 1, false), Link("Copenhagen", 1, false), });
		network.AddNode("SemNome", { Link("London", 1, false), Link("Paris", 1, false), Link("Zurich", 1, false), Link("Amesterdam", 1, false), });
		network.AddNode("Zurich", { Link("SemNome", 1, false), Link("Paris", 1, false), Link("Milan", 1, false), Link("Prague", 1, false), });
		network.AddNode("Milan", { Link("Paris", 1, false), Link("Zurich", 1, false), Link("Berlim", 1, false),  Link("Prague", 1, false), Link("Zagreb", 1, false), Link("Rome", 1, false), });
		network.AddNode("Oslo", { Link("London", 1, false), Link("Copenhagen", 1, false), Link("Stockholm", 1, false), });
		network.AddNode("Copenhagen", { Link("Oslo", 1, false), Link("Amesterdam", 1, false), Link("Stockholm", 1, false), });
		network.AddNode("Berlim", { Link("Amesterdam", 1, false), Link("London", 1, false), Link("Milan", 1, false), Link("Prague", 1, false), Link("Vienna", 1, false), Link("Moscow", 1, false), Link("Stockholm", 1, false), });
		network.AddNode("Prague", { Link("Berlim", 1, false), Link("Amesterdam", 1, false), Link("Zurich", 1, false), Link("Milan", 1, false), Link("Vienna", 1, false), });
		network.AddNode("Rome", { Link("Milan", 1, false), Link("Zagreb", 1, false), Link("Athens", 1, false), });
		network.AddNode("Zagreb", { Link("Milan", 1, false), Link("Rome", 1, false), Link("Athens", 1, false),  Link("Vienna", 1, false), });
		network.AddNode("Vienna", { Link("Zagreb", 1, false), Link("Prague", 1, false), Link("Berlim", 1, false), });
		network.AddNode("Stockholm", { Link("Oslo", 1, false), Link("Copenhagen", 1, false), Link("Berlim", 1, false), Link("Moscow", 1, false), });
		network.AddNode("Athens", { Link("Rome", 1, false), Link("Zagreb", 1, false), });
		network.AddNode("Moscow", { Link("Berlim", 1, false), Link("Stockholm", 1, false), });
		return network;
	};

	auto Run = [](Network network)
	{
		bool saveToFile = true; // salvar resultados em arquivo em vez de exibir na tela
		int debugLevel = 0; // nivel de depuracao
		int maxLambda = INFINITE_VAL;

		{

			Network tnetwork = network;
			PRINT_SEPARATOR();
			printf("\n");
			printf("[%s] Iniciando...\n", tnetwork.name.c_str());
			printf("\n");
			PRINT_SEPARATOR();

			StaticSimulation staticSim = StaticSimulation(&tnetwork, maxLambda);
			staticSim.debugLevel = debugLevel;

			if (saveToFile)
				RedirectStdoutToFile("resultado/result_dbg" + std::to_string(staticSim.debugLevel) + "_" + tnetwork.name + ".txt");

			staticSim.Run();
			ShowResults(tnetwork, staticSim);

			RestoreStdout();
		}

		PRINT_SEPARATOR();
		printf("\n");

		{
			Network tnetwork = network;

			// remove conversor de todos os nós
			for (auto& node : tnetwork.nodes) {
				for (auto& link : node.second.links)
					link.conversor = false;
			}

			PRINT_SEPARATOR();
			printf("\n");
			printf("[%s] Iniciando sem conversor...\n", tnetwork.name.c_str());
			printf("\n");
			PRINT_SEPARATOR();

			StaticSimulation staticSim = StaticSimulation(&tnetwork, maxLambda);
			staticSim.debugLevel = debugLevel;

			if (saveToFile)
				RedirectStdoutToFile("resultado/result_dbg" + std::to_string(staticSim.debugLevel) + "_sem_conversor_" + tnetwork.name + ".txt");

			staticSim.Run();
			ShowResults(tnetwork, staticSim);

			RestoreStdout();
		}

		PRINT_SEPARATOR();
		printf("\n");

		{
			Network tnetwork = network;

			// ativa conversor em todos os nós
			for (auto& node : tnetwork.nodes) {
				for (auto& link : node.second.links)
					link.conversor = true;
			}

			PRINT_SEPARATOR();
			printf("\n");
			printf("[%s] Iniciando com conversor em todos os links...\n", tnetwork.name.c_str());
			printf("\n");
			PRINT_SEPARATOR();

			StaticSimulation staticSim = StaticSimulation(&tnetwork, maxLambda);
			staticSim.debugLevel = debugLevel;

			if (saveToFile)
				RedirectStdoutToFile("resultado/result_dbg" + std::to_string(staticSim.debugLevel) + "_com_conversor_" + tnetwork.name + ".txt");

			staticSim.Run();
			ShowResults(tnetwork, staticSim);

			RestoreStdout();
		}

		PRINT_SEPARATOR();
		printf("\n");
		printf("[%s] Finalizado.\n", network.name.c_str());
		printf("\n");
	};
	
	//Run(TEST0());
	//Run(TEST1());
	//Run(TEST2());
	//Run(TEST3());
	//Run(TEST4());
	//Run(TEST5());
	//Run(TEST6());
	//Run(TEST7());	
	Run(RNP());
	Run(NFSNET());
	Run(GERMANY());
	Run(EON());

	PRINT_SEPARATOR();
	printf("\n");
	printf("Pressione qualquer tecla para continuar...\n");
	printf("\n");
	getchar();

	return 0;
}

void ShowResults(Network& network, const StaticSimulation& staticSim)
{
	if (staticSim.debugLevel >= 2)
	{
		PRINT_SEPARATOR();
		printf("* Matriz de custo:\n");
		network.PrintNodeMatrixNotInf(staticSim.costMatrix);
	}
	
	PRINT_SEPARATOR();
	printf("* Numero de nos: ");
	printf("%d\n", network.nodeCount);

	PRINT_SEPARATOR();
	printf("* Numero medio de hops: ");
	printf("%f\n", staticSim.avgHops);

	PRINT_SEPARATOR();
	int links = 0;
	int utilizationSum = 0;
	int costSum = 0;
	for (int i = 0; i < staticSim.connectionsMatrix.N; i++)
	{
		for (int j = 0; j < staticSim.connectionsMatrix.M; j++)
		{
			if (staticSim.connectionsMatrix[i][j] != 0 && staticSim.connectionsMatrix[i][j] != INFINITE_VAL)
			{
				links++;
				utilizationSum += staticSim.utilizationMatrix[i][j];
				costSum += staticSim.costMatrix[i][j];
			}
		}		
	}
	printf("* Numero de enlaces: %d\n", links);

	PRINT_SEPARATOR();
	printf("* Utilizacao media de enlace: %f\n", (float)utilizationSum / (float)links);

	PRINT_SEPARATOR();
	printf("* Custo medio: %f\n", (float)costSum / (float)links);

	PRINT_SEPARATOR();
	int numPaths = staticSim.minimumCostPaths.size();
	printf("* Numero de caminhos minimos: %d\n", numPaths);

	PRINT_SEPARATOR();
	printf("* Utilizacao de enlace:\n");
	//network.PrintNodeMatrix(staticSim.utilizationMatrix);
	//printf("\n");
	network.PrintPairMatrix(staticSim.utilizationMatrix);

	PRINT_SEPARATOR();
	printf("* Resultados das estrategias de alocacao de lambda com no maximo %d lambdas:\n", staticSim.maxLambda);

	for (int i = 0; i < STRATEGIES_COUNT; i++)
	{
		printf("%s: %d lambdas usados / %d conversoes feitas / %d alocacoes com falha\n", strategiesName[i], staticSim.strategiesLambdasCount[i], staticSim.strategiesConversionCount[i], staticSim.strategiesFailCount[i]);
	}

	printf("  Melhor estrategia encontrada para reduzir lambda: %s\n", strategiesName[staticSim.bestStrategyToReduceLambda]);
	printf("  Melhor estrategia encontrada para reduzir conversao: %s\n", strategiesName[staticSim.bestStrategyToReduceConversion]);
	printf("  Melhor estrategia encontrada para reduzir falhas: %s\n", strategiesName[staticSim.bestStrategyToReduceFailure]);

	PRINT_SEPARATOR();
	printf("* Alocacao de lambda por caminho minimo:\n");
	network.PrintLambdaPairMatrix(staticSim.lambdaMatrix);

	std::map<int, bool> usedLambdas;
	for (int i = 0; i < staticSim.lambdaMatrix.N; i++)
	{
		for (int j = 0; j < staticSim.lambdaMatrix.M; j++)
		{
			if (i == j)
				continue;

			for (const auto& k : staticSim.lambdaMatrix[i][j]) // percorre a lista de lambdas que foram alocados de i ate j
			{
				usedLambdas[k.lambda] = true; // salva o lambda para exibir depois					
			}
		}
	}
	printf("Lambdas usados (%d): ", usedLambdas.size());
	for (const auto& k : usedLambdas)
	{
		printf("L%d ", k.first);
	}
	printf("\n");


	PRINT_SEPARATOR();
	printf("* Conversoes por no:\n");
	for (unsigned i = 0; i < staticSim.conversionCounts.size(); i++)
	{
		auto nodeName = network.FindNameById(i);
		printf("%s %d\n", nodeName.c_str(), staticSim.conversionCounts[i]);
	}

	if (staticSim.debugLevel >= 3)
	{
		PRINT_SEPARATOR();
		printf("* Matriz de conexoes:\n");
		network.PrintNodeMatrix(staticSim.connectionsMatrix);

		PRINT_SEPARATOR();
		printf("* Matriz de distancia minima:\n");
		network.PrintNodeMatrixNotInf(staticSim.minimumDistanceMatrix);

		PRINT_SEPARATOR();
		printf("* Matriz de alocacao de lambda por caminho minimo (caso nao use conversor):\n");
		network.PrintLambdaPairMatrix(staticSim.lambdaMatrix);
		
		std::map<int, bool> usedLambdas;

		Matrix<int> tmpM; // matriz que mostra o numero de lambda alocado de (de i, ate j)
		tmpM.Create(staticSim.lambdaMatrix.N, staticSim.lambdaMatrix.M);
		tmpM.Fill(0);

		for (int i = 0; i < staticSim.lambdaMatrix.N; i++)
		{
			for (int j = 0; j < staticSim.lambdaMatrix.M; j++)
			{
				if (i == j)
					continue;

				for (const auto& k : staticSim.lambdaMatrix[i][j]) // percorre a lista de lambdas que foram alocados de i ate j
				{
					usedLambdas[k.lambda] = true; // salva o lambda para exibir depois
					tmpM[k.from][k.to] = k.lambda; // salva o numero de lambda que foi escolhido para o caminho do lambda atual
				}
			}
		}

		printf("Lambdas usados: ");
		for (const auto& k : usedLambdas)
		{
			printf("L%d ", k.first);
		}
		printf("\n");

		network.PrintNodeMatrix(tmpM);

		auto printRoute = [&](const std::vector<PathInfo>& paths)
		{
			int last = -1;
			for (const auto& p : paths)
			{
				auto fromName = network.FindNameById(p.from);
				auto toName = network.FindNameById(p.to);

				if (p.from != last)
				{
					printf("Partindo de %s\n", fromName.c_str());
				}
				if (p.hops.size() > 0)
				{
					printf(" ate %s: ", toName.c_str());

					printf("%s->", fromName.c_str());
					for (unsigned i = 0; i < p.hops.size(); i++)
					{
						auto& h = p.hops[i];
						printf("%s", network.FindNameById(h).c_str());
						if (i != p.hops.size() - 1)
							printf("->");
					}
				}
				else
				{
					printf(" nao pode chegar ate %s.", toName.c_str());
				}

				last = p.from;
				printf("\n");
			}
			printf("\n");
		};

		PRINT_SEPARATOR();
		printf("* Rotas minimas:\n");
		printRoute(staticSim.minimumCostPaths);

		PRINT_SEPARATOR();
		printf("* Todas as rotas:\n");

		std::vector<PathInfo> tmp;
		int N = network.nodeCount;

		for (int i = 0; i < N; i++)
		{
			for (int j = 0; j < N; j++)
			{
				if (i == j)
					continue;

				for (const auto& p : staticSim.allPaths[i][j])
					tmp.push_back(p);
			}
		}

		printRoute(tmp);
	}
}

void RedirectStdoutToFile(std::string file)
{
	if ((redirectFile = freopen(file.c_str(), "w", stdout)) == NULL)
	{
		printf("Cannot open file %s.\n", file.c_str());
		exit(1);
	}
}

void RestoreStdout()
{
	if (redirectFile != NULL)
		fclose(redirectFile);

	freopen("CONOUT$", "w", stdout);
	redirectFile = NULL;
}

