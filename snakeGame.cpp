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

void menu(){
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
	printf("1. Login\n");
	printf("2. Register\n");
	printf("3. How to Play\n");
	printf("4. HighScore\n");
	printf("5. Exit\n");
}

int main(){
	
	menu();
	
	return 0;
}
