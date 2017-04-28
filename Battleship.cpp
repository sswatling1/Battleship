/*
Scott Swatling
Professor Brown
CS I
Semester Project
*/

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <conio.h>

#define aircraft_Carrier_Length 5
#define battleship_Length 4
#define cruiser_Length 3
#define destroyer_Length 3
#define submarine_Length 2
#define grid_Length 10
#define hit 'X'
#define miss 'O'
#define blank ' '
#define aircraft_Carrier_Marker = 'A'
#define battleship_Marker = 'B'
#define cruiser_Marker = 'C'
#define destroyer_Marker = 'D'
#define submarine_Marker = 'S'
#define player1_Val 0
#define player2_Val 1
#define human 0
#define computer 1
#define ship_Total 5
#define PBF_Name "PBF"
#define IRF_Name "IRF"
#define ship_Not_Placed 0
#define ship_Placed 1
#define ship_Destroyed 2
#define ship_Wreck 3

typedef struct
//A coordinate on a plane
{
	int x;
	int y;
}Coordinate;

typedef struct
//A single tile of the game board
{
	Coordinate point;
	char coordinate_Character;
}Tile;

typedef struct
//A single ship
{
	Tile ship[5];
}Ship;


typedef struct
//The fleet information
{
	char marker;
	int ship_Length;
	char* ship_Name;
	Coordinate ships[5][5];
}Fleet;

typedef struct
//The player's information
{
	char* player_Name;
	int player_Num;
	int automation;
	int difficulty;
	int ship_Status[5];
	bool ship_Sunk[5];

}player_Information;

typedef struct
//The game board
{
	Tile grid[grid_Length][grid_Length];
}Grid;

void DISPLAY_OPENING(void);
void INITIALIZE_GRID(Grid* player_Grid);
void DISPLAY_GRID(Grid player_Grid);
int getGAME_MODE(void);
int getDIFFICULTY(player_Information player);
void PLACE_SHIPS(player_Information* player, Grid* player_Grid, Fleet* player_Fleet);
void MANUAL_PLACEMENT(Grid* player_Grid, player_Information* player, Fleet* player_Fleet);
void RANDOM_PLACEMENT(Grid* player_Grid, player_Information* player, Fleet* player_Fleet);
bool CHECK_COORDINATE(Coordinate placement, Fleet ship, Grid* player_Grid, int count);
Coordinate getADJACENT_RANDOM_PLACEMENT(Coordinate placement, int direction);
int getRANDOM_DIRECTION();
bool getPLAYER_FLEET_STATUS(player_Information* player, Grid* enemy_Grid, Grid* player_Guess_Grid, Fleet* player_Fleet);
int CHECK_SHIP(player_Information* player, Grid* enemy_Guess_Grid, Grid* player_Grid, Fleet* player_Fleet, int counter);
void PLAYER_GUESS(player_Information* player, player_Information* enemy_Player, Grid* player_Guess_Grid, Grid* enemy_Grid, Tile* guess, Coordinate* previous_Guess, Coordinate* previous_Correct_Guess, bool is_Hit, bool* player_Fleet_Status, Fleet* enemy_Fleet);
Coordinate getGUESS(player_Information* player, Grid* player_Guess_Grid, bool is_Hit, Coordinate* previous_Guess, Coordinate* previous_Correct_Guess, int direction);
bool GUESS_CHECK(player_Information* player, Grid* player_Guess_Grid, Coordinate guess);
void WAITMSG(void);
void DISPLAY_VICTORY(player_Information* victor);
bool BATTLESHIP();
void DISPLAY_HIT(player_Information* player);
bool getPLAY_AGAIN(void);


int main()
{
	bool play_Again;

	srand(time(NULL));


	play_Again = BATTLESHIP();
	while (play_Again == true)
	{
		system("cls");
		play_Again = BATTLESHIP();
	}
	printf("End of Program.\n");
}

void DISPLAY_OPENING(void)
/*
Displays the title "BATTLESHIP".

Recieves: Nothing

Returns: Nothing
*/
{
	printf("---------------------------------------------------------------------\n");
	printf("| XXXXX   XXXX  XXXXXX XXXXXX XX     XXXXXX  XXXXX XX  XX XX XXXX   |\n");
	printf("| XX  XX XX  XX   XX     XX   XX     XX     XX     XX  XX XX XX  XX |\n");
	printf("| XXXXX  XX  XX   XX     XX   XX     XXXX    XXXX  XXXXXX XX XXXX   |\n");
	printf("| XX  XX XXXXXX   XX     XX   XX     XX         XX XX  XX XX XX     |\n");
	printf("| XXXXX  XX  XX   XX     XX   XXXXXX XXXXXX XXXXX  XX  XX XX XX     |\n");
	printf("---------------------------------------------------------------------\n");
}

void INITIALIZE_GRID(Grid* player_Grid)
/*
Sets the character on each Tile to the defined value of "blank".

Recieves: Grid*
Grid* player_Grid
A pointer to the player's grid

Returns: Nothing
*/
{
	for (int y = 0; y < grid_Length; y++)//Row (Y)
	{
		for (int x = 0; x < grid_Length; x++)//Column (X)
		{
			(*player_Grid).grid[x][y].coordinate_Character = blank;
		}
	}

}

void DISPLAY_GRID(Grid player_Grid)
/*
Prints the current players grid to the screen.

Recieves: Grid
Grid player_Grid
A copy of the player's current grid

Returns: Nothing
*/
{
	printf("X   0 1 2 3 4 5 6 7 8 9\n");
	printf("   ---------------------\n");
	for (int y = 0; y < grid_Length; y++)
	{
		printf("%d | ", y);

		for (int x = 0; x < grid_Length; x++)
		{
			printf("%c ", player_Grid.grid[x][y].coordinate_Character);
		}

		printf("|\n");
	}
	printf("   ---------------------\n");
}

int getGAME_MODE(void)
/*
Prompts the user for an integer value representing the game type.
Checks to make sure the user entered 1, 2, or 3.

Recieves: Nothing

Returns: int
int game_Mode
Represents the game type:
1 = Player vs. Player
2 = Player vs. Computer
3 = Computer vs. Computer

*/
{
	int game_Mode;

	printf("Select a game mode.\n");
	printf("1: Player vs. Player\n");
	printf("2: Player vs. Computer\n");
	printf("3: Computer vs. Computer\n\n");
	printf("Mode: ");
	scanf_s("%d", &game_Mode);

	if (game_Mode < 1 && game_Mode > 3)
	{
		game_Mode = getGAME_MODE();
	}

	return game_Mode;
}

int getDIFFICULTY(player_Information player)
/*
Prompts the user to enter an integer value representing the AI player's skill level.
Checks to make sure the user entered 1,2 or 3.

Recieves: player_Informaton
player_Information player
The players informaton, used to print player's number.

Returns: int
int difficulty
Represents the AI's skill level:
1 = Beginner
2 = Intermediate
3 = Expert
*/
{
	int difficulty;

	printf("Select %s's AI level.\n", player.player_Name);
	printf("1: Beginner\n");
	printf("2: Intermediate\n");
	printf("3: Expert\n\n");

	printf("%s level: ", player.player_Name);
	scanf_s("%d", &difficulty);

	if (difficulty < 1 || difficulty > 3)
	{
		printf("Invalid entry.\n");
		difficulty = getDIFFICULTY(player);
	}

	return difficulty;
}

void DISPLAY_ADMIRAL_INTRO(int player)
/*
Displays an intro for each player depending on the players number

Recieves: int player
int player
Represents the player's number

Returns: Nothing
*/
{
	if (player == 1)
	{
		printf("Welcome aboard, you are the Admiral of the Pacific Blue Fleet, an elite US reconnaissance unit \n");
		printf("whose mission is to seek out and destroy the terrorist naval force known as the Imperial Red Fleet.\n");
		printf("The Imperials have been plundering our shipping lanes for too long, but with you at the helm,\n");
		printf("victory is assured. Semper Fortis(Always Strong), Admiral.\n\n");
	}
	else
	{
		printf("Welcome aboard, you are Admiral of what has come to be known as the Imperial Red Fleet.\n");
		printf("You and your crew are sworn enemies of the US, and have been plundering their shipping\n");
		printf("lanes for some time. Your scouts report that the US is sending an elite\n");
		printf("unit of ships to confront you. This is the perfect opportunity to show the \n");
		printf("US who they're dealing with. Long live the Imperial Red Fleet!\n\n");
	}
}

void PLACE_SHIPS(player_Information* player, Grid* player_Grid, Fleet* player_Fleet)
/*
Determines whether ship placement is randomized or manual based on whether or not the player is human

Recieves: player_Information* player, Grid* player_Grid, Fleet* player_Fleet
player_Information* player
-	A pointer to the player's information

Grid* player_Grid
-	A pointer to the player's grid

Fleet* player_Fleet
-	A pointer to the player's fleet

Returns: Nothing
*/
{
	char* PBF = PBF_Name;
	char* IRF = IRF_Name;

	if ((*player).automation == human)
	{
		MANUAL_PLACEMENT(player_Grid, player, player_Fleet);
	}
	else
	{
		RANDOM_PLACEMENT(player_Grid, player, player_Fleet);
	}
}

void MANUAL_PLACEMENT(Grid* player_Grid, player_Information* player, Fleet* player_Fleet)
/*
Prompts the user for to place each of their ships

Recieves: Grid* player_Grid, player_Information* player, Fleet* player_Fleet
Grid* player_Grid
-	A pointer to the player's grid

player_Information* player
-	A pointer to the player's information

Fleet* player_Fleet
- A pointer to the player's Fleet

Returns: Nothing*/
{
	Coordinate placement[5];
	bool isValid;
	int total_Placements = aircraft_Carrier_Length + battleship_Length + cruiser_Length + destroyer_Length + submarine_Length;
	int count = 0;

	Fleet fleet[ship_Total] = { { 'A', aircraft_Carrier_Length, "aircraft carrier" },
	{ 'B', battleship_Length, "battleship" },
	{ 'C', cruiser_Length, "cruiser" } ,
	{ 'D', destroyer_Length, "destroyer" },
	{ 'S', submarine_Length, "submarine" } };

	DISPLAY_ADMIRAL_INTRO((*player).player_Num);

	for (int i = 0; i < ship_Total; i++)
	{
		printf("Admiral enter the %d coordnates you wish to place your %s at.\n", fleet[i].ship_Length, fleet[i].ship_Name);
		count = 0;
		while (count < fleet[i].ship_Length)
		{
			printf("%s coordinate #%d (x, y): ", fleet[i].ship_Name, (count + 1));
			scanf_s("%d %d", &placement[count].x, &placement[count].y);
			isValid = CHECK_COORDINATE(placement[count], fleet[i], player_Grid, count);
			if (isValid == true)
			{
				(*player_Grid).grid[placement[count].x][placement[count].y].coordinate_Character = fleet[i].marker;
				(*player_Fleet).ships[i][count] = placement[count];
			}
			else
			{
				printf("Invalid placement.\n");
				while (count > 0)
				{
					(*player_Grid).grid[placement[count - 1].x][placement[count - 1].y].coordinate_Character = blank;
					count--;
				}
				count = -1;
			}
			count++;
		}
		system("cls");
		DISPLAY_GRID(*player_Grid);
		(*player).ship_Status[i] = true;
	}
}

void RANDOM_PLACEMENT(Grid* player_Grid, player_Information* player, Fleet* player_Fleet)
/*
Randomly places each ship on the board

Recieves : Grid* player_Grid, player_Information* player, Fleet* player_Fleet
Grid* player_Grid
- A pointer to the player's grid

player_Information* player
- A pointer to the player's information

Fleet* player_Fleet
- A pointer to the player's Fleet

Returns: Nothing
*/
{
	Coordinate placement[6];
	int direction;
	bool isValid;
	int count = 0;
	Fleet fleet[ship_Total] = { { 'A', aircraft_Carrier_Length, "aircraft carrier" },
	{ 'B', battleship_Length, "battleship" },
	{ 'C', cruiser_Length, "cruiser" } ,
	{ 'D', destroyer_Length, "destroyer" },
	{ 'S', submarine_Length, "submarine" } };
	for (int i = 0; i < ship_Total; i++)//For each ship in the fleet
	{
		count = 0;
		placement[count] = { rand() % 10, rand() % 10 };//Get a new, random starting coordinate
		direction = getRANDOM_DIRECTION();

		while (count < fleet[i].ship_Length)//while the ship is not fully placed
		{

			isValid = CHECK_COORDINATE(placement[count], fleet[i], player_Grid, count);

			if (isValid == true)//If the coordinate is valid
			{
				(*player_Grid).grid[placement[count].x][placement[count].y].coordinate_Character = fleet[i].marker;
				(*player_Fleet).ships[i][count] = placement[count];
				placement[count + 1] = getADJACENT_RANDOM_PLACEMENT(placement[count], direction);//Get an adjacent coordiante
			}
			else
			{
				while (count > 0)
				{
					(*player_Grid).grid[placement[count - 1].x][placement[count - 1].y].coordinate_Character = blank;
					count--;
				}
				placement[count] = { rand() % 10, rand() % 10 };//Get a new random starting coordinate
				count = -1;
			}
			count++;
		}
		(*player).ship_Status[i] = 1;
	}
}

int getRANDOM_DIRECTION()
/*
Gets a random number between 0 and 3

Recieves: Nothing

Returns: int direction
int direction
-	An integer representing one of the 4 cardinal directions

*/
{
	int direction;

	direction = rand() % 4;

	return direction;
}

Coordinate getADJACENT_RANDOM_PLACEMENT(Coordinate placement, int direction)
/*
Determines what the next coordinate should be based on the previous one and the direction

Recieves: Coordinate placement, int direction
Coordiate placement
-	A coordinate structure representing the previously placed coordinate

int direction
-	An integer representing a direction

Returns Coordinate next_Coordinate
Coordinate next_Coordinate
-	A coordinate structure representing an adjacent coordinate
*/
{
	Coordinate next_Coordinate;

	if (direction == 0)//If direction is North
	{
		next_Coordinate.x = placement.x;
		next_Coordinate.y = placement.y - 1;//Move one unit north
	}
	else if (direction == 1)//If direction is South
	{
		next_Coordinate.x = placement.x;
		next_Coordinate.y = placement.y + 1;//Move one unit south
	}
	else if (direction == 2)//If direction is East
	{
		next_Coordinate.x = placement.x + 1;//Move one unit east
		next_Coordinate.y = placement.y;
	}
	else if (direction == 3)//If direction is West
	{
		next_Coordinate.x = placement.x - 1;//Move one unit west
		next_Coordinate.y = placement.y;
	}
	else
	{
		next_Coordinate = getADJACENT_RANDOM_PLACEMENT(placement, direction);
	}

	return next_Coordinate;
}

bool CHECK_COORDINATE(Coordinate placement, Fleet ship, Grid* player_Grid, int count)
/*
Checks a coordinate value with the player's grid to make sure it is a valid placement

Recieves: Coordinate placement, Fleet ship, Grid* player_Grid, int count
Coordinate placement
-	A coordinate structure representing the potential placement of a ship section

Fleet ship
-	a Fleet structure representing the player's fleet information

Grid* player_Grid
-	A pointer to the player's grid

int count
-	An integer representing the current number of placed ship sections

Returns: bool check
bool check
-	A boolean value determining whether the spot is valid or not
*/
{
	bool check = true;

	if (placement.x > 9 || placement.x < 0 || placement.y > 9 || placement.y < 0)//If the guess is out of bounds.
	{
		check = false;
	}
	if ((*player_Grid).grid[placement.x][placement.y].coordinate_Character != blank)//If there is a ship already there.
	{
		check = false;
	}
	if (count > 0)//If this is the second guess or later.
	{
		if ((*player_Grid).grid[placement.x - 1][placement.y].coordinate_Character != ship.marker && (*player_Grid).grid[placement.x + 1][placement.y].coordinate_Character != ship.marker)//If the guess is not continuous along the x-axis to either side.
		{
			if ((*player_Grid).grid[placement.x][placement.y - 1].coordinate_Character != ship.marker && (*player_Grid).grid[placement.x][placement.y + 1].coordinate_Character != ship.marker)//If the guess is not continuous along the y-axis to either side.
			{
				check = false;
			}
		}
	}
	return check;
}

bool getPLAYER_FLEET_STATUS(player_Information* player, Grid* enemy_Guess_Grid, Grid* player_Grid, Fleet* player_Fleet)
/*
Determines if the player's fleet has been destroyed or not

Recieves: player_Information* player, Grid* enemy_Guess_Grid, Grid* player_Grid, Fleet* player_Fleet
player_Information* player
-	A pointer to the player's information

Grid* enemy_Guess_Grid
-	A pointer the enemy player's guess grid

Grid* player_Grid
-	A pointer to the player's grid

Fleet* player_Fleet
-	A pointer to the player's fleet information

Returns: bool fleet_Status
bool fleet_Status
-	A boolean value representing whether the player's fleet is active(true) or destroyed(false)
*/
{
	bool fleet_Status = false;
	int count = 0;
	Fleet fleet[ship_Total] = { { 'A', aircraft_Carrier_Length, "aircraft carrier" },
	{ 'B', battleship_Length, "battleship" },
	{ 'C', cruiser_Length, "cruiser" },
	{ 'D', destroyer_Length, "destroyer" },
	{ 'S', submarine_Length, "submarine" } };

	while (count < ship_Total)//While there are ships to check
	{
		(*player).ship_Status[count] = CHECK_SHIP(player, player_Grid, enemy_Guess_Grid, player_Fleet, count);//Check if a ship is still afloat
		count++;
	}
	count = 0;

	while (count < ship_Total)//While there are still ships to check
	{
		if ((*player).ship_Status[count] == 0 && ((*player).ship_Sunk[count] == false))//If the players ship is sunk AND you just sunk it
		{
			printf("\nYou sunk their %s\n\n", fleet[count].ship_Name);
			(*player).ship_Sunk[count] = true;
		}
		if ((*player).ship_Status[count] == 1)//If a ship is still afloat
		{
			fleet_Status = true;
		}
		count++;
	}
	return fleet_Status;
}

int CHECK_SHIP(player_Information* player, Grid* enemy_Guess_Grid, Grid* player_Grid, Fleet* player_Fleet, int counter)
/*
Checks if at least one part of each ship is still active

Recieves: player_Information* player, Grid* enemy_Guess_Grid, Grid* player_Grid, Fleet* player_Fleet, int counter)
player_Information* player
-	A pointer to the player's information

Grid* enemy_Guess_Grid
-	A pointer the enemy player's guess grid

Grid* player_Grid
-	A pointer to the player's grid

Fleet* player_Fleet
-	A pointer to the player's fleet information

int counter
-	An integer representing the index used to access each ship

Returns: int isShip
int isShip
-	An integer value representing whether the ship is still active(1) or not(0)
*/
{
	int count = 0;
	int shipCount = 0;
	int isShip = 0;
	Fleet fleet[ship_Total] = { { 'A', aircraft_Carrier_Length, "aircraft carrier" },
	{ 'B', battleship_Length, "battleship" },
	{ 'C', cruiser_Length, "cruiser" },
	{ 'D', destroyer_Length, "destroyer" },
	{ 'S', submarine_Length, "submarine" } };

	while (isShip == 0 && count < fleet[counter].ship_Length)//While there is no intact ship AND there are still ship sections to be checked
	{
		if ((*enemy_Guess_Grid).grid[(*player_Fleet).ships[counter][count].x][(*player_Fleet).ships[counter][count].y].coordinate_Character == blank)//If the ship has not been hit
		{
			isShip = 1;
		}
		count++;
	}

	return isShip;
}

void PLAYER_GUESS(player_Information* player, player_Information* enemy_Player, Grid* player_Guess_Grid, Grid* enemy_Grid, Tile* guess, Coordinate* previous_Guess, Coordinate* previous_Correct_Guess, bool is_Hit, bool* player_Fleet_Status, Fleet* enemy_Fleet)
/*
Retrieves a guess from the user, determines whether or not it is a hit, and adjusts the necessary variable

Recieves: player_Information* player, player_Information* enemy_Player, Grid* player_Guess_Grid, Grid* enemy_Grid, Tile* guess, Coordinate* previous_Guess, Coordinate* previous_Correct_Guess, bool is_Hit, bool* player_Fleet_Status, Fleet* enemy_Fleet
player_Information* player
-	A pointer to the player's information

player_Information* enemy_Player
-	A pointer to the enemy player's information

Grid* player_Guess_Grid
-	A pointer to the player's guess grid

Grid* enemy_Grid
-	A pointer the enemy player's grid

Tile* guess
-	A pointer to a tile structure representing the player's guess

Coordinate* previous_Guess
-	A pointer to a coordinate structure representing the player's previous guess

Coordinate* previous_Correct_Guess
-	A pointer to a coordinate structure representing the player's original guess

bool is_Hit
-	A boolean value representing whether or not the guess is a hit

bool* player_Fleet_Status
-	A pointer to a boolean value representing the player's fleet status

Fleet*	enemy_Fleet
-	A pointer to the enemy player's fleet information

Returns: Nothing
*/
{
	int direction = 0;

	if ((*player).automation == human)//If the player is human
	{
		DISPLAY_GRID(*player_Guess_Grid);
	}

	if ((*player_Fleet_Status) == true)//If the enemy fleet is still there
	{
		(*guess).point = getGUESS(player, player_Guess_Grid, is_Hit, previous_Guess, previous_Correct_Guess, direction);//Get a guess
		*previous_Guess = (*guess).point;
		if ((*player_Guess_Grid).grid[(*guess).point.x][(*guess).point.y].coordinate_Character == blank)//If the guess has not been guessed before
		{
			printf("%s Guess: (%d, %d)\n",(*player).player_Name, (*guess).point.x, (*guess).point.y);
			if ((*enemy_Grid).grid[(*guess).point.x][(*guess).point.y].coordinate_Character != blank)//If the guess is a hit
			{
				*previous_Correct_Guess = (*guess).point;//Set previous_Correct_Guess as the guess
				is_Hit = true;
				(*player_Guess_Grid).grid[(*guess).point.x][(*guess).point.y].coordinate_Character = hit;
				(*enemy_Grid).grid[(*guess).point.x][(*guess).point.y].coordinate_Character = hit;

				if ((*player).automation == human)//If the player is human
				{
					DISPLAY_HIT(player);
					DISPLAY_GRID(*player_Guess_Grid);
				}
				if ((*player).automation == computer)//If the player is the computer
				{
					DISPLAY_HIT(player);
					DISPLAY_GRID(*player_Guess_Grid);
				}
				(*player_Fleet_Status) = getPLAYER_FLEET_STATUS(player, enemy_Grid, player_Guess_Grid, enemy_Fleet);
				PLAYER_GUESS(player, enemy_Player, player_Guess_Grid, enemy_Grid, guess, previous_Guess, previous_Correct_Guess, is_Hit, player_Fleet_Status, enemy_Fleet);
			}
			else//If the guess is not a hit
			{
				(*player_Guess_Grid).grid[(*guess).point.x][(*guess).point.y].coordinate_Character = miss;//Set the players guess board
				(*enemy_Grid).grid[(*guess).point.x][(*guess).point.y].coordinate_Character = miss;

				if ((*player).automation == human)//If the player is human
				{
					printf("\n%s Miss.\n\n", (*player).player_Name);
					DISPLAY_GRID((*player_Guess_Grid));
				}

				if ((*player).automation == computer)//If the player is the computer
				{
					printf("\n%s Miss.\n\n", (*player).player_Name);
					DISPLAY_GRID(*player_Guess_Grid);
				}

				//printf("\n\nPress ENTER to begin %s's turn.\n", (*enemy_Player).player_Name);
				//_getch();


			}
		}
		else//If the guess has been guessed before
		{
			(*guess).point = getGUESS(player, player_Guess_Grid, is_Hit, previous_Guess, previous_Correct_Guess, direction);
			PLAYER_GUESS(player, enemy_Player, player_Guess_Grid, enemy_Grid, guess, previous_Guess, previous_Correct_Guess, is_Hit, player_Fleet_Status, enemy_Fleet);
		}

	}

}

Coordinate getGUESS(player_Information* player, Grid* player_Guess_Grid, bool is_Hit, Coordinate* previous_Guess, Coordinate* previous_Correct_Guess, int direction)
/*
Gets the player's guess. If the player is an AI, the guess it determined by the difficulty

Recieves: player_Information* player, Grid* player_Guess_Grid, bool is_Hit, Coordinate* previous_Guess, Coordinate* previous_Correct_Guess, int direction
player_Information* player
-	A pointer to the player's information

Grid* player_Guess_Grid
-	A pointer to the player's guess grid

bool is_Hit
-	A boolean value representing whether or not the previous guess was a hit

Coordinate* previous_Guess
-	A pointer to a coordinate value representing the player's previous guess

Coordinate* previous_Correct_Guess
-	A pointer to a coordinate valuye representing the player's original guess

int direction
-	An integer value representing the direction in which the guess should go

Returns: Coordinate guess
Coordinate guess
-	A coorinate value representing the player's next guess
*/
{
	Coordinate  guess;
	bool guess_Check;
	int rand_Check = 0, rand2, rand4;

	rand2 = rand() % 2;
	rand4 = rand() % 4;


	if ((*player).automation == human)//If the player is human
	{
		printf("%s Admiral, Enter attack coordinates: ", (*player).player_Name);
		scanf_s("%d %d", &guess.x, &guess.y);
		guess_Check = GUESS_CHECK(player, player_Guess_Grid, guess);
		if (guess_Check == false)//If the guess check fails
		{
			printf("Invalid guess. Try again.\n");
			guess = getGUESS(player, player_Guess_Grid, is_Hit, previous_Guess, previous_Correct_Guess, direction);
		}
	}
	else if ((*player).automation == computer && (*player).difficulty == 1)//If the player is a computer and the difficulty is 1
	{

		guess = { rand() % 10, rand() % 10 };
		guess_Check = GUESS_CHECK(player, player_Guess_Grid, guess);

	}
	else if ((*player).automation == computer && (*player).difficulty == 2)//If the player is a computer and the difficulty is 2
	{
		if (is_Hit == true)
		{
			switch (direction)
			{
			case 0:
				guess.x = (*previous_Guess).x;
				guess.y = ((*previous_Guess).y + 1);//Guess one South
				guess_Check = GUESS_CHECK(player, player_Guess_Grid, guess);
				if (!guess_Check)
				{

					guess = getGUESS(player, player_Guess_Grid, is_Hit, previous_Guess, previous_Correct_Guess, direction + 1);
				}
				break;
			case 1:

				guess.x = (*previous_Guess).x;
				guess.y = ((*previous_Guess).y - 1);//Guess one North
				guess_Check = GUESS_CHECK(player, player_Guess_Grid, guess);
				if (!guess_Check)
				{
					guess = getGUESS(player, player_Guess_Grid, is_Hit, previous_Guess, previous_Correct_Guess, direction + 1);
				}
				break;
			default:
				guess = { rand() % 10, rand() % 10 };
				guess_Check = GUESS_CHECK(player, player_Guess_Grid, guess);
				break;
			}
		}
		else
		{
			guess = { rand() % 10, rand() % 10 };
			guess_Check = GUESS_CHECK(player, player_Guess_Grid, guess);
		}
	}
	else//If the player is a computer and the difficulty is 3
	{

		if ((*player_Guess_Grid).grid[(*previous_Correct_Guess).x][(*previous_Correct_Guess).y].coordinate_Character == hit)//If the previous guess was a hit
		{

			if ((*previous_Guess).x == (*previous_Correct_Guess).x && (*previous_Guess).y == (*previous_Correct_Guess).y)//If this is the first correct guess
			{
				switch (direction)
				{
				case 0:
					guess.x = (*previous_Guess).x;
					guess.y = ((*previous_Guess).y - 1);//Guess one North
					guess_Check = GUESS_CHECK(player, player_Guess_Grid, guess);
					if (!guess_Check)
					{

						guess = getGUESS(player, player_Guess_Grid, is_Hit, previous_Guess, previous_Correct_Guess, direction + 1);
					}
					break;
				case 1:
					guess.x = (*previous_Guess).x;
					guess.y = ((*previous_Guess).y + 1);//Guess one South
					guess_Check = GUESS_CHECK(player, player_Guess_Grid, guess);
					if (!guess_Check)
					{

						guess = getGUESS(player, player_Guess_Grid, is_Hit, previous_Guess, previous_Correct_Guess, direction + 1);
					}
					break;
				case 2:
					guess.x = (*previous_Guess).x + 1;
					guess.y = ((*previous_Guess).y);//Guess one East
					guess_Check = GUESS_CHECK(player, player_Guess_Grid, guess);
					if (!guess_Check)
					{

						guess = getGUESS(player, player_Guess_Grid, is_Hit, previous_Guess, previous_Correct_Guess, direction + 1);
					}
					break;
				case 3:
					guess.x = (*previous_Guess).x - 1;
					guess.y = ((*previous_Guess).y);//Guess one West
					guess_Check = GUESS_CHECK(player, player_Guess_Grid, guess);
					if (!guess_Check)
					{

						guess = getGUESS(player, player_Guess_Grid, is_Hit, previous_Guess, previous_Correct_Guess, direction + 1);
					}
				case 4:
					guess = { rand() % 10, rand() % 10 };
					guess_Check = GUESS_CHECK(player, player_Guess_Grid, guess);
					if (!guess_Check)
					{
						guess = getGUESS(player, player_Guess_Grid, is_Hit, previous_Guess, previous_Correct_Guess, direction);
					}
				}
			}
			else//If the guess is not the first guess but is a hit
			{
				if ((*previous_Correct_Guess).x == (*previous_Guess).x - 1)//If the guess was one to the East
				{
					guess.x = (*previous_Correct_Guess).x - 1;
					guess.y = (*previous_Correct_Guess).y;//Guess = the original correct guess, one to the West
					guess_Check = GUESS_CHECK(player, player_Guess_Grid, guess);
					if (!guess_Check)
					{
						(*previous_Guess) = { -10, -10 };
						guess = getGUESS(player, player_Guess_Grid, is_Hit, previous_Guess, previous_Correct_Guess, 4);
					}
				}
				else if ((*previous_Correct_Guess).x == (*previous_Guess).x + 1)//If the guess was one to the West
				{
					guess.x = (*previous_Correct_Guess).x + 1;
					guess.y = (*previous_Correct_Guess).y;//Guess = the original correct guess, one to the East
					guess_Check = GUESS_CHECK(player, player_Guess_Grid, guess);
					if (!guess_Check)
					{
						(*previous_Guess) = { -10, -10 };
						guess = getGUESS(player, player_Guess_Grid, is_Hit, previous_Guess, previous_Correct_Guess, 4);
					}
				}
				else if ((*previous_Correct_Guess).y == (*previous_Guess).y - 1)//If the guess was one to the South
				{
					guess.x = (*previous_Correct_Guess).x;
					guess.y = (*previous_Correct_Guess).y - 1;//Guess = the original correct guess, one to the North
					guess_Check = GUESS_CHECK(player, player_Guess_Grid, guess);
					if (!guess_Check)
					{
						(*previous_Guess) = { -10, -10 };
						guess = getGUESS(player, player_Guess_Grid, is_Hit, previous_Guess, previous_Correct_Guess, 4);
					}
				}
				else if ((*previous_Correct_Guess).y == (*previous_Guess).y + 1)//If the guess was one to the North
				{
					guess.x = (*previous_Correct_Guess).x;
					guess.y = (*previous_Correct_Guess).y + 1;//Guess = the original correct guess, one to the South
					guess_Check = GUESS_CHECK(player, player_Guess_Grid, guess);
					if (!guess_Check)
					{
						(*previous_Guess) = { -10, -10 };
						guess = getGUESS(player, player_Guess_Grid, is_Hit, previous_Guess, previous_Correct_Guess, 4);
					}
				}
				else//If the guess is not a hit
				{
					guess = { rand() % 10, rand() % 10 };
				}
			}

		}
		else//If the guess is not a hit
		{
			guess = { rand() % 10, rand() % 10 };
		}
	}
	return guess;
}

bool BATTLESHIP(void)
/*
Main game function

Recieves: Nothing

Returns: bool play_Again
bool play_Again
-	A boolean value representing whether the user wants to play again or not
*/
{
	bool play_Again = false;
	bool player1_Fleet_Status = false;
	bool player2_Fleet_Status = false;
	Coordinate previous_Guess1, previous_Guess2, previous_Correct_Guess1, previous_Correct_Guess2;
	Grid player1_Grid;
	Grid player1_Guess_Grid;
	Grid player2_Grid;
	Grid player2_Guess_Grid;
	Fleet player1_Fleet;
	Fleet player2_Fleet;
	Tile guess;
	bool is_Hit = false;
	int game_Mode;
	player_Information player[2] = { { PBF_Name, 1,0,1,0,0,0,0,0,false,false,false,false,false },
	{ IRF_Name, 2,0,1,0,0,0,0,0,false,false,false,false,false } };
	previous_Guess1 = { -10, -10 };
	previous_Guess2 = { -10, -10 };
	previous_Correct_Guess1 = { -10, -10 };
	previous_Correct_Guess2 = { -10, -10 };

	DISPLAY_OPENING();
	INITIALIZE_GRID(&player1_Grid);
	INITIALIZE_GRID(&player2_Grid);

	game_Mode = getGAME_MODE();
	system("cls");


	switch (game_Mode)//Determines which players are human/computer.
	{
	case 1:
		player[player1_Val].automation = human;
		player[player2_Val].automation = human;
		break;
	case 2:
		player[player1_Val].automation = human;
		player[player2_Val].automation = computer;
		player[player2_Val].difficulty = getDIFFICULTY(player[player2_Val]);
		break;
	case 3:
		player[player1_Val].automation = computer;
		player[player1_Val].difficulty = getDIFFICULTY(player[player1_Val]);
		player[player2_Val].automation = computer;
		player[player2_Val].difficulty = getDIFFICULTY(player[player2_Val]);
		break;
	default:
		player[player1_Val].automation = human;
		player[player2_Val].automation = human;
	}
	system("cls");
	PLACE_SHIPS(&player[player1_Val], &player1_Grid, &player1_Fleet);
	system("cls");
	PLACE_SHIPS(&player[player2_Val], &player2_Grid, &player2_Fleet);
	system("cls");
	INITIALIZE_GRID(&player1_Guess_Grid);

	INITIALIZE_GRID(&player2_Guess_Grid);

	player1_Fleet_Status = getPLAYER_FLEET_STATUS(&player[player1_Val], &player1_Grid, &player2_Guess_Grid, &player1_Fleet);
	player2_Fleet_Status = getPLAYER_FLEET_STATUS(&player[player2_Val], &player2_Grid, &player1_Guess_Grid, &player2_Fleet);

	while (player1_Fleet_Status == true && player2_Fleet_Status == true)//While each player still has at least one part of a ship left
	{
		system("cls");
		PLAYER_GUESS(&player[player1_Val], &player[player2_Val], &player1_Guess_Grid, &player2_Grid, &guess, &previous_Guess1, &previous_Correct_Guess1, is_Hit, &player2_Fleet_Status, &player2_Fleet);

		if (player2_Fleet_Status == true)//If player 2s fleet is still active
		{
			PLAYER_GUESS(&player[player2_Val], &player[player1_Val], &player2_Guess_Grid, &player1_Grid, &guess, &previous_Guess2, &previous_Correct_Guess2, is_Hit, &player1_Fleet_Status, &player1_Fleet);
	
		}

	}

	if (player1_Fleet_Status == false)
	{
		DISPLAY_VICTORY(&player[player2_Val]);
	}
	else
	{
		DISPLAY_VICTORY(&player[player1_Val]);
	}

	play_Again = getPLAY_AGAIN();
	return play_Again;
}

bool GUESS_CHECK(player_Information* player, Grid* player_Guess_Grid, Coordinate guess)
/*
Checks a guess to make sure that the guess has not already been guessed, or is not out of bounds

Recieves: player_Information* player, Grid* player_Guess_Grid, Coordinate guess
player_Information* player
-	A pointer to the player's information

Grid* player_Guess_Grid
-	A pointer the the player's guess grid

Coordinate guess
-	A coordinate value representing the player's guess

Returns: bool isValid
bool isValid
-	A boolean value which determines if the guess is valid(true) or not(false)
*/
{
	bool isValid = true;

	if ((*player_Guess_Grid).grid[guess.x][guess.y].coordinate_Character != blank)//If the coordiante has already been guessed
	{
		isValid = false;
	}

	if (guess.x > 9 || guess.x < 0 || guess.y > 9 || guess.y < 0)//If the guess is out of bounds
	{
		isValid = false;
	}

	return isValid;
}

void WAITMSG(void)
/*
Prompts the user before the program may continue

Recieves: Nothing

Returns: Nothing
*/
{
	char x;

	printf("Press ENTER to continue.");
	x = getchar();

	return;

}

void DISPLAY_VICTORY(player_Information* victor)
/*
Displays the winning message

Recieves: player_Information victor
player_Information victor
-	The winning player's information

Returns: Nothing
*/
{
	printf("\nCongratulations, Admiral of the %s.\n\n", (*victor).player_Name);
	if ((*victor).automation == human)
	{
		if ((*victor).player_Name == PBF_Name)
		{
			printf("The battle is done, and you, Admiral of the PBF, have achieved victory, and in doing so\n");
			printf("have imortalized the name of the PBF for generations to come. The United States expects no\n");
			printf("less from her Admirals, Non Sibi, Sed Patriae(Not for ourselves, but our country), Admiral.\n");
		}
		else
		{
			printf("Through unparalleled military might and flawless strategy you stand upon the watery grave of the\n");
			printf("once proud PBF. With this victory, the flames of the IRF writhe with burning zeal.\n");
			printf("Your name will be spread to every cornor of the oceans, and all nations will come to fear\n");
			printf("the might of the IRF. Long live the IRF!\n");
		}
	}
}

void DISPLAY_HIT(player_Information* player)
/*
Determines a random message to play when a the player gets a hit

Recieves: player_Information* player
player_Informatin* player
-	A pointer to the player information

Returns: Nothing
*/
{
	int x;

	x = rand() % 4;

	switch (x)
	{
	case 0:
		printf("\nA Direct Hit!\n\n");
		break;
	case 1:
		printf("\nGreat Shot!\n\n");
		break;
	case 2:
		printf("\nHit Her Again!\n\n");
		break;
	case 3:
		printf("\nFINISH THEM!!\n\n");
		break;
	}
}

bool getPLAY_AGAIN(void)
/*
Prompts the user to play again

Recieves: Nothing

Returns bool ;
bool x
-	A boolean value representing whether the player wants to play again(true) or not(false)
*/
{
	bool x;
	char play_Again;
	printf("\nWould you like to play again(Y/N): ");
	scanf_s(" %c", &play_Again);

	switch (play_Again)
	{
	case 'y':
		x = true;
		break;
	case 'Y':
		x = true;
		break;
	case 'n':
		x = false;
		break;
	case 'N':
		x = false;
		break;
	default:
		printf("\nInvalid response.\n");
		x = getPLAY_AGAIN();
		break;
	}

	return x;
}

