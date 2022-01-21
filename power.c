// Author: Jeremy Lee z5316983
// Date: Yes
// UNSW Freefall assignment.

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define SIZE 15
#define EMPTY 0
#define STONE 1
#define MOVEMENT 1  // lower case is my #defines
#define LASER 2
#define SHIFT 3
#define FLIP 4

// TODO: Add any extra #defines here.

void printMap(int map[SIZE][SIZE], int playerX);
void printBlocks(int map[SIZE][SIZE], int playerX);
void move(int map[SIZE][SIZE], int *playerXY);
void shoot(int map[SIZE][SIZE], int playerX, int *gameCon);
void shift_down(int map[SIZE][SIZE], int playerX, int *gameCon, int *march);
void flip(int map[SIZE][SIZE], int *flip_count, int playerX);
void detonate(int map[SIZE][SIZE], int tnt_size, int lowest_row, int playerX);

// TODO: Add any extra function prototypes here.

int main (void) {
    // This line creates our 2D array called "map" and sets all
    // of the blocks in the map to EMPTY.
    int map[SIZE][SIZE] = {EMPTY};  
    int playerX = SIZE / 2;
    printBlocks(map, playerX);
     
    int march = 1;
    int command;    
    int winCondition = 0;
    int flipped = 0;
    while (winCondition == 0 && scanf("%d", &command) == 1) {
        if (command == MOVEMENT) {
            move(map, &playerX); 
            printMap(map, playerX);
        } else if (command == LASER) {
            shoot(map, playerX, &winCondition);
        } else if (command == SHIFT) {
            shift_down(map, playerX, &winCondition, &march);            
        } else if (command == FLIP) {
            flip(map, &flipped, playerX);
        }
    }   
    return 0;
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
void printBlocks(int map[SIZE][SIZE], int playerX) { 
    // This line creates out playerX variable. The player starts in the
    // middle of the map, at position 7.
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

// This function allows us to move
void move(int map[SIZE][SIZE], int *playerXY) {    
    int direction;
    scanf("%d", &direction);           
    if ((direction == 1 || direction == -1) && 
        *playerXY < SIZE - 1 && 
        *playerXY > 0) { // If we are still within the endpoints
        *playerXY += direction;         
    } else if (*playerXY == SIZE - 1) { // if we hit right wall
        if (direction == -1) { //move left, print
            *playerXY += direction;
        }            
    } else if (*playerXY == 0) { // if we hit left wall            
        if (direction == 1) { //move right, print
            *playerXY += direction;
        }            
    }      
}

// This function allows us to shoot 
void shoot(int map[SIZE][SIZE], int playerX, int *gameCon) {
    // shooting     
    int lowest_row = 14;
    int blocks_destroyed = 0;
    while (lowest_row >= 0 && blocks_destroyed < 3) {
        if (map[lowest_row][playerX] == 1) {
            map[lowest_row][playerX] = 0;
            lowest_row--;
            blocks_destroyed++;
        } else if (map[lowest_row][playerX] > 2) { // if it is a tnt block
            int tnt_size = 0;
            tnt_size = map[lowest_row][playerX];
            detonate(map, tnt_size, lowest_row, playerX);        
            blocks_destroyed = 3; // stop shooting
        
        } else if (map[lowest_row][playerX] == 0) {
            lowest_row--;
        }
    }   
    
    // scan every line
    int row = 14;
    while (row >= 0) {
        int col = 0;
        while (col < SIZE) {
            if (map[row][col] > 0) { // haven't won yet
                col = SIZE; // end 2nd loop
                row = -1; // end 1st loop                
            } else if (row == 0 && 
                       col == SIZE - 1 && 
                       map[0][SIZE - 1] == 0) { // if the top right box = 0
                *gameCon = 1; 
            }                        
            col++;
        }                
        row--;
    }        
    if (*gameCon == 1) { // empty map
        printMap(map, playerX);
        printf("Game Won!\n");
    } else { // still has blocks
        printMap(map, playerX);
    }   
}

// tnt detonator
void detonate(int map[SIZE][SIZE], int tnt_size, int lowest_row, int playerX) {    
    int row = lowest_row - tnt_size + 1;
    while (row <= lowest_row + tnt_size - 1) {
        if (row < SIZE && row >= 0) {
            int col = 0;
            while (col < SIZE) {
                double distance = sqrt(pow(col - playerX, 2) + 
                                       pow(row - lowest_row, 2));
                if (distance < tnt_size && map[row][col] > 0) { // check range                  
                    map[row][col] = 0;                    
                }                
                col++;
            }    
            row++;
        } else {
            row++;
        }
    }  
}    

// This function shifts the entire map down 1 
void shift_down(int map[SIZE][SIZE], int playerX, int *gameCon, int *march) {
    int row = 13; // starting at row 14 (array[13][?]) since row 15 clears    
    int j = 0;
    printf("shiftdown\n");
    
    while (j < SIZE && *gameCon == 0) { // checking the last line for loss
        if (map[14][j] > 0) {
            printMap(map, playerX);
            printf("Game Lost!\n");
            *gameCon = 1;
        }
        j++;
    } // TODO: FIX THIS PART 
    if (*gameCon == 0) {        
        while (row >= 0) { // until we reach the first row
            int col = 0; // scanning across the row
            
            while (col < SIZE) { // until we reach the end of the row
                
                if (*march == 5) {
                    *march = 1;
                }
                if (map[row][col] != 2 && map[row + 1][col] != 2) {
                    map[row + 1][col] = map[row][col];
                    if (row > 0) {
                        if (map[row - 1][col] == 2 && map[row][col] != 2) {
                            map[row][col] = 0;
                        }
                    }
                    col++;                    
                } else if (map[row][col] == 2) { // row 13
                    if (*march % 2 == 1) { // down
                        map[row + 1][col] = map[row][col];
                        map[row][col] = map[row - 1][col];
                        
                    } else if (*march == 2) { // right
                        int col3 = 14;
                        int col2 = 0;
                        while (col3 > 0) {
                            if (col3 == 1) {
                                row--;
                            }
                            col2 = SIZE - col - 1;
                            if (map[row][col3 - 1] == 2) {
                                if (col3 == 14 && map[row][col3 - 1] == 2 && map[row][col3] == 2) {
                                    map[row][col3 - 1] = 0;
                                } else if (col3 == 14 && map[row][col3 - 1] == 2) {
                                    map[row][col3] = map[row][col3 - 1];
                                    if (map[row - 1][col3 - 1] == 2) {
                                        map[row][col3 - 1] = 0;
                                    }
                                
                                } else { 
                                    if (col3 < 14) {
                                        
                                        
                                        map[row][col3] = map[row][col3 - 1];
                                        
                                        if (map[row - 1][col3 - 1] == 2) {
                                            map[row][col3 - 1] = 0;
                                        } else {
                                            map[row][col3 - 1] = map[row - 1][col3 - 1];
                                        }
                                        
                                        
                                    
                                    }
                                    
                                }
                            }
                            col3--;
                        }
                        
                        
                    } else if (*march == 4) { // left
                        
                        map[row][col - 1] = map[row][col];
                        
                            map[row][col] = 0;
                        
                    }
                    col++;
                } else {
                    col++;
                }
            }
            row--;
        }    
        int i = 0;
        while (i < SIZE) {
            map[0][i] = 0;
            i++;
        }    
        printMap(map, playerX);    
    }
    
    *march = *march + 1;
    printf("march incremented, %d\n", *march);
}

// Flips the map upside down, only once
void flip(int map[SIZE][SIZE], int *flip_count, int playerX) {
    if (*flip_count == 0) {
        int tempMap[SIZE][SIZE] = {};
        int row = 0;
        while (row < SIZE) {
            int col = 0;
            while (col < SIZE) {
                tempMap[row][col] = map[row][col];
                col++;
            }
            row++;
        }
        int row2 = 14;
        int row3 = 0;
        while (row2 >= 0) {
            int col = 0;
            while (col < SIZE) {
                map[row3][col] = tempMap[row2][col];
                col++;
            }
            row2--;
            row3++;
        }
        printMap(map, playerX);
        *flip_count = 1;
        
    } else if (*flip_count == 1) {
        printMap(map, playerX);
    }

}


