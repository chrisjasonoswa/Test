#include "connect4.h"
#include <stdio.h>
#include <stdlib.h>
#define GOAL    4


//HELPER FUNCTIONS DECLARATION
int get_row_of_movetest(int board[][COLUMNS], int move_test);
int get_row_of_move(int board[][COLUMNS], int last_move);
int check_winning_horizontal(int board[][COLUMNS], int last_move, int row_move, int player, int goal);
int check_winning_vertical(int board[][COLUMNS], int last_move, int row_move, int player, int goal);
int check_winning_diagonal1(int board[][COLUMNS], int last_move, int row_move, int player, int goal);
int check_winning_diagonal2(int board[][COLUMNS], int last_move, int row_move, int player, int goal);
int is_move_good(int board[][COLUMNS], int player, int opponent, int move);


//MAIN FUNCTIONS
int valid_move(int board[][COLUMNS], int move)
{
    //Check if the topmost cell in the column is already filled
    if(board[0][move] != 0)
        return 0;
    //Move (column) is out of bounds
    if(move < 0 || move >= COLUMNS)
        return 0;
    return 1;
}


int add_move_board(int board[][COLUMNS], int move, int player)
{
    if(!valid_move(board, move))//Invalid move
        return 0;

    //Start from the bottommost part of the column
    int row = 0;
    for(row = ROWS - 1; row >= 0; row--)
    {
        //If an empty cell is encountered, fill it with the player's number
        if(board[row][move] == 0)
        {
            board[row][move] = player;
            break; //Stop checking for other cells
        }
    }
    return 1;
}


int player_move(int board[][COLUMNS])
{
    int move = -1;
    while(!valid_move(board, move))
    {
        printf("Which column do you want to play? ");
        scanf("%d", &move);
        move -= 1; //convert to the 0-6(COLUMNS - 1) numbering
    }
    return move;
}

int computer_move(int board[][COLUMNS], int player, int mode)
{
    int move = -1;
    if(mode == 0) //Task 1
    {
        while(!valid_move(board, move))
            move = rand() % COLUMNS;
        return move;
    }

    else if(mode == 1)
    {
        int move_test;
        int row;
        //1. Move if the computer can win in the next move
        for(move_test = 0; move_test < COLUMNS; move_test++)
        {
            if(valid_move(board, move_test))
            {
                row = get_row_of_movetest(board, move_test);

                //Check if the computer can possibly win
                if( check_winning_horizontal(board, move_test, row, player, GOAL) ||
                    check_winning_vertical(board, move_test, row, player, GOAL)   ||
                    check_winning_diagonal1(board, move_test, row, player, GOAL)  ||
                    check_winning_diagonal2(board, move_test, row, player, GOAL))
                        return move_test;
            }
        }

        //Get the opponent's player number(1 or 2)
        int opponent = 1;
        if(player == 1)
            opponent  = 2;

        //2. Move if the computer can block the player from winning
        for(move_test = 0; move_test < COLUMNS; move_test++)
        {
            if(valid_move(board, move_test))
            {
                row = get_row_of_movetest(board, move_test);
                //Check if the opponent can possibly win
                if( check_winning_horizontal(board, move_test, row, opponent, GOAL) ||
                    check_winning_vertical(board, move_test, row, opponent, GOAL)   ||
                    check_winning_diagonal1(board, move_test, row, opponent, GOAL)  ||
                    check_winning_diagonal2(board, move_test, row, opponent, GOAL))
                        return move_test;
            }
        }

        //3. Make a computer move that makes a 3 connecting cells
        for(move_test = 0; move_test < COLUMNS; move_test++)
        {
            if(valid_move(board, move_test))
            {
                row = get_row_of_movetest(board, move_test);
                if( check_winning_horizontal(board, move_test, row, player, 3) ||
                    check_winning_vertical(board, move_test, row, player, 3)   ||
                    check_winning_diagonal1(board, move_test, row, player, 3)  ||
                    check_winning_diagonal2(board, move_test, row, player, 3))
                {
                    if(is_move_good(board, player, opponent, move_test))
                        return move_test;
                }

            }
        }


        //4. Keeping positions in the middle - good strat
        //Avoid this move if it will make the opponent win
        int is_mid_good = 1;
        if((valid_move(board, COLUMNS/2)))
        {
            if(is_move_good(board, player, opponent, COLUMNS/2))
                return COLUMNS/2;
        }

        //5. Make a random move and check if the move will make the opponent win
        int random_ctr; //The counter of finding a good random move (up to 10 trial and error)
        for(random_ctr = 0; random_ctr < 10; random_ctr++)
        {
            move_test = rand() % COLUMNS;
            if(valid_move(board, move_test))
            {
                if(is_move_good(board, player, opponent, move_test))
                    return move_test;
            }
        }

        //6. Computer plays a random move
        //(Another random generator in case all columns are a bad move)
        while(!valid_move(board, move))
            move = rand() % COLUMNS;
        return move;
    }
}


int check_winning(int board[][COLUMNS], int last_move)
{
    int row_move = get_row_of_move(board, last_move);
    int player = board[row_move][last_move];
    if( check_winning_vertical(board, last_move, row_move, player, GOAL)   ||
        check_winning_horizontal(board, last_move, row_move, player, GOAL) ||
        check_winning_diagonal1(board, last_move, row_move, player, GOAL)  ||
        check_winning_diagonal2(board, last_move, row_move, player, GOAL))
        return player;
    return 0;
}



//HELPER FUNCTIONS
int get_row_of_movetest(int board[][COLUMNS], int move_test)
{
    //Get the row to where the computer's move will be hypothetically placed
    //Start from the bottom
    int row;
    for(row = ROWS - 1; row >= 0; row--)
    {
        if(board[row][move_test] == 0)
            return row;
    }
    return -1; //to indicate that an invalid row is returned
}


int get_row_of_move(int board[][COLUMNS], int last_move)
{
    //Get the row to where the player's move is placed
    //Start from the top
    int row;
    for(row = 0; row < ROWS; row++)
    {
        if(board[row][last_move] != 0)
            return row;
    }
    return -1; //to indicate that an invalid row is returned
}

int check_winning_horizontal(int board[][COLUMNS], int last_move, int row_move, int player, int goal)
{

    int col;
    int total_connects = 1; //Number of matching connects based on the player
    //Check to the left
    for(col = last_move - 1; col >= 0 ; col--)
    {
        if(board[row_move][col] == player)
            total_connects += 1;
        else
            break;
    }

    //Check to the right
    for(col = last_move + 1; col < COLUMNS ; col++)
    {
        if(board[row_move][col] == player)
            total_connects += 1;
        else
            break;
    }
    if(total_connects >= goal)
        return 1;
    return 0;
}

int check_winning_vertical(int board[][COLUMNS], int last_move, int row_move, int player, int goal)
{
    int row;
    //Number of matching connects based on the player - start at 1 to include the last_move
    int total_connects = 1;
    //Check from the next row of the row of the player's move then downwards
    for(row = row_move + 1; row < ROWS; row++)
    {
        if(board[row][last_move] == player)
            total_connects += 1;
        else
            break;
    }
    if(total_connects >= goal)
        return 1;
    return 0;
}

//Diagonal from upperleft to lowerright
int check_winning_diagonal1(int board[][COLUMNS], int last_move, int row_move, int player, int goal)
{
    //Number of matching connects based on the player - start at 1 to include the last_move
    int total_connects = 1;
    int row = row_move - 1;
    int col = last_move - 1;

    while(row >= 0 && col >= 0)
    {
        if(board[row][col] == player)
            total_connects += 1;
        else
            break;
        row--;
        col--;
    }

    row = row_move + 1;
    col = last_move + 1;

    while(row < ROWS && col < COLUMNS)
    {
        if(board[row][col] == player)
            total_connects += 1;
        else
            break;
        row++;
        col++;
    }
    if(total_connects >= goal)
        return 1;
    return 0;
}

//Diagonal from lowerleft to upperright
int check_winning_diagonal2(int board[][COLUMNS], int last_move, int row_move, int player, int goal)
{
    //Number of matching connects based on the player - start at 1 to include the last_move
    int total_connects = 1;
    int row = row_move + 1;
    int col = last_move - 1;

    while(row < ROWS && col >= 0)
    {
        if(board[row][col] == player)
            total_connects += 1;
        else
            break;
        row++;
        col--;
    }

    row = row_move - 1;
    col = last_move + 1;

    while(row >= 0 && col < COLUMNS)
    {
        if(board[row][col] == player)
            total_connects += 1;
        else
            break;
        row--;
        col++;
    }
    if(total_connects >= goal)
        return 1;
    return 0;
}

//Function to check whether the computer's move will not make the opponent win
int is_move_good(int board[][COLUMNS], int player, int opponent, int move)
{
    add_move_board(board, move, player); //Temporarily add the computer's move in the board
    int row_move = get_row_of_move(board, move); //Get the move's row
    int row, move_test;

    for(move_test = 0; move_test < COLUMNS; move_test++)
    {
        if(valid_move(board, move_test))
        {
            row = get_row_of_movetest(board, move_test); //Get row of opponents hypothetical move
            //Check if the opponet can win with the move_test
            if( check_winning_horizontal(board, move_test, row, opponent, 4) ||
                check_winning_vertical(board, move_test, row, opponent, 4)   ||
                check_winning_diagonal1(board, move_test, row, opponent, 4)  ||
                check_winning_diagonal2(board, move_test, row, opponent, 4))
            {
                board[row_move][move] = 0; //Remove computer's move in the board
                return 0;
            }
        }
    }
    board[row_move][move] = 0; //Remove computer's move in the board
    return 1; //move is good
}
