#include<SFML/Network.hpp>
#include<iostream>
#include "NetWork.hpp"
#include <SFML/Graphics.hpp>


unsigned short MyPort = 0;

void foo1(sf::TcpSocket * socket)
{
	while(1)
	{
		SData S;
		std::getline(std::cin,S.Com);
		if(!_SendData(*socket, &S, 1))
			return;
	}

}

void foo2(sf::TcpSocket * socket)
{
	while(1)
	{
		SData *D = 0;
		int itsize;
		if((D = _RecData(*socket,itsize)) == 0)
			continue;
		for(int i = 0; i < itsize; i++)
			if(D[i].Num != MyPort)
				std::cout << "from " 
				       << D[i].Num << ": "
			       	       << D[i].Com << std::endl;
		delete [] D;
	}
}

int main()
{	
	std::string IP_name;
	sf::TcpSocket socket;
	char buffer[2000];
	std::string text;
	char con = 0, mode = 0 ;
	std::cout << "Enter (s) for server, Enter (c) for client " << std::endl;
	std::cin >> con;	
	sf::Packet packet;
	sf::IpAddress IP = sf::IpAddress::getLocalAddress(); // ServIP
	if(con == 's')
	{
		std::cout << IP << std::endl;
		Server();
	}	
	else if(con == 'c')
	{	
		std::cout << "Enter IP:";
		std::cin >> IP_name;
		MyPort = SerCon(socket, IP_name);
		std::cout << "==================================" << std::endl;
		std::cout << "  You connect to port " << MyPort << std::endl;
		std::cout << "==================================" << std::endl;
	}
	std::getline(std::cin,text);
	sf::Thread treadS(&foo1, &socket);
	sf::Thread treadR(&foo2, &socket);
	treadS.launch();
	treadR.launch();
	treadS.wait();
	treadR.wait();	
	return 0;

}
