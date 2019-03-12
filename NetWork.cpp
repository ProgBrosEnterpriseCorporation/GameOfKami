#include<SFML/Network.hpp>
#include<iostream>
#include "NetWork.hpp"
//TODO
// Реализовать серверное поведение, когда определимся с передоваемо структурои
// чтобы не заморачиваться с указателями на методы
// реализуется через неблокируемыи сокет, о таблицу общих данных

void OnePortLic(sf::TcpSocket &socket,int port, bool DEBAG)
{
	sf::TcpListener lis;

	if(DEBAG = true) // IP должен выдаваться отдельно, это исключиетльно для тестов
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
	for(i = 10; i < 3000; i++)
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
	for(SData* i = data; i < data+size; i++)
	{
		packet << data -> Num;
		packet << data -> NewX;
		packet << data -> NewY;
		packet << data -> Act;
		packet << data -> Com;
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
	}

	packet >> size;
	SData * data = new SData[size+1];
	data[size].itis = false;
	for(SData* i = data; i < data+size; i++)
	{
		packet >> data -> Num;
		packet >> data -> NewX; 
		packet >> data -> NewY;
		packet >> data -> Act;
		packet >> data -> Com;
	}
	if(size == 0)
		return 0;
	return data;
}

// * * * * * * * SERVER * * * * * * * //
/*
void ServerConcel( bool * done, sf::Thread * Lthread)
{
	while(!(*done))
	{	
		String comend;
		std::getlin(std::cin, comend);

		if(comend == "!end")
		{
			*done = true;
			Lthread -> 
		}
	}
}
*/
void BigLins(sf::TcpSocket * sockets)
{
	while(1)
	{
		for(int i = 10; i < 3000; i++)
		{		
			sf::TcpListener lis;	
			if(sockets[i-10].getLocalPort() != 0)
				continue;
			lis.listen(i);
			lis.accept(sockets[i-10]);

			std::cout << "==================================" << std::endl;
			std::cout << "  Client connect to port " << i << std::endl;
			std::cout << "==================================" << std::endl;
			break;
		}
	}
}

int NumOfClient(sf::TcpSocket * sockets)
{
	int s = 0;
	for(int i = 10; i < 3000; i++)
		if(sockets[i-10].getLocalPort())
			s++;
	return s;
}

SData ** RecAllData(sf::TcpSocket * sockets, int N)
{
	SData ** data = new SData * [N+1];
	data[N] = 0; // end of big-mass //
	int k = 0, size = 0;
	for(int i = 10; i < 3000; i++)
	{
		if(sockets[i-10].getLocalPort() == 0)
			continue;
		sockets[i-10].setBlocking(false);
		data[k++] = _RecData(sockets[i-10],size);

		for(int j = 0; j < size; j++)
			data[k-1][j].Num = i;

		if(size != 0)
		{
			std::cout << "==================================" << std::endl;
			std::cout << "  Data come from port " 
				<< sockets[i-10].getLocalPort() << std::endl;
			std::cout << "==================================" << std::endl;
		}
		sockets[i-10].setBlocking(true);
	}
	return data;
}

void SendAllData(sf::TcpSocket * sockets, SData ** data, int N)
{
	int size = 0;
	int k = 0;
	for(int i = 0; i < N; i++)
	{
		if(data[i] == 0)
			continue;
		for(int j = 0; data[i][j].itis != false; j++)
			size++;
	}
	SData * DataForSend = new SData [size];
	
	for(int i = 0; i < N; i++)
	{
		if(data[i] == 0)
			continue;
		for(int j = 0; data[i][j].itis != false; j++)
			DataForSend[k++] = data[i][j];
	}

	for(int i = 10; i < 3000; i++)
		if(sockets[i-10].getLocalPort() != 0)
			_SendData(sockets[i-10], DataForSend, size);	
}

void Server()
{
	bool done = false;
	sf::TcpSocket sockets[3000];
	sf::Thread Lthread(&BigLins, sockets);
	Lthread.launch();
	while(!done)
	{
		int N = NumOfClient(sockets);
		SData ** data;
		data = RecAllData(sockets, N);


		// * * * * TEST_OF_SERVER * * * * //
		for(int i = 0; i < N; i++)
		{
			if(data[i] == 0)
				continue;
			for(int j = 0; data[i][j].itis != false; j++)
				std::cout << data[i][j].Com << std::endl;
		}


		// you backend will be hear !!!!!


		SendAllData(sockets, data, N);

		for(int i = 0; i < N+1; i++)
			delete [] data[i];
		delete [] data;
	}
}


SData::SData(double x, double y, int act, std::string com):
	NewX(x), NewY(y), Act(act), Com(com) {}
