// Author: 
// Date: 
// UNSW Freefall assignment.

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define SIZE 15
#define EMPTY 0
#define STONE 1

// TODO: Add any extra #defines here.

void printMap(int map[SIZE][SIZE], int playerX);
void printBlocks(int map[SIZE][SIZE]);
void shoot_move(int map[SIZE][SIZE]);
void move(int command, int direction, int map[SIZE][SIZE], int playerX);
void shoot(int command, int map[SIZE][SIZE], int playerX);

// TODO: Add any extra function prototypes here.

int main (void) {
    // This line creates our 2D array called "map" and sets all
    // of the blocks in the map to EMPTY.
    int map[SIZE][SIZE] = {EMPTY};  
    printBlocks(map);
    shoot_move(map); // contains 2 functions to shoot or move
   
}


// Print out the contents of the map array. Then print out the player line
// which will depends on the playerX variable.
void printMap(int map[SIZE][SIZE], int playerX) {    
    // Print values from the map array.
    int i = 0;
    while (i < SIZE) {
        int j = 0;
        while (j < SIZE) {
            printf("%d ", map[i][j]);
            j++;
        }
        printf("\n");
        i++;
    }        
    // Print the player line.
    i = 0;
    while (i < playerX) {
        printf("  ");
        i++;
    }
    printf("P\n");
}

// This function counts, scans and prints lines of stone on the map
void printBlocks(int map[SIZE][SIZE]) { 
    // This line creates out playerX variable. The player starts in the
    // middle of the map, at position 7.
    int playerX = SIZE / 2; 
    int lines;    
    printf("How many lines of stone? ");
    scanf("%d", &lines);
    
    printf("Enter lines of stone:\n");   
    int row;
    int column;
    int length;
    int value;    
    int i = 0;
    
    while (i < lines) { // as long as there are lines to be displayed      
        scanf("%d %d %d %d", &row, &column, &length, &value);        
        if (column + length > SIZE || 
            row > SIZE - 1 || 
            row < 0 || 
            column < 0) { // if it exceeds max size, skip it
            i++;
        } else { //if not, display it
            int j = 0;
            while (j < length) { 
                map[row][column++] = value;               
                j++; 
            }
            i++;
        }
    }
    printMap(map, playerX);
}

// This function contains two functions where we either shoot, or move
void shoot_move(int map[SIZE][SIZE]) {
    int playerX = SIZE / 2; 
    int endLoop = 0;
    while (endLoop == 0) {
        int command = 0;
        int direction = 0;     
          
        move(command, direction, map, playerX);
        shoot(command, map, playerX);    
                      
    }
}

// this function allows us to move
void move(int command, int direction, int map[SIZE][SIZE], int playerX) {

    int end_movement = 0;
    while (scanf("%d", &command) == 1 && 
           scanf(" %d", &direction) == 1 && 
           end_movement == 0 &&
           command == 1) {             
        if (command != 2 && 
            (direction == 1 || direction == -1) && 
            playerX < SIZE - 1 && 
            playerX > 0) { // If we are still within the endpoints
            playerX = playerX + direction;
            printMap(map, playerX);            
        } else if (playerX == SIZE - 1) { // if we hit right wall
            if (direction == 1) { // dont move, print
                printMap(map, playerX);
            } else if (direction == -1) { //move left, print
                playerX = playerX + direction;
                printMap(map, playerX);
            }            
        } else if (playerX == 0) { // if we hit left wall
            if (direction == -1) { //dont move, print
                printMap(map, playerX);
            } else if (direction == 1) { //move right, print
                playerX = playerX + direction;
                printMap(map, playerX);
            }            
        } else if (command != 1 && command != 2) { //invalid input, print
            printMap(map, playerX);
        } else if (command == 2) {          
            end_movement = 1;
        }
    }  
}

// this function allows us to shoot TODO COMPLETE THIS
void shoot(int command, int map[SIZE][SIZE], int playerX) {
    // shooting 
    int end_shooting = 0;
    while (scanf("%d", &command) == 1 && command == 2 && end_shooting == 0) {       
        int Lrow = 14; // count from bottom row to top
        int blocks_destroyed = 0; // count up to 3 destroyed
        int finish_shooting = 0;
        // if we reach the top, finish shooting            
        while (Lrow >= 0 && finish_shooting == 0) { 
            if (map[Lrow][playerX - 1] == 0) { //no block
                Lrow--;
            } else if (map[Lrow][playerX - 1] == 1 && blocks_destroyed < 3) {
                Lrow--; // destroys block
                blocks_destroyed++;
                map[Lrow][playerX - 1] = 0;
            } else if (map[Lrow][playerX - 1] == 1 && blocks_destroyed == 3) { 
                finish_shooting = 1;
                // if we have shot 3 blocks, finish shooting
            } else if (map[Lrow][playerX - 1] == 0 && blocks_destroyed > 0) {
                finish_shooting = 1;
                /* if we have destroyed a block and 
                nothing is above it,    finish shooting */
            }
        }
        
        
        printMap(map, playerX);   
         
        end_shooting = 1; 
    }      

}
