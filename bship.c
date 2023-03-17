#include "bship.h"
#include <stdio.h>
#include <stdlib.h>

//Other helper funtions
void initialize_board(char board[ROWS][COLUMNS]);
void add_ship_onboard(int ship_index, char board[ROWS][COLUMNS], Placement placement, Ship ships[]);
int check_sunk_ship(Ship ships[], int index, char board[ROWS][COLUMNS]);
int shot_fired(Ship ships[], char board[ROWS][COLUMNS], int x, int y);

int main()
{
    //Create Board
    char board[ROWS][COLUMNS];
    initialize_board(board);

    //Array of all possible ships
    Ship ships[FLEETSIZE];

    /**** Building Block 1 - Reading in the ship information ****/
    //Fill the ships array
    intialize_ships(ships);

    /**** Building Block 2 - Getting the ship placements ****/
    Placement placement; //placement info of each ship
    int ship_index;
    //Loop to get ship placements and put it in board
    while((ship_index = get_placement_one_ship (&placement)) != -1)
        add_ship_onboard(ship_index, board, placement, ships);


    Coordinate crdnt;
    while(1)
    {
        /**** Building Block 3 - Getting the coordinates for the shots ****/
        crdnt = ask_shot();
        if(crdnt.x == -1 && crdnt.y == -1)
            break;

        /**** Building Block 4 - Detecting when a ship is hit ****/
        ship_index = shot_fired(ships, board, crdnt.x, crdnt.y);

        /**** Building Block 5 - Detecting when a ship is sunk ****/
        if(check_sunk_ship(ships, ship_index, board) == 0) //Memory is not availble, end the program
            break;
    }
    msg_end();
}

// ********************************************************************
// Checks if a ship has just sunk
// Returns -1, if no ship has sunk
// Returns 1, if a ship has sunk
// Returns 0, if a ship has sunk but not memory is available
// ********************************************************************
int check_sunk_ship(Ship ships[], int index, char board[ROWS][COLUMNS])
{
    if(index == -1) //No ship was hit, no ship has sunk
        return -1;
    int i, j;

    //If the ship symbol is still in the board, ship has not sunk yet
    for(i = 0; i < ROWS; i++)
    {
        for(j = 0; j < COLUMNS; j++)
        {
            if(board[i][j] == (char)(index + '0'))
                return -1;
        }
    }

    //Ship symbol is not found, meaning ship has sunk
    //Print the ship that sunk
    char *mssg = "The ship that was sunk is a ";
    char* final_mssg = (char*)malloc(sizeof(char) * (50));
    if(final_mssg == NULL) //Memory not available
        return 0;

    int f_i = 0;
    //Adds the mssg to the final_mssg
    while(mssg[i] != '\0')
    {
        final_mssg[f_i] = mssg[i];
        i++;
        f_i++;
    }

    i = 0;
    //Adds the ship name to the final_mssg
    while(ships[index].name[i] != '\0')
    {
        final_mssg[f_i] = ships[index].name[i];
        i++;
        f_i++;
    }
    final_mssg[f_i] = '.';
    final_mssg[f_i + 1] = '\0';
    msg_sunk(final_mssg);
    return 1;
}


// ********************************************************************
// Calls the appropriate helper function based on the shot fired
// Puts an X mark to a square based on the x,y coordinate
// Calls mssg_hit if a ship was hit, return the index of the hit ship
// Calls mssg_miss if no ship was hit, return -1
// ********************************************************************
int shot_fired(Ship ships[], char board[ROWS][COLUMNS], int x, int y)
{
    char char_at_shot = board[x][y];
    int index;
    //Miss
    if(char_at_shot == 'X')
    {
        index = -1;
        msg_miss();
    }

    //Ship was hit
    else
    {
        index = char_at_shot - '0';
        msg_hit(ships[index].name);
    }

    //Put the shot on the square
    board[x][y] = 'X';

    return index;
}


// ********************************************************************
// Populates the board array with the ships
// Ships structure are represented by their indexes
// ********************************************************************
void add_ship_onboard(int ship_index, char board[ROWS][COLUMNS], Placement placement, Ship ships[])
{

    char ship_symbol = ship_index + '0'; //converts int to character
    char orientation = placement.orientation;
    int length = ships[ship_index].length;
    int row = placement.coord.x;
    int column = placement.coord.y;
    int ctr = 0;
    //Add the ship to the actual board
    while(ctr < length)
    {
        board[row][column] = ship_symbol;
        if(orientation == 'H')
            column++;
        else
            row--;
        ctr++;
    }
}


// ********************************************************************
// Initializes the board with 'X' characters denoting for empty square
// ********************************************************************
void initialize_board(char board[ROWS][COLUMNS])
{
    int i, j ;
    for(i = 0; i < ROWS; i++)
    {
        for(j = 0; j < COLUMNS; j++)
            board[i][j] = 'X';
    }
}