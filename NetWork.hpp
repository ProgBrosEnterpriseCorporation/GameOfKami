#ifndef NETWORK_HPP
#define NETWORK_HPP

void OnePortLic(sf::TcpSocket &socket, int port, bool DEBAG = false);

bool SerCon(sf::TcpSocket &socket, std::string IpName, bool DEBAG = false);

bool SendData(sf::TcpSocket &socket, char* data, size_t size);

char * RecData(sf::TcpSocket &socket, size_t size); // use delete [] to free arg;



#endif
