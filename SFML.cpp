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
	window.setFramerateLimit(30); // call it once, after creating the window
	
	sf::Clock clock;
	
	double aveSugar;
	double aveVision;
	int years=0;

	// we create sugarscape
	static Tile tile[GRIDW][GRIDH];
	for(int i=0;i<GRIDW;i++){
		for(int j=0;j<GRIDH;j++){
			tile[i][j] = Tile(i,j);
		}
	}

	// we create random agents
	//Agent agent[AGENTS];
	std::list<Agent> agent(AGENTS, Agent());
	for(std::list<Agent>::iterator it=agent.begin(); it != agent.end(); ++it){
	//for(int c=0;c<AGENTS;c++){
		int x,y;
		do{
			x = rand()%GRIDW;
			y = rand()%GRIDW;
		}while(tile[x][y].isTaken());
		*it = Agent(x,y);
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
		years++;

		for(int i=0;i<GRIDW;i++){
			for(int j=0;j<GRIDH;j++){
				tile[i][j].grow();
			}
		}
		
		int sugar = 0;
		double vision = 0;
		double metabol = 0;
		bool temp;
		int people = 0;
		//for(int c=0;c<agent.size();c++){
		//for(std::list<Agent>::iterator it=agent.begin(); it != agent.end(); ++it){
		std::list<Agent>::iterator it=agent.begin();
		while(it!=agent.end()){
			people++;
			temp =				(*it).update(tile, agent, aveVision);
			sugar +=			(*it).getWealth();
			vision +=			(*it).getVision();
			metabol +=			(*it).getMetabolRate();
			sf::Vector2i vecT = (*it).getCoord();
			if(!temp) {
				// //erase moves back all elements
				it =			agent.erase(it);
				//c--;
				tile[vecT.x][vecT.y].freeUp();
			}
			else
				++it;
		}


		if(agent.size()){
		aveSugar = (int)sugar/people;
		aveVision = vision/people;
		std::cout << (int)years/10 << "\t" << agent.size() << "\tS: " << aveSugar << "\tM: " << (double)(metabol/people) << "\tV: " << aveVision  << '\n';
		//std::cout << "People " << (int) humans.size() << "\t" << count  << "\t" << canes  << "\t" << empty << "\t" << (count+canes+empty)  << '\n';
		}


		//DRAW
        window.clear(sf::Color::White);

		for(int i=0;i<GRIDW;i++)
		for(int j=0;j<GRIDH;j++){
			window.draw(tile[i][j]);
		}
		
		for(std::list<Agent>::iterator it=agent.begin(); it != agent.end(); ++it){
			window.draw(*it);
		}
        window.display();
    }

    return 0;
}