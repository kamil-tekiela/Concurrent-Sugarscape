#include "main.h"
/*
\cond HIDDEN_SYMBOLS
*/
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
			line[0].position = sf::Vector2f(0, static_cast<float> (i*h/lines));
			line[1].position = sf::Vector2f(w, static_cast<float> (i*h/lines));
			draw(line, 2, sf::Lines);

			text.setString(std::to_string((long long)( maxN-i*maxN/lines )));
			text.setPosition(static_cast<float> (w-40), static_cast<float> (i*h/lines));
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

/*
\endcond
*/

int main()
{
    sf::RenderWindow window(sf::VideoMode(GRIDW*TILEW*2, GRIDH*TILEH+HISTOGRAMH), "Sugarscape", sf::Style::Titlebar | sf::Style::Close, sf::ContextSettings::ContextSettings 	(0,0,0,0,0));
	sf::Font font;
	font.loadFromFile("C:\\Windows\\Fonts\\GARA.TTF");

	Graph graph(GRIDW*TILEW, GRIDH*TILEH, 2000, font);
	
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

	std::vector<Disease> masterDisease;
	masterDisease.resize(MASTERDISEASE);
	std::vector<unsigned int> indices(masterDisease.size());
	std::iota(indices.begin(), indices.end(), 0);

	// we create sugarscape
	static Tile tile[GRIDW][GRIDH];
	for(int i=0;i<GRIDW;i++){
		for(int j=0;j<GRIDH;j++){
			tile[i][j] = Tile(i,j);
		}
	}

	// we create random agents
	Agent* agent[GRIDW*GRIDH];
	for(int i=0; i<GRIDW*GRIDH; i++){
		agent[i] = NULL;
	}
	for(int i=0; i<AGENTS; i++){
		int x,y;
		do{
			x = rand()%GRIDW;
			y = rand()%GRIDW;
		}while(tile[x][y].isTaken());
		Agent* a = new Agent(x,y);
		if(DISEASE){
			std::random_shuffle(indices.begin(), indices.end());
			for(int di=0;di<DISEASESGIVEN; ++di)		a->receiveDisease(masterDisease.at(indices.at(di)));
		}
		agent[y*GRIDW + x] = a;
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
	int killed = 0;

	bool movedToggler = false;

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
		int people = 0;
		movedToggler = !movedToggler;
		
		int threads = 4;
		int rows = 2;
		int tperrow = threads/rows;
		int wThread = floor(GRIDW/(tperrow));//25
		int hThread = floor(GRIDH/(rows));
		#pragma omp parallel num_threads(threads)
		{
			int t = omp_get_thread_num();
			int xStart =	(t%tperrow)*wThread;
			int xEnd =		(t%tperrow) ? GRIDW : wThread; //25 / 51
			int yStart =	(t>=tperrow)*hThread;	//0 / 25
			int yEnd =		(t>=tperrow) ? GRIDH : hThread;
			int slices = 3;

			for(int i=0; i<slices; ++i)
			for(int j=0; j<slices; ++j){
				int sy = yStart + floor((yEnd-yStart)/3)*j;
				int ey = yStart + floor((yEnd-yStart)/3.0*(j+1));
				for(int y=sy; y<ey; ++y){
					int sx = xStart + floor((xEnd-xStart)/3)*i;
					int ex = xStart + floor((xEnd-xStart)/3.0*(i+1));
					for(int x=sx; x<ex; ++x){
						int pos = y*GRIDW + x;
						if(!agent[pos]) continue;
						Agent * a = agent[pos];
						if(a->moved==movedToggler || a->isDead()) continue;
						#pragma omp atomic
						people++;
						temp =				(a)->update(tile, agent);
						a->moved =			movedToggler;
						agent[pos] = NULL;
						agent[a->getCoord().y*GRIDW + a->getCoord().x] = a;

						sugar +=			(a)->getWealth();
						vision +=			(a)->getVision();
						metabol +=			(a)->getMetabolRate();
						for(int i=0;i<TAGCOUNT;i++){ 
							histogramData[i] += (a)->tagString.tags[i];//collect tagStrings for histogram data; no abstraction
						}
					}
				}
				#pragma omp barrier
			}
		}
		//for(int i=0; i<GRIDW*GRIDH; ++i){
		//for(int i=GRIDW*GRIDH-1; i>=0; --i){
		//	if(!agent[i]) continue;
		//	Agent * a = agent[i];
		//	if(a->moved==movedToggler || a->isDead()) continue;
		//	people++;
		//	temp =				(a)->update(tile, agent);
		//	a->moved =			movedToggler;
		//	agent[i] = NULL;
		//	agent[a->getCoord().y*GRIDW + a->getCoord().x] = a;

		//	sugar +=			(a)->getWealth();
		//	vision +=			(a)->getVision();
		//	metabol +=			(a)->getMetabolRate();
		//	for(int i=0;i<TAGCOUNT;i++){ 
		//		histogramData[i] += (a)->tagString.tags[i];//collect tagStrings for histogram data; no abstraction
		//	}
		//}
		//death
		for(int i=0; i<GRIDW*GRIDH; ++i){
			if(!agent[i]) continue;
			Agent * obj = agent[i];
			if((obj)->isDead()) {
				sf::Vector2i vecT = (obj)->getCoord();
				//inheritance rule
				if(INHERITANCE)	(obj)->leaveLegacy(agent);
				//delete this object
 				tile[vecT.x][vecT.y].freeUp();
				delete (obj);
				agent[i] = NULL;
				if(REPLACEMENT){
					int x,y;
					do{
						x = rand()%GRIDW;
						y = rand()%GRIDH;
					}while(tile[x][y].isTaken());
					Agent* a = new Agent(x,y);
					a->addSugar(tile[x][y].eat());
					agent[y*GRIDW + x] = a;
				}
			}
		}
		if(MATING){
			for(int i=0; i<GRIDW*GRIDH; ++i){
				if(!agent[i]) continue;
				Agent * obj = agent[i];

				int x = obj->getCoord().x, y=obj->getCoord().y;
				std::vector<sf::Vector2i> positions;
				positions.push_back(sf::Vector2i(x+1>=GRIDW ? x+1-GRIDW : x+1, y));
				positions.push_back(sf::Vector2i(x, y+1>=GRIDH ? y+1-GRIDH : y+1));
				positions.push_back(sf::Vector2i(x-1<0 ? x-1+GRIDW : x-1, y));
				positions.push_back(sf::Vector2i(x, y-1<0 ? y-1+GRIDH : y-1));
				for(unsigned i=0; i<positions.size(); ++i){
					//find agent living on this tile
					int pos = positions.at(i).y * GRIDW + positions.at(i).x;
					if(agent[pos]){
						obj->sex(tile, agent, agent[pos]);
					}
				}
			}
		}
		if(CULTURE){
			for(int i=0; i<GRIDW*GRIDH; ++i){
				if(!agent[i]) continue;
				Agent * obj = agent[i];

				int x = obj->getCoord().x, y=obj->getCoord().y;
				std::vector<sf::Vector2i> positions;
				positions.push_back(sf::Vector2i(x+1>=GRIDW ? x+1-GRIDW : x+1, y));
				positions.push_back(sf::Vector2i(x, y+1>=GRIDH ? y+1-GRIDH : y+1));
				positions.push_back(sf::Vector2i(x-1<0 ? x-1+GRIDW : x-1, y));
				positions.push_back(sf::Vector2i(x, y-1<0 ? y-1+GRIDH : y-1));
				for(unsigned i=0; i<positions.size(); ++i){
					//find agent living on this tile
					int pos = positions.at(i).y * GRIDW + positions.at(i).x;
					if(agent[pos]){
						obj->tagString.affected(agent[pos]->tagString);
					}
				}
			}
		}
		if(DISEASE){
			for(int i=0; i<GRIDW*GRIDH; ++i){
				if(!agent[i]) continue;
				Agent * obj = agent[i];

				int x = obj->getCoord().x, y=obj->getCoord().y;
				std::vector<sf::Vector2i> positions;
				positions.push_back(sf::Vector2i(x+1>=GRIDW ? x+1-GRIDW : x+1, y));
				positions.push_back(sf::Vector2i(x, y+1>=GRIDH ? y+1-GRIDH : y+1));
				positions.push_back(sf::Vector2i(x-1<0 ? x-1+GRIDW : x-1, y));
				positions.push_back(sf::Vector2i(x, y-1<0 ? y-1+GRIDH : y-1));
				for(unsigned i=0; i<positions.size(); ++i){
					//find agent living on this tile
					int pos = positions.at(i).y * GRIDW + positions.at(i).x;
					if(agent[pos]){
						obj->giveDisease(agent[pos]);
						obj->immuneResponse();
					}
				}
			}
		}
		if(TRADING){
			for(int i=0; i<GRIDW*GRIDH; ++i){
				if(!agent[i]) continue;
				Agent * obj = agent[i];

				int x = obj->getCoord().x, y=obj->getCoord().y;
				std::vector<sf::Vector2i> positions;
				positions.push_back(sf::Vector2i(x+1>=GRIDW ? x+1-GRIDW : x+1, y));
				positions.push_back(sf::Vector2i(x, y+1>=GRIDH ? y+1-GRIDH : y+1));
				positions.push_back(sf::Vector2i(x-1<0 ? x-1+GRIDW : x-1, y));
				positions.push_back(sf::Vector2i(x, y-1<0 ? y-1+GRIDH : y-1));
				for(unsigned i=0; i<positions.size(); ++i){
					//find agent living on this tile
					int pos = positions.at(i).y * GRIDW + positions.at(i).x;
					if(agent[pos]){
						obj->trade(agent[pos]);
					}
				}
			}
		}
		if(CREDITRULE){
			for(int i=0; i<GRIDW*GRIDH; ++i){
				if(!agent[i]) continue;
				Agent * obj = agent[i];
				if(!obj->wantsToBorrow()) continue;

				int x = obj->getCoord().x, y=obj->getCoord().y;
				std::vector<sf::Vector2i> positions;
				positions.push_back(sf::Vector2i(x+1>=GRIDW ? x+1-GRIDW : x+1, y));
				positions.push_back(sf::Vector2i(x, y+1>=GRIDH ? y+1-GRIDH : y+1));
				positions.push_back(sf::Vector2i(x-1<0 ? x-1+GRIDW : x-1, y));
				positions.push_back(sf::Vector2i(x, y-1<0 ? y-1+GRIDH : y-1));
				for(unsigned i=0; i<positions.size(); ++i){
					//find agent living on this tile
					int pos = positions.at(i).y * GRIDW + positions.at(i).x;
					if(agent[pos]){
						if(agent[pos]->canLend() > 0){
							int amount = agent[pos]->canLend();
							Loan loan(agent[pos], obj, amount, years);
							agent[pos]->givesLoan(loan);
							obj->takesLoan(loan);
						}
					}
				}
				//repay
				obj->payDebts(years);
			}
		}

		if(years % 10==0){
			graph.plotData(people);
		}
		histogram.plotData(histogramData, people);

		if(people){
			aveSugar = (int)sugar/people;
			aveVision = vision/people;
			std::cout << (int)years/10 << "\t" << people << "\tTS: " << sugar << "\tS: " << aveSugar << "\tM: " << (double)(metabol/people) << "\tV: " << aveVision  << '\n';
		}
		

		//tile update
		for(int i=0;i<GRIDW;i++){
			for(int j=0;j<GRIDH;j++){
				if(GROWBACK == NormalG)			tile[i][j].grow(tile, years);
				else if(GROWBACK == Seasonal)	tile[i][j].seasonalGrow(tile, years);
			}
		} 


		//DRAW
        window.clear(sf::Color::White);

		window.setView(histogramView);
		window.draw(lineSeparator);
		window.draw(histogram);

		window.setView(normalView);

		for(int i=0;i<GRIDW;i++){
			for(int j=0;j<GRIDH;j++){
				if(!tile[i][j].isTaken())
					window.draw(tile[i][j]);
			}
		}
		
		for(int i=0; i<GRIDW*GRIDH; ++i){
			if(agent[i])
				window.draw(*agent[i]);
		}

		window.draw(sideSprite);
		window.draw(text);
        window.display();
    }

    return 0;
}