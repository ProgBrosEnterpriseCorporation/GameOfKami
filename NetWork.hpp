#ifndef NETWORK_HPP
#define NETWORK_HPP

#include <SFML/Network.hpp>
#include <iostream>
#include <unistd.h>
#include <chrono>
#include <thread>
#include <mutex>
#include <list>


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
unsigned short SerCon(sf::TcpSocket &socket, std::string IpName, unsigned short START = 10, unsigned short LAST = 3000, bool DEBAG = false);
bool _SendData(sf::TcpSocket &socket,SData* data, int size = 1);
SData * _RecData(sf::TcpSocket &socket, int &size);

class Srv 
{		
	std::vector<bool> updata;
	std::vector<std::vector<SData>> data;
	std::vector<std::thread*> ClientTread;
	std::mutex bigmutex;
	sf::TcpSocket * sockets;
	int STARtPORT;
        int LAStPORT;
	
	Srv(int,int);
	void BigLins(sf::TcpSocket * sockets);
	void WorkingWithClient(sf::TcpSocket * sockets, int * ind);
	void SendAllData(sf::TcpSocket & socket);
	bool __RecData(sf::TcpSocket & socket);//you need char * only
public:
	static Srv & get(int SPort, int FPort)
	{
		static Srv __MyServer(SPort,FPort);
		return __MyServer;
	}	
	void Server();
	~Srv();
};

#endif

