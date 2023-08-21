#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>

void red () {
  printf("\033[1;31m");
}
void reset () {
  printf("\033[0m");
}
void clearScreen() {
	system("cls");
}
void enter(){
	red();
	printf("Press ENTER to continue...");
	while(true){
		int inp = _getch();
		if(inp == 0 || inp == 224) inp = _getch();
		if(inp == 13) break;
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

player* newPlayer(name, password, highscore){
	player *temp = (player *)malloc(sizeof(player));
	strcpy(temp->name, name);
	strcpy(temp->name, password);
	temp->highScore = highscore;
	
	temp->height = 1;
	temp->left = temp->right = NULL;
	return temp;
}

void mainMenu(){
	int choice = 1;
	
	while(true){
		char logo[7][256] = {
			"     _______..__   __.      ___       __  ___  _______   _______      ___      .___  ___.  _______ ",
			"    /       ||  \\ |  |     /   \\     |  |/  / |   ____| /  _____|    /   \\     |   \\/   | |   ____|",
			"   |   (----`|   \\|  |    /  ^  \\    |  '  /  |  |__   |  |  __     /  ^  \\    |  \\  /  | |  |__   ",
			"    \\   \\    |  . `  |   /  /_\\  \\   |    <   |   __|  |  | |_ |   /  /_\\  \\   |  |\\/|  | |   __|  ",
			".----)   |   |  |\\   |  /  _____  \\  |  .  \\  |  |____ |  |__| |  /  _____  \\  |  |  |  | |  |____ ",
			"|_______/    |__| \\__| /__/     \\__\\ |__|\\__\\ |_______| \\______| /__/     \\__\\ |__|  |__| |_______|"   
		};
		
		red();
		for(int i = 0; i < 7; i++){
			printf("%s\n", logo[i]);
		}
		reset();
		printf("\n\n");
		
		if(choice == 1){
			red();
			printf("-> Login\n");
			reset();
		}
		else printf("- Login\n");
		
		if(choice == 2){
			red();
			printf("-> Register\n");
			reset();
		}
		else printf("- Register\n");
		
		if(choice == 3){
			red();
			printf("-> How to Play\n");
			reset();
		}
		else printf("- How to Play\n");
		
		if(choice == 4){
			red();
			printf("-> HighScore\n");
			reset();
		}
		else printf("- HighScore\n");
		
		if(choice == 5){
			red();
			printf("-> Exit\n");
			reset();
		}
		else printf("- Exit\n");
		
		int inp;
		inp = _getch();
		if(inp == 0 || inp == 224) inp = _getch();
		
		if(inp == 72 && choice - 1 >= 1){
			choice -= 1;
		}
		else if(inp == 80 && choice + 1 <= 5){
			choice += 1;
		}
		else if(inp == 13){
			if(choice == 1){
				
			}
			else if(choice == 2){
				
			}
			else if(choice == 3){
				
			}
			else if(choice == 4){
				
			}
			else if(choice == 5){
				break;
			}
		}
		clearScreen();
	}
}

int main(){
	
	mainMenu();
	clearScreen();
	red();
	printf("Thanks For Playing");
	reset();
	
	return 0;
}
