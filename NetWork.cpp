#include <SFML/Network.hpp>
#include <iostream>
#include <unistd.h>
#include <chrono>
#include <thread>
#include <mutex>
#include <list>
#include "NetWork.hpp"


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


unsigned short SerCon(sf::TcpSocket &socket, std::string IpName, unsigned short START, unsigned short LAST, bool DEBAG)
{
	sf::Time t1 = sf::seconds(0.25);//Ожидание
	sf::IpAddress IP = IpName;
	sf::Packet packet;
	packet.clear();
	bool done = false;
	unsigned short i;
	for(i = START; i < LAST; i++)
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

/*
int NumClient()
{
	int s = 0;
	for(int i = 0; i < ClientTread.size(); i++)
		if(ClientTread[i] != 0)
			s++;
	return s;
}
*/
bool Srv::__RecData(sf::TcpSocket & socket)//you need char * only
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

	data[PortNum].resize(size);
	for(int i = 0; i < size; i++)
		new_data[i].Num = PortNum;

	// // // // // // 
	bigmutex.lock();

	for(int i = 0; i < size; i++)
		data[PortNum][i] = new_data[i];
	for(int i = 0; i < LAStPORT; i++)
		updata[i] = true;

	bigmutex.unlock();
	// // // // // // 
	delete [] new_data;

	std::cout << "==================================" << std::endl;
	std::cout << "  data come from port " 
		<< PortNum << std::endl;
	std::cout << "  itis: " 
		<< data[PortNum][0].Com << std::endl;
	std::cout << "==================================" << std::endl;
	
	return true;
}

void Srv::SendAllData(sf::TcpSocket & socket)
{
	int size = 0;
	int k = 0;

	bigmutex.lock();

	for(int i = 0; i < data.size(); i++)
		for(int j = 0; j < data[i].size(); j++)
			size++;

	SData * DataForSend = new SData [size];
	
	if(updata[socket.getLocalPort()]) // <- updata only
		for(int i = 0; i < data.size(); i++)
		{
			updata[socket.getLocalPort()] = false; 
			for(int j = 0; j < data[i].size(); j++)
				if(data[i][j].Num != 0)
				{
					DataForSend[k++] = data[i][j];
				}
		}

	bigmutex.unlock();
	_SendData(socket, DataForSend, size);

	delete [] DataForSend;	
}

void Srv::WorkingWithClient(sf::TcpSocket * sockets, int * ind)
{
	int MyPort = *ind - STARtPORT;
	delete ind;
	while(1)
	{
		if(!__RecData(sockets[MyPort]))
		{
			data[MyPort].clear();
			break;
		}

		SendAllData(sockets[MyPort]);

		usleep(15);
	}
}

void Srv::BigLins(sf::TcpSocket * sockets)
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

			auto _WWC = [&](int * ind)
			{
				WorkingWithClient(sockets, ind);
			};

			ClientTread[i - STARtPORT] = new std::thread(_WWC, ind);

			std::cout << "==================================" << std::endl;
			std::cout << "  Client connect to port " << i << std::endl;
			std::cout << "==================================" << std::endl;
			break;
		}
	}
}


void Srv::Server()
{
	auto _BL = [&]
	{
		BigLins(sockets);
	};
	std::thread LThread(_BL);
	char i;
	while(1)
	{
		std::cin >> i;
		for(int i = 0; i < data.size(); i++)
		{
			if(data[i].size() != 0)
			{
				std::cout << data[i][0].Num << ':';
				for(int j = 0; j < data[i].size(); j++)
					if(data[i][j].Num != 0)
						std::cout << data[i][j].Com << ' ';
				std::cout << std::endl;
			}
		}
	}
	LThread.join();
}

Srv::Srv(int SPort, int FPort)
{
	LAStPORT = FPort;
	STARtPORT = SPort;
	data.resize(LAStPORT);
	ClientTread.resize(LAStPORT);
	updata.resize(LAStPORT);
	sockets = new sf::TcpSocket[LAStPORT];
}

Srv::~Srv()
{
	delete [] sockets;
}

SData::SData(double x, double y, int act, std::string com):
	NewX(x), NewY(y), Act(act), Com(com) {}
