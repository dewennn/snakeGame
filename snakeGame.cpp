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

void menu(){
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
		clearScreen();
	}
}

int main(){
	
	menu();
	
	return 0;
}
