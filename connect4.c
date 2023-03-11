#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "connect4.h"

#include "connect4.h"
#include <stdio.h>
#include  "display_board_ASCII.c"

//Game over
int is_game_over(int board[][COLUMNS])
{
    int col;
    for(col = 0; col < COLUMNS; col++)
    {
        if(valid_move(board, col))
            return 0;
    }
    return 1;
}

//Start the simulation
int main()
{
    int board[ROWS][COLUMNS] = { {0, 0, 0, 0, 0, 0, 0},
                                 {0, 0, 0, 0, 0, 0, 0},
                                 {0, 0, 0, 0, 0, 0, 0},
                                 {0, 0, 0, 0, 0, 0, 0},
                                 {0, 0, 0, 0, 0, 0, 0},
                                 {0, 0, 0, 0, 0, 0, 0} };
    //Seed the random number generator
    srand(time(NULL));

    //Inform the user
    printf("\nProgram has started ...\n");

    int mode;
    printf("\nEnter a mode: ");
    scanf(" %d", &mode);
    //Display the game board
    display_board_ASCII(board);
    printf("\n");


    int player = 1, comp_player = 2, move;
    int winner = 0;
    char choice;
    printf("Do you want to make the first move(y/n)? ");
    scanf(" %c", &choice);

    if(choice == 'n')
    {
        player = 2;
        comp_player = 1;
    }


    while(!is_game_over(board))
    {
        if(player == 1)
        {
            move = player_move(board);
            add_move_board(board, move, player);
            winner = check_winning(board, move);
            if(winner != 0)
                break;

            move = computer_move(board, comp_player, mode);
            add_move_board(board, move, comp_player);
            printf("The computer played in column %d\n", move + 1);
            winner = check_winning(board, move);
            if(winner != 0)
                break;
        }

        else
        {
            move = computer_move(board, comp_player, mode);
            add_move_board(board, move, comp_player);
            printf("The computer played in column %d\n", move + 1);
            winner = check_winning(board, move);
            if(winner != 0)
                break;

            move = player_move(board);
            add_move_board(board, move, player);
            winner = check_winning(board, move);
            if(winner != 0)
                break;
        }

        //Display the game board
        display_board_ASCII(board);
        printf("\n");

    }
    //Display the game board
    display_board_ASCII(board);
    printf("\n");

    if(winner == 0)
        printf("ITS A TIE !!!\n");
    else if(winner == player)
        printf("Player WON !!!\n");
    else if(winner == comp_player)
        printf("Computer WON !!!\n");
}
