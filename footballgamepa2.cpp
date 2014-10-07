#include <iostream>
#include <ctime>
#include <iomanip>
#include <cstdlib>
#include <fstream>
#include <cstring>
#include <sstream>

using namespace std;

//////////declaration////////////
bool watch = false;
bool playing[8] = {false};
bool* play1;
bool* play2;
int count = 0;
string* home;
string* away;
string input;
double* hometdChance;
double* homefgChance;
double* awaytdChance;
double* awayfgChance; 
double prob;
//////////////////
double entireProb[2][8];
double probbracket1[2][8];
double probbracket2[2][8];
double probbracket3[2][8];
string names[8];
string bracket1[8];
string bracket2[4] = {"GAME 1 W", "GAME 2 W", "GAME 3 W", "GAME 4 W"};
string bracket3[2] = {"GAME 5 W", "GAME 6 w"};
//////////////////
void shuffle(); //shuffle teams
void play(bool* play1, bool* play2, string* home, string* away, double* hometdChance, double* homefgChance, double* awaytdChance, double* awayfgChance); //play function
void stats(); //stats function
bool playwatch(); //watch the games or not
int read(); //read text
int bracketprint(); //print out the bracket
double randomProb(); //generate a random probability
//////////////////
string* brackethome;
string* bracketaway;
int alltd[2][8] = {0};
int allfg[2][8] = {0};
int atttd[2][8] = {0};
int attfg[2][8] = {0};
/////////////////////////////////////////////////////////////////////////////////
int main() 
{
  srand(time(0));
  read();
  shuffle();
  playwatch();
  bracketprint();

  //start playing round 1 of tournament
  playwatch();
  //bracket 1
  for (int i=0; i<4; i++) {
	play1 = &playing[i*2];
	play2 = &playing[(i*2)+1];
	hometdChance = &probbracket1[0][i*2];
	homefgChance = &probbracket1[1][i*2];
	awaytdChance = &probbracket1[0][(i*2)+1];
	awayfgChance = &probbracket1[1][(i*2)+1];
	
	home = &bracket1[i*2];
	away = &bracket1[(i*2)+1];

	play(play1, play2, home, away, hometdChance, homefgChance, awaytdChance, awayfgChance);
  }
  //bracket 2
  for (int j=0; j<4; ) {
	for (int i=0; i<8; i++) {
	  if (playing[i] == false) {
		bracket2[j] = bracket1[i];
		probbracket2[0][j] = probbracket1[0][j];
		probbracket2[1][j] = probbracket1[1][j];
		j++;
	  }
	}
  }
  
  bracketprint();

  //round 2
  for (int i=0; i<4; i++) {
	if (playing[i] == false) {
	  play1 = &playing[i];
	  for (int j=i; j<4; j++) {
		if (playing[j] == false) {
		  play2 = &playing[j];
		}
	  }
	}
  }

  playwatch();

  //next game round
  home = &bracket2[0];
  away = &bracket2[1];
  hometdChance = &probbracket2[0][0];
  homefgChance = &probbracket2[1][0];
  awaytdChance = &probbracket2[0][1];
  awayfgChance = &probbracket2[1][1];
  
  play(play1, play2, home, away, hometdChance, homefgChance, awaytdChance, awayfgChance);

  for (int i=4; i<8; i++) {
	if (playing[i] == false) {
	  play1 = &playing[i];
	  
	  for (int j=i; j<8; j++) {
		if (playing[j] == false) {
		  play2 = &playing[j];
		}
	  }
	}
  }

  home = &bracket2[2];
  away = &bracket2[3];
  hometdChance = &probbracket2[0][2];
  homefgChance = &probbracket2[1][2];
  awaytdChance = &probbracket2[0][3];
  awayfgChance = &probbracket2[1][3];

  play(play1, play2, home, away, hometdChance, homefgChance, awaytdChance, awayfgChance);
  //third bracket
  for (int j=0; j<2; ) {
	for (int i=0; i<8; i++) {
	  if (playing[i] == false) {
		bracket3[j] = bracket1[i];
		probbracket3[0][j] = probbracket1[0][i];
		probbracket3[1][j] = probbracket1[1][i];
		j++;
	  }
	}
  }

  bracketprint();

  //final round
  for (int i=0; i<4; i++) {
	if (playing[i] == false) {
	  play1 = &playing[i];
	
	  for (int j=4; j<8; j++) {
		if (playing[j] == false) {
		  play2 = &playing[j];
		}
	  }
	}
  }

  playwatch(); //watch the game yes/no
  
  home = &bracket3[0];
  away = &bracket3[1];
  hometdChance = &probbracket2[0][0];
  homefgChance = &probbracket2[1][0];
  awaytdChance = &probbracket2[0][1];
  awayfgChance = &probbracket2[1][1];
 
  play(play1, play2, home, away, hometdChance, homefgChance, awaytdChance, awayfgChance);

  //winner
  for (int i=0; i<8; i++) {
	if (playing[i] == false) {
	  cout << bracket1[i] << " win the conference!" << endl << endl;
	}
  }

  stats();

return 0;
} //end of main
//////////////////////////////////////////////////////////////////////////////////
int read() 
{
  int counter = 0;
  double value;
  char buffer [100];
  char *token;
  char filein[265];

  cout << "Welcome to the football bracket game!" << endl;
  cout << "Enter the input file: " << endl;
  cin.get(filein, 265);
  fstream file;
  file.open (filein);

  if (!file) {
	while (!file) {
	    cout << "Please reformat the file and try again. " << endl; 
		cin.clear();
		cin.ignore(10000, '\n');
		cin.get(filein, 265);
		file.open (filein);
	}
  }

  // use a stringstream to extract a double value from the buffer
  stringstream ss;
  // if the file opened correctly, start reading it
  if (file.is_open()) {
	for (int k=0; k<8; k++) {
	  file.getline(buffer, 100); //read lines into the buffer one by one
	  counter =0;
	  token = strtok(buffer, ","); //use strtok to pull out the first comma-delimited string from the buffer

	  // while the token is not null, parse it
	  while (token) {
		if (counter == 0) { // print out the string so we know it's working 
		  names[k] = token;
		}
		
		else if (counter == 1) {
		  ss << token; // convert the string to a double
		  ss >> value;
		  if (value > 1 || value < 0) {
			cout << "Please reformat the file and try again. " << endl;      
			exit(EXIT_FAILURE);             // the file didn't open correctly, so print an error message
			
		   }
		   if (ss.fail()) {
			cout << "Please reformat the file and try again. " << endl;
			exit(EXIT_FAILURE);
  		    }
		    entireProb[0][k] = value;
		    ss.clear();
		}
		
		else if (counter == 2) {
		  ss << token;
		  ss >> value;
		  if (value > 1 || value < 0) {
			cout << "Please reformat the file and try again. " << endl;
			exit(EXIT_FAILURE);
			
		   }
		   if (ss.fail()) {
			cout << "Please reformat the file and try again. " << endl;
			exit(EXIT_FAILURE);
			
  		    }
		    entireProb[1][k] = value;
		    ss.clear();
		 }
		 
		 else if (counter > 2) {
		    cout << "Please reformat the file and try again. " << endl;
		    exit(EXIT_FAILURE);
		    
		 }
	         // read a new comma-delimited string 
		 token = strtok(NULL, ",");
		 counter++;
	}
	if (counter < 3) {
	 	cout << "Please reformat the file and try again. " << endl;	
		exit(EXIT_FAILURE);
	}
     }
  }

  file.close();
  cout << "Here are the brackets: " << endl;
  return 0;
} //end of read
////////////////////////////////////////////////////////////////////////////////
bool playwatch() 
{ //prompts user for input
  cout << "Would you like to watch the games in Round 1? ";
  cin >> input;
  if (input!= "Yes" & input != "No") {
	cout << "Please put in either Yes or No \n";
  cin.clear();
  cin.ignore(10000, '\n');
  cin >> input;
  }
  cout << endl;
  
  if (input == "Yes") {  //whenever input is yes means they want to see the game breakdowns
	watch = true;
  }
  if (input == "No") {
	watch = false;
  }
  return watch;
} //end of watch
////////////////////////////////////////////////////////////////////////////////
void shuffle() 
{ //shuffling of the teams
  double temptouchdown;
  double tempfieldgoal;
  string tempname;
  int random;

  for (int i=0; i<10; i++) {
	for (int j=0; j<8; j++) {

	  tempname = names[j];
	  temptouchdown = entireProb[0][j];
	  tempfieldgoal = entireProb[1][j];

	  random = rand()%8;
	  names[j] = names[random];
	  entireProb[0][j] = entireProb[0][random];
	  entireProb[1][j] = entireProb[1][random];
	  names[random] = tempname;
	  entireProb[0][random] = temptouchdown;
	  entireProb[1][random] = tempfieldgoal;
	}
  }

  for (int i=0; i<8; i++) {
	bracket1[i] = names[i];
	probbracket1[0][i] = entireProb[0][i];
	probbracket2[1][i] = entireProb[1][i];
  }
} //end of shuffle
////////////////////////////////////////////////////////////////////////////////////////////
int bracketprint()
{	  
  	string* home = &bracket1[0];
	string* away = &bracket1[1];
	cout << "---------------" << endl;
	cout << "| " << setw(8) << *home;
	cout << "    |" << endl;
	cout << "| " << setw(11) << left << "  GAME 1";
	cout << " |" << "----|" << endl;
	cout << "| " << setw(8) << right << *away;
	cout << "    |    |    ---------------" << endl;
	home = &bracket2[0];
	away = &bracket2[1];
	cout << "---------------    |    | " << setw(10) << *home;
	cout << "  |" << endl;
	cout << "                   |----| " << setw(11) << left << "   GAME 5";
	cout << " |----|" << endl;
	cout << "---------------    |    | " << setw(10) << right << *away;
	cout << "  |    |" << endl;
	home = &bracket1[2];
	away = &bracket1[3];
	cout << "| " << setw(8) << *home;
	cout << "    |    |    ---------------    |" << endl;
	cout << "| " << setw(11) << left << "  GAME 2";
	cout << " |----|                       |" << endl;
	cout << "| " << setw(8) << right << *away;
	cout << "    |                            |    ---------------"<< endl;
	home = &bracket3[0];
	away = &bracket3[1];
	cout << "---------------                            |    | "<< setw(11) << *home;
	cout << " |" << endl;
	cout << "                                           |----| " << setw(8) << left << "   GAME 7";
	cout << "   |" << endl;
	cout << "---------------                            |    | " << setw(10) << right << *away;
	cout << "  |" << endl;
	home = &bracket1[4];
	away = &bracket1[5];
	cout << "| " << setw(8) << *home;
	cout << "    |                            |    ---------------" << endl;
	cout << "| " << setw(11) << left << "  GAME 3";
	cout << " |----|                       |" << endl;
	cout << "| " << setw(8) << right << *away;
	cout << "    |    |    ---------------    |" << endl;
	home = &bracket2[2];
	away = &bracket2[3];
	cout << "---------------    |    | " << setw(10) << *home;
	cout << "  |    |" << endl;
	cout << "                   |----| " << setw(11) << left << "   GAME 6" << " |~~~~|" << endl;
	cout << "---------------    |    | " << setw(10) << right << *away;
	cout << "  |" << endl;
	home = &bracket1[6];
	away = &bracket1[7];
	cout << "| " << setw(8) << *home;
	cout << "    |    |    ---------------" << endl;
	cout << "| " << setw(11) << left << "  GAME 4";
	cout << " |----|" << endl;
	cout << "| " << setw(8) << right << *away;
	cout << "    |" << endl;
	cout << "---------------" << endl << endl;
} //end bracketprint
/////////////////////////////////////////////////////////////////////////////////////////////////////
void play(bool* play1, bool* play2, string* home, string* away, double* hometdChance, double* homefgChance, double* awaytdChance, double* awayfgChance)
{
  string hometeam = *home;
  string awayteam = *away;
  double hometdchance = *hometdChance;
  double homefgchance = *homefgChance;
  double awaytdchance = *awaytdChance;
  double awayfgchance = *awayfgChance;

  int homescore = 0; //initializing some more variables (way too many haha)
  int awayscore = 0;
  int hometd = 0;
  int awaytd = 0;
  int homefg = 0;
  int awayfg = 0;
  int hometdtries = 0;
  int awaytdtries = 0;
  int homefgtries = 0;
  int awayfgtries = 0;
  int overtimecounter = 0;

  cout << "Game " << count+1 << " - " << hometeam << " vs " << awayteam << endl;
  cout << "------" << endl;

  //actual 4 quarters football game
  //team 1
  //follows the skeleton of if scored TD, plus 7 points
  //else new rand probability 
  //if scored FG, plus 3 points
  //else no score 
  //field goals SHOULD WORK!! It suddenly stopped
  for (int j=0; j<4; j++) {
	if (watch == true) {
	  cout << "QUARTER " << j+1 << endl;
	  cout << hometeam << " ball - ";
	}
	hometdtries++;
	if (randomProb() < hometdchance) {
	  hometd++;
	  if (watch == true) {
		cout << "TOUCHDOWN" << endl;
	  }
	}
	else {
	  homefgtries++;
	  if (randomProb() < homefgchance) {
	    homefg++;
	    if (watch == true) {
		cout << "FIELD GOAL" << endl; 
	    }
	  }
	else {
	  if (watch == true) {
		cout << "NO SCORE" << endl;
	  }
	}
      }
	//team 2
  	if (watch == true) {
	  cout << awayteam << " ball - ";
	}
	awaytdtries++;
	if (randomProb() < awaytdchance) {
	  awaytd++;
	  if (watch == true) {
		cout << "TOUCHDOWN" << endl << endl;
	  }
	}
	else {
	  awayfgtries++;
	  if (randomProb() < awayfgchance) {
		awayfg++;
		if (watch == true) {
		  cout << "FIELD GOAL" << endl << endl;
		}
	  }
	  else {
		if (watch == true) {
		  cout << "NO SCORE" << endl << endl;
		}
	  }
	}
  } //end of the 4 quarters

  //compute the team scores
  homescore = (hometd*7)+(homefg*3);
  awayscore = (awaytd*7)+(awayfg*3);

  //compare the team scores
  if (homescore != awayscore) {
	if (count < 4) {
	  allfg[0][count*2] = homefg;
	  allfg[0][(count*2)+1] = awayfg;
	  alltd[0][count*2] = hometd;
	  alltd[0][(count*2)+1] = awaytd;
	  atttd[0][count*2] = hometdtries;
	  atttd[0][(count*2)+1] = awaytdtries;
	  attfg[0][(count*2)] = homefgtries;
	  attfg[0][(count*2)+1] = awayfgtries;
  }

  	else if (count >3 && count <6) {
	  for (int i=0; i<8; i++) {
	    if (playing[i] == false && i<4) {
		alltd[1][i] = hometd;
		allfg[1][i] = homefg;
		atttd[1][i] = hometdtries;
		attfg[1][i] = homefgtries;
	    }
	    if (playing[i] == false && i>3) {
		alltd[1][i] = awaytd;
		allfg[1][i] = awayfg;
		atttd[1][i] = awaytdtries;
		attfg[1][i] = awayfgtries;
	    }
	  }
	}
	else if (count > 5) {
	  for (int i=0; i<8; i++) {
	    if (playing[i] == false && i<4) {
		alltd[2][i] = hometd;
		allfg[2][i] = homefg;
		atttd[2][i] = hometdtries;
		attfg[2][i] = homefgtries;
	    }
	    if (playing[i] == false && i>3) {
		alltd[2][i] = awaytd;
		allfg[2][i] = awayfg;
		atttd[2][i] = awaytdtries;
		attfg[2][i] = awayfgtries;
	    }
	  }
	}
  } //end of saving stats for a case of no overtime

  if (homescore > awayscore) {
	cout << hometeam << " beat the " << awayteam << " " << homescore << " - " << awayscore << "." << endl << endl;
	*play2 = true;
  }
  else if (awayscore > homescore) {
	cout << awayteam << " beat the " << hometeam << " " << awayscore << " - " << homescore << "." << endl << endl;
	*play1 = true;
  }
  else { //overtime
	while (awayscore == homescore) {
	  overtimecounter++;
	  if (watch == true) {
		cout << "The teams are tied with " << homescore << " points. Overtime!" << endl << endl;
		cout << hometeam << " ball - ";
	  }
	  if (randomProb() < hometdchance) { //team 1 with the ball
	    hometd++;
	    if (watch == true) {
		cout << "TOUCHDOWN" << endl;
	    }
	  }
	  else {
	    if (randomProb() < homefgchance) {
	     homefg++;
	     if (watch == true) {
		cout << "FIELD GOAL" << endl;
	     }
	    }
	    else {
		if (watch == true) {
		  cout << "NO SCORE" << endl << endl;
	   	}
		continue;
	    }
  	  }
	  if (watch == true) {
		cout << awayteam << " ball - ";
	  }
	  if (randomProb() < awaytdchance) {
		awaytd++;
		if (watch == true) {
		  cout << "TOUCHDOWN" << endl;
		}
	  }
	  else {
		if (randomProb() < awayfgchance) {
		  awayfg++;
		  if (watch == true) {
		    cout << "FIELD GOAL" << endl;
		  }
		}
	        else {
		  if (watch == true) {
			cout << "NO SCORE" << endl << endl;
		  }
          }
	  if (homescore > awayscore) { //ends the overtime when one team has more points after possessions
		cout << hometeam << " beat the " << awayteam << " " << homescore << " - " << awayscore << "." << endl << endl;
		*play2 = true;
          }
  	  else if (awayscore > homescore) {
		cout << awayteam << " beat the " << hometeam << " " << awayscore << " - " << homescore << "." << endl << endl;
		*play1 = true;
		}
	  }

  //compare scores
  homescore = (hometd*7)+(homefg*3);
  awayscore = (awaytd*7)+(awayfg*3);
  }

  if (count < 4) {  //save counters to stats arrays
	  allfg[0][count*2] = homefg;
	  allfg[0][(count*2)+1] = awayfg;
	  alltd[0][count*2] = hometd;
	  alltd[0][(count*2)+1] = awaytd;
	  atttd[0][count*2] = hometdtries;
	  atttd[0][(count*2)+1] = awaytdtries;
	  attfg[0][(count*2)] = homefgtries;
	  attfg[0][(count*2)+1] = awayfgtries;
  }

  else if (count >3 && count <6) {
	  for (int i=0; i<8; i++) {
	    if (playing[i] == false && i<4) {
		alltd[1][i] = hometd;
		allfg[1][i] = homefg;
		atttd[1][i] = hometdtries;
		attfg[1][i] = homefgtries;
	    }
	    if (playing[i] == false && i>3) {
		alltd[1][i] = awaytd;
		allfg[1][i] = awayfg;
		atttd[1][i] = awaytdtries;
		attfg[1][i] = awayfgtries;
	    }
	  }
	}
	else if (count > 5) {
	  for (int i=0; i<8; i++) {
	    if (playing[i] == false && i<4) {
		alltd[2][i] = hometd;
		allfg[2][i] = homefg;
		atttd[2][i] = hometdtries;
		attfg[2][i] = homefgtries;
	    }
	    if (playing[i] == false && i>3) {
		alltd[2][i] = awaytd;
		allfg[2][i] = awayfg;
		atttd[2][i] = awaytdtries;
		attfg[2][i] = awayfgtries;
	    }
	  }
	}

	if (homescore > awayscore) {
	  cout << hometeam << " beat the " << awayteam << " " << homescore << " - " << awayscore << "." << endl << endl;
	  *play2 = true;
        }
        else if (awayscore > homescore) {
	  cout << awayteam << " beat the " << hometeam << " " << awayscore << " - " << homescore << "." << endl << endl;
	  *play1 = true;
	}
  }
  count++;
} //end of play
/////////////////////////////////////////////////////////////////////////////////////
void stats()
{
  int td1=0; //initializing more variables!!
  int td2=0;
  int td3=0;
  int fg1=0;
  int fg2=0;
  int fg3=0;
  int atttd1=0;
  int atttd2=0;
  int atttd3=0;
  int attfg1=0;
  int attfg2=0;
  int attfg3=0;
  
  double difftd = 0.0;
  double difffg = 0.0;

  cout << "TOUCHDOWN TABLE" << endl;
  cout << "              ROUND #" << endl;
  cout << "Team       1     2     3   Total   %   Prob Diff" << endl;
  cout << "----     ----- ----- ----- ----- ----- ---- -----" << endl;
  
  for (int i=0; i<8; i++) { //outputting of the statistics
	cout << setw(10) << left << bracket1[i] << alltd[0][i] << "/" << setw(4) << atttd[0][i] << alltd[1][i] << "/" << setw(4) << atttd[1][i];
        cout << alltd[2][i] << "/" << setw(3) << atttd[2][i] << alltd[0][i]+alltd[1][i]+alltd[2][i] << "/";
	cout << setw(4) << atttd[0][i]+atttd[1][i]+atttd[2][i];
	cout.precision(2);
	cout << setw(6) << (double)(alltd[0][i]+alltd[1][i]+alltd[2][i])/(atttd[0][i]+atttd[1][i]+atttd[2][i]);
	cout << setw(6) << probbracket1[0][i];
	cout.precision(2);
	cout << (probbracket1[0][i]-((double)(alltd[0][i]+alltd[1][i]+alltd[2][i])/(atttd[0][i]+atttd[1][i]+atttd[2][i]))) << endl;
  }

  cout << setw(9) << left << "TOTAL";
	for (int i=0; i<8; i++) {
		td1 = td1+alltd[0][i];
		atttd1 = atttd1+atttd[0][i];
	}
	cout << td1 << "/" << setw(4) << atttd1;
	for (int i=0; i<8; i++) {
		td2 = td2+alltd[1][i];
		atttd2 = atttd2+atttd[1][i];
	}
	cout << td2 << "/" << setw(3) << atttd2;
	for (int i=0; i<8; i++) {
		td3 = td3+alltd[2][i];
		atttd3 = atttd3+atttd[2][i];
	}
	cout << td3 << "/" << setw(3) << atttd3 << (td1+td2+td3) << "/" << setw(16) << (atttd1+atttd2+atttd3);
	for (int i=0; i<8; i++) {
		difftd = difftd + ((probbracket1[0][i]-((double)(alltd[0][i]+alltd[1][i]+alltd[2][i])/(atttd[0][i]+atttd[1][i]+atttd[2][i]))));
	}
	cout << difftd << endl << endl;


  cout << "FIELD GOAL TABLE" << endl;
  cout << "               ROUND #" << endl;
  cout << "Team      1     2     3   Total   %   Prob Diff" << endl;
  cout << "----    ----- ----- ----- ----- ----- ---- -----" << endl;

  for (int i = 0; i < 8; i ++) {
	cout << setw(9) << left << bracket1[i] << allfg[0][i] << "/" << setw(4) << attfg[0][i] << allfg[1][i] << "/" << setw(4) << allfg[1][i];
	cout << allfg[2][i] << "/" << setw(3) << attfg[2][i] << allfg[0][i]+allfg[1][i]+allfg[2][i] << "/";
	cout << setw(4) << attfg[0][i]+attfg[1][i]+attfg[2][i];
	if ((attfg[0][i]+attfg[1][i]+attfg[2][i]) == 0) {
		cout << setw(6) << "0";
	}
	else {
		cout.precision(2);
		cout << setw(6) << (double)(allfg[0][i]+allfg[1][i]+allfg[2][i])/(attfg[0][i]+attfg[1][i]+attfg[2][i]);
	}
	cout << setw(6) << probbracket1[1][i];
	if ((attfg[0][i]+attfg[1][i]+attfg[2][i]) == 0) {
		cout << probbracket1[1][i] << endl;
	}
	else {
	        cout.precision(2);
		cout << probbracket1[1][i]-((double)allfg[0][i]+allfg[1][i]+allfg[2][i])/(attfg[0][i]+attfg[1][i]+attfg[2][i]) << endl;
        }
  }
  cout << setw(8) << left << "TOTAL";
  for (int i=0; i<8; i++) {
	fg1 = fg1+allfg[0][i];
	attfg1 = attfg1+attfg[0][i];
  }
  cout << fg1 << "/" << setw(4) << attfg1;
  for (int i=0; i<8; i++) {
	fg2 = fg2+allfg[1][i];
	attfg2 = attfg2+attfg[1][i];
  }
  cout << fg2 << "/" << setw(4) << attfg2;
  for (int i=0; i<8; i++) {
	fg3 = fg3+allfg[2][i];
	attfg3 = attfg3+attfg[2][i];
  }
  cout << fg3 << "/" << setw(3) << attfg3 << (fg1+fg2+fg3) << "/" << setw(15) << (attfg1+attfg2+attfg3);
  for (int i=0; i<8; i++) {
	if ((attfg[0][i]+attfg[1][i]+attfg[2][i])==0) {
		difffg = difffg;
	}
	else {
	  difffg = difffg + (probbracket1[1][i]-((double)allfg[0][i]+allfg[1][i]+allfg[2][i])/(attfg[0][i]+attfg[1][i]+attfg[2][i]));
	}
  }
  cout << difftd << endl << endl;
  cout << endl << endl;
}
////////////////////////////////////////////////////////////////////////////////
double randomProb()
{ //generating a random probability
	double y = 0.0;
	y = (rand() % 101)/100.0;
	return y;
} //end prob and end of PROGRAM!
///////////////////////////////////////////////////////////////////////////////////////////
