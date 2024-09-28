#ifndef BASIC_H
#define BASIC_H
#endif

#include <vector>
#include <chrono>

typedef std::chrono::time_point<
	std::chrono::system_clock> chromt;
	
class CTimer
{
	public:
	CTimer(){}
	
	chromt time_now;
	std::time_t tt;
	chromt time_start;
	int time_d = 0;
	int time_end = 0;
	
	// more useful
	int day = 0;
	int month = 0;
	int year = 0;
	
	void getDate()
	{
		
	}
	
	void printT()
	{
		time_now =
			std::chrono::system_clock::now();
		 tt =
			std::chrono::system_clock::to_time_t(
				time_now);
	}
};

class CGrid
{
  public:
	CGrid()
	{
		//cell_size = size;
		for (int i = 0; i < grid_size * grid_size; ++i)
		{
			int temp = i * cell_size;
			points.push_back(temp);
		}
	}
	
	int grid_size = 50;
	int cell_size = 12;

	int grid_offset_x{100};
	int grid_offset_y{100};
	int screen_width{0};
	std::vector<int> points{};

	void setScreenWidth(int size)
	{
		screen_width = size;
	}

	int getScreenWidth()
	{
		return screen_width;
	}
};

// for collision
static int line_id = 0;

class CLine
{
  
  public:
	CLine(CGrid grid, int s, int e)
	{
		id = line_id;
		line_id++;
		grid_width = grid.grid_size;
		
		// cant have made up cells
		if(s<0)
		{
			start = 0;
		}
		if(e > grid_width*grid_width-1)
		{
			end = grid_width*grid_width-1;
		}
			
		if (s > e)
		{
			start = e;
			end = s;
		}
		else
		{
			start = s;
			end = e;
		}
	}
	int id;

	int start = 0;
	int startx = 0;
	int starty = 0;
	int end = 0;
	int endx = 0;
	int endy = 0;

	int width = 0;
	int cwidth = 0;
	int height = 0;
	int cheight = 0;
	int grid_width = 0;

	int advance = 0;
	int extend = 0;

	std::vector<int> line{};
	std::vector<int> split{};

	//
	// getColumn functions
	//
	void setStartPoint()
	{
		starty = int(start / grid_width);
		startx = start - (grid_width * starty);
	}
	void setEndPoint()
	{
		endy = int(end / grid_width);
		endx = end - (grid_width * endy);
	}
	void advanceExtend()
	{
		int left_pixel{1};
		int right_pixel{-1};
		int upper_pixel = grid_width;
		int upper_left = grid_width +1;
		int upper_right = grid_width -1;

		if (width > height)
		{
			if (startx < endx)
			{
				advance = upper_left;
				extend = left_pixel;
			}
			else
			{
				advance = upper_right;
				extend = right_pixel;
			}
		}
		else
		{
			if (startx < endx)
			{
				advance = upper_left;
				extend = upper_pixel;
			}
			else
			{
				advance = upper_right;
				extend = upper_pixel;
			}
		}
	}
	void findCompSize()
	{
		int reduce = end - start;
		//float f = (float)reduce / grid_width;
		cheight = (int)reduce / grid_width;
		
		if(startx > endx) cheight++;
		
		int xx = grid_width * cheight - reduce;
		cwidth = std::abs(xx);
	}

	void findSize()
	{
		findCompSize();
		width = cwidth + 1;
		height = cheight + 1;
	}
	
	void create()
	{
		//int nom = (width > height) ? width : height;
		//int den = (height < width) ? height : width;
		int nom;
		int den;
		if(width>height)
		{
			nom = width;
			den = height;
		}
		else
		{
			nom = height;
			den = width;
		}
			
		int mul = nom;

		split.push_back(0);
		for (int i = 0; i < den; ++i)
		{
			split.push_back((int)mul / den);
			mul += nom;
		}

		int subtract = end;
		int current = 1;
		
		for (int i = 0; i < den; ++i)
		{
			line.push_back(subtract);
			int extend_by = split[current] - split[i];
			
			// extend includes 1 advance
			for(int j = 1; j < extend_by; ++j)
			{
				subtract -= extend;
				line.push_back(subtract);
			}
			
			current++;
			subtract -= advance;
		}
	}
};

class CPlayer
{
	public:
	CPlayer(int bet, int bank, int stake)
	{
		bet_amount = bet;
		bank_amount = bank;
		stake_amount = stake;
	}
	int bet_amount = 0;
	int bet_total = 0;
	int bank_amount = 0;
	int stake_amount = 0;
	int prev_bet = 0;
	
	bool out_of_game = false;
	bool is_fluke = false;
	int fluke_amount = 0;
};