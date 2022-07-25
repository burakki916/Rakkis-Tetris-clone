#include <iostream>
#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>  
void randomPermutation(){
    int order[7] = {0,1,2,3,4,5,6};
    
    for(int i =0; i <10; i++){
        int rand1 = rand() % 7;
        int rand2 = rand() % 7; 
        while(rand1==rand2){
            rand2 = rand() % 7;
        }
        std::swap(order[rand1],order[rand2]);
    }
    for(int i = 0; i<7; i++){
        std::cout << order[i] <<","; 
    }
    std::cout << "\n"; 
}
int main(){
    int mod =0; 
    int newMatrix[4][4] ={
            {0, 0, 0, 0},
            {0, 0, 0, 0},
            {0, 0, 0, 0},
            {0, 0, 0, 0},
        };
    int oldMatrix[4][4] =         
        {
            {1, 2, 3, 4},
            {5, 6, 7, 8},
            {9, 10, 11, 12},
            {13, 14, 15, 16},
        };

    for(int y = 0; y<4; y++){
        for(int x = 0; x<4; x++){
            newMatrix[y][x] = oldMatrix[x][(3+mod)-y];
        }
    }
    for(int y = 0; y<4; y++){
        for(int x = 0; x<4; x++){
            std::cout << newMatrix[y][x] << " ";
        }
        std::cout << "\n";
    } 
    
}