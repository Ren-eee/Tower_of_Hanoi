#include <iostream>

#include <SDL.h>

#include <SDL_image.h>

#include <vector>

#include <SDL_ttf.h>

#include <string>

#include <chrono>

#include <fstream>




using namespace std;

bool init();

void quit();

bool loop();

void renderText(const std::string& message, SDL_Color color, int x, int y, int x_size, int y_size);

struct record {
	int level;
	int score;
	int time;

};

void make_rating(struct record* rating,int level, int moves, int time);
int make_rating(struct record* rating, int level);

int print_rating(struct record* rating, int size);




SDL_Window* window;

SDL_Renderer* renderer;

SDL_Texture* texture;

SDL_DisplayMode displayMode;

SDL_Surface* winSurface;


class Timer
{
private:
	using clock_t = std::chrono::high_resolution_clock;
	using second_t = std::chrono::duration<double, std::ratio<1> >;

	std::chrono::time_point<clock_t> m_beg;

public:
	Timer() : m_beg(clock_t::now())
	{
	}

	void reset()
	{
		m_beg = clock_t::now();
	}

	int elapsed() const
	{
		return std::chrono::duration_cast<second_t>(clock_t::now() - m_beg).count();
	}
};

class obj {

public:

	SDL_Rect* rect;

	SDL_Texture* texture;

	obj(SDL_Texture* texture, int size_x, int size_y , int  x, int  y) {

		rect = new SDL_Rect;

		this->texture = texture;

		

		*rect = { 0, 0, 0, 0 };

		moveTo(x, y);
		resizeOn(size_x, size_y);

	}

	void move(double x, double y)

	{

		rect->x += x;

		rect->y += y;

	}

	void moveTo(int x, int y)

	{

		rect->x = x;

		rect->y = y;

	}

	void resizeOn(int w, int h)

	{

		rect->w = w;

		rect->h = h;

	}


	virtual void render()

	{

		SDL_RenderCopy(renderer, texture, NULL, rect);

	}

};

class Disk: public obj {
public:
	int width;
	int height;
	int position;
	int state;
	int pos_y;

	Disk(SDL_Texture* texture, int size_x, int size_y, int x, int y) : obj(texture,size_x, size_y, x = x - size_x / 2, y)  {
		width = size_x;
		height = size_y;
		pos_y = y;
		position = 1;
		state = 0;
	}


};

int main(int argc, char** args) {
	if (!init()) {

		system("pause");

		return 1;

	}

	while (!loop()) {

		SDL_Delay(10);

	}

	quit();

	return 0;

}

bool loop() {

	Timer t;
	int flag = 1;
	int position_x1 = 415;
	int position_x2 = 670;
	int position_x3 = 900;
	int position_y = 470;//467
	int cursor = 1;
	int moves = 0;
	Disk* selected = nullptr;
	vector<Disk*> objects;
	struct record ratings[5][11] ;
	
	int level=1;
	SDL_Color color = { 0, 0, 0, 0 };
		
	SDL_Texture* background_img = IMG_LoadTexture(renderer, "image//background.png");

	SDL_Texture* base_img = IMG_LoadTexture(renderer, "image//base.png");

	SDL_Texture* disk_img = IMG_LoadTexture(renderer, "image//disk.png");

	SDL_Texture* point_img = IMG_LoadTexture(renderer, "image//point.png");

	SDL_Texture* victory_img = IMG_LoadTexture(renderer, "image//victory.png");

	for (int i = 0,  k = level + 2; i < level + 2; i++, k--) {
		objects.push_back(new Disk(disk_img, k*25 + 209*0.1, 60 *0.4, position_x1, position_y - (i * 60 * 0.4)));
	}

	obj background(background_img, 1280, 720,0,0);

	obj point(point_img, 50, 50, 0, 0);

	obj base(base_img, 700, 700, 1280 / 2 - 700 / 2, 720 / 2 - 700 / 2);

	obj victory(victory_img, 361, 93, 1280 / 2 - 361 / 2, 0);

	bool run = true;

	const Uint8* keyboardState = SDL_GetKeyboardState(NULL);

	SDL_Event e;

	SDL_Rect dest;

	int count = 0;


	while (run)

	{

		while (run)

		{

			SDL_Delay(30);

			SDL_PumpEvents();

			
			

			if (selected == nullptr) {
				switch (cursor) {
				case 1: point.moveTo(position_x1 - 50 / 2, 25); break;
				case 2: point.moveTo(position_x2 - 50 / 2, 25); break;
				case 3: point.moveTo(position_x3 - 50 / 2, 25); break;
				}
			}
			else {
				switch (cursor) {
				case 1: selected->moveTo(position_x1 - selected->width / 2, 25); break;
				case 2: selected->moveTo(position_x2 - selected->width / 2, 25); break;
				case 3: selected->moveTo(position_x3 - selected->width / 2, 25); break;
				}

			}
			int a = 15;


			if (SDL_PollEvent(&e))

			{

				if (e.type == SDL_QUIT) {
					run = false;
				}

				if (e.type == SDL_KEYDOWN) {

					if (e.key.keysym.sym == SDLK_TAB) {
						print_rating(ratings[level-1] , make_rating(ratings[level-1], level));
						
					}

					
					if (e.key.keysym.sym == SDLK_ESCAPE) {
						run = false;
						break;
					}

					if ((e.key.keysym.sym == SDLK_UP) & (level<6)) {
						level++;
						objects.clear();
						for (int i = 0, k = level + 2; i < level + 2; i++, k--) {
							objects.push_back(new Disk(disk_img, k * 25 + 209 * 0.1, 60 * 0.4, position_x1, position_y - (i * 60 * 0.4)));
						}

						cursor = 1;

						selected = nullptr;
						moves = 0;
						t.reset();
						flag = 1;
						
					}

					if ((e.key.keysym.sym == SDLK_DOWN) & (level > 1)) {
						level--;
						objects.clear();
						for (int i = 0, k = level + 2; i < level + 2; i++, k--) {
							objects.push_back(new Disk(disk_img , k * 25 + 209 * 0.1, 60 * 0.4, position_x1, position_y - (i * 60 * 0.4)));
						}

						cursor = 1;
						selected = nullptr;
						moves = 0;
						t.reset();
						flag = 1;
					}



					if ((e.key.keysym.sym == SDLK_RIGHT)&&(cursor<3)) {
						cursor++;
					}
					
					if ((e.key.keysym.sym == SDLK_LEFT) && (cursor > 1)) {
						cursor--;
					}

					if (e.key.keysym.sym == SDLK_r) {
						objects.clear();
						for (int i = 0, k = level + 2; i < level + 2; i++, k--) {
							objects.push_back(new Disk(disk_img, k * 25 + 209 * 0.1, 60 * 0.4, position_x1, position_y - (i * 60 * 0.4)));
						}

						cursor = 1;
						selected = nullptr;
						moves = 0;
						t.reset();
						flag = 1;
					}

					if ((e.key.keysym.sym == SDLK_SPACE)) {
						if (selected == nullptr) {
							Disk* min = nullptr;
							for (int i = 0; i < objects.size(); i++) {
								if ((objects[i]->position == cursor)) {
									min = objects[i];
									break;
								}
							}

							if (min == nullptr) break;

							for (int i = 0; i < objects.size(); i++) {
								if ((objects[i]->position == cursor) && (objects[i]->width < min->width))
									min = objects[i];
							}

							switch (cursor) {
							case 1: min->moveTo(position_x1 - 25 / 2, 25); break;
							case 2: min->moveTo(position_x2 - 25 / 2, 25); break;
							case 3: min->moveTo(position_x3 - 25 / 2, 25); break;
							}

							point.moveTo(5000,0);

							min->state = 1;

							selected = min;

							min = nullptr;

						}

						else {
							Disk* min = nullptr;
							for (int i = 0; i < objects.size(); i++) {
								if ((objects[i]->position == cursor)&&(objects[i]->state==0)) {
									min = objects[i];
									break;
								}
							}

							if (min == nullptr) {
								switch (cursor) {
								case 1: selected->moveTo(position_x1 - selected->width / 2, position_y); selected->pos_y = position_y; break;
								case 2: selected->moveTo(position_x2 - selected->width / 2, position_y); selected->pos_y = position_y; break;
								case 3: selected->moveTo(position_x3 - selected->width / 2, position_y); selected->pos_y = position_y; break;
								}

								if (selected->position != cursor) {
									moves++;
								}



								selected->position = cursor;
								selected->state = 0;
								selected = nullptr;
								break;
							}

							for (int i = 0; i < objects.size(); i++) {
								if ((objects[i]->position == cursor) && (objects[i]->width < min->width) & (objects[i]->state == 0))
									min = objects[i];
							}

							if (min->width > selected->width) {

								switch (cursor) {
								case 1: selected->moveTo(position_x1 - selected->width / 2, min->pos_y - 60 * 0.4); selected->pos_y = min->pos_y - 60 * 0.4; break;
								case 2: selected->moveTo(position_x2 - selected->width / 2, min->pos_y - 60 * 0.4); selected->pos_y = min->pos_y - 60 * 0.4; break;
								case 3: selected->moveTo(position_x3 - selected->width / 2, min->pos_y - 60 * 0.4); selected->pos_y = min->pos_y - 60 * 0.4; break;
								}

								if (selected->position != cursor) {
									moves++;
								}

								selected->position = cursor;
								selected->state = 0;
								
								selected = nullptr;
								min = nullptr;
							}

							
						}
						


					}

				}

			}

			
			background.render();

			string p = "Moves: ";

			p += to_string(moves);

			renderText(p, color,  5, 0, 100, 50);

	
			



			renderText("Esc - exit", color, 1050, 0, 100, 50);
			renderText("R - restart", color, 1050, 30, 100, 50);
			renderText("Right/left - move cursor", color,  1050, 60, 220, 50);
			renderText("Space - lift/lover disk", color, 1050, 90, 220, 50);
			renderText("Up/down - change difficulty", color, 1050, 120, 220, 50);
			renderText("Tab - rating", color, 1050, 150, 100, 50);
			
			if ((moves == 1)&&flag) {
				t.reset();
				flag = 0;
			}

			
				
			string tm = "Time: ";
			tm += to_string(t.elapsed());
			if (moves >= 1) {
				renderText(tm, color,   5, 30, 100, 50);
			}

			

			point.render();

			base.render();

			


			for (int i = 0; i < level + 2; i++) {
				objects[i]->render();

			}

			for (int i = 0; i < objects.size(); i++) {

				if (objects[i]->position == 3) count++;

			}

			if (count == objects.size()) {
				
				victory.render();

				make_rating(ratings[level-1],level , moves,t.elapsed());
				
				SDL_RenderPresent(renderer);

				

				objects.clear();
				for (int i = 0, k = level + 2; i < level + 2; i++, k--) {
					objects.push_back(new Disk(disk_img, k * 25 + 209 * 0.1, 60 * 0.4, position_x1, position_y - (i * 60 * 0.4)));
				}

				cursor = 1;
				selected = nullptr;
				moves = 0;
				t.reset();
				flag = 1;
				SDL_Delay(2000);
			}

			count = 0;

			SDL_RenderPresent(renderer);


		}

		

		

	}



	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

	SDL_RenderClear(renderer);

	SDL_UpdateWindowSurface(window);

	SDL_RenderCopy(renderer, texture, NULL, NULL);

	SDL_RenderPresent(renderer);

	return true;

}

bool init() {

	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {

		cout << "Error initializing SDL: " << SDL_GetError() << endl;

		return false;

	}

	if (SDL_Init(SDL_INIT_VIDEO))

	{

		std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;

		return false;

	}

	if (IMG_Init(IMG_INIT_PNG) < 0) {

		cout << "Error initializing SDL_image: " << IMG_GetError() << endl;

		return false;

	}

	if (TTF_Init() != 0) {
		cout << "Error initializing SDL_ttf: " << TTF_GetError() << endl;

		return false;
	}

	window = SDL_CreateWindow("Example", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1280, 720, SDL_WINDOW_SHOWN);

	if (!window) {

		cout << "Error creating window: " << SDL_GetError() << endl;

		return false;

	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	if (!renderer) {

		cout << "Error creating renderer: " << SDL_GetError() << endl;

		return false;

	}

	SDL_Surface* buffer = IMG_Load("image//background.png");

	if (!buffer) {

		cout << "Error loading image test.png: " << SDL_GetError() << endl;

		return false;

	}

	texture = SDL_CreateTextureFromSurface(renderer, buffer);

	SDL_FreeSurface(buffer);

	buffer = NULL;

	if (!texture) {

		cout << "Error creating texture: " << SDL_GetError() << endl;

		return false;

	}



	return true;

}

void quit() {

	SDL_DestroyTexture(texture);

	texture = NULL;

	SDL_DestroyRenderer(renderer);

	window = NULL;

	renderer = NULL;

	IMG_Quit();

	SDL_Quit();
}

void renderText(const std::string& message, SDL_Color color,  int x, int y, int x_size, int y_size) {

	//Открываем шрифт
	TTF_Font* font = TTF_OpenFont("font.otf", 64);
	if (font == nullptr) {
		cout << "Error TTF_OpenFont " << SDL_GetError() << endl;
	}
	 
	 SDL_Surface* text_sur = TTF_RenderText_Solid(font, message.c_str(), color);
	if (text_sur == nullptr) {
		TTF_CloseFont(font);
		cout << "Error TTF_RenderText " << SDL_GetError() << endl;

	}
	SDL_Texture* text_texture = SDL_CreateTextureFromSurface(renderer, text_sur);
	if (texture == nullptr) {
		cout << "Error CreateTexture " << SDL_GetError() << endl;
	}

	obj text(text_texture, x_size, y_size, x, y);

	text.render();
	//Очистка поверхности и шрифта
	SDL_DestroyTexture(text_texture);
	SDL_FreeSurface(text_sur);
	TTF_CloseFont(font);

}

int print_rating(struct record* rating, int size){
	if (size == 0) {
		return 1;
	}
	SDL_Color color = { 0, 0, 0, 0 };
	renderText("moves time", color, 50, 250, 100, 30);
	for (int i = 1, j = 165; i <= 10 && i <= size; i++, j += 30)
		renderText(to_string(i) + ". " + to_string(rating[i-1].score) + "  " + to_string(rating[i-1].time), color, 0,100+j, 150, 50);
	
	SDL_RenderPresent(renderer);
	SDL_Delay(2000);
}


void make_rating(struct record* rating,int level, int moves, int time)
{
	SDL_Color color = { 0, 0, 0, 0 };
	std::fstream file;
	int size = 0;
	string filename = "rating"+ to_string(level)+ ".bin";
	


	file.open(filename, std::ios::binary | std::ios::in);
	if (file.is_open())
	{
		file.seekg(0, std::ios::end);
		size = static_cast<int>(file.tellg() / (sizeof(int)*2));
		file.seekg(0, std::ios::beg);
		for (int i = 0; i < size; i++)
		{
			file.read((char*)&rating[i].score, sizeof(int));
			file.read((char*)&rating[i].time, sizeof(int));
			
		}
		file.close();
	}
	rating[size].score = moves;
	rating[size].time = time;
	
	struct record buff;
	int j;

	for (int i = 1; i <= size; i++)
	{
		buff = rating[i];
		for (j = i - 1; (j >= 0) && ((rating[j].score > buff.score)|| ((rating[j].score == buff.score) && (rating[j].time > buff.time))); j--)
			rating[j + 1] = rating[j];
		rating[j + 1] = buff;
	}


	

	file.open(filename, std::ios::out);
	for (int i = 0; i <= size && i < 10; i++) {
		file.write((char*)&rating[i].score, sizeof(int));
		file.write((char*)&rating[i].time, sizeof(int));
	}
		
}

int make_rating(struct record* rating, int level) {
	std::fstream file;
	int size = 0;
	string filename = "rating" + to_string(level) + ".bin";
	file.open(filename, std::ios::binary | std::ios::in);
	if (file.is_open())
	{
		file.seekg(0, std::ios::end);
		size = static_cast<int>(file.tellg() / (sizeof(int) * 2));
		file.seekg(0, std::ios::beg);
		for (int i = 0; i < size; i++)
		{
			file.read((char*)&rating[i].score, sizeof(int));
			file.read((char*)&rating[i].time, sizeof(int));
		}
		

		return size;

		file.close();

	}
	return 0;
}