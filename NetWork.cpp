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

	sf::Packet packet;
	packet << true; // Отправляем сообщение о том, что соединение успешно;
	socket.send(packet); // Других и не бывает;
	packet.clear();
}


bool SerCon(sf::TcpSocket &socket, std::string IpName, bool DEBAG)
{
	sf::Time t1 = sf::seconds(0.25);//Ожидание
	sf::IpAddress IP = IpName;
	sf::Packet packet;
	bool done = false;
	short tern = 0; 
	do
		for(int i = 10; i < 3000; i++)
		{
			if(socket.connect(IP, i, t1) == 0)
			{
				done = true;
				if(DEBAG == true)
					std::cout << "Port:" << i << std::endl;
				break;
			}

		}
	while(tern++ != 100 && !done);//будет 100 попытки подключится к серверу

	if(!done)
		return false;
	socket.receive(packet);//принимает сообщение thue
	packet.clear();
	
	return true;
}

bool _SendData(sf::TcpSocket &socket, SData* data, int size)
{
	sf::Packet packet;
	packet << size;
	for(SData* i = data; i < data+size; i++)
	{
		packet << data -> NewX;
		packet << data -> NewY;
		packet << data -> Act;
		packet << data -> Com;
	}
	socket.send(packet);
	if(data -> Com == "!end")
	{
		socket.disconnect();
		return false;
	}
	return true;
}

SData * _RecData(sf::TcpSocket &socket, int &size)//you need char * only
{
	sf::Packet packet;
	//socket.setBlocking(true);
	socket.receive(packet);
	//socket.setBlocking(false);
	packet >> size;
	SData * data = new SData[size];
	for(SData* i = data; i < data+size; i++)
	{
		packet >> data -> NewX; 
		packet >> data -> NewY;
		packet >> data -> Act;
		packet >> data -> Com;
	}
	if(data -> Com == "!end")
	{
		socket.disconnect();
		return 0;
	}
	return data;
}

// ***********SERVER******************* //

void Server()
{
	sf::TcpSocket socket[3000];
	
}



SData::SData(double x, double y, int act, std::string com):
		NewX(x), NewY(y), Act(act), Com(com) {}
