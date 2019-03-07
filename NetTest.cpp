#include<SFML/Network.hpp>
#include<iostream>
#include "NetWork.hpp"

struct A
{
	double a;
	void show()
	{
		std::cout << a << std::endl;
	}
};	

int main()
{	
	sf::IpAddress IP;	
	/* sf::IpAddress IP = sf::IpAddress::getLocalAddress(); // ServIP
	 * std::cout << IP << ' ' << sf::IpAddress::getPublicAddress() << std::endl;
	 */
	std::string IP_name;
	sf::TcpSocket socket;
	char buffer[2000];
	std::string text;
	char con = 0, mode = 0 ;
	//size_t received;
	std::cout << "Enter (s) for server, Enter (c) for client " << std::endl;
	std::cin >> con;	
	sf::Packet packet;

	if(con == 's')
	{
		//Server

		sf::IpAddress IP = sf::IpAddress::getLocalAddress(); // ServIP	
		OnePortLic(socket, 1233, true);
		std::cout <<  "Con. to Client" << std::endl;
		packet << "Con. to Server";
		mode = 's';
		socket.send(packet);
		packet.clear();
	}	
	else if(con == 'c')
	{	
		//Client
		
		std::cout << "Enter IP:";
		std::cin >> IP_name;
		SerCon(socket, IP_name);
		mode = 'r';
		socket.receive(packet);
		packet >> buffer;
		std::cout << buffer << std::endl;
		packet.clear();
	}
	std::getline(std::cin,text);
	bool done = false;
	while(!done)
		if(mode == 's')
		{
			double a;
			std::cout << "My:";
			//std::getline(std::cin,text);
			A B;
			std::cin >> a;
		       	B.a = a;
			//char * c = reinterpret_cast<char*>(&B);
			
			if(SendData(socket, &B, 1) == false)
				return 0;
			mode = 'r';
		}
		else if (mode == 'r')
		{
			//text
			A *B;
			if((B = RecData<A>(socket)) == 0)
				return 0;
			B -> show();
			delete [] B;
			mode = 's';
		}
	return 0;
		
}
