#ifndef NETWORK_HPP
#define NETWORK_HPP

struct SData
{
	bool itis = true;
	int Num = 0;
	double NewX;
	double NewY;// location of hero;
	int Act;// all info about hero;
	std::string Com;// use 0 to show that 'Com' clear;
	SData(double x = 0, double y = 0, int act = 0, std::string com = "None");
};

void OnePortLic(sf::TcpSocket &socket, int port, bool DEBAG = false);

unsigned short SerCon(sf::TcpSocket &socket, std::string IpName, bool DEBAG = false);

bool _SendData(sf::TcpSocket &socket,SData* data, int size = 1);

SData * _RecData(sf::TcpSocket &socket, int &size);

void Server();

#endif

