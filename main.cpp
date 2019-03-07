#include <SFML/Graphics.hpp>
#include <iostream>
#include "background.hpp"
#include "her.hpp"
#include<SFML/Network.hpp>
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

	

//    video mode initiation
	sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
	sf::RenderWindow window(desktop, "Omae wa mou shinderu? - NANIII!!??", sf::Style::Fullscreen);
    window.setVerticalSyncEnabled (true);
    sf::Vector2f resize_vector = sf::Vector2f((float)desktop.width/DefaultVideoWidth, 
                (float)desktop.height/DefaultVideoHeight);
    Background background("K3RpkJEqXeg.png", resize_vector);
    Hero hero("iii.png", resize_vector);
    Hero teammate("jjj.png", resize_vector);

    sf::Clock clock;

    while (window.isOpen())
    {
        float time = clock.restart().asSeconds();
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            window.close();
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
        {
            // Esc key is pressed: exit
            window.close();
        }

        hero.doPhysics(time);
	mode = 's';
	hero.sendNetPos(socket);
	mode = 'r';
	teammate.getNetPos(socket);

        window.clear();
        //window.draw(fight);
        window.draw(background);
        window.draw(hero);
	window.draw(teammate);
        window.display();
    }


    return 0;
}
