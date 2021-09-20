/********************************************************************************************
 * castle_mine.c: program that allows player to do battle and other cool things (like swords)
 * By Eli and Max
 ********************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>

typedef enum {earth, water, fire, wind, metal} magic;

#define ABIL 7
#define LOWER 3
#define UPPER 18
#define NAME 32
#define AGE 1024
#define ROOM_SIZE 3600
#define DUNGN_SIZE 120000

/* Global variables */
enum c_ability_enum{strength, intelligence, wisdom, dexterity, constitution, psychic, magical};
char * c_ability[] = {"strength", "intelligence", "wisdom", "dexterity", "constitution", "psychic ability", "magical ability"};

/* main room object */
struct d_room {
	int r_id;	/* room number */
	int r_size; 	/* room size in square feet */
	char r_name[NAME];
	struct d_room * d_room_ptr_prev;
	struct d_room * d_room_ptr_next;
};

/* main dungeon object, uses doubly-linked list */
struct dungeon {
	int d_id;
	char d_name[NAME];
	struct d_room * room_strcptr;
	struct dungeon * d_strcptr_prev;
	struct dungeon * d_strcpt_next;
};

/* struct pointer declarations */
/* this link always points to the first link */
struct d_room *head_room = NULL;

/* this link always points to the last link */
struct d_room *last_room = NULL;

struct d_room *curr_room = NULL;


/* Prototypes */
int print_random(int lower, int upper);
void get_player_attributes(int i, int n, int lower, int upper);
void get_player_info(void);
int main_menu();
bool create_dungeon();
bool isEmpty();
int length();
void displayForward();
void displayBackward();
void insertFirst(int r_id, int r_size);
void insertLast(int r_id, int r_size);
struct d_room *deleteFirst();
struct d_room *deleteLast();
struct d_room *deleteSpecific(int r_id);
bool insertAfter(int r_id, int new_r_id, int r_size);
bool insertBefore(int r_id, int new_r_id, int r_size);

/******************************************************************
 * TODO:
 * 1.  build links to other rooms in dungeon by adding other d_room_ptrs like d_room_ptr_a OR d_room_ptr_south, etc.
 * 2.  make connections random every time game loads so that dungeon is random number of rooms and connections are random
 * 3.  build map function with each game so that can visualize layout of dungeon (simple at first like a specific char represents a room
 * 4.  flesh out rooms
 * 5.  interconnect dungeons in same way as room struct was built out, even add them as separate files, connect with make operation, h files
 * 6.  flesh out characters
 * 7.  design game flow, object of game
 * 8.  make this game different in that it invokes philosophical and/or ethical dilemmas (showing that there is no "right" response)
 * 9.  embellish magical ability, but magic is of different type, like ability to do super-normal human things like vision, equinimity
 *     in face of death, patience, etc.
 * 10. build in machine learning, see if Cython/C + Python is an option
 * 11. continue to explore simple graphics
 * 12. think about challenges characters face and how they can customize their experience
 *
 * ****************************************************************/

int main(void)
{
	int i;
       	int n = 6;
	int lower = 3;
	int upper = 18;

	/* int gd = DETECT, gm;

	initgraph(&gd, &gm, "C:\\TC\\BGI");

	getch();
	closegraph();
	*/

	enum c_ability_enum abil;
	
	srand(time(0));
	
	printf("\tWelcome to Castle Mine!\n");
	printf("\tYou kills monsters and collect coins...\n");
	printf("\n\n");

	get_player_info();
	get_player_attributes(i, n, lower, upper);
	main_menu();
	printf("List length is %d: \n", length());
	printf("\n\n");

	return 0;
}

bool isEmpty(void)
{

	return 1;	/* return True for now */
}

int length()
{
	int length = 0;
	struct d_room *curr_room;

	for (curr_room = head_room; curr_room != NULL; curr_room = curr_room->d_room_ptr_next) {
			length++;
	}

	return length;
}

void displayForward()
{
	/* start from the beginning */
	struct d_room * r_ptr = head_room;

	/* navigate to the end of the list */
	while(r_ptr != NULL) {
		printf("Dungeon room number: %d, Dungeon room size (in square feet): %d\n\n", r_ptr->r_id, r_ptr->r_size);
		r_ptr = r_ptr->d_room_ptr_next;
	}


	return;
}

void insertFirst(int r_id, int r_size)
{
	/* Create a link */
	struct d_room * r_link = (struct d_room *) malloc(sizeof(struct d_room));
       	r_link->r_id = r_id;
	r_link->r_size = r_size;

	if (isEmpty()) {
		/* make it the last link */
		last_room = r_link;
	} else {
		/* update first previous link */
		head_room->d_room_ptr_prev = r_link;
	}

	/* point it to the old first link */
	r_link->d_room_ptr_next = head_room;

	/* point first to new first link */
	head_room = r_link;

	return;
}

void displayBackward()
{
	/* start from last node */
	struct d_room * r_ptr = last_room;

	/* navigate to the start of the list */
	while (r_ptr != NULL) {
		printf("Dungeon room number: %d, Dungeon room size (in square feet): %d\n\n", r_ptr->r_id, r_ptr->r_size);
		r_ptr = r_ptr->d_room_ptr_prev;
	}

	return;
}

void insertLast(int r_id, int r_size)
{
	struct d_room * r_link = (struct d_room *) malloc(sizeof(struct d_room));
	r_link->r_id = r_id;
	r_link->r_size = r_size;

	if (isEmpty()) {
		/* if no rooms (list empty) then make this new node (rooom) the last link */
		last_room = r_link;
	} else {
		/* if list not empty then make link a new last link */
		last_room->d_room_ptr_next = r_link;
	
		/* mark the old last node as previous of new link */
		r_link->d_room_ptr_prev = last_room;
	}

	/* point to the last node */
	last_room = r_link;

	return;
}

/* deletes first (head) node (room) */
struct d_room *deleteFirst()
{
	/* save reference to the first link */
	struct d_room * temp_r_link = head_room;

	/* if only only link (only head) exists */
	if (head_room->d_room_ptr_next == NULL) {
		last_room = NULL;
	} else {
		head_room->d_room_ptr_next->d_room_ptr_prev = NULL;
	}

	head_room = head_room->d_room_ptr_next;

	/* return the deleted link/node/room */
	return temp_r_link;

}
struct d_room *deleteLast()
{
	/* save the reference to the last link */
	struct d_room * temp_r_link = last_room;

	/* if there is only one room (head) in the list */
	if (head_room->d_room_ptr_next == NULL) {
		head_room = NULL;
	} else {
		last_room->d_room_ptr_prev->d_room_ptr_next = NULL;
	}

	last_room = last_room->d_room_ptr_prev;

	/* return the deleted link/node/room */
	return temp_r_link;
}

struct d_room *deleteSpecific(int r_id)
{
	/* start from the first link (room) */
	struct d_room * curr_room = head_room;
	struct d_room * prev_room = NULL;

	/* if the list is empty (no rooms) */
	if (head_room == NULL) {
		return NULL;
	}

	/* navigate through the list */
	while (curr_room->r_id != r_id) {

		/* if at the last node */
		if (curr_room->d_room_ptr_next == NULL) {
			return NULL;
		} else {
			/* store reference to the current link/room */
			prev_room = curr_room;

			/* move to the next link */
			curr_room = curr_room->d_room_ptr_next;
		}
	}

	/* if find a match, update the link */
	if (curr_room == head_room) {
		/* change first to point to the next link */
		head_room = head_room->d_room_ptr_next;
	} else {
		/* bypass the current link/room */
		curr_room->d_room_ptr_prev->d_room_ptr_next = curr_room->d_room_ptr_next;
	}

	if (curr_room == last_room) {
		/* change last to point to prev link */
		last_room = curr_room->d_room_ptr_prev;
	} else {
		curr_room->d_room_ptr_next->d_room_ptr_prev = curr_room->d_room_ptr_prev;
	}

	return curr_room;
}

bool insertAfter(int r_id, int new_r_id, int r_size)
{
	/* start from the first link */
	struct d_room * curr_room = head_room;

	/* check if list is empty, if so no room can be added */
	if (head_room == NULL) {
		return false;
	}

	/* navigate through list */
	while (curr_room->r_id != r_id) {
		
		/* if this is the last node (room) */ 
		if (curr_room->d_room_ptr_next == NULL) {
			return false;
		} else {
			/* if not at the end of the list, move to the next link (room) */
			curr_room = curr_room->d_room_ptr_next;
		}
	}

	/* create a link */
	struct d_room * new_r_link = (struct d_room *) malloc(sizeof(struct d_room));
	new_r_link->r_id = new_r_id;
	new_r_link->r_size = r_size;

	if (curr_room == last_room) {
		new_r_link->d_room_ptr_next = NULL;
		last_room = new_r_link;
	} else {
		new_r_link->d_room_ptr_next = curr_room->d_room_ptr_next;
		curr_room->d_room_ptr_next->d_room_ptr_prev = new_r_link;
	}

	new_r_link->d_room_ptr_prev = curr_room;
	curr_room->d_room_ptr_next = new_r_link;

	return true;
}

bool insertBefore(int r_id, int new_r_id, int r_size)
{
	/* start from the first link */
	struct d_room * curr_room = last_room;

	/* check if list is empty, if so no room can be added */
	if (head_room == NULL) {
		return false;
	}

	/* navigate through list */
	while (curr_room->r_id != r_id) {
		
		/* if this is the last node (room) */ 
		if (curr_room->d_room_ptr_prev == NULL) {
			return false;
		} else {
			/* if not at the end of the list, move to the next link (room) */
			curr_room = curr_room->d_room_ptr_prev;
		}
	}

	/* create a link */
	struct d_room * new_r_link = (struct d_room *) malloc(sizeof(struct d_room));
	new_r_link->r_id = new_r_id;
	new_r_link->r_size = r_size;

	if (curr_room == head_room) {
		new_r_link->d_room_ptr_prev = NULL;
		head_room = new_r_link;
	} else {
		new_r_link->d_room_ptr_prev = curr_room->d_room_ptr_prev;
		curr_room->d_room_ptr_prev->d_room_ptr_next = new_r_link;
	}

	new_r_link->d_room_ptr_next = curr_room;
	curr_room->d_room_ptr_prev = new_r_link;

	return true;	
}

void get_player_attributes(int i, int n, int lower, int upper)
{
	int a = 0;
	/* Print 10 random numbers from 0 to 100 */
	for (i = 0; i <= n; i++) {
                printf("\tYour %s is (3 - 18 scale): %d\n", c_ability[i], print_random(lower, upper)); /* rand() % 18); */
        }
	printf("\n\n");

	return;
}

int print_random(int lower, int upper)
{
	time_t t;
	int num = 0;

	/* Initializes random number generator */
	return num = (int) ((rand() % (upper - lower + 1)) + lower);
	/* return num = (int) ((srand((unsigned) time(&t)) % (upper - lower + 1)) + lower); */
}

void get_player_info(void)
{
	char name[NAME] = "";
	char age[AGE] = "";
	int num_age = 0;
	int lower = 8;
	int upper = 256;


	printf("\tPlease enter your player name: ");

	fgets(name, sizeof(name), stdin);
	//scanf("%s", name);

	printf("\tYou chose the name: %s\n", name);
	//fflush(stdin);

	//	setbuf(stdin, NULL);
	printf("\tNow enter your age or just press enter to have an age chosen for you: ");
	fgets(age, sizeof(age), stdin);
	if (age[0] == '\n') {
		num_age = print_random(8, 256); // (int) ((rand() % (upper - lower + 1)) + lower);
		printf("Your age is: %d\n", num_age);
	}
	else {
		num_age = atoi(age);
		printf("\tYour age is %d\n", num_age);
	}
	printf("\n\n");

	return;
}

int main_menu(void)
{	
	char choice;

	printf("Please choose which adventure on which you'd like to embark: \n");
	printf("Type the letter corresponding to each choice:\n");
	printf("\tM: 	Mountains\n\n");
	printf("\tS: 	Sea\n\n");
	printf("\tP: 	Plains\n\n");
	printf("\tF: 	Forest\n\n");
	printf("\tB: 	Bog\n\n");
	printf("\tD:	Dungeon\n\n");
	printf("\tO: 	Other\n\n");

	scanf("%c", &choice);

	switch(choice) {
	case 'M':
		printf("Off to the mountains!\n");
		break;
	case 'S':
		printf("Beyond is the sea...\n");
		break;
	case 'P':
		printf("The desolate plains await\n");
		break;
	case 'F':
		printf("The forest beckons...\n");
		break;
	case 'B':
		printf("You've chosen the swamps!\n");
		break;
	case 'D':
		printf("Be prepared to enter the dungeon...\n");
		create_dungeon();
		break;
	case 'O':
		printf("We will find a suitable place for you...\n");
		break;
	default:
		printf("I couldn't find your choice, please try again!\n");
	}
		
	printf("\n\n");
	return 0;
}

bool create_dungeon(void)
{
	insertFirst(1, 2500);
	insertAfter(1, 4, 1500); /* meaning insert new room id 4 after room id 2 */
	insertAfter(4, 7, 3500);
	insertAfter(7, 9, 1500);
	insertAfter(9, 13, 2500);
	insertBefore(13, 11, 5000);
	insertBefore(7, 6, 7500);
	
	printf("\n\tHere is the list of rooms in the dungeon (from first to last): \n");
	displayForward();

	printf("\n\tHere is the list of rooms in the dungeon (from last to first): \n");
	displayBackward();

	return true;
}

