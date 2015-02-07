#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <unistd.h>

bool lost = false;
int maxx, maxy;
int length = 0;
int ch = 0;
int worth = 7;

struct point{
	int x;
	int y;
};

point head;
std::vector<point> snake;
point food;
point dir;

void genfood(){
	food.x = rand() % (maxx - 2) + 1;
	food.y = rand() % (maxy - 2) + 1;
}

void setup(){
	initscr();
	noecho();
	curs_set(FALSE);
	srand(time(NULL));
	cbreak();
	keypad(stdscr, TRUE);
	getmaxyx(stdscr, maxy, maxx);
	head.x = maxx / 2;
	head.y = maxy / 2;
	genfood();
	dir.x = 1;
	dir.y = 0;
}

void box (int x, int y){
	for (int i = 0; i < x; i++){
		mvprintw(0, i, "#");
		mvprintw(y - 1, i, "#");
	}
	for (int i = 0; i < y; i++){
		mvprintw(i, 0, "#");
		mvprintw(i, x - 1, "#");
	}
}

void get_input(){
	ch = getch();
	switch (ch) {
	case KEY_UP:
		if (dir.y == 0){
			dir.y = -1;
			dir.x = 0;
		}
		break;
	case KEY_DOWN:
		if (dir.y == 0){
			dir.y = 1;
			dir.x = 0;				
		}
		break;
	case KEY_LEFT:
		if (dir.x == 0){
			dir.x = -1;
			dir.y = 0;
		}
		break;
	case KEY_RIGHT:
		if (dir.x == 0){
			dir.x = 1;
			dir.y = 0;
		}
		break;
	default:
		break;
	}
}

void play(){
	nodelay(stdscr, TRUE);
	get_input();
	getmaxyx(stdscr, maxy, maxx);
	snake.push_back(point());
	snake[snake.size() - 1].x = head.x;
	snake[snake.size() - 1].y = head.y;
	head.x += dir.x;
	head.y += dir.y;
	while (snake.size() > length)
		snake.erase(snake.begin());
	if (head.x <= 0 || head.y <= 0 || head.x >= maxx - 1 || head.y >= maxy - 1)
		lost = true;
	erase();
	box(maxx, maxy);
	mvprintw(head.y, head.x, "0");
	for (int i = 0; i < snake.size(); i++){
		mvprintw(snake[i].y, snake[i].x, "o");
		if (head.x == snake[i].x && head.y == snake[i].y)
			lost = true;
	}
	mvprintw(food.y, food.x, "@");
	mvprintw(0, maxx / 2 - 5, "Length: %i", length + 1);
	if (head.x == food.x && head.y == food.y){
		length += worth;
		genfood();
	}	
	refresh();
}

void lose(){
	timeout(-1);
	mvprintw(maxy / 2 - 1, maxx / 2 - 4, "GAME OVER");
	mvprintw(maxy / 2 + 1, maxx / 2 - 4, "Length: %i", length + 1);
	mvprintw(maxy / 2 + 5, maxx / 2 - 7, "Press q to quit");
	refresh;
	while (true){
		ch = getch();
		if (ch == 'q'){
			endwin();
			exit(0);
		}
	}
}


int main(){
	setup();
	while (true){
		while (!lost){
			play();
			usleep(100000);
		}
		lose();
	}
}
