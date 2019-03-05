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

void SendData(sf::TcpSocket &socket, const char* data)
{
	sf::Packet packet;
	std::string Data = data;
	packet << Data;
	socket.send(packet);
}

char * RecData(sf::TcpSocket &socket)//you need char * only
{
	std::string str;
	sf::Packet packet;
	socket.receive(packet);
	packet >> str;
	char * data = new char[str.size()+1];
	for(size_t i = 0; i < str.size()+1;i++)
		data[i] = str[i];
	return data;
}

