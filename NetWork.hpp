#ifndef NETWORK_HPP
#define NETWORK_HPP

void OnePortLic(sf::TcpSocket &socket, int port, bool DEBAG = false);

bool SerCon(sf::TcpSocket &socket, std::string IpName, bool DEBAG = false);

bool _SendData(sf::TcpSocket &socket,char* data, size_t size);

char * _RecData(sf::TcpSocket &socket, size_t size);//you need char * only

template <typename T>
bool SendData(sf::TcpSocket &socket, T* data, size_t num)
{
	return _SendData(socket, (char *)data, sizeof(T)*num);
}

template <typename T>
T* RecData(sf::TcpSocket &socket)
{
	return (T*)_RecData(socket, sizeof(T));
}


#endif
