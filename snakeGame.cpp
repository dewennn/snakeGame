#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>

char logo[5][100] = {
	"                       __              ",
	"  _____  ____  _____  |  | __  ____    ",
	" |  ___||    ||__   | |  |/  || __ |   ",
	" |___ | |  | | _/ _ | |  |  / | ___/_  ",
	" |____/ |__/_/|_____| |__|___||______| "
};
char screen[32][120] = {};
int mark;
void red () {
  printf("\033[1;31m");
}
void reset () {
  printf("\033[0m");
}
void printAt(int x, int y, char c){
   printf("\033[%d;%dH%c", x, y, c);
}
void updateScreen(){
	for(int i = 0; i < 32; i++){
		if(i <= 5) red();
		if(i == mark) red();
		if(i == 16) red();
		for(int j = 0; j < 120; j++){
			printAt(i, j, screen[i][j]);
		}
		reset();
	}
}
void clear(){
	for(int i = 0; i < 32; i++){
		for(int j = 0; j < 120; j++){
			screen[i][j] = ' ';
		}
	}
}
void enter(){
	printf("Press ");
	red();
	printf("ENTER ");
	reset();
	printf("to continue...\n");
	while(true){
		int inp = _getch();
		if(inp == 0 || inp == 224) inp = _getch();
		if(inp == 13) break;
	}
}
void displayLogo(){
	red();
	for(int i = 0; i < 5; i++){
		printf("%s\n", logo[i]);
	}
	reset();	
}

struct player{
	char name[11];
	char password[11];
	int highScore;
	int height;
	player *left;
	player *right;
} *root = NULL;
player* newPlayer(char name[], char password[], int highscore){
	player *temp = (player *)malloc(sizeof(player));
	strcpy(temp->name, name);
	strcpy(temp->name, password);
	temp->highScore = highscore;
	
	temp->height = 1;
	temp->left = temp->right = NULL;
	return temp;
}

void login(){
	
}
void regis(){
	printAt(0, 0, ' ');
	system("cls");
	
}
void highScore(){
	
}
void howToPlay(){
	printAt(0, 0, ' ');
	system("cls");
	
	displayLogo();
	
	red();
	printf("\n\nWelcome to the Snake Game!\n\n");
	reset();
	
    printf("Instructions:\n");
    printf("1. Use the arrow keys to control the snake.\n");
    printf("2. Eat the food (represented by 'F') to grow the snake.\n");
    printf("3. Avoid hitting the walls or the snake's own body.\n");
    printf("4. The game ends when the snake collides with a wall or itself.\n");
    printf("5. Your score increases for each food item eaten.\n");
    printf("6. Have fun and try to achieve the highest score!\n\n\n");
    
    enter();
}

void mainMenu(){
	int choice = 1;
	while(true){
		clear();
		
		for(int i = 0; i < strlen(logo[0]); i++){
			screen[1][i+1] = logo[0][i];
		}
		for(int i = 0; i < strlen(logo[1]); i++){
			screen[2][i+1] = logo[1][i];
		}
		for(int i = 0; i < strlen(logo[2]); i++){
			screen[3][i+1] = logo[2][i];
		}
		for(int i = 0; i < strlen(logo[3]); i++){
			screen[4][i+1] = logo[3][i];
		}
		for(int i = 0; i < strlen(logo[4]); i++){
			screen[5][i+1] = logo[4][i];
		}
		
		mark = choice+7;
		if(choice == 1){
			char login[20] = "-> Login";
			for(int i = 0; i < strlen(login); i++){
				screen[8][i+1] = login[i];
			}	
		}
		else{
			char login[20] = "- Login";
			for(int i = 0; i < strlen(login); i++){
				screen[8][i+1] = login[i];
			}
		}
		
		if(choice == 2){
			char regis[20] = "-> Register";
			for(int i = 0; i < strlen(regis); i++){
				screen[9][i+1] = regis[i];
			}
		}
		else{
			char regis[20] = "- Register";
			for(int i = 0; i < strlen(regis); i++){
				screen[9][i+1] = regis[i];
			}
		}
		
		if(choice == 3){
			char highScore[20] = "-> High Score";
			for(int i = 0; i < strlen(highScore); i++){
				screen[10][i+1] = highScore[i];
			}
		}
		else{
			char highScore[20] = "- High Score";
			for(int i = 0; i < strlen(highScore); i++){
				screen[10][i+1] = highScore[i];
			}	
		}
		
		if(choice == 4){
			char how[20] = "-> How to Play";
			for(int i = 0; i < strlen(how); i++){
				screen[11][i+1] = how[i];
			}	
		}
		else{
			char how[20] = "- How to Play";
			for(int i = 0; i < strlen(how); i++){
				screen[11][i+1] = how[i];
			}
		}
		
		if(choice == 5){
			char exit[20] = "-> Exit";
			for(int i = 0; i < strlen(exit); i++){
				screen[12][i+1] = exit[i];
			}
		}
		else{
			char exit[20] = "- Exit";
			for(int i = 0; i < strlen(exit); i++){
				screen[12][i+1] = exit[i];
			}
		}
		
		char command[100] = "Press ARROW UP or DOWN to navigate the MENU!!";
		for(int i = 0; i < strlen(command); i++){
			screen[16][i+1] = command[i];
		}
		
		updateScreen();
		
		int inp = _getch();
		if(inp == 0 || inp == 224) inp = _getch();
		if(inp == 72 && choice-1 >= 1) choice--;
		else if(inp == 80 && choice+1 <= 5) choice++;
		if(inp == 13){
			if(choice == 1) login();
			else if(choice == 2) regis();
			else if(choice == 3) highScore();
			else if(choice == 4) howToPlay();
			else if(choice == 5) break;
		}
	}
	
}

int main(){
	printf("\e[?25l");
	mainMenu();
	printf("\e[?25h");
	printAt(0, 0, ' ');
	system("cls");
	printf("THANKS FOR PLAYING :) <3");
	
	return 0;
}
