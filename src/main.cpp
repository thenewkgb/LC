#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"
#include "SDL2/SDL_mixer.h"
#include "SDL2/SDL_image.h"

#include <iostream>
#include <fstream>

#include "basic.h"

/*
Betting game?
i like gambling games and wanted
poker, blackjack etc
you bet on throwing sticks (lines)
and win if the lines do not overlap
i found the accidental fluke
good enough to include as gameplay
*/

//
// Cxxdroid example code
// Function for loading font and drawing
// text into SDL_Texture
//
SDL_Surface *loadText(
	const char *text,
	SDL_Color back_color = {255, 255, 255})
{
	// moved TTF_Init into main

	TTF_Font *font = TTF_OpenFont("/system/fonts/Roboto-Regular.ttf", 80);
	if (font == NULL)
	{
		fprintf(stderr, "TTF_OpenFont Error: %s\n", TTF_GetError());
		return NULL;
	}
	SDL_Color color = {0, 0, 0};
	//SDL_Color back_color = {255,255,255}

	SDL_Surface *surface =
		TTF_RenderText_Shaded(font, text, color, back_color);

	TTF_CloseFont(font);

	return surface;
}

SDL_Surface *loadGUIText(
	TTF_Font* font,
	const char *text,
	SDL_Color text_color = {255, 255, 255})
{
	SDL_Surface* img1 =
		IMG_Load("images/lc_bank_1000.png");
		
	SDL_Surface *font_surface =
		TTF_RenderText_Solid(
			font, text, text_color);

	// new surface creation
	int border_left = 180;
	int border_right = 220;
	int font_w = font_surface->w;
	int font_h = font_surface->h;
	font_w += border_right;

	SDL_Surface* result =
		SDL_CreateRGBSurface(
			0,
			font_w, font_h,
			32,
			0,0,0,0);
			
	// measurements
	SDL_Rect border{border_left,0,0,0};
	SDL_Rect crop{0,0,font_w,font_h};
	SDL_Rect zero{};
	
	SDL_BlitSurface(
		img1,
		&crop,
		result,
		&zero);
	SDL_BlitSurface(
		font_surface,
		NULL,
		result,
		&border);
		
	SDL_FreeSurface(font_surface);
	font_surface = nullptr;
	SDL_FreeSurface(img1);
	img1 = nullptr;
			
	return result;
}

enum EState
{
	LOST = 0,
	WON,
	NONE,
	INSTRUCTIONS
};

/*
 * CDot
 * here is a surface that 
 * could be used for images
 * but FillRect was used instead
 * CDot needed SDL functions
 * found in main.cpp
*/
class CDot
{
	SDL_Surface *surface = NULL;
	Uint32 color = 0;
	// position rect
	SDL_Rect rect{0, 0, 80, 80};
	// fill rect
	SDL_Rect fill{0, 0, 10, 10};

  public:
	CDot(int xsize, int ysize)
	{
		fill.w = xsize;
		fill.h = ysize;
	}

	void del()
	{
		if (surface != NULL)
			SDL_FreeSurface(surface);
	}

	void makeBlit()
	{
		surface =
			SDL_CreateRGBSurface(
				0,
				fill.w, fill.h,
				32,
				0, 0, 0, 0);
		if (surface == NULL)
		{
			std::cout << "Err. " << SDL_GetError();
		}

		color = SDL_MapRGB(surface->format,
						   255, 0, 0);

		if (SDL_FillRect(surface, &fill, color) != 0)
		{
			std::cout << "Err. " << SDL_GetError();
		}
	}

	SDL_Surface *getBlit()
	{
		return surface;
	}
	SDL_Rect getRect()
	{
		return rect;
	}
	void setXY(int x, int y)
	{
		rect.x = x;
		rect.y = y;
	}
};

struct SHighscore
{
	SDL_Surface* title = loadText("High Score");
	SDL_Surface* high = loadText("0000");

	SDL_Rect title_rect{0,1700,0,0};
	SDL_Rect high_rect{0, 1800, 0,0};
};

struct SPanel
{
	int x = 0;
	int y = 800;
	int right = 600;
	// moto g54 1080 x 2400
	int w = 1080;
	int h = 2400;

	SDL_Rect all_gui{0, 800, w, h - 800};
	SDL_Rect all_grid{0, 0, w, 800};

	SDL_Rect bet_rect{right, 800, 0, 0};
	SDL_Rect bet_less_rect{50, 800, 0, 0};
	SDL_Rect bet_more_rect{300, 800, 0, 0};
	SDL_Rect bet_amount_rect{0, 950, 0, 0};
	SDL_Rect pot_rect{right, 1100, 0, 0};
	SDL_Rect cont_rect{0, 1250, 0, 0};
	SDL_Rect stake_rect{0, 1400, 0, 0};
	SDL_Rect bank_rect{0, 1550, 0, 0};
	SDL_Rect replay_rect{w / 2, 1700, 0, 0};
	SDL_Rect fluke_rect{w / 3, 700, 0, 0};


	SDL_Surface *pot = loadText("test");
	SDL_Surface *bet = loadText("test");
	SDL_Surface *bet_more = loadText("test");
	SDL_Surface *bet_less = loadText("test");
	SDL_Surface *bet_amount = loadText("test");
	SDL_Surface *cont = loadText("test");
	SDL_Surface *stake_a =
		loadText("test");
	SDL_Surface *bank = loadText("test");

	SDL_Surface *replay = loadText("test");
	SDL_Surface *fluke = loadText("fluke!");
};

class CFancyPanel
{
	public:
	CFancyPanel(
		SDL_Surface* S,
		TTF_Font* F,
		CPlayer P) : 
			window_surface(S),
			font(F),
			player(P){};
			
	void drawGUI();
	void clearGUI();
	void loadText();
	
	// never changes
	/*
	plenty of time to code at Bloom
	*/
	void setGFXPositions()
	{
		screen_width = window_surface->w;
		screen_height = window_surface->h;
		std::cout << screen_height;
		
		// colors
		monkey_color = SDL_MapRGB(
			window_surface->format,
			34,139,34);
			
		play_color = SDL_MapRGB(
			window_surface->format,
			26,108,128);
			
		// positions
		monkey_rect.x = border;
		monkey_rect.y = border;
		monkey_rect.w = screen_width - border*2;
		monkey_rect.h = monkey_rect.w;
		
		in_monkey_rect.x = monkey_rect.x + border;
		in_monkey_rect.y = monkey_rect.y + border;
		
		play_rect.x = border;
		play_rect.y = monkey_rect.y + monkey_rect.h;
		play_rect.w = screen_width - border*2;
		play_rect.h = screen_height-monkey_rect.h-border*2;
		
		in_play_rect.x = play_rect.x + border;
		in_play_rect.y = play_rect.y + border;
		in_play_rect.w = screen_width - border*4;
		
		bank_rect.x = in_play_rect.x + in_play_rect.w;
		bank_rect.y = in_play_rect.y;
			
		//bkgd = IMG_Load("images/bkgd2.png");
	}
	
	private:
	// vars need setFancyGUI
	SDL_Surface* window_surface = nullptr;
	SDL_Surface* bkgd = nullptr;
	SDL_Surface* test = nullptr;
	TTF_Font* font = nullptr;
	
	int screen_width = 0;
	int screen_height = 0;
	int border = 40;
	
	CPlayer player;
	Uint32 monkey_color = 0;
	Uint32 play_color = 0;
	
	SDL_Rect zero{};
	SDL_Rect in_play_rect{};
	SDL_Rect in_monkey_rect{};
	
	SDL_Rect monkey_rect{};
	SDL_Rect play_rect{};
	
	SDL_Rect bank_rect{};
};

void CFancyPanel::loadText()
{
	std::string bank_s =
		std::to_string(player.bank_amount);
		
	bkgd = IMG_Load("images/bkgd2.png");
	
	test = loadGUIText(font, bank_s.c_str());
}

// reference panel to swap sizes
void CFancyPanel::drawGUI()
{
	SDL_BlitSurface(
		bkgd,
		NULL,
		window_surface,
		&zero);
		
	SDL_FillRect(window_surface, &monkey_rect,
	monkey_color);
	SDL_FillRect(window_surface, &play_rect, play_color);

		
	//test = loadGUIText(font, bank_s.c_str());
	
	SDL_BlitSurface(
		test,
		NULL,
		window_surface,
		&bank_rect);
}

void CFancyPanel::clearGUI()
{
	SDL_FreeSurface(test);
	test = nullptr;
	SDL_FreeSurface(bkgd);
	bkgd = nullptr;
	SDL_FreeSurface(window_surface);
	window_surface = nullptr;
}


//
// custom functions here
//

void clearGUI(SPanel &panel)
{
	SDL_FreeSurface(panel.bet);
	panel.bet = nullptr;
	SDL_FreeSurface(panel.bet_less);
	panel.bet_less = nullptr;
	SDL_FreeSurface(panel.bet_more);
	panel.bet_more = nullptr;
	SDL_FreeSurface(panel.bet_amount);
	panel.bet_amount = nullptr;

	SDL_FreeSurface(panel.pot);
	panel.pot = nullptr;
	SDL_FreeSurface(panel.bank);
	panel.bank = nullptr;
	SDL_FreeSurface(panel.cont);
	panel.cont = nullptr;
	SDL_FreeSurface(panel.stake_a);
	panel.stake_a = nullptr;

	SDL_FreeSurface(panel.replay);
	panel.replay = nullptr;
	SDL_FreeSurface(panel.fluke);
	panel.fluke = nullptr;
}

void clearHighscore(SHighscore score)
{
	SDL_FreeSurface(score.high);
	score.high = nullptr;
}

// having control over different aspects
// of the screen seemed simpler
void clearSurface(
	SDL_Surface *surface, SDL_Rect area)
{
	Uint32 color =
		SDL_MapRGB(surface->format,
				   0, 0, 0);
	if (
		SDL_FillRect(surface,
					 &area, color) != 0)
	{
		std::cout << "Err. " << SDL_GetError();
	}
}

//
// grid
//

void drawGrid(
	SDL_Surface *win_surface,
	CGrid grid)
{
	for (int i = 0; i < grid.points.size(); ++i)
	{
		// j = [i]
		int j = i * grid.cell_size;
		int x = j % (grid.cell_size * grid.grid_size);
		int y = j / (grid.cell_size * grid.grid_size);

		y *= grid.cell_size;
		x += grid.grid_offset_x;
		y += grid.grid_offset_y;

		SDL_Rect r{x, y, 1, 1};
		Uint32 c = SDL_MapRGB(
			win_surface->format,
			255, 255, 255);
		SDL_FillRect(
			win_surface, &r, c);
	}
}

//
// line
//

void printSplit(CLine l)
{
	for (int i = 0; i < l.split.size(); ++i)
	{
		std::cout << i << "-" << l.split[i] << " ";
	}
}

void printLine(CLine l)
{
	std::cout << l.cwidth << "/";
	std::cout << l.cheight << "/";
	std::cout << l.width << "/";
	std::cout << l.height << "-/";
	std::cout << l.start << "/";
	std::cout << l.startx << "/";
	std::cout << l.starty << "-/";
	std::cout << l.end << "/";
	std::cout << l.endx << "/";
	std::cout << l.endy << "-/";
	std::cout << l.advance << "/";
	std::cout << l.extend << ".";
}

CLine createLine(
	CGrid grid,
	int start_cell,
	int end_cell)
{
	CLine line(grid, start_cell, end_cell);

	line.setStartPoint();
	line.setEndPoint();
	line.findSize();
	line.advanceExtend();
	line.create();

	return line;
}

void drawLine(
	SDL_Surface *surface,
	CLine line,
	CGrid grid)
{
	Uint32 line_color =
		SDL_MapRGB(
			surface->format,
			255, 0, 0);

	for (int i = 0; i < line.line.size(); ++i)
	{
		int y = (int)
					line.line[i] /
				grid.grid_size;
		int x = (int)
					line.line[i] -
				grid.grid_size * y;

		x *= grid.cell_size;
		y *= grid.cell_size;
		x += grid.grid_offset_x;
		y += grid.grid_offset_y;

		SDL_Rect r{x, y,
				   grid.cell_size, grid.cell_size};
		SDL_FillRect(
			surface,
			&r, line_color);
	}
}

void drawLines(
	SDL_Surface *surface,
	std::vector<CLine> lines,
	CGrid grid)
{
	for (CLine l : lines)
	{
		drawLine(surface, l, grid);
	}
}

bool checkLineData(
	CLine line1, CLine line2)
{
	for (int i : line1.line)
	{
		for (int j : line2.line)
		{
			if (i == j)
			{
				std::cout << " Match "
						  << line1.id
						  << " and " << line2.id << ".";
				return true;
			}
		}
	}
	return false;
}

bool checkIntersection(
	std::vector<CLine> all_lines)
{
	for (int i = 0; i < all_lines.size(); ++i)
	{
		for (int j = 0; j < all_lines.size(); ++j)
		{
			if (all_lines[i].id == all_lines[j].id)
			{
				std::cout << " skip "
						  << i << "," << j << ".";
				continue;
			}
			else
			{
				if (checkLineData(
						all_lines[i], all_lines[j]))
				{
					return true;
				}
			}
		}
	}
	return false;
}

bool ymxc(
	CLine line1,
	CLine line2)
{
	int gradientAx =
		line1.startx - line1.endx;
	int gradientAy =
		line1.starty - line1.endy;

	float gradientA =
		(float)gradientAy / gradientAx;

	int gradientBx =
		line2.startx - line2.endx;
	int gradientBy =
		line2.starty - line2.endy;

	float gradientB =
		(float)gradientBy / gradientBx;

	float c = line1.starty - (gradientA * line1.startx);
	float d = line2.starty - (gradientB * line2.startx);

	float x = (d - c) / (gradientA - gradientB);
	float y = gradientA * x + c;

	// line 1
	/*
	std::cout << " id" << line1.id;
	std::cout << "S" << line1.start;
	std::cout << "E" << line1.end;
	std::cout << "Ax" << gradientAx;
	std::cout << "Ay" << gradientAy;
	std::cout << " g:" << gradientA;

	std::cout << "s("
			  << line1.startx << ","
			  << line1.starty << ")";
	std::cout << "e("
			  << line1.endx << ","
			  << line1.endy << ")";

	// line 2
	std::cout << " id" << line2.id;
	std::cout << "S" << line2.start;
	std::cout << "E" << line2.end;
	std::cout << "Bx" << gradientBx;
	std::cout << "By" << gradientBy;
	std::cout << " g:" << gradientB;
	std::cout << " x:" << x << " y:" << y;

	std::cout << "s("
			  << line2.startx << ","
			  << line2.starty << ")";
	std::cout << "e("
			  << line2.endx << ","
			  << line2.endy << ")";
*/
	bool line1_collision = false;
	bool line2_collision = false;

	// what about vertical lines?
	if (line1.startx < line1.endx)
	{
		if (line1.startx < x && x < line1.endx)
		{
			if (line1.starty < y && y < line1.endy)
			{
				line1_collision = true;
			}
		}
	}
	else
	{
		if (line1.startx > x && x > line1.endx)
		{
			if (line1.starty < y && y < line1.endy)
			{
				line1_collision = true;
			}
		}
	}

	if (line2.startx < line2.endx)
	{
		if (line2.startx < x && x < line2.endx)
		{
			if (line2.starty < y && y < line2.endy)
			{
				line2_collision = true;
			}
		}
	}
	else
	{
		if (line2.startx > x && x > line2.endx)
		{
			if (line2.starty < y && y < line2.endy)
			{
				line2_collision = true;
			}
		}
	}

	if (line1_collision && line2_collision)
		return true;
	else
		return false;
}

// not what i needed
bool checkFluke(
	std::vector<CLine> all_lines)
{
	int size = all_lines.size();
	if (size > 1)
	{
		if (ymxc(
				all_lines[size - 1],
				all_lines[size - 2]))
		{
			return true;
		}
	}

	return false;
}

bool checkAllFlukes(
	std::vector<CLine> all_lines,
	CPlayer &player)
{
	/*
	it might be nice to find a
	fluke! x 2
	fluke! x 3
	
	if allflukes is a multiple line checker
	if makes sense to change
	fluke_amount here
	and we only want the current line
	*/
	int size = all_lines.size();

	for (int i = 0; i < all_lines.size(); ++i)
	{
		// only the current bet
		if (all_lines[i].id == all_lines[size - 1].id)
		{
			continue;
		}
		else
		{
			if (ymxc(
					all_lines[i], all_lines[size - 1]))
			{
				++player.fluke_amount;
			}
		}
	}

	if (player.fluke_amount > 0)
		return true;

	return false;
}

//
// gui
//

/*
looking at UpperBlit
src.w = dest.w = w
thats why i reference panel
as blit changes the correct
size for me
*/
void drawGUI(
	SDL_Surface *surface,
	SPanel &panel,
	CPlayer player,
	EState state)
{
	std::string s = "";

	switch (state)
	{
	case LOST:
	{
		panel.pot =
			loadText("   pot   ",
					 SDL_Color{100, 100, 100});

		panel.bet =
			loadText("   bet   ",
					 SDL_Color{100, 100, 100});

		panel.bet_less =
			loadText("   -   ",
					 SDL_Color{100, 100, 100});

		panel.bet_more =
			loadText("   +   ",
					 SDL_Color{100, 100, 100});

		panel.cont =
			loadText(" continue ");

		s = "your bet is " +
			std::to_string(player.bet_amount);
		panel.bet_amount =
			loadText(s.c_str(), SDL_Color{120, 50, 50});

		break;
	}
	case NONE:
	{
		// special cases
		// not sure if adding bits helps
		if (!player.out_of_game)
			panel.pot =
				loadText("   pot   ");
		else
			panel.pot =
				loadText("   pot   ",
						 SDL_Color{100, 100, 100});

		// special case again
		if (player.fluke_amount < 2)
			panel.fluke =
				loadText("fluke!");
		else
		{
			s = "fluke! x" +
				std::to_string(player.fluke_amount);
			panel.fluke =
				loadText(s.c_str());
		}

		panel.bet =
			loadText("   bet   ");
		panel.bet_less =
			loadText("   -   ");
		panel.bet_more =
			loadText("   +   ");
		panel.cont =
			loadText(" continue ",
					 SDL_Color{100, 100, 100});

		s = "your bet is " +
			std::to_string(player.bet_amount);
		panel.bet_amount =
			loadText(s.c_str());

		break;
	}
	case WON:
	{
		panel.pot =
			loadText("   pot   ",
					 SDL_Color{100, 100, 100});

		panel.bet =
			loadText("   bet   ",
					 SDL_Color{100, 100, 100});

		panel.bet_less =
			loadText("   -   ",
					 SDL_Color{100, 100, 100});

		panel.bet_more =
			loadText("   +   ",
					 SDL_Color{100, 100, 100});

		panel.cont =
			loadText(" continue ");

		s = "your bet is " +
			std::to_string(player.bet_amount);
		panel.bet_amount =
			loadText(s.c_str(), SDL_Color{120, 120, 50});

		break;
	}
	case INSTRUCTIONS:
	{
		// impossible?
		break;
	}
	}

	// switch state
	// is core game loop
	// replay and fluke are
	// separate to state
	// one offs
	if (player.out_of_game)
	{
		panel.replay =
			loadText(" start again ",
					 SDL_Color{100, 255, 100});

		SDL_BlitSurface(
			panel.replay,
			NULL,
			surface,
			&panel.replay_rect);
	}
	if (player.is_fluke)
	{
		SDL_BlitSurface(
			panel.fluke,
			NULL,
			surface,
			&panel.fluke_rect);
	}

	//
	// blit, blit, blit
	//

	SDL_BlitSurface(
		panel.bet,
		NULL,
		surface,
		&panel.bet_rect);

	SDL_BlitSurface(
		panel.bet_less,
		NULL,
		surface,
		&panel.bet_less_rect);

	SDL_BlitSurface(
		panel.bet_more,
		NULL,
		surface,
		&panel.bet_more_rect);

	SDL_BlitSurface(
		panel.pot,
		NULL,
		surface,
		&panel.pot_rect);

	SDL_BlitSurface(
		panel.cont,
		NULL,
		surface,
		&panel.cont_rect);

	//
	// changing gui
	//

	SDL_BlitSurface(
		panel.bet_amount,
		NULL,
		surface,
		&panel.bet_amount_rect);

	s = "bank : " +
		std::to_string(player.bank_amount);
	panel.bank =
		loadText(s.c_str());

	SDL_BlitSurface(
		panel.bank,
		NULL,
		surface,
		&panel.bank_rect);

	s = " Stake " +
		std::to_string(player.stake_amount);
	panel.stake_a =
		loadText(s.c_str());

	SDL_BlitSurface(
		panel.stake_a,
		NULL,
		surface,
		&panel.stake_rect);
}


//
// player
//

void quitAndSave(
CPlayer player)
{
	std::string file = "high.txt";
	std::string score = std::to_string(player.bank_amount);
	std::string date = "/no date/";
	
	CTimer timer;
	timer.printT();
	
	std::ifstream in{file};
	if(!in)
		std::cout << "Make a file first";
	else
	{
		in >> score;
		in >> date;
	}
	
	int ss = std::stoi(score);
	
	std::ofstream out{file, std::ios::trunc};
	if(!out)
		std::cout << "No out file available";
	else if ( player.bank_amount <= ss)
	{
		std::cout << "No new high score";
		out << score;
		out << "\n";
		out << date;
	}
	else if ( player.bank_amount > ss)
	{
		// add score
		out << player.bank_amount << "\n";
		// add date
		out << std::put_time(std::localtime(&timer.tt), "%F %T") << std::endl;
	}
	else
		std::cout << "Err. File in / out";
}

std::string readHighScore()
{
	std::string file = "high.txt";
	std::string score = "NaN";
	std::string date = "...";
	std::string result = "";
	
	std::ifstream in{file};
	if(in)
	{
		in >> score;
		in >> date;
		result = score + "  on  " + date;
	}
	else
	{
		std::cout << "in readHighScore... no data...";
		result = "No high score";
	}
	
	return result;
}

void takePlayerMoney(CPlayer &player)
{
	player.bank_amount -= player.bet_amount;
}
void givePlayerMoney(CPlayer &player)
{
	player.bank_amount +=
		player.stake_amount;
}

//
// main
//

int main(int argc, char *argv[])
{
	SDL_Init(SDL_INIT_VIDEO);
	TTF_Init();
	IMG_Init(IMG_INIT_PNG);
	Mix_Init(MIX_INIT_OGG);
	if (
		Mix_OpenAudio(
			48000,
			AUDIO_S16SYS,
			1,
			1024) != 0)
		return 1;
		
	TTF_Font* font =
		TTF_OpenFont("/system/fonts/Roboto-Regular.ttf", 128);

	srand(time(0));
	//int timer = 0;

	SDL_Window *window =
		SDL_CreateWindow(
			NULL,
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			100, 100,
			SDL_WINDOW_MAXIMIZED);

	SDL_Surface *window_surface =
		SDL_GetWindowSurface(window);
	if (window_surface == NULL)
		std::cout << "Err. No window surface.";

	// not needed
	int sw = window_surface->w;
	int sh = window_surface->h;

	Mix_Chunk *fluke_wav =
		Mix_LoadWAV("sfx/space1.wav");
	if (fluke_wav == NULL)
		return 1;
	Mix_Music *click =
		Mix_LoadMUS("sfx/misc_05.ogg");
	if (click == NULL)
		return 1;
	Mix_Music *click2 =
		Mix_LoadMUS("sfx/misc_07.ogg");
	if (click2 == NULL)
		return 1;
	Mix_Music *potting =
		Mix_LoadMUS("sfx/misc_06_quiet.ogg");
	if (potting == NULL)
		return 1;

	SDL_Surface *ins =
		IMG_Load("images/lc_instructions.png");
	if (ins == NULL)
		return 1;

	//
	// GUI
	//

	CGrid grid;
	SPanel panel;
	SHighscore highscore;
	CPlayer player(10, 100, 0);
	EState state = EState::INSTRUCTIONS;
	
	CFancyPanel fancy_panel(
		window_surface, font, player);
	fancy_panel.setGFXPositions();
	fancy_panel.loadText();

	// vars
	int rand_start;
	int rand_end;
	int stake_multiplier = 0;
	int first_half_grid =
		grid.grid_size * grid.grid_size / 2 - 1;

	std::vector<CLine> lines;

	bool see_instructions = true;
	see_instructions= false;

	SDL_Event e{};
	SDL_TouchFingerEvent f{};
	SDL_bool betting{SDL_FALSE};
	SDL_bool running{SDL_TRUE};


	//
	// instructions
	//
	
	SDL_Rect ins_r{0, 0, sw, sh};
	clearSurface(window_surface, ins_r);
	SDL_BlitSurface(
		ins,
		NULL,
		window_surface,
		&ins_r);
		
	//
	// high score
	//
	
	std::string high_s = readHighScore();
	highscore.high =
		loadText(high_s.c_str());
		
	SDL_BlitSurface(
		highscore.title,
		NULL,
		window_surface,
		&highscore.title_rect);
	SDL_BlitSurface(
		highscore.high,
		NULL,
		window_surface,
		&highscore.high_rect);

	
	// test
	
	//
	// go
	//

	while (running)
	{
		while (SDL_PollEvent(&e) > 0)
		{
			switch (e.type)
			{
			case SDL_QUIT:
			{
				std::cout << "Quit and save";
				quitAndSave(player);
				running = SDL_FALSE;
				break;
			}
			case SDL_FINGERDOWN:
			{
				SDL_Rect touch{0, 0, 2, 2};
				f = e.tfinger;
				touch.x = f.x * sw;
				touch.y = f.y * sh;

				switch (state)
				{
				case INSTRUCTIONS:
				{
					see_instructions = false;
					clearSurface(window_surface, ins_r);
					SDL_FreeSurface(ins);
					ins = nullptr;
					clearHighscore(highscore);
					// allow gui to draw
					state = EState::NONE;
					break;
				}
				case LOST:
				{
					// continue button enables
					if (SDL_HasIntersection(
							&touch, &panel.cont_rect))
					{
						player.bet_total = 0;
						player.stake_amount = 0;
						stake_multiplier = 0;
						// go for next game
						/*
						SDL_Rect r{
							grid.grid_offset_x,
							grid.grid_offset_y,
							grid.grid_size *
								grid.cell_size,
							grid.grid_size *
								grid.cell_size};
						*/
						clearSurface(
							window_surface,
							panel.all_grid);

						lines.clear();
						state = EState::NONE;
					}
					break;
				}
				case NONE:
				{
					// we can only bet when in NONE
					if (SDL_HasIntersection(
							&touch, &panel.bet_rect))
					{
						betting = SDL_TRUE;
					}
					if (SDL_HasIntersection(
							&touch, &panel.bet_more_rect))
					{
						Mix_PlayMusic(click2, 0);

						// possible duplicate
						if (player.bet_amount <
							player.bank_amount)
							player.bet_amount += 10;
						/*
						else if (player.bet_amount >
								 player.bank_amount)
							player.bet_amount =
								player.bank_amount;
						*/
					}
					if (SDL_HasIntersection(
							&touch, &panel.bet_less_rect))
					{
						Mix_PlayMusic(click2, 0);

						if (player.bet_amount > 0)
							player.bet_amount -= 10;
					}
					if (!player.out_of_game)
						if (SDL_HasIntersection(
								&touch, &panel.pot_rect))
						{
							state = EState::WON;
							Mix_PlayMusic(potting, 0);
						}

					//
					// replay
					//

					if (SDL_HasIntersection(
							&touch, &panel.replay_rect))
					{
						player.out_of_game = false;
						player.bank_amount = 100;
						player.bet_amount = 10;

						clearSurface(
							window_surface,
							panel.all_grid);
						lines.clear();
					}
					break;
				}
				case WON:
				{
					// WON will hold on
					// until continue pressed
					if (SDL_HasIntersection(
							&touch, &panel.cont_rect))
					{
						// if we do not bet again
						// and pot is pushed
						player.is_fluke = false;
						player.fluke_amount = 0;

						// keep hold
						player.bet_amount =
							player.prev_bet;
						givePlayerMoney(player);

						// after money issued
						player.stake_amount = 0;
						player.bet_total = 0;
						stake_multiplier = 0;

						// if we won and bank 0
						// give some bet
						//player.bet_amount = 10;

						// go for next game
						/*
						SDL_Rect r{
							grid.grid_offset_x,
							grid.grid_offset_y,
							grid.grid_size *
								grid.cell_size,
							grid.grid_size *
								grid.cell_size};
						*/
						clearSurface(
							window_surface,
							panel.all_grid);

						lines.clear();
						state = EState::NONE;
					}
					break;
				}
				}
			}
			}
		}

		if (betting)
		{
			Mix_PlayMusic(click, 0);

			// get fluke out the way
			// we might continue with a fluke
			// or pot
			// 2 use cases
			// 1. either we keep betting
			// 2. we pot
			//
			// but fluke only cares at one stage
			// in the game, before another bet
			if (player.is_fluke)
			{
				player.is_fluke = false;
				player.fluke_amount = 0;

				// remove fluke text next bet
				clearSurface(
					window_surface,
					panel.all_grid);
				drawLines(window_surface,
						  lines, grid);
			}

			//
			// lines
			//

			rand_start = rand() % first_half_grid;
			rand_end = first_half_grid +
					   rand() % first_half_grid;

			CLine l = createLine(
				grid, rand_start,
				rand_end);

			lines.push_back(l);
			drawLine(window_surface, l, grid);

			//
			// money
			//

			player.bet_total +=
				player.bet_amount;

			player.stake_amount =
				(player.bet_total *
				 stake_multiplier) +
				player.bet_total;

			// this works very well
			if (stake_multiplier == 0)
				++stake_multiplier;
			else
				stake_multiplier *= 2;

			// deal with bank
			takePlayerMoney(player);

			//
			// intersections
			//

			// first - we definitely lost
			// second - how many did we cross?
			if (checkIntersection(lines))
			{
				// update gui
				state = EState::LOST;
			}
			/*
			else if (checkFluke(lines))
			{
				std::cout << "Fluke!";
				player.is_fluke = true;
				Mix_PlayChannel(
					-1, fluke_wav, 0);
			}
			*/
			// we update a reference in AllFlukes
			else if (checkAllFlukes(lines, player))
			{
				player.is_fluke = true;
				Mix_PlayChannel(
					-1, fluke_wav, 0);
			}

			// keep hold
			if (player.bet_amount > 0 &&
				player.bank_amount == 0)
				player.prev_bet = player.bet_amount;
			else if (lines.size() == 1)
				player.prev_bet = player.bet_amount;

			betting = SDL_FALSE;
		}

		//
		// always check
		//

		if (
			player.bank_amount == 0 &&
			player.bet_amount == 0 &&
			player.stake_amount == 0 &&
			state == EState::NONE)
		{
			player.out_of_game = true;
		}

		// keeps bet to a limit
		if (player.bet_amount >
			player.bank_amount)
			player.bet_amount =
				player.bank_amount;

		//
		// end of loop
		//
		if(!see_instructions)
		{
			fancy_panel.drawGUI();
		}
/*
		if (!see_instructions)
		{
			clearSurface(
				window_surface, panel.all_gui);
			drawGUI(
				window_surface,
				panel, player,
				state);
		}
*/

		SDL_UpdateWindowSurface(
			window);
	}

	//
	// end of program
	//

	lines.clear();
	//clearGUI(panel);
	//clearHighscore(highscore);
	fancy_panel.clearGUI();

	Mix_FreeChunk(fluke_wav);
	Mix_FreeMusic(click);
	Mix_FreeMusic(click2);
	Mix_FreeMusic(potting);

	Mix_CloseAudio();
	Mix_Quit();
	std::cout << "Mix Quit. ";
	IMG_Quit();
	TTF_CloseFont(font);
	TTF_Quit();
	std::cout << "TTF Quit. ";
	SDL_DestroyWindow(window);
	SDL_Quit();
	std::cout << "SDL Quit. ";

	return 0;
}