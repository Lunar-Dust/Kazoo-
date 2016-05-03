#include "enemies.h"
#include "main.h"

#define row 30
#define col 125

MENU *menu;
ITEM **items;
WINDOW *wmenu;
WINDOW *wmain;
WINDOW *info;
WINDOW *user;

player plr ={{"Player"}, {25, 7, 1, 2, 15, 10}, {0, 20, 1, 10}};

const char *choices[][11] = {
		{"Test battle", "Exit"},
		{"Training dummy", "Fodder enemy", "Test boss", "Back"},
		{"Attack", "Spells", "Check enemy", "Items", "RUNNN!!!"},
		{"Firebolt", "Frost", "Bolt", "Quake", "Poison", "Lifesteal", "Heal"},
		{},
		{"   Initiates debug battle", "   Closes the game"},
		{"   Not implemented", "   Could be a bit stronger", "   Not implemented", "   Return to main menu"},
		{"   Basic attack", "   Pew pew magic!", "   Display enemy stats and info", "   View/Use items", "   Run away"},
		{"   An incendiary explosion with a chance to inflict burn", "   An ancient spell from the ice age- chance to inflict freeze", "   A strike of lightning from the sky- chance to inflict paralysis", "   A spell that erupts the ground to pummel the enemy", "   A venomous spell that will probably inflict poison", "   A spell from the vampire underworld that absorbs the enemy's health", "   An angelic spell to restore some of your health"},
		{}
	};
int c_choices[4] = {2, 4, 5, 7};

std::string inventory[][10] = {
		{"Book", "Another Book", "Random Book", "Long Book", "Short Book"},
		{"Amazing Book", "MLG Book", "Fab Book", "Debug Book", "Kappa Book"},
		{"Book Book", "Not Book", "Imaginary Book", "Trippy Book", "THE BOOK"},
		{},
		{},
		{"   A simple book", "   Another simple book", "   You don't know where this came from", "   The weight of this book is astonishing!", "   It's just a piece of paper"},
		{"   You have read this book hundreds of times and still love it", "   A book that is 2 1337 4 u", "   The fab levels of this book are beyond comprehension", "   A book that kills bugs- a programmers dream", "   A book full of the greatest memes in history"},
		{"   A book-ish book", "   What even?", "   The book you wanted to write but couldn't be bothered to", "   Now with 420 pages!", "   The only book"}
};

void clean() {
	if(scrn==true) {
		erase();
	}
	lines = 0;
	ulines = 0;
}

int main() {
	system("printf '\e[8;30;125t'");
	plr.spell[firebolt] = 1;
	plr.spell[lfirebolt] = 1; plr.spell[lfrost] = 1; plr.spell[lbolt] = 1; plr.spell[lquake] = 1;
	plr.spell[lpoison] = 1; plr.spell[llifesteal] = 1; plr.spell[lheal] = 1;
	initscr();
	signal(SIGWINCH, NULL);
	prints("This is the remake of the original Kazoo Quest.");
	prints("This is a alpha build- expect there to be bugs.");
	prints("Press any key to continue.");
	getch();
	mainm();
}

void mainm() {
	pages = 3;
	endwin();
	system("clear");
	text.resize(0);
	text.push_back("Select a thing:");
	usr = cmenu(0, text);
	if(usr == 1) {
		clean();
		battle();
	}
	if(usr == 2) {
		clean();
		delwin(wmenu);
		delwin(user);
		delwin(info);
		endwin();
		exit(EXIT_SUCCESS);

	}
}

void battle() {
	plr.stat[0] = 25;
	text.resize(0);
	text.push_back("Choose enemy:");
	usr = cmenu(1, text);
	if(usr == 1) {
		queue.push_back("No");
		battle();
	}
	if(usr == 2) {
		clean();
		enerand = 1 + rand() % 2;
		if(enerand == 1) {
			now = wolf;
		}
		if(enerand == 2) {
			now = slime;
		}
		queue.push_back(("A " + now.info[1] + now.info[0] + " appears!"));
		maxhp = now.stat[0];
		prompt();
	}
	if(usr == 3) {
		queue.push_back("No");
		battle();
	}
	if(usr == 4) {
		clean();
		mainm();
	}
}

void printu(std::string text) {
	move(ulines+2, (col * 4 / 5) + 1);
	printw(text.c_str());
	++ulines;
	wrefresh(stdscr);
	refresh();
	wborder(user, 1, 1, 1, 1, 0, 0, 0, 0);
	refresh();
	wrefresh(user);
}
void prints(std::string text) {
	mvwprintw(stdscr, lines + 1, 1, text.c_str());
	++lines;
	refresh();
	wrefresh(stdscr);
}
void printi(std::string text, WINDOW *info) {
	mvwprintw(stdscr, lines + 1, 1, text.c_str());
	++lines;
	refresh();
	wrefresh(stdscr);
}

void makeitems(int set) {
	clean();
	n_choices = 5;
	for(i = 0; i < n_choices; ++i)
		items[i] = new_item(inventory[page][i].c_str(), inventory[page+5][i].c_str());
	items[n_choices] = (ITEM *)NULL;
	menu = new_menu((ITEM **)items);
	system("echo yup");
	cmenu(set, text);
}

int cmenu(int set, std::vector<std::string> text) {
	clean();
	highlight = 1;
	ch = 0;

	if(scrn==false) {
		initscr();
		scrn=true;
		cbreak();
		noecho();
		keypad(stdscr, TRUE);
		refresh();
	}

	//WINDOW *wmain = newwin(0, 0, 0, 0);
	WINDOW *wmenu = newwin(7, 100, 11, 0);
	WINDOW *info = newwin(8, 70, 0, 0);
	WINDOW *user = newwin(11, 25, 1, (col * 4 / 5));

	//Isn't inventory call
	if(set != 100) {
		n_choices = c_choices[set];
		items = (ITEM **)calloc(n_choices + 1, sizeof(ITEM *));
		for(i = 0; i < n_choices; ++i)
			items[i] = new_item(choices[set][i], choices[set+5][i]);
		items[n_choices] = (ITEM *)NULL;
		menu = new_menu((ITEM **)items);
	}

	//Is inventory call
	if(set == 100) {
		n_choices = 5;
		items = (ITEM **)calloc(n_choices + 1, sizeof(ITEM *));
		for(i = 0; i < n_choices; ++i)
			items[i] = new_item(inventory[page][i].c_str(), inventory[page+5][i].c_str());
		items[n_choices] = (ITEM *)NULL;
		menu = new_menu((ITEM **)items);
	}

	set_menu_win(menu, wmenu);
	set_menu_sub(menu, derwin(wmenu, 5, 70, 5, 0));
	set_menu_mark(menu, " > ");

	wclear(stdscr);
	wborder(info, 0, 0, 0, 0, 0, 0, 0, 0);
	wborder(user, 0, 0, 0, 0, 0, 0, 0, 0);
	refresh();
	wrefresh(info);
	wrefresh(user);
	for(i=0; i < text.size(); i++) {
		std::string str = text[i];
		prints(str);
	}
	for(i=0; i < queue.size(); i++) {
		std::string str = queue[i];
		prints(str);
	}

	mvprintw(0, col * 4 / 5, "Player stats:");
	printu("HP: " + std::to_string(plr.stat[0]));
	printu("Mana: " + std::to_string(plr.stat[4]));
	printu("Attack: " + std::to_string(plr.stat[1]));
	printu("Intelligence: " + std::to_string(plr.stat[5]));
	printu("Defense: " + std::to_string(plr.stat[2]));
	printu("Agility: " + std::to_string(plr.stat[3]));
	printu("");
	printu("Level: " + std::to_string(plr.xp[2]));
	printu("EXP: " + std::to_string(plr.xp[0]) + "/" + std::to_string(plr.xp[1]));
	refresh();
	post_menu(menu);
	wrefresh(wmenu);
	move(11, 0);
	mvprintw(28, 1, "%d, %d", (highlight + (page*5)), page);
	refresh();

	while((c = getch())){
		switch(c) {
			//If user presses up arrow
			case KEY_UP:
				if(highlight == 1)
					highlight=1;
				else
					--highlight;
				menu_driver(menu, REQ_UP_ITEM);
				mvprintw(28, 1, "%d, %d", (highlight + (page*5)), page);
				refresh();
				break;
			//If user presses down arrow
			case KEY_DOWN:
				if(highlight == n_choices) {
					highlight=n_choices;
				} else {
					++highlight;
				}
				mvprintw(28, 1, "%d, %d", (highlight + (page*5)), page);
				refresh();
				menu_driver(menu, REQ_DOWN_ITEM);
				break;
			case KEY_RIGHT:
				if(set == 100) {
					if(page < pages) {
						++page;
						highlight = (page * 5) + 1;
					} else {
						page = pages;
					}
				makeitems(100);
				}
				break;
			case KEY_LEFT:
				if(set == 100) {
					if(page > 0) {
						--page;
						highlight = (page * 5) + 1;
					} else {
						page = 0;
					}
				makeitems(100);
				}
				break;
			//If user presses enter key
			case 10:
				page = 0;
				ch = highlight;
				if(set==100)
					ch = highlight + (page*5);
				break;
			//If user presses control key
			case BUTTON_CTRL:
				//This is beautiful, works like a charm :)
				while((t = getch())) {
					switch(t) {
						case 'c':
							system("clear");
							free_menu(menu);
							clean();
							delwin(wmenu);
							delwin(wmain);
							endwin();
							exit(EXIT_SUCCESS);
							break;
						default:
							refresh();
							wrefresh(wmenu);
							break;
					}
					wrefresh(wmenu);
				}
				if(t != 0) {
					break;
				}
				break;
			default:
				mvprintw(28, 1, "%d, %d", highlight, page);
				refresh();
				wrefresh(wmenu);
				break;
		}
		wrefresh(wmenu);
	if(ch != 0)
		break;
	}
	unpost_menu(menu);
	for(i=0; i < item_count(menu); i++) {
		free_item(items[i]);
	}
	text.clear();
	text.shrink_to_fit();
	queue.clear();
	queue.shrink_to_fit();
	//free_menu(menu);
	clean();
	//isBattle = false;
	return(ch);
}

void prompt() {
	dam = plr.stat[1];
	agil = plr.stat[3];
	if(plr.stat[0] <= 0) {
		death();
	}
	for(i=0; i < queue.size(); i++) {
		std::string str = queue[i];
		prints(str);
	}
	text.resize(0);
	text.push_back("Enemy HP: " + std::to_string(now.stat[0]));
	plract(cmenu(2, text));
}

void enemyact() {
	int diff = dam * 20 / 100;
	dam = (dam-diff) + rand() % (int)((dam+diff)-(dam-diff)+1);
	agil = plr.stat[3];
	dodge = agil + rand() % (int)(10 - agil + 1);

	if(dodge == 10) {
		queue.push_back("You dodged!");
	} else if(dodge!=10) {
		int diff = dam * 20 / 100;
		dam = (dam-diff) + rand() % (int)((dam+diff)-(dam-diff)+1);
		//Reduce damage by player defense, but keep above 0
		dam = std::max(0, dam-plr.stat[2]);
		plr.stat[0] -= dam;
		queue.push_back("The " + now.info[0] + " dealt " + std::to_string(dam) + " damage to you!");
		if(plr.stat[0] <= 0) {
			death();
		}
	}
	if(plr.stat[0] <= 0) {
		death();
	}
	prompt();
}

void enemydefeat() {
	queue.clear();
	queue.shrink_to_fit();

	queue.push_back(tempstr);
	queue.push_back("You defeated the " + now.info[0] + "!");

	int diff = maxhp * 20 / 100;
	int xp = maxhp + diff;
	queue.push_back("You gained " + std::to_string(xp) + " xp!");

	//Get value of excess xp
	if(xp + plr.xp[0] >= plr.xp[1]) {
		//Set xp equal to gained xp + current xp - needed xp (e.g- at level one, needed xp is 20: have 12xp, gain 12xp, subtract 20 = 4 left over)
		xp += plr.xp[0] - plr.xp[1];

		//Add a level
		plr.xp[2]++;
		queue.push_back("Level up!");

		//Handling decimal values
		double temp = plr.xp[2] / 2;
		plr.xp[1] = 35 * temp;
		plr.xp[0] = xp;

		//Player stat changes will go here


	//Gained xp wasn't enough to level up
	} else if(xp + plr.xp[0] < plr.xp[1]) {
		plr.xp[0] += xp;
	}

	WINDOW *info = newwin(8, 70, 0, 0);
	refresh();
	wborder(info, 0, 0, 0, 0, 0, 0, 0, 0);
	wrefresh(info);
	for(i=0; i < queue.size(); i++) {
		std::string str = queue[i];
		printi(str, info);
	}
	getch();
	queue.clear();
	queue.shrink_to_fit();
	battle();
}

void plract(int usr) {
	if(usr == 1) {
		//Deals damage of random amount either 20% -/+ or equal to damage
		agil = now.stat[3];
		dodge = agil + rand() % (int)(10 - agil + 1);

		//Enemy dodged
		if(dodge == 10) {
			queue.push_back("The " + now.info[0] + " dodged!");

		//Enemy didn't dodge
		} else if(dodge != 10) {
			//Reduce damage by enemy defense, but keep above 0
			dam = std::max(-1, dam-now.stat[2]);
			now.stat[0] -= dam;
			clean();
			if(dam >= 1) {
				tempstr = "You dealt " + std::to_string(dam) + " to the " + now.info[0] + "!";
				queue.push_back("You dealt " + std::to_string(dam) + " damage to the " + now.info[0] + "!");
			} else {
				queue.push_back("Your attack did no damage!");
			}
		}

		if(now.stat[0] < 1) {
			enemydefeat();
		} else {
			enemyact();
		}
	}
	//Spells
	if(usr == 2) {
		text.push_back("Select spell:");
		usr = cmenu(3, text);
		int didSpell = 0;
		if(usr == 1 && plr.spell[0] == 1) {
			//Deals damage of random amount either 20% -/+ or equal to damage
			dam = plr.stat[5];
			int diff = dam * 20 / 100;
			dam = (dam-diff) + rand() % (int)((dam+diff)-(dam-diff)+1);
			now.stat[0] -= dam;
			tempstr = "You dealt " + std::to_string(dam) + " to the " + now.info[0] + "!";
			queue.push_back("You dealt " + std::to_string(dam) + " to the " + now.info[0] + "!");
			didSpell = 1;
		} else if(usr == 1) {
			queue.push_back("You don't have that spell!");
		}

		if(usr == 2 && plr.spell[1] == 1) {
			//Deals damage of random amount either 20% -/+ or equal to damage
			dam = plr.stat[5];
			int diff = dam * 20 / 100;
			dam = (dam-diff) + rand() % (int)((dam+diff)-(dam-diff)+1);
			now.stat[0] -= dam;
			tempstr = "You dealt " + std::to_string(dam) + " to the " + now.info[0] + "!";
			queue.push_back("You dealt " + std::to_string(dam) + " to the " + now.info[0] + "!");
			didSpell = 1;
		} else if(usr == 2)  {
			queue.push_back("You don't have that spell!");
		}

		if(usr == 3 && plr.spell[2] == 1) {
			//Deals damage of random amount either 20% -/+ or equal to damage
			dam = plr.stat[5];
			int diff = dam * 20 / 100;
			dam = (dam-diff) + rand() % (int)((dam+diff)-(dam-diff)+1);
			now.stat[0] -= dam;
			tempstr = "You dealt " + std::to_string(dam) + " to the " + now.info[0] + "!";
			queue.push_back("You dealt " + std::to_string(dam) + " to the " + now.info[0] + "!");
			didSpell = 1;
		} else if(usr == 3) {
			queue.push_back("You don't have that spell!");
		}

		if(usr == 4 && plr.spell[3] == 1) {
			//Deals damage of random amount either 20% -/+ or equal to damage
			dam = plr.stat[5];
			int diff = dam * 20 / 100;
			dam = (dam-diff) + rand() % (int)((dam+diff)-(dam-diff)+1);
			now.stat[0] -= dam;
			tempstr = "You dealt " + std::to_string(dam) + " to the " + now.info[0] + "!";
			queue.push_back("You dealt " + std::to_string(dam) + " to the " + now.info[0] + "!");
			didSpell = 1;
		} else if(usr == 4) {
			queue.push_back("You don't have that spell!");
		}

		if(usr == 5 && plr.spell[4] == 1) {
		} else if(usr == 5) {
			queue.push_back("You don't have that spell!");
		}

		if(usr == 6 && plr.spell[5] == 1) {
		} else if(usr == 6) {
			queue.push_back("You don't have that spell!");
		}

		if(usr == 7 && plr.spell[6] == 1) {
		} else if(usr == 7) {
			queue.push_back("You don't have that spell!");
		}

		if(now.stat[0] < 1) {
			enemydefeat();
		} else {
			if(didSpell == 1)
				enemyact();
			else
				prompt();
		}
	}
	//Check enemy
	if(usr == 3) {
		queue.push_back("Enemy damage: " + std::to_string(now.stat[1]));
		queue.push_back("Enemy defense: " + std::to_string(now.stat[2]));
		queue.push_back("Enemy agility: " + std::to_string(now.stat[3]));
		//queue.push_back("Not yet implemented!");
		prompt();
	}
	//Items
	if(usr == 4) {
		text.resize(0);
		text.push_back("Select item:");
		int usr = cmenu(100, text);

		//queue.push_back("Not yet implemented!");
		prompt();
	}
	//Run!
	if(usr == 5) {
		clean();
		queue.push_back("Not yet implemented!");
		prompt();
	}
}

void death() {
	//print("You have died!");
	for(i=0; i < n_choices + 1; i++) {
		free_item(items[i]);
	}
	free_menu(menu);
	clean();
	delwin(wmenu);
	delwin(user);
	delwin(info);
	endwin();
	exit(EXIT_SUCCESS);
}
