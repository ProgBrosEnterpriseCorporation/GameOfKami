#include <SFML/Network.hpp>
#include <iostream>
#include <unistd.h>
#include <chrono>
#include <thread>
#include <mutex>
#include <list>
#include "NetWork.hpp"
#define STARtPORT 10
#define LAStPORT 3000


void OnePortLic(sf::TcpSocket &socket,int port, bool DEBAG)
{
	sf::TcpListener lis;

	if(DEBAG = true)
	{
		sf::IpAddress IP = sf::IpAddress::getLocalAddress();
		std::cout << "Server local IP:" << IP << std::endl;
	}

	lis.listen(port);
	lis.accept(socket);
}


unsigned short SerCon(sf::TcpSocket &socket, std::string IpName, bool DEBAG)
{
	sf::Time t1 = sf::seconds(0.25);//Ожидание
	sf::IpAddress IP = IpName;
	sf::Packet packet;
	packet.clear();
	bool done = false;
	unsigned short i;
	for(i = STARtPORT; i < LAStPORT; i++)
	{
		if(socket.connect(IP, i, t1) == 0)
		{
			done = true;
			if(DEBAG)
				std::cout << "Port:" << i << std::endl;
			break;
		}

	}
	if(!done)
		return 0;	
	return i;
}

bool _SendData(sf::TcpSocket &socket, SData* data, int size)
{
	sf::Packet packet;
	packet.clear();
	packet << size;
	for(int i = 0; i < size; i++)
	{
		packet << data[i].Num;
		packet << data[i].NewX;
		packet << data[i].NewY;
		packet << data[i].Act;
		packet << data[i].Com;
	}
	socket.send(packet);
	return true;
}

SData * _RecData(sf::TcpSocket &socket, int &size)//you need char * only
{
	sf::Socket::Status mys;
	size = 0;
	sf::Packet packet;
	packet.clear();
	mys = socket.receive(packet);

	if(mys == sf::Socket::Disconnected || mys == sf::Socket::Error)
	{
		std::cout << "==================================" << std::endl;
		std::cout << "  Client disconnect to port "
				<< socket.getLocalPort() << std::endl;
		std::cout << "==================================" << std::endl;
		socket.disconnect();
		size = -1;
		return 0;
	}

	packet >> size;
	SData * data = new SData[size];
	for(int i = 0; i < size; i++)
	{
		packet >> data[i].Num;
		packet >> data[i].NewX;
		packet >> data[i].NewY;
		packet >> data[i].Act;
		packet >> data[i].Com;
	}
	if(size == 0)
	{
		delete [] data;
		return 0;
	}
	return data;
}

// * * * * * * * SERVER * * * * * * * //
namespace srv
{
	std::vector<bool> updata;
	std::vector<std::vector<SData>> data;
	std::vector<std::thread*> ClientTread;
	std::mutex bigmutex;
}

int NumClient()
{
	int s = 0;
	for(int i = 0; i < srv::ClientTread.size(); i++)
		if(srv::ClientTread[i] != 0)
			s++;
	return s;
}

bool __RecData(sf::TcpSocket & socket)//you need char * only
{
	SData * new_data;
	int PortNum = socket.getLocalPort();
	int size = 0;

	socket.setBlocking(false);
	new_data = _RecData(socket,size);
	socket.setBlocking(true);

	if(size == -1)
		return false;

	if(size == 0)
		return true;

	srv::data[PortNum].resize(size);
	for(int i = 0; i < size; i++)
		new_data[i].Num = PortNum;

	// // // // // // 
	srv::bigmutex.lock();

	for(int i = 0; i < size; i++)
		srv::data[PortNum][i] = new_data[i];
	for(int i = 0; i < LAStPORT; i++)
		srv::updata[i] = true;

	srv::bigmutex.unlock();
	// // // // // // 
	delete [] new_data;

	std::cout << "==================================" << std::endl;
	std::cout << "  data come from port " 
		<< PortNum << std::endl;
	std::cout << "  itis: " 
		<< srv::data[PortNum][0].Com << std::endl;
	std::cout << "==================================" << std::endl;
	
	return true;
}

void SendAllData(sf::TcpSocket & socket)
{
	int size = 0;
	int k = 0;

	srv::bigmutex.lock();

	for(int i = 0; i < srv::data.size(); i++)
		for(int j = 0; j < srv::data[i].size(); j++)
			size++;

	SData * DataForSend = new SData [size];
	
	if(srv::updata[socket.getLocalPort()]) // <- updata only
		for(int i = 0; i < srv::data.size(); i++)
		{
			srv::updata[socket.getLocalPort()] = false; 
			for(int j = 0; j < srv::data[i].size(); j++)
				if(srv::data[i][j].Num != 0)
				{
					DataForSend[k++] = srv::data[i][j];
				}
		}

	srv::bigmutex.unlock();
	_SendData(socket, DataForSend, size);

	delete [] DataForSend;	
}

void WorkingWithClient(sf::TcpSocket * sockets, int * ind)
{
	int MyPort = *ind - STARtPORT;
	delete ind;
	while(1)
	{
		if(!__RecData(sockets[MyPort]))
		{
			srv::data[MyPort].clear();
			break;
		}

		SendAllData(sockets[MyPort]);

		usleep(15);
	}
}

void BigLins(sf::TcpSocket * sockets)
{
	while(1)
	{
		for(int i = STARtPORT; i < LAStPORT; i++)
		{		
			sf::TcpListener lis;	
			if(sockets[i-STARtPORT].getLocalPort() != 0)
				continue;

			if(lis.listen(i) == sf::Socket::Error)
				continue;

			lis.accept(sockets[i - STARtPORT]);

			int * ind = new int;
			*ind = i;
			srv::ClientTread[i - STARtPORT] = new std::thread(WorkingWithClient, sockets, ind);

			std::cout << "==================================" << std::endl;
			std::cout << "  Client connect to port " << i << std::endl;
			std::cout << "==================================" << std::endl;
			break;
		}
	}
}


void Server()
{
	srv::data.resize(LAStPORT);
	srv::ClientTread.resize(LAStPORT);
	srv::updata.resize(LAStPORT);
	sf::TcpSocket * sockets = new sf::TcpSocket[LAStPORT];
	std::thread LThread(BigLins, sockets);

	char i;
	while(1)
	{
		std::cin >> i;
		for(int i = 0; i < srv::data.size(); i++)
		{
			if(srv::data[i].size() != 0)
			{
				std::cout << srv::data[i][0].Num << ':';
				for(int j = 0; j < srv::data[i].size(); j++)
					if(srv::data[i][j].Num != 0)
						std::cout << srv::data[i][j].Com << ' ';
				std::cout << std::endl;
			}
		}
	}
	LThread.join();
}


SData::SData(double x, double y, int act, std::string com):
	NewX(x), NewY(y), Act(act), Com(com) {}
