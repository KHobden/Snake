#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <iostream>
#include <algorithm>
#include <deque>


// Coordinates object for snake and fruit
struct Coords {
	int x;
	int y;
};


// Define possible directions for the snake to travel
enum class Direction {
	stop,
	up,
	down,
	left,
	right
};


// Comparison operator for Coords
bool operator==(Coords a, Coords b) {
	return (a.x == b.x) && (a.y == b.y);
}


// Generate random coords for a new fruit within boundaries
Coords new_fruit_coords(int height, int width) {
	Coords new_coords;
	new_coords.x = (rand() % (width-2)) + 1;
	new_coords.y = (rand() % (height-2)) + 1;
	return new_coords;
}


// Translate a keypress into a direction s.t. direction remains if invalid key is pressed
Direction translate_keypress(char key, Direction direction) {
	switch (key) {
		case 'w':
			direction = Direction::up;
			break;
		case 's':
			direction = Direction::down;
			break;
		case 'a':
			direction = Direction::left;
			break;
		case 'd':
			direction = Direction::right;
			break;
	}
	return direction;
}

// Given a direction, get new snake head coords
Coords move_head(Direction direction, std::deque<Coords> &snake) {
	Coords head = snake.front();
	switch(direction) {
		case Direction::up:
			head.y--;
			break;
		case Direction::down:
			head.y++;
			break;
		case Direction::left:
			head.x--;
			break;
		case Direction::right:
			head.x++;
			break;
	}
	return head;
}


// Check if snake has hit boundary
bool boundary_collision(int height, int width, std::deque<Coords> &snake) {
	Coords head_pos = snake.front();
	return (head_pos.x <= 0) || (head_pos.x >= width-1)
		|| (head_pos.y <= 0) || (head_pos.y >= height-1);
}


// Check if the game should end
bool is_finished(char key, int height, int width, std::deque<Coords> &snake) {
	bool out_of_bounds = boundary_collision(height, width, snake);
	bool user_ended = (key=='x');
	return (out_of_bounds || user_ended);
}





// Check for a valid keypress
char get_input(char key) {
	if (kbhit()) {
		char key_pressed = getch();
		if (key_pressed=='w' || key_pressed=='a' ||
			key_pressed=='s' || key_pressed=='d' ||
			key_pressed=='x')
			key = key_pressed;
	}
	return key;
}



// Driver Code
int main()
{
	// Define params
	constexpr int height = 20, width = 20;
	int score = 0;
	bool finished = false;
	Direction direction = Direction::stop;

	// Initialise grid
	char grid[height][width];
	for (int row=0; row<height; row++) {
		for (int col=0; col<width; col++) {
			if (row==0 || row==height-1 ||
				col==0 || col==width-1)
				grid[row][col] = '#';
			else
				grid[row][col] = ' ';
		}
	}

	// Initialise snake at centre of grid
	Coords initial_pos = {.x=width/2, .y=height/2};
	grid[initial_pos.y][initial_pos.x] = '0';
	std::deque<Coords> snake = {initial_pos};

	// Initialise fruit position
	Coords fruit = new_fruit_coords(height, width);
	grid[fruit.y][fruit.x] = '*';

	// Draw game
	for (int row=0; row<height; row++) {
		for (int col=0; col<width; col++) {
			std::cout << grid[row][col];
		}
		std::cout << '\n';
	}
	std::cout << "Press any direction key to begin!";

	// Get valid input to begin game
	char key = 'e';
	while (!(key=='w' || key=='a' ||
		key=='s' || key=='d'))
		key = getch();


	while (!finished) {

		// Get a valid input to change direction
		key = get_input(key);
		if (key=='x')
			finished = true;
		direction = translate_keypress(key, direction);

		// Move snake in direction
		Coords new_head = move_head(direction, snake);
		if (grid[new_head.y][new_head.x]=='0') // If snake eats itself, terminate
			finished = true;
		grid[new_head.y][new_head.x] = '0';
		snake.push_front(new_head);
		
		// If snake eats fruit, generate a new fruit and increase the score
		// If snake eats itself, terminate
		// If snake hits boundary, terminate
		// Otherwise drop the old tail coords as the tail has moved
		if (new_head == fruit) {
			while (grid[fruit.y][fruit.x] == '0')
				fruit = new_fruit_coords(height, width);
			grid[fruit.y][fruit.x] = '*';
			score += 10;
		// } else if (std::find(snake.begin(), snake.end(), new_head) != snake.end() && !(snake.front()==snake.back())) {
		// 	// Note the snake can eat its tail as when it moves its head, its tail also moves
		// 	finished = true;
		} else if (boundary_collision(height, width, snake)) {
			finished = true;
		} else {
			Coords old_tail = snake.back();
			grid[old_tail.y][old_tail.x] = ' ';
			snake.pop_back();
		}

		// Pause to control speed of snake
		Sleep(0.05);

		// Draw game
		system("cls");
		for (int row=0; row<height; row++) {
			for (int col=0; col<width; col++) {
				std::cout << grid[row][col];
			}
			std::cout << '\n';
		}
		// Show game progress
		std::cout << "Score = " << score << '\n';
		std::cout << "Press X to quit the game\n";
	}

	return 1;
}