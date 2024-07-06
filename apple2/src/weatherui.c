#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	<conio.h>
#include	"weatherdefs.h"
#include	"openmetro.h"
#include	"hgrtext.h"


extern LOCATION current;

int	err;
//
// handle_err
//
void handle_err(char *message) {
    if (err <  0) {
        printf("err: %s\n", message);
        exit(1);
    } 
}
void disp_menu(char *str) {
	gotoxy(0, 20);
	cprintf(str);
	
}
void clear_display() {
	set_text();
	clrscr();	
}
void change_location(LOCATION *loc) {
	char	input[40];

	set_text();
	clrscr();
	gotoxy(10, 2);
	revers(1);
	cprintf("Change location");
	revers(0);
	gotoxy(2, 10);
	cprintf("Input city name");
	gotoxy(2, 11); 
	cprintf("[ or hit return: current location ]"); 
	gotoxy(0, 13);
	cprintf(">");
	gotoxy(2, 13);
	gets(input);
	if (strlen(input) == 0) {
		*loc = current;
	}
	else {
		if (!om_geocoding(loc, input)) {
		gotoxy(2, 15);
		cprintf("city '%s'was not found", input);
		gotoxy(2, 16);
		cprintf("using current location");
		gotoxy(2, 17);
		cprintf("(please hit return to continue)");
		gets(input);
		*loc = current;
		}
	}
	clrscr();
}

void disp_message(char *msg) {
	clrscr();
	gotoxy(0,10);
	cprintf("%s", msg);
}
char get_keyin() {
	while (!kbhit()) {
		;
	}
	return (cgetc());
}
