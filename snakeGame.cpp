#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

time_t start;
time_t now;

// Colors
void red(){
  printf("\e[1;31m");
}
void yellow(){
	printf("\e[0;33m");
}
void green(){
  printf("\e[1;32m");
}
void reset(){
  printf("\033[0m");
}

// Terminal Manipulation
char logo[5][100] = {
	"                       __            ",
	"  _____  ____  _____  |  | __  ____  ",
	" |  ___||    ||__   | |  |/  || __ | ",
	" |___ | |  | | _/ _ | |  |  / | ___/_",
	" |____/ |__/_/|_____| |__|___||_____|"
};
char screen[32][120] = {};
int mark;
void printAt(int x, int y, char c){
   printf("\033[%d;%dH%c", x, y, c);
}
void updateScreen(){
	for(int i = 0; i < 32; i++){
		if(i <= 5) red();
		if(i == mark) green();
		if(i == 16) red();
		for(int j = 0; j < 120; j++){
			printAt(i, j, screen[i][j]);
		}
		reset();
	}
}
void fixScreen(){
	reset();
	for(int i = 0; i < 32; i++){
		for(int j = 0; j < 120; j++){
			printAt(i, j, screen[i][j]);
		}
	}
}
void clearScreen(){
	for(int i = 0; i < 32; i++){
		for(int j = 0; j < 120; j++){
			screen[i][j] = ' ';
		}
	}
}
void printLogo(){
	red();
	for(int i = 0; i < 5; i++){
		printf("%s\n", logo[i]);
	}
	reset();	
}
void printAtLogo(){
	for(int i = 0; i < strlen(logo[0]); i++){
		screen[1][i+42] = logo[0][i];
	}
	for(int i = 0; i < strlen(logo[1]); i++){
		screen[2][i+42] = logo[1][i];
	}
	for(int i = 0; i < strlen(logo[2]); i++){
		screen[3][i+42] = logo[2][i];
	}
	for(int i = 0; i < strlen(logo[3]); i++){
		screen[4][i+42] = logo[3][i];
	}
	for(int i = 0; i < strlen(logo[4]); i++){
		screen[5][i+42] = logo[4][i];
	}
}
void enter(){
	printf("Press ");
	green();
	printf("ENTER ");
	reset();
	printf("to continue...\n");
	while(true){
		int inp = _getch();
		if(inp == 0 || inp == 224) inp = _getch();
		if(inp == 13) break;
	}
}

// High Score
struct scores{
	char name[11];
	char pass[11];
	int score;
	scores *next;
	scores *prev;
} *headS = NULL, *tailS = NULL;
scores *newScore(char name[], char pass[], int score){
	scores *temp = (scores*)malloc(sizeof(scores));
	strcpy(temp->name, name);
	strcpy(temp->pass, pass);
	temp->score = score;
	temp->next = temp->prev = NULL;
	
	return temp;
}
void pushScore(scores *newScore){
	if(!headS){
		headS = tailS = newScore;
	}
	else if(headS->score < newScore->score){
		headS->prev = newScore;
		newScore->next = headS;
		headS= newScore;
	}
	else if(tailS->score >= newScore->score){
		tailS->next = newScore;
		newScore->prev = tailS;
		tailS = newScore;
	}
	else{
		scores *curr = headS;
		while(curr && curr->next->score > newScore->score){
			curr = curr->next;
		}
		scores *currNext = curr->next;
		
		curr->next = newScore;
		newScore->prev = curr;
		newScore->next = currNext;
		currNext->prev = newScore;
	}
}
void clearScore(){
	while(headS){
        scores *temp = headS;
        headS = headS->next;
        free(temp);
    }
    tailS = NULL;
}
void displayScore(){
	scores* curr = headS;
	while(curr){
		printf("| %-12s |   %-6d|\n", curr->name, curr->score);
		curr = curr->next;
	}
}

// Data Management
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
	strcpy(temp->password, password);
	temp->highScore = highscore;
	
	temp->height = 1;
	temp->left = temp->right = NULL;
	return temp;
}
int getMax(int a, int b){
	return a > b ? a : b;
}
int getHeight(player* curr){
	if(!curr) return 0;
	else return 1 + getMax(getHeight(curr->left), getHeight(curr->right));
}
int getBalance(player* curr){
	if(!curr) return 0;
	return getHeight(curr->left) - getHeight(curr->right);
}
player* leftRotate(player* curr){
	player* rightChild = curr->right;
	player* rightChildLeft = rightChild->left;
	
	rightChild->left = curr;
	curr->right = rightChildLeft;
	
	rightChild->height = getHeight(rightChild);
	curr->height = getHeight(curr);
	
	return rightChild;
}
player* rightRotate(player* curr){
	player* leftChild = curr->left;
	player* leftChildsright = leftChild->right;
	
	leftChild->right = curr;
	curr->left = leftChildsright;
	
	leftChild->height =getHeight(leftChild);
	curr->height = getHeight(curr);
	
	return leftChild;
}
player* rebalancing(player* root){
	if(!root) return NULL;
	
	root->height = getHeight(root);
	int bal = getBalance(root);
	if(bal > 1 && getBalance(root->left)>= 0){
		return rightRotate(root);
	}
	else if(bal > 1 && getBalance(root->left)< 0){
		root->left = leftRotate(root->left);
		return rightRotate(root);
	}
	else if(bal < -1 && getBalance(root->right) <= 0){
		return leftRotate(root);
	}
	else if(bal < -1 && getBalance(root->right)> 0){
		root->right = rightRotate(root->right);
		return leftRotate(root);
	}
	return root;
}
player* insert(player* root, player* newPlayer){
	if(!root){
		pushScore(newScore(newPlayer->name, newPlayer->password, newPlayer->highScore));
		return newPlayer;
	}
	
	else if(strcmp(root->name, newPlayer->name) > 0){
		root->left = insert(root->left, newPlayer);
	}
	else if(strcmp(root->name, newPlayer->name) < 0){
		root->right = insert(root->right, newPlayer);
	}
	
	return rebalancing(root);
}
player* search(player* curr, char username[]){
	if(!curr) return NULL;
	else if(strcmp(curr->name, username) == 0) return curr;
	else{
		if(strcmp(curr->name, username) > 0){
			return search(curr->left, username);
		}
		else if(strcmp(curr->name, username) < 0){
			return search(curr->right, username);
		}	
	}
}
void updateScore(player *curr){
	if(curr){
		updateScore(curr->left);
		pushScore(newScore(curr->name, curr->password, curr->highScore));
		updateScore(curr->right);
	}
}

// File Handling
void readData(){
	FILE *f = fopen("players.txt", "r");
	while(true){
		char name[11] = {};
		char pass[11] = {};
		int score = 0;
		
		fscanf(f, "%[^,]", &name);
		if(strlen(name) <= 0) break;
		printf("1");
		fgetc(f);
		fscanf(f, "%[^,]", &pass);
		fgetc(f);
		fscanf(f, "%d", &score);
		fgetc(f);
		printf("%s %s %d", name, pass, score);
		root = insert(root, newPlayer(name, pass, score));
	}
	fclose(f);
}
void writeData(){
	FILE *f = fopen("players.txt", "w");
	scores* curr = headS;
	while(curr){
		fprintf(f, "%s,%s,%d\n", curr->name, curr->pass, curr->score);
		curr = curr->next;
	}
	fclose(f);
}

// THE GAME
// X -> Vertical, Y -> Horizontal
bool gameIsRunning = false;
int score = 0;

//// Snake
struct snake{
	int x, y;
	int direction;
	snake *next;
} *head = NULL, *tail = NULL;
void newSnake(){
	snake *temp = (snake*)malloc(sizeof(snake));
	temp->direction = 1;
	temp->x = 16;
	temp->y = 60;
	temp->next = NULL;
	
	head = tail = temp;
}
bool snakeBody(int x, int y){
	snake *curr = head->next;
	while(curr){
		if(curr->x == x && curr->y == y) return true;
		curr = curr->next;
	}
	return false;
}
bool checkDeath(){
	if(head->x <= 8 || head->x >= 25 || head->y <= 42 || head->y >= 79){
		return true;
	}
	if(snakeBody(head->x, head->y)) return true;
	return false;
}
void move(){
	snake* curr = head->next;
	if(curr){
		int tempX = head->x;
		int tempY = head->y;
		int tempDir = head->direction;
		while(curr){
			int tempXX = curr->x;
			int tempYY = curr->y;
			int tempDirr = curr->direction;
			curr->x = tempX;
			curr->y = tempY;
			curr->direction = tempDir;
			tempX = tempXX;
			tempY = tempYY;
			tempDir = tempDirr;
			curr = curr->next;
		}	
	}
	
	if(head->direction == 1){
		head->x--;
	}
	else if(head->direction == 2){
		head->y--;
	}
	else if(head->direction == 3){
		head->y++;
	}
	else if(head->direction == 4){
		head->x++;
	}
	
	gameIsRunning = checkDeath() ? false : true;
}
void snakeGrow(){
	snake *temp = (snake*)malloc(sizeof(snake));
	if(tail->direction == 1){
		temp->x = tail->x + 1;
		temp->y = tail->y;
	}
	else if(tail->direction == 2){
		temp->x = tail->x;
		temp->y = tail->y + 1;
	}
	else if(tail->direction == 3){
		temp->x = tail->x;
		temp->y = tail->y - 1;
	}
	else if(tail->direction == 4){
		temp->x = tail->x - 1;
		temp->y = tail->y;
	}
	temp->direction = tail->direction;
	temp->next = NULL;
	
	tail->next = temp;
	tail = temp;
}

//// Points
int amountOfPoints = 0;
int possiblePoints = 0;

struct point{
	int x, y;
	point *next;
}*headP = NULL, *tailP = NULL;
void newPoint(int x, int y){
	amountOfPoints++;
	point* temp = (point *)malloc(sizeof(point));
	temp->x = x;
	temp->y = y;
	temp->next = NULL;
	
	if(!headP){
		headP = tailP = temp;
	}
	else{
		tailP->next = temp;
		tailP = temp;
	}
}
void delPoint(int x, int y){
	if(!headP->next){
		if(headP->x == x && headP->y == y){
			headP = tailP = NULL;
			amountOfPoints--;
		}
	}
	else{
		if(headP->x == x && headP->y == y){
			point *temp = headP;
			headP = headP->next;
			temp = NULL;
			amountOfPoints--;
		}
		else{
			point* curr = headP;
			while(curr->next->x != x && curr->next->y != y && curr->next){
				curr = curr->next;
			}
			point *temp = curr->next;
			curr->next = temp->next;
			temp = NULL;
			amountOfPoints--;
		}
	}
}
void clearPoint(){
	amountOfPoints = 0;
	if(headP){
		if(headP == tailP){
			headP = tailP = NULL;
		}
		else{
			while(headP){
				point* temp = headP;
				headP = headP->next;	
				temp = NULL;
			}
		}
	}
}

struct possibleCoord{
	int x, y;
	possibleCoord* next;
} *headC = NULL, *tailC = NULL;
void pushCoord(int x, int y){
	possiblePoints++;
	possibleCoord* temp = (possibleCoord*)malloc(sizeof(possibleCoord));
	temp->x = x;
	temp->y = y;
	temp->next = NULL;
	
	if(!headC){
		headC = tailC = temp;
	}
	else{
		tailC->next = temp;
		tailC = temp;
	}
}
void clearCoord(){
	possiblePoints = 0;
	if(headC){
		if(headC == tailC){
			headC = tailC = NULL;
		}
		else{
			while(headC){
				possibleCoord* temp = headC;
				headC = headC->next;	
				temp = NULL;
			}
		}
	}
}
void createPoints(){
	clearCoord();
	for(int i = 8; i < 26; i++){
		for(int j = 42; j < 80; j++){
			if(screen[i][j] == ' ') pushCoord(i, j);
		}
	}
}
void generatePoints(){
	if(amountOfPoints <= 8){
		createPoints();
		int iterator = rand()%possiblePoints + 1;
		int ctr = 1;
		if(headC){
			possibleCoord* curr = headC;
			while(curr->next && ctr <= iterator){
				curr = curr->next;
				ctr++;
			}
			newPoint(curr->x, curr->y);
		}	
	}
}
bool searchPoints(int x, int y){
	if(headP){
		point* curr = headP;
		while(curr){
			if(curr->x == x && curr->y == y) return true;
			curr = curr->next;
		}	
	}
	return false;
}

//// Engine
void updateGame(){
	for(int i = 8; i < 26; i++){
		for(int j = 0; j < 120; j++){
			reset();
			if(searchPoints(i, j)){
				yellow();
			}
			if((head->x == i && head->y == j) || snakeBody(i, j)){
				green();
			}
			else if(i == 8 || j == 42 || i == 25 || j == 79){
				red();
			}
			printAt(i, j, screen[i][j]);
		}
	}
	reset();
	printf("\n");
	printf("\t\t\t\t\t Keep the ");
	green();
	printf("SNAKE");
	reset();
	printf(" alive!         Score: ");
	green();
	printf("%d\n", score);
	reset();
	printf("\t\t\t\t\t Eat the ");
	yellow();
	printf("FOOD");
	reset();
	printf(" to grow!\n");
	red();
	printf("\t\t\t\t\t Don't hit the walls or your own body!!");
	reset();
}
void map(){
	for(int i = 8; i < 26; i++){
		for(int j = 42; j < 80; j++){
			if(searchPoints(i, j)){
				screen[i][j] = 'F';
			}
			if((head->x == i && head->y == j) || snakeBody(i, j)){
				screen[i][j] = 'O';
				if(searchPoints(i, j)){
					delPoint(i, j);
					snakeGrow();
					score++;
				}
			}
			else if(i == 8 || j == 42 || i == 25 || j == 79){
				screen[i][j] = '#';
			}
		}
	}	
}

void* gameEngine(void *arg){
	clearScreen();
	printAtLogo();
	fixScreen();
	map();
	updateGame();
	
	srand(time(NULL));
	start = time(NULL);
	
	while(gameIsRunning){
		now = time(NULL);
		if(difftime(now, start) >= 5){
			start = time(NULL);
			generatePoints();
		}
		sleep(0.5);
		clearScreen();
		printAtLogo();
		move();
		map();
		updateGame();
	}
	
	clearPoint();
	clearScreen();
	fixScreen();
	
	printAt(0, 0, ' ');
	printf("\b");
	printLogo();
	printf("\n\nYour Score: %d\n", score);
	printf("Press any ");
	
	green();
	printf("KEY ");
	reset();
	
	printf(" to continue...");
	return NULL;
}

void game(char name[]){
	score = 0;
	gameIsRunning = true;
	newSnake();
	pthread_t gameEngineThread;
	pthread_create(&gameEngineThread, NULL, gameEngine, NULL);
	
	while(gameIsRunning){
		int inp = _getch();
		if(inp == 0 || inp == 224) inp = _getch();
		if(inp == 72){
			head->direction = 1;
		}
		else if(inp == 75){
			head->direction = 2;
		}
		else if(inp == 77){
			head->direction = 3;
		}
		else if(inp == 80){
			head->direction = 4;
		}	
	}
	
	player *p = search(root, name);
	p->highScore = p->highScore >= score ? p->highScore : score;
	clearScore();
	updateScore(root);
	writeData();
}

// Features
bool checkForEmptySpace(char string[]){
	bool result = false;
	for(int i = 0; i < strlen(string); i++){
		if(string[i] == 32){
			result = true;
			break;
		}
	}
	return result;
}
void clearAlertTab(){
	for(int i = 0; i < 200; i++){
		printAt(31, i+1, ' ');
	}
	printAt(31, 0, ' ');
}
bool checkUsername(char string[], int option){
	red();
	bool stringLength = strlen(string) >= 3 && strlen(string) <= 10;
	bool emptySpace = checkForEmptySpace(string);
	if(!stringLength && emptySpace || strlen(string) == 0){
		clearAlertTab();
		char alert[50] = {};
		
		if(strlen(string) < 3){
			strcpy(alert, "USERNAME IS TOO SHORT AND CONTAIN EMPTY SPACES!");
		}
		else{
			strcpy(alert, "USERNAME IS TOO LONG AND CONTAIN EMPTY SPACES!");
		}
		
		for(int i = 0; i < strlen(alert); i++){
			printAt(31, i+1, alert[i]);
		}
		return false;
	}
	else if(!stringLength && !emptySpace){
		clearAlertTab();
		char alert[50] = {};
		if(strlen(string) < 3){
			strcpy(alert, "USERNAME IS TOO SHORT!");
		}
		else{
			strcpy(alert, "USERNAME IS TOO LONG!");
		}
		
		for(int i = 0; i < strlen(alert); i++){
			printAt(31, i+1, alert[i]);
		}
		return false;
	}
	else if(stringLength && emptySpace){
		clearAlertTab();
		char alert[50] = "USERNAME CONTAIN EMPTY SPACES!";
		for(int i = 0; i < strlen(alert); i++){
			printAt(31, i+1, alert[i]);
		}
		return false;
	}
	else{
		if(option == 1){
			if(search(root, string) == NULL){
				clearAlertTab();
				char alert[50] = "USERNAME DOESN'T EXIST!";
				for(int i = 0; i < strlen(alert); i++){
					printAt(31, i+1, alert[i]);
				}
				return false;
			}
			else{
				green();
				clearAlertTab();
				char alert[50] = "USERNAME HAS BEEN VERIFIED!";
				for(int i = 0; i < strlen(alert); i++){
					printAt(31, i+1, alert[i]);
				}
				return true;
			}
		}
		else if(option == 2){
			if(search(root, string) != NULL){
				clearAlertTab();
				char alert[50] = "USERNAME ALREADY TAKEN!";
				for(int i = 0; i < strlen(alert); i++){
					printAt(31, i+1, alert[i]);
				}
				return false;
			}
			else{
				green();
				clearAlertTab();
				char alert[50] = "USERNAME HAS BEEN VERIFIED!";
				for(int i = 0; i < strlen(alert); i++){
					printAt(31, i+1, alert[i]);
				}
				return true;
			}
		}
	}
}
bool checkPassword(char string[]){
	red();
	bool stringLength = strlen(string) >= 3 && strlen(string) <= 10;
	bool emptySpace = checkForEmptySpace(string);
	if(!stringLength && emptySpace || strlen(string) == 0){
		clearAlertTab();
		
		char alert[50] = {};
		if(strlen(string) < 3){
			strcpy(alert, "PASSWORD IS TOO SHORT AND CONTAIN EMPTY SPACES!");
		}
		else{
			strcpy(alert, "PASSWORD IS TOO LONG AND CONTAIN EMPTY SPACES!");
		}
		
		for(int i = 0; i < strlen(alert); i++){
			printAt(31, i+1, alert[i]);
		}
		return false;
	}
	else if(!stringLength && !emptySpace){
		clearAlertTab();
		
		char alert[50] = {};
		if(strlen(string) < 3){
			strcpy(alert, "PASSWORD IS TOO SHORT!");
		}
		else{
			strcpy(alert, "PASSWORD IS TOO LONG!");
		}
		
		for(int i = 0; i < strlen(alert); i++){
			printAt(31, i+1, alert[i]);
		}
		return false;
	}
	else if(stringLength && emptySpace){
		clearAlertTab();
		char alert[50] = "PASSWORD CONTAIN EMPTY SPACES!";
		for(int i = 0; i < strlen(alert); i++){
			printAt(31, i+1, alert[i]);
		}
		return false;
	}
	else{
		return true;
	}
}
bool checkPasswordLogin(char name[], char pass[]){
	player* temp = search(root, name);
	if(temp != NULL && strcmp(temp->password, pass) == 0) return true;
	else{
		red();
		clearAlertTab();
		char alert[50] = "WRONG PASSWORD!";
		for(int i = 0; i < strlen(alert); i++){
			printAt(31, i+1, alert[i]);
		}
		reset();
		return false;
	}
}

void login(){
	printf("\e[?25h");
	printAt(0, 0, ' ');
	system("cls");
	
	char name[100] = {};
	char pass[100] = {};
	
	printLogo();
	printf("\n\nLogin Page | ");
	red();
	printf("[INPUT");
	green();
	printf(" ESC ");
	red();
	printf("TO QUIT]\n");
	reset();
	
	printf("Enter username [NO EMPTY SPACE | USERNAME MUST BE BETWEEN 3 - 10 LETTER (INCLUSIVE) ]: ");
	
	while(true){
		green();
		scanf("%[^\n]", &name);
		getchar();
		if(strcmp(name, "ESC") == 0){
			printf("\e[?25l");
			return;
		}
		
		if(checkUsername(name, 1)) break;
		for(int i = 0; i < strlen(name); i++){
			printAt(9, strlen("Enter username [NO EMPTY SPACE | USERNAME MUST BE BETWEEN 3 - 10 LETTER (INCLUSIVE) ]: ") + 1 + i, ' ');	
		}
		printAt(9, strlen("Enter username [NO EMPTY SPACE | USERNAME MUST BE BETWEEN 3 - 10 LETTER (INCLUSIVE) ]: "), ' ');
		green();
	}
	
	reset();
	printAt(10, 0, ' ');
	printf("\b");
	printf("Enter password [NO EMPTY SPACE | USERNAME MUST BE BETWEEN 3 - 10 LETTER (INCLUSIVE) ]: ");
	
	while(true){
		green();
		scanf("%[^\n]", &pass);
		getchar();
		if(strcmp(pass, "ESC") == 0){
			printf("\e[?25l");
			return;
		}
		
		if(checkPasswordLogin(name, pass)) break;
		for(int i = 0; i < strlen(pass); i++){
			printAt(10, strlen("Enter password [NO EMPTY SPACE | USERNAME MUST BE BETWEEN 3 - 10 LETTER (INCLUSIVE) ]: ") + 1 + i, ' ');	
		}
		printAt(10, strlen("Enter password [NO EMPTY SPACE | USERNAME MUST BE BETWEEN 3 - 10 LETTER (INCLUSIVE) ]: "), ' ');
		green();
	}
	
	printf("\e[?25l");
	green();
	clearAlertTab();
	char alert1[100] = "USERNAME AND PASSWORD HAVE BEEN VERIFIED! YOU ARE LOGGED IN!";
	for(int i = 0; i < strlen(alert1); i++){
		printAt(29, i+1, alert1[i]);
	}
	char alert2[50] = "Press ENTER to start SNAKE";
	for(int i = 0; i < strlen(alert2); i++){
		printAt(31, i+1, alert2[i]);
	}
	while(true){
		int inp = _getch();
		if(inp == 0 || inp == 224) inp = _getch();
		if(inp == 13) break;
	}
	
	game(name);
}
void regis(){
	printf("\e[?25h");
	printAt(0, 0, ' ');
	system("cls");
	
	char name[100] = {};
	char pass[100] = {};
	
	printLogo();
	printf("\n\nRegister Page | ");
	red();
	printf("[INPUT");
	green();
	printf(" ESC ");
	red();
	printf("TO QUIT]\n");
	reset();
	
	printf("Enter username [NO EMPTY SPACE | USERNAME MUST BE BETWEEN 3 - 10 LETTER (INCLUSIVE) ]: ");
	
	while(true){
		green();
		scanf("%[^\n]", &name);
		getchar();
		if(strcmp(name, "ESC") == 0){
			printf("\e[?25l");
			return;
		}
		
		if(checkUsername(name, 2)) break;
		for(int i = 0; i < strlen(name); i++){
			printAt(9, strlen("Enter username [NO EMPTY SPACE | USERNAME MUST BE BETWEEN 3 - 10 LETTER (INCLUSIVE) ]: ") + 1 + i, ' ');	
		}
		printAt(9, strlen("Enter username [NO EMPTY SPACE | USERNAME MUST BE BETWEEN 3 - 10 LETTER (INCLUSIVE) ]: "), ' ');
		green();
	}
	
	reset();
	printAt(10, 0, ' ');
	printf("\b");
	printf("Enter password [NO EMPTY SPACE | USERNAME MUST BE BETWEEN 3 - 10 LETTER (INCLUSIVE) ]: ");
	
	while(true){
		green();
		scanf("%[^\n]", &pass);
		getchar();
		if(strcmp(pass, "ESC") == 0){
			printf("\e[?25l");
			return;
		}
		
		if(checkPassword(pass)) break;
		for(int i = 0; i < strlen(pass); i++){
			printAt(10, strlen("Enter password [NO EMPTY SPACE | USERNAME MUST BE BETWEEN 3 - 10 LETTER (INCLUSIVE) ]: ") + 1 + i, ' ');	
		}
		printAt(10, strlen("Enter password [NO EMPTY SPACE | USERNAME MUST BE BETWEEN 3 - 10 LETTER (INCLUSIVE) ]: "), ' ');
		green();
	}
	printf("\e[?25l");
	root = insert(root, newPlayer(name, pass, 0));
	writeData();
	green();
	clearAlertTab();
	char alert1[100] = "USERNAME AND PASSWORD HAVE BEEN VERIFIED! YOUR ACCOUNT HAS BEEN CREATED!";
	for(int i = 0; i < strlen(alert1); i++){
		printAt(29, i+1, alert1[i]);
	}
	char alert2[50] = "Press ENTER to go back to MAIN MENU";
	for(int i = 0; i < strlen(alert2); i++){
		printAt(31, i+1, alert2[i]);
	}
	while(true){
		int inp = _getch();
		if(inp == 0 || inp == 224) inp = _getch();
		if(inp == 13) break;
	}
}
void highScore(){
	printAt(0, 0, ' ');
	system("cls");
	printLogo();
	
	printf("\n\n");
	
	if(!headS){
		yellow();
		printf("NO DATA\n");
		reset();
		printf("\n\n");
		enter();
		return;
	}
	
	yellow();
	printf("SCOREBOARD\n");
	printf("*------------------------*\n");
	printf("|    %-10s|  %s  |\n", "Name", "Score");
	printf("*------------------------*\n");
	displayScore();
	printf("*------------------------*\n");
	reset();
	printf("\n\n");
	enter();
}
void howToPlay(){
	printAt(0, 0, ' ');
	system("cls");
	
	printLogo();
	
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

// Main Menu
void mainMenu(){
	int choice = 1;
	while(true){
		clearScreen();
		
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
	readData();
	printf("\e[?25l");
	mainMenu();
	printf("\e[?25h");
	printAt(0, 0, ' ');
	system("cls");
	
	red();
	printf("  _______ _                 _                        \n");
	printf(" |__   __| |               | |                       \n");
	printf("    | |  | |__   __ _ _ __ | | __  _   _  ___  _   _ \n");
	printf("    | |  | '_ \\ / _` | '_ \\| |/ / | | | |/ _ \\| | | |\n");
	printf("    | |  | | | | (_| | | | |   <  | |_| | (_) | |_| |\n");
	printf("    |_|  |_| |_|\\__,_|_| |_|_|\\_\\  \\__, |\\___/ \\__,_|\n");
	printf("                                    __/ |            \n");
	printf("                                   |___/             \n");
	reset();
	printf("    Made By Derren Malaka :)\n");
	
	return 0;
}
