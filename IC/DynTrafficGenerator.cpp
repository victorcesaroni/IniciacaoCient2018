#include "stdafx.h"
#include "DynamicSimulation.h"
#include "DynTrafficGenerator.h"
#include "DynPacket.h"

namespace DynamicSimulation
{
	static tick_t gPacketCounter = 0;

	TrafficGenerator::TrafficGenerator()
		: maxNodes(0), lastCreatedTick(0)
	{
		numberOfPackets = 1;
		interval = 0;
		chance = 1.0f;
	}

	bool TrafficGenerator::CreatePackets(tick_t tick, int currentNode, std::vector<Packet>& packets)
	{
		// espera o intervalo
		if (tick < lastCreatedTick + interval)
			return false;

		int source = currentNode;

		std::vector<int> tmp(availableNodes);

		// remove o no atual da lista de possiveis nos
		for (auto it = tmp.begin(); it != tmp.end(); )
		{
			if (*it == source)
				it = tmp.erase(it);
			else
				++it;
		}

		// tenta criar numberOfPackets pacotes
		for (int i = 0; i < numberOfPackets; i++)
		{
			if ((rand() % 100) / 100.0f <= chance)
			{
				Packet packet = Packet(-1, -1, -1, -1);
				
				int destination = tmp[rand() % tmp.size()]; // seleciona um destino aleatorio

				packet.currentNode = currentNode;
				packet.id = gPacketCounter;
				packet.size = 1;
				packet.source = source;
				packet.destination = destination;

				packets.push_back(packet);

				gPacketCounter++;
				lastCreatedTick = tick;
			}
		}

		return !packets.empty();
	}
	
	void TrafficGenerator::UpdateNodeCount(int newCount)
	{
		maxNodes = newCount;
		availableNodes.clear();

		for (int i = 0; i < newCount; i++)
			availableNodes.push_back(i);
	}
};
