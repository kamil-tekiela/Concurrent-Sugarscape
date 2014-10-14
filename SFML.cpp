#include "stdafx.h"

//duplicated constants; see tile.h
#define GRIDW 50
#define GRIDH 50
#define TILEW 10
#define TILEH 10
#define AGENTS 400


int main()
{
    sf::RenderWindow window(sf::VideoMode(GRIDW*TILEW, GRIDH*TILEH), "SFML works!", sf::Style::Titlebar | sf::Style::Close, sf::ContextSettings::ContextSettings 	(0,0,4,0,0));
	window.setFramerateLimit(3); // call it once, after creating the windo
	
	sf::Clock clock;

	// we create sugarscape
	static Tile tile[GRIDW][GRIDH];
	for(int i=0;i<GRIDW;i++){
		for(int j=0;j<GRIDH;j++){
			tile[i][j] = Tile(i,j);
		}
	}

	// we create random agents
	//Agent agent[AGENTS];
	std::vector<Agent> agent(AGENTS, Agent());
	for(int c=0;c<AGENTS;c++){
		int x,y;
		do{
			x = rand()%GRIDW;
			y = rand()%GRIDW;
		}while(tile[x][y].isTaken());
		agent.at(c) = Agent(x,y);
		tile[x][y].eat();
	}
		
	sf::Time time = clock.getElapsedTime();


    while (window.isOpen())
    {
		time = clock.getElapsedTime();

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
			else if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::Escape)
				{
					window.close();
				}
			}
        }
		
		//update
		for(int i=0;i<GRIDW;i++){
			for(int j=0;j<GRIDH;j++){
				tile[i][j].grow();
			}
		}
		
		int sugar = 0;
		double metabol = 0;
		bool temp;
		for(int c=0;c<agent.size();c++){
			sugar += agent.at(c).getWealth();
			metabol += agent.at(c).getMetabolRate();
			temp = agent.at(c).update(tile);
			sf::Vector2i vecT = agent.at(c).getCoord();
			if(!temp) {
				//erase moves back all elements
				agent.erase(agent.begin()+c);
				c--;
				tile[vecT.x][vecT.y].freeUp();
			}
		}


		
		std::cout << agent.size() << '\t' << sugar << '\t' << (int)(sugar/agent.size()) << '\t' << (double)(metabol/agent.size())  << '\n';
		//std::cout << "People " << (int) humans.size() << "\t" << count  << "\t" << canes  << "\t" << empty << "\t" << (count+canes+empty)  << '\n';


		//DRAW
        window.clear(sf::Color::White);

		for(int i=0;i<GRIDW;i++)
		for(int j=0;j<GRIDH;j++){
			window.draw(tile[i][j]);
		}

		for(int j=0;j<agent.size();j++){
			window.draw(agent.at(j));
		}
        window.display();
    }

    return 0;
}