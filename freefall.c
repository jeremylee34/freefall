// Author: Jeremy Lee z5316983
// Date completed: 12 July 2020
// UNSW Freefall assignment.

// I used the TODO highlight tool to indicate where the main "functions" are
// and contain functions and their sub-functions within visual sections.

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define SIZE 15
#define EMPTY 0
#define STONE 1
#define MOVEMENT 1 
#define LASER 2
#define SHIFT 3
#define FLIP 4
#define GRAVITY 5
#define NEG_SIZE 150

void printMap(int map[SIZE][SIZE], int playerX);
void printBlocks(int map[SIZE][SIZE], int playerX, int negativeMap[NEG_SIZE][SIZE]);
void move(int map[SIZE][SIZE], int *playerXY);
void shoot(int map[SIZE][SIZE], int playerX, int *gameCon);
void shiftDown(int map[SIZE][SIZE], int playerX, int *gameCon, int *marching, 
               int negativeMap[NEG_SIZE][SIZE], int *negativeRow);
void flip(int map[SIZE][SIZE], int *flipCount, int playerX);
void detonate(int map[SIZE][SIZE], int tntSize, int lowestRow, int playerX);
void gravityStorm(int map[SIZE][SIZE], int playerX, int *moved);
void gravityLeft(int map[SIZE][SIZE]);
void gravityRight(int map[SIZE][SIZE]);
void march(int map[SIZE][SIZE], int marching);
void blockMarch(int map[SIZE][SIZE], int marching, int row);
void detonate2(int map[SIZE][SIZE], int tntSize, int lowestRow, 
               int playerX, int *row);
void regularShift (int map[SIZE][SIZE], int row, int colRight, int *counter);
void shiftNonMarchers(int map[SIZE][SIZE], int row);
void findMarcher(int map[SIZE][SIZE], int *l, int *foundMarchingBlock, int row);

int main (void) {
    // This line creates our 2D array called "map" and sets all
    // of the blocks in the map to EMPTY.
    int map[SIZE][SIZE] = {EMPTY};  
    int negativeMap[NEG_SIZE][SIZE] = {EMPTY};
    int playerX = SIZE / 2;
    printBlocks(map, playerX, negativeMap);
     
    int command;    
    int winCondition = 0;
    int flipped = 0;
    int negativeRow = 1;
    int marching = 1;
    int moved = 0;
    while (winCondition == 0 && scanf("%d", &command) == 1) {
        if (command == MOVEMENT) {
            move(map, &playerX); 
            printMap(map, playerX);
        } else if (command == LASER) {
            shoot(map, playerX, &winCondition);
        } else if (command == SHIFT) { 
            shiftDown(map, playerX, &winCondition, &marching, 
                       negativeMap, &negativeRow);            
        } else if (command == FLIP) {
            flip(map, &flipped, playerX);
        } else if (command == GRAVITY) {
            gravityStorm(map, playerX, &moved);
        }
    }   
    return 0;
}


// TODO PRINT MAP FUNCTION
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



// TODO PRINT BLOCKS
// This function counts, scans and prints lines of stone on the map
void printBlocks(int map[SIZE][SIZE], int playerX, 
                 int negativeMap[NEG_SIZE][SIZE]) { 
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
    
    while (i < lines) { // As long as there are lines to be displayed      
        scanf("%d %d %d %d", &row, &column, &length, &value);        
        if (column + length > SIZE || 
            row > SIZE - 1 ||  
            column < 0) { // If it exceeds max size, skip it
            i++;
        } else if (row < 0) { // If a negative row, put above map
            int j = 0;
            while (j < length) { 
                negativeMap[row * (- 1) ][column++] = value;               
                j++; 
            }
            i++;
        
        } else { //If not, display it
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



// TODO MOVE
// This function allows us to move
void move(int map[SIZE][SIZE], int *playerXY) {    
    int direction;
    scanf("%d", &direction);           
    if ((direction == 1 || direction == -1) && 
        *playerXY < SIZE - 1 && 
        *playerXY > 0) { // If we are still within the endpoints
        *playerXY += direction;         
    } else if (*playerXY == SIZE - 1) { // If we hit right wall
        if (direction == -1) { // Move left
            *playerXY += direction;
        }            
    } else if (*playerXY == 0) { // If we hit left wall            
        if (direction == 1) { // Move right
            *playerXY += direction;
        }            
    }      
}


// TODO SHOOT
// This function allows us to shoot 
void shoot(int map[SIZE][SIZE], int playerX, int *gameCon) {
    // Shooting     
    int lowestRow = SIZE - 1;
    int blocksDestroyed = 0;
    while (lowestRow >= 0 && blocksDestroyed < 3) {
        if (map[lowestRow][playerX] == 1 || map[lowestRow][playerX] == 2) {
            map[lowestRow][playerX] = 0;
            lowestRow--;
            blocksDestroyed++;
        } else if (map[lowestRow][playerX] > 2) { // If it is a TNT block
            int tntSize = 0;
            tntSize = map[lowestRow][playerX];
            detonate(map, tntSize, lowestRow, playerX);        
            blocksDestroyed = 3; // Stop shooting
        
        } else if (map[lowestRow][playerX] == 0) {
            lowestRow--;
        }
    }   
    
    // Scan every line
    int row = SIZE - 1;
    while (row >= 0) {
        int col = 0;
        while (col < SIZE) {
            if (map[row][col] > 0) { // Haven't won yet
                col = SIZE; // End 2nd loop
                row = -1; // End 1st loop                
            } else if (row == 0 && 
                       col == SIZE - 1 && 
                       map[0][SIZE - 1] == 0) { // If the top right box = 0
                *gameCon = 1; 
            }                        
            col++;
        }                
        row--;
    }        
    if (*gameCon == 1) { // Empty map
        printMap(map, playerX);
        printf("Game Won!\n");
    } else { // Still has blocks
        printMap(map, playerX);
    }   
}



// TODO TNT
// TNT detonator
void detonate(int map[SIZE][SIZE], int tntSize, int lowestRow, int playerX) {    
    int row = lowestRow - tntSize + 1;
    while (row <= lowestRow + tntSize - 1) {
        detonate2(map, tntSize, lowestRow, playerX, &row);
    }  
}    

// Split into another function because of overnesting
void detonate2(int map[SIZE][SIZE], int tntSize, int lowestRow, 
               int playerX, int *row) {
    if (*row < SIZE && *row >= 0) {
        int col = 0;
        while (col < SIZE) {
            double distance = sqrt(pow (col - playerX, 2) + 
                                   pow (*row - lowestRow, 2));
            if (distance < tntSize && map[*row][col] > 0) { // check range                  
                map[*row][col] = 0;                    
            }                
            col++;
        }    
        *row = *row + 1;
    } else {
        *row = *row + 1;
    }
}



// TODO SHIFTDOWN
// This function shifts the entire map down 1 
void shiftDown(int map[SIZE][SIZE], int playerX, 
                int *gameCon, int *marching, 
                int negativeMap[NEG_SIZE][SIZE], int *negativeRow) {
      
    int j = 0;
    if (*marching == 5) {    
        *marching = 1;
    }
    while (j < SIZE && *gameCon == 0) { // checking the last line for loss
        if (map[SIZE - 1][j] > 0) {
            printMap(map, playerX);
            printf("Game Lost!\n");
            *gameCon = 1;
        }
        j++;
    }
    if (*gameCon == 0) {   
        march(map, *marching); // Calling the marching command                          
        int i = 0;
        while (i < SIZE) {
            map[0][i] = 0;
            i++;
        }            
        int col = 0;
        while (col < SIZE) { // Printing from hidden array
            map[0][col] = negativeMap[*negativeRow][col];
            col++;
        }       
        *negativeRow = *negativeRow + 1;
        printMap(map, playerX);    
    }
    *marching = *marching + 1; // Move to the next stage in marching cycle
}



// TODO MARCHING
void march(int map[SIZE][SIZE], int marching) {
    int row = 13; // Starting at row SIZE - 1 (array[13][?]) since row 15 clears  
    while (row >= 0) { // Until we reach the first row
        int col = 0; // Scanning across the row
        int foundMarchingBlock = 0;
        while (col < SIZE) { // Until we reach the end of the row                                
            // Scan left to right, move everything else down
            int l = 0;
            while (l < SIZE) { // Finding a marching block
                findMarcher(map, &l, &foundMarchingBlock, row);                          
            }                
            if (foundMarchingBlock == 0) { // Didn't find a marching block
                shiftNonMarchers(map, row); // Shift row down normally                   
                col = SIZE; // End the loop
            }               
            if (foundMarchingBlock == 1) {  // Found a marching block                        
                blockMarch(map, marching, row); // March blocks                   
                col = SIZE; // End the loop            
            } else {
                col++;                    
            }                
        }
        row--;
    }
}

// Determines whether there is a marching block in the row
void findMarcher(int map[SIZE][SIZE], int *l, int *foundMarchingBlock, int row) {
    if (map[row][*l] == 2) {
        *foundMarchingBlock = 1;
        *l = SIZE;
    } else {
        *l = *l + 1;
    }      
}    

// Shifts blocks normally when there are no marching blocks
void shiftNonMarchers(int map[SIZE][SIZE], int row) {
    int col4 = 0;
    while (col4 < SIZE) {
        if (map[row + 1][col4] == 2) {
            col4++;
        } else {
            map[row + 1][col4] = map[row][col4];
            map[row][col4] = EMPTY;
            col4++;
        }
    }
} 

// This function marches blocks based on the march cycle
void blockMarch(int map[SIZE][SIZE], int marching, int row) {
    if (marching % 2 == 1) { 
        int col2 = 0;
        // Scan left to right, move marching blocks down
        while (col2 < SIZE) { 
            map[row + 1][col2] = map[row][col2];
            map[row][col2] = EMPTY;
            col2++;
        }                        
        // Scan right to left, move marching blocks right
    } else if (marching == 2) { 
        int colRight = SIZE - 1;                        
        while (colRight >= 1) {
            int counter = 0;
            if (colRight > 1) {
                // Regular move down
                regularShift(map, row, colRight, &counter);                
            }
            // 0 2| - Do nothing
            if (map[row][SIZE - 1] == 2 &&                                 
                map[row][colRight - 1] != 2) { 
                // 2 2| or 0 2 0| - Replace the -> element   
            } else if (map[row][colRight - 1] == 2) {                              
                map[row][colRight] = map[row][colRight - 1];                               
                map[row][colRight - 1] = EMPTY;                   
                // Regular move down            
            } else if (map[row][colRight - 1] 
                       != 2 || counter != 1) { 
                map[row + 1][colRight - 1] 
                = map[row][colRight - 1];
            }                            
            colRight--;  
        }
        // Scan left to right, move marching blocks left
    } else if (marching == 4) { 
        int col3 = 0;
        while (col3 < SIZE - 1) {                
            // |2 0 - Do nothing        
            if (map[row][0] == 2 && map[row][1] != 2) { 
                // 2 2| or 0 2 0| - Destroy <- element    
            } else if (map[row][col3 + 1] == 2) { 
                map[row][col3] = map[row][col3 + 1];
                map[row + 1][col3 + 1] = EMPTY;
                map[row][col3 + 1] = EMPTY;
                // Regular move down
            } else if (map[row][col3 + 1] != 2 && 
                       map[row][col3 + 2] != 2) { 
                map[row + 1][col3 + 1] = map[row][col3 + 1];
            }                            
            col3++;  
        }
    }                       
    
}

// Shifts the row down regularly since there are no marching blocks
void regularShift (int map[SIZE][SIZE], int row, int colRight, int *counter) {
    if (map[row][colRight - 1] != 2 && 
        map[row][colRight - 2] != 2) { 
        map[row + 1][colRight - 1] 
        = map[row][colRight - 1];
    }
    *counter = 1;
}



// TODO FLIP
// Flips the map upside down, only once
void flip(int map[SIZE][SIZE], int *flipCount, int playerX) {
    if (*flipCount == 0) {
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
        int row2 = SIZE - 1;
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
        *flipCount = 1;
        
    } else if (*flipCount == 1) {
        printMap(map, playerX);
    }
}



// TODO GRAVITY STORM
// This functions causes a gravity storm !!!
void gravityStorm(int map[SIZE][SIZE], int playerX, int *moved) {    
    int direction;
    scanf("%d", &direction);
    int left = 1;
    int topLeft = 2;
    int right = 3;
    int topRight = 4;
    int invalid = 5;
    // Stack all blocks to the left
    if (direction == left) { 
        gravityLeft(map);      
        // Stack all blocks to the top left  
    } else if (direction == topLeft) {
        // Stack all blocks to the right
    } else if (direction == right) { 
        gravityRight(map);
        // Stack all blocks to the top right
    } else if (direction == topRight) {
        // Invalid input
    } else if (direction == invalid)  {
        // Do nothing
    }
    printMap(map, playerX);
}

// Makes all the blocks "fall" to the left
void gravityLeft(int map[SIZE][SIZE]) {
    int row = 0;
    while (row < SIZE) {
        int col = 0;
        int j = 0;
        while (col < SIZE) {
            if (map[row][col] != 0 && j != col) {
                map[row][j] = map[row][col];
                map[row][col] = EMPTY;                    
                j++;
            } else if (j == col && map[row][col] != 0) {
                map[row][j] = map[row][col];
                j++;
            }       
            col++;
        }            
        row++;            
    }
}

// Makes all the blocks "fall" to the right
void gravityRight(int map[SIZE][SIZE]) {
    int row = 0;
    while (row < SIZE) {
        int col = SIZE - 1;
        int j = SIZE - 1;
        while (col >= 0) {            
            if (map[row][col] != 0 && j != col) {
                map[row][j] = map[row][col];
                map[row][col] = EMPTY;                    
                j--;
            } else if (j == col && map[row][col] != 0) {
                map[row][j] = map[row][col];
                j--;
            }       
            col--;
        }            
        row++;            
    }
}


