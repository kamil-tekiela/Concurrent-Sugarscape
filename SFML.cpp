#include "stdafx.h"

//duplicated constants; see tile.h
#define GRIDW 50
#define GRIDH 50
#define TILEW 10
#define TILEH 10
#define AGENTS 400


int main()
{
    sf::RenderWindow window(sf::VideoMode(GRIDW*TILEW*2, GRIDH*TILEH), "SFML works!", sf::Style::Titlebar | sf::Style::Close, sf::ContextSettings::ContextSettings 	(0,0,4,0,0));
	//window.setFramerateLimit(30); // call it once, after creating the window

	sf::RenderTexture sideTexture;
	sideTexture.create(500, 500);
	sideTexture.clear(sf::Color::Black);
	sideTexture.display();
	int prevHGraph = 2420;
	
	sf::Sprite sideSprite(sideTexture.getTexture());
	sideSprite.setPosition(GRIDW*TILEW,0);


	sf::Font font;
	font.loadFromFile("C:\\Windows\\Fonts\\GARA.TTF");
	
	sf::Clock clock;
	
	double aveSugar = 0;
	double aveVision = 0;
	int years=0;
	int del=0;

	// we create sugarscape
	static Tile tile[GRIDW][GRIDH];
	for(int i=0;i<GRIDW;i++){
		for(int j=0;j<GRIDH;j++){
			tile[i][j] = Tile(i,j);
		}
	}

	// we create random agents
	std::vector<Agent*> agent;
	agent.reserve(2500);
	//for(std::vector<Agent*>::iterator it=agent.begin(); it != agent.end(); ++it){
	for(int i=0; i<AGENTS; i++){
		int x,y;
		do{
			x = rand()%GRIDW;
			y = rand()%GRIDW;
		}while(tile[x][y].isTaken());
		Agent* a = new Agent(x,y);
		agent.push_back(a);
		tile[x][y].eat();
	}
		
	sf::Time time = clock.getElapsedTime();
	std::string str = "0";
	sf::Text text;
	text.setFont(font);
	text.setString(str);
	text.setPosition(10,10);
	text.setCharacterSize(20);
	text.setColor(sf::Color::Black);
	int lastTime = time.asMilliseconds();
	int lastFrameCount = 0;


    while (window.isOpen())
    {
		time = clock.getElapsedTime();
		if(time.asMilliseconds() >= lastTime+1000){
			int fpsInt = time.asMilliseconds()-lastTime;
			lastTime = time.asMilliseconds();
			fpsInt = 1000 / (fpsInt / (years-lastFrameCount));
			lastFrameCount = years;
			str = std::to_string((long double)( fpsInt ));
			text.setString(str);
		}

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

		double sugar = 0;
		double vision = 0;
		double metabol = 0;
		bool temp;
		int people = 0;
		std::vector<Agent*>::iterator it=agent.begin();
		while(it!=agent.end()){
			people++;
			temp =				(*(*it)).update(tile, agent, aveVision);
			sugar +=			(*(*it)).getWealth();
			vision +=			(*(*it)).getVision();
			metabol +=			(*(*it)).getMetabolRate();
			sf::Vector2i vecT = (*(*it)).getCoord();
			if(!temp) {
				//inheritance rule
				(*(*it)).leaveLegacy(agent);
				//delete this object
				tile[vecT.x][vecT.y].freeUp();
				delete (*it);
				it = agent.erase(it);
			}
			else
				++it;
		}

		if(years % 10==0){
			int maxH = GRIDH*TILEH;
			int maxW = GRIDW*TILEW;
			if(years > maxW){
			}
			int maxN = 300000;
			float calcY = (float)sugar / maxN;
			calcY *= maxH;
			int step = years/10;
			sf::Vertex line[] =
			{
				sf::Vertex(sf::Vector2f(step, maxH-calcY)),
				sf::Vertex(sf::Vector2f(step-1, prevHGraph))
			};
			prevHGraph = maxH-calcY;
			sideTexture.draw(line, 2, sf::Lines);
			sideTexture.display();
		}

		if(agent.size()){
			aveSugar = (int)sugar/people;
			aveVision = vision/people;
			std::cout << (int)years/10 << "\t" << agent.size() << "\tTS: " << sugar << "\tS: " << aveSugar << "\tM: " << (double)(metabol/people) << "\tV: " << aveVision  << '\n';
		}
		

		//tile update
		for(int i=0;i<GRIDW;i++){
			for(int j=0;j<GRIDH;j++){
				tile[i][j].grow();
			}
		}


		//DRAW
        window.clear(sf::Color::White);

		for(int i=0;i<GRIDW;i++)
		for(int j=0;j<GRIDH;j++){
			window.draw(tile[i][j]);
		}
		
		for(std::vector<Agent*>::iterator it=agent.begin(); it != agent.end(); ++it){
			window.draw(*(*it));
		}

		window.draw(sideSprite);
		window.draw(text);
        window.display();
    }

    return 0;
}