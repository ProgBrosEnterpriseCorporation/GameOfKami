#include<SFML/Network.hpp>
#include<iostream>

void OnePortLic(sf::TcpSocket &socket,int port, bool DEBAG = false)
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


bool SerCon(sf::TcpSocket &socket, std::string IpName, bool DEBAG = false)
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

bool SendData(sf::TcpSocket &socket,char* data, size_t size)
{
	sf::Packet packet;
	for(size_t i = 0; i < size; i++)
		data[i]++;
	packet << data;
	socket.send(packet);
	if(data == "!end")
	{
		socket.disconnect();
		return false;
	}
	return true;
}

char * RecData(sf::TcpSocket &socket, size_t size)//you need char * only
{
	std::string Data;
	char * data = new char[size];
	sf::Packet packet;
	socket.receive(packet);
	packet >> data;
	for(size_t i = 0; i < size; i++)
		data[i]--;	
	Data = data;
	if(Data == "!end")
	{
		socket.disconnect();
		return 0;
	}
	return data;
}
