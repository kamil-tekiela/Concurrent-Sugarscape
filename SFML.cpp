#include "stdafx.h"

//duplicated constants; see tile.h
#define GRIDW 50
#define GRIDH 50
#define TILEW 10
#define TILEH 10
#define AGENTS 400
#define HISTOGRAMH 200
#define TAGCOUNT 11

class Graph : public sf::RenderTexture {
private: 
	int h;
	int w;
	int i;
	int prevHGraph;
	int maxN;
	void drawGrid(sf::Font &font, int lines=5){
		sf::Text text;
		text.setFont(font);
		text.setCharacterSize(16);
		text.setColor(sf::Color::White);
		sf::Vertex line[] = {
			sf::Vertex(sf::Vector2f(0, 0), sf::Color::Color(150,150,150)),
			sf::Vertex(sf::Vector2f(0, 0), sf::Color::Color(150,150,150))
		};

		int i=lines;
		while(--i){
			line[0].position = sf::Vector2f(0, i*h/lines);
			line[1].position = sf::Vector2f(w, i*h/lines);
			draw(line, 2, sf::Lines);

			text.setString(std::to_string((long long)( maxN-i*maxN/lines )));
			text.setPosition(w-40, i*h/lines);
			draw(text);
		}
	}

public:
	Graph();
	Graph(int width, int height, int maxValue, sf::Font &font){
		h=  height;
		w = width;
		create(w,h);
		clear(sf::Color::Black);
		i=0;
		prevHGraph = h-80;
		maxN = maxValue;
		drawGrid(font);
		display();
	}
	void plotData(int data){
		float calcY = (float)data / maxN;
		calcY *= h;
		int step = i;
		sf::Vertex line[] =
		{
			sf::Vertex(sf::Vector2f(i, h-calcY)),
			sf::Vertex(sf::Vector2f(step-1, prevHGraph))
		};
		prevHGraph = h-calcY;
		draw(line, 2, sf::Lines);
		display();
		
		i++;
	}

};

class Histogram : public sf::Drawable, sf::Transformable {
private: 
	int w,h;
	int x,y;
	sf::RectangleShape clearRect;
    sf::VertexArray rectangles;

public:
	Histogram();
	Histogram(float x, float y, int width, int height){
		h = height;
		w = width;
		this->x = x;
		this->y = y;
		clearRect.setSize(sf::Vector2f(width, height));
		clearRect.setFillColor(sf::Color::Black);
		clearRect.setPosition(x,y);
		
        rectangles.setPrimitiveType(sf::Quads);
        rectangles.resize(TAGCOUNT * 4);
	}
	void plotData(int data[TAGCOUNT], int agentsTotal){
		float calcY; 
		float calcX = w/TAGCOUNT;
		for(int i=0; i<TAGCOUNT; i++)
		{
			calcY = (float)data[i] / agentsTotal;
			calcY *= h;

			// get a pointer to the current tile's quad
            sf::Vertex* quad = &rectangles[i * 4];

            // define its 4 corners
            quad[0].position = sf::Vector2f(x + calcX*i, y + (h-calcY));//top left
            quad[1].position = sf::Vector2f(x + calcX*i + calcX, y + (h-calcY));
			quad[2].position = sf::Vector2f(x + calcX*i + calcX, y + h);
            quad[3].position = sf::Vector2f(x + calcX*i, y + h);

            quad[0].color = sf::Color::Red;
            quad[1].color = sf::Color::Red;
            quad[2].color = sf::Color::Red;
            quad[3].color = sf::Color::Red;

		}
	}
	void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		states.transform *= getTransform();
		states.texture = NULL;
		
		target.draw(clearRect, states);
		target.draw(rectangles, states);
	}

};


int main()
{
    sf::RenderWindow window(sf::VideoMode(GRIDW*TILEW*2, GRIDH*TILEH+HISTOGRAMH), "SFML works!", sf::Style::Titlebar | sf::Style::Close, sf::ContextSettings::ContextSettings 	(0,0,4,0,0));
	//window.setFramerateLimit(30); // call it once, after creating the window
	sf::Font font;
	font.loadFromFile("C:\\Windows\\Fonts\\GARA.TTF");

	Graph graph(GRIDW*TILEW, GRIDH*TILEH, 2500, font);
	
	sf::Sprite sideSprite(graph.getTexture());
	sideSprite.setPosition(GRIDW*TILEW,0);
	
	int screenHeight = (GRIDH*TILEH+HISTOGRAMH);
	sf::View normalView(sf::FloatRect(0, 0, GRIDW*TILEW*2, GRIDH*TILEH));
	normalView.setViewport(sf::FloatRect(0, 0, 1.0f, (float)(GRIDH*TILEH)/screenHeight));
	sf::View histogramView(sf::FloatRect(0, GRIDH*TILEH, GRIDW*TILEW*2, HISTOGRAMH));
	histogramView.setViewport(sf::FloatRect(0, (float)(GRIDH*TILEH)/screenHeight, 1.0f, (float)(HISTOGRAMH)/screenHeight));
	sf::RectangleShape lineSeparator(sf::Vector2f(GRIDW*TILEW*2, 2));
	lineSeparator.setFillColor(sf::Color::Cyan);
	lineSeparator.setPosition(0, GRIDH*TILEH);
	
	int histogramData[TAGCOUNT];
	Histogram histogram(GRIDW*TILEW, GRIDH*TILEH+2, GRIDW*TILEW, HISTOGRAMH-2);

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
	std::vector<Agent*> newAgent;
	newAgent.reserve(800);
	//
	static Agent* agents[GRIDW][GRIDH];
	for(int i=0;i<GRIDW;i++) for(int j=0;j<GRIDH;j++)
			agents[i][j] = NULL;
	for(int i=0; i<AGENTS; i++){
		int x,y;
		do{
			x = rand()%GRIDW;
			y = rand()%GRIDW;
		}while(tile[x][y].isTaken());
		Agent* a = new Agent(x,y);
		agents[x][y] = a;
		tile[x][y].eat();//discard food; why?
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
	int fpsInt=0;

	/////////////////////////////////////////////////////////////////////////////
	//***************************************************************************
	/////////////////////////////////////////////////////////////////////////////
    while (window.isOpen())
    {
		time = clock.getElapsedTime();
		if(time.asMilliseconds() >= lastTime+1000){
			fpsInt = time.asMilliseconds()-lastTime;
			lastTime = time.asMilliseconds();
			fpsInt = 1000 / (fpsInt / (years-lastFrameCount));
			lastFrameCount = years;
			str = std::to_string((long long)( fpsInt ));
			text.setString(str+" FPS");
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
			else if(event.type ==sf::Event::MouseButtonReleased)
			{
				int x = event.mouseButton.x;
				int y = event.mouseButton.y;
				x = x/TILEW;
				y = y/TILEH;
				if(agents[x][y])
					std::cout << agents[x][y]->getCoord().x << " " << agents[x][y]->getCoord().y << " ";
				std::cout << (tile[x][y].isTaken()?"Yes":"No") <<std::endl;
			}
        }
		
		//update
		years++;

		for(int i=0;i<TAGCOUNT;i++)
		{ 
			histogramData[i]=0;
		}

		double sugar = 0;
		double vision = 0;
		double metabol = 0;
		bool temp;
		//bool isAlive;
		int people = 0;
		for(int x=0; x<GRIDW; x++){
			for(int y=0; y<GRIDH; y++){
				if(agents[x][y]){
					Agent* a = agents[x][y];
					(*a).update(tile, agents, years, newAgent);
					sugar +=	(*a).getWealth();
					vision +=	(*a).getVision();
					metabol +=	(*a).getMetabolRate();
					sf::Vector2i vecT = (*a).getCoord();
					for(int i=0;i<TAGCOUNT;i++)
					{ 
						histogramData[i] += (*a).tagString.tags[i];//collect tagStrings for histogram data; no abstraction
					}
				}
			}
		}

		Agent* agentsTemp[GRIDW][GRIDH] = {NULL};
		for(int x=0; x<GRIDW; x++){
			for(int y=0; y<GRIDH; y++){
				if(agents[x][y]){
					Agent * a = agents[x][y];
					sf::Vector2i vecT = (*a).getCoord();
					if((*a).toDelete){
						(*a).leaveLegacy(agents);

						delete a;
					}
					else{//move
						agentsTemp[vecT.x][vecT.y] = a;
					}
				}
			}
		}		
		std::vector<Agent*>::iterator it = newAgent.end() ;
		while(it!=newAgent.begin())
		{
			Agent * a = newAgent.back();
			sf::Vector2i vecT = (*a).getCoord();
			assert(agentsTemp[vecT.x][vecT.y]==NULL);
			agentsTemp[vecT.x][vecT.y] = a;
			newAgent.pop_back();
			it = newAgent.end();
		}
		//clear
		/*for(int x=0; x<GRIDW; x++){
			for(int y=0; y<GRIDH; y++){
				agents[x][y] = NULL;
			}
		}*/
		//swap back array values
		for(int x=0; x<GRIDW; x++){
			for(int y=0; y<GRIDH; y++){
				agents[x][y] = agentsTemp[x][y];
				if(agents[x][y])
					people++;
			}
		}


		if(years % 10==0){
			graph.plotData(people);
		}
		histogram.plotData(histogramData, people);

		if(people>0){
			aveSugar = (int)sugar/people;
			aveVision = vision/people;
			//std::cout << "   " << tile[25][24].pollution << "   " << std::endl;
			//std::cout << tile[24][25].pollution << " " << tile[25][25].pollution << " " << tile[26][25].pollution << std::endl;
			//std::cout << "   " << tile[25][26].pollution << "   " << std::endl;
			//std::cout << (int)years/10 << "\t" << people << "\tTS: " << sugar << "\tS: " << aveSugar << "\tM: " << (double)(metabol/people) << "\tV: " << aveVision  << '\n';
		}
		

		//tile update
		for(int i=0;i<GRIDW;i++){
			for(int j=0;j<GRIDH;j++){
				tile[i][j].grow();
				//tile[i][j].seasonalGrow(years, tile);
			}
		}


		//DRAW
        window.clear(sf::Color::White);

		window.setView(histogramView);
		window.draw(lineSeparator);
		window.draw(histogram);

		window.setView(normalView);

		for(int i=0;i<GRIDW;i++)
		for(int j=0;j<GRIDH;j++){
			window.draw(tile[i][j]);
		}
		
		for(int x=0;x<GRIDW; x++)
			for(int y=0; y<GRIDH; y++)
				if(agents[x][y])
					window.draw(*agents[x][y]);
		/*for(std::vector<Agent*>::iterator it=agent.begin(); it != agent.end(); ++it){
			window.draw(*(*it));
		}*/

		window.draw(sideSprite);
		window.draw(text);
        window.display();
    }

    return 0;
}