#ifndef NETWORK_HPP
#define NETWORK_HPP

void OnePortLic(sf::TcpSocket &socket, int port, bool DEBAG = false);

bool SerCon(sf::TcpSocket &socket, std::string IpName, bool DEBAG = false);

void SendData(sf::TcpSocket &socket, const char* data);

char * RecData(sf::TcpSocket &socket);
#endif
