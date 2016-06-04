#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<sstream>

using namespace std;

void makeList(vector<string>&listOutput, ifstream &listInput);
void makeParticipantEntryList(vector<string>&participantEntry, ifstream &fin, vector<string>validTeamList, string currentParticipant, ofstream &notes);
void isValid(vector<string>&validTeamList, vector<string>&participantEntry, string currentParticipant, ofstream &notes);
void compareList(vector<int>&correct, vector<string>&winnerList, vector<string>&participantEntry);
int tally(vector<int>&correct);
int addition(int input);
void printResult(ofstream &output, int score, string currentParticipant, vector<string>&winnerList, vector<string>&participantEntry, vector<string>&gameList);
void printNullResult(ofstream &output, int score, string currentParticipant, vector<string>&winnerList, vector<string>&gameList);
void sortPlaces(vector<string>&participantList, vector<int>&participantScores);
void printPlaces(ofstream &places, int index, vector<string>&participantList, vector<int>&participantScores);

int main()
{
    ifstream winners, participants, gameFile, teams;
    ofstream places, notes;
    participants.open("participants.txt");
    winners.open("resultsMaster.txt");
    gameFile.open("games.txt");
    teams.open("teams.txt");
    places.open("places.txt");
    notes.open("notes.txt");
    vector<string>winnerList, participantList, gameList, participantEntry, validTeamList;
    makeList(participantList, participants);
    makeList(winnerList, winners);
    makeList(gameList, gameFile);
    makeList(validTeamList, teams);
    vector<int>participantScores (participantList.size(),0);
    
	for(int index=1; index<=participantList.size(); index++)
    {
       string entryFile = participantList[index-1] + ".txt";
       ifstream fin(entryFile.c_str());
       
       if(fin)
       {
           participantEntry.clear();
	       makeParticipantEntryList(participantEntry, fin, validTeamList, participantList[index-1], notes);
	       vector<int>correct (winnerList.size(),0);
	       compareList(correct, winnerList, participantEntry);
	       int score = tally(correct);
	       participantScores[index-1] = score;
           ofstream output;
           string outfile = participantList[index-1] + "Results.txt";
           output.open(outfile.c_str());
	       printResult(output, score, participantList[index-1], winnerList, participantEntry, gameList);
	       fin.close();
	       output.close();
       }
       
       else 
       {    
           cout << "\nWarning: No bracket file found for participant " << participantList[index-1] << ".\n";
           notes << "\nWarning: No bracket file found for participant " << participantList[index-1] << ".\n";
           participantScores[index-1] = 0;
           ofstream output;
           string outfile = participantList[index-1] + "Results.txt";
           output.open(outfile.c_str());
           printNullResult(output, 0, participantList[index-1], winnerList, gameList);
           fin.close();
	       output.close();
       }
	}
	
	sortPlaces(participantList, participantScores);
    cout << "\n";
    notes << "\n";

    for(int i=0; i<participantList.size(); i++)
    {
       cout << participantList[i] << ": " << participantScores[i] << "\n";
       notes << participantList[i] << ": " << participantScores[i] << "\n";
       places << participantList[i] << ": " << participantScores[i] << "\n";
    }
    
    cout << "\nDone.";
    notes << "\nDone.";
    
    participants.close();
    winners.close();
    gameFile.close();
    places.close();
    notes.close();

    cin.ignore();
    cin.get();
	return 0;
}

void makeList(vector<string>&listOutput, ifstream &listInput)
{
    string line, word;
    int characters;
    
    while (!listInput.eof())
    {
       getline(listInput,line);      
       characters = line.length();
	   int lastSpace = -1;
   	 
       for(int i=0; i<line.length(); i++)
 	    {
 	       if(i==line.length()-1)
  	       {
		   		word = line.substr(lastSpace+1, (i-lastSpace));
		   		listOutput.push_back(word);
  	       }
  	    }
    }
    listInput.close();
}

void makeParticipantEntryList(vector<string>&participantEntry, ifstream &fin, vector<string>validTeamList, string currentParticipant, ofstream &notes)
{
    string line, word;
    int characters;
    
    while (!fin.eof())
    {
       getline(fin,line);      
       characters = line.length();
	   int lastSpace = -1;
   	 
       for(int i=0; i<line.length(); i++)
 	    {
 	       if(i==line.length()-1)
  	       {
		   		word = line.substr(lastSpace+1, (i-lastSpace));
		   		participantEntry.push_back(word);
		   		isValid(validTeamList, participantEntry, currentParticipant, notes);
  	       }
  	    }
    }
    fin.close();
}

void isValid(vector<string>&validTeamList, vector<string>&participantEntry, string currentParticipant, ofstream &notes)
{
    for(int i=0; i<participantEntry.size(); i++)
    {
        bool valid = false;
        for(int j=0; j<validTeamList.size(); j++)
        {
            if(participantEntry[i]==validTeamList[j]) valid = true;
        }
        if(valid==false)
        {
            cout << "\nTeam '" << participantEntry[i] << "' is invalid in entry '" << currentParticipant << "'.\n";
            notes << "\nTeam '" << participantEntry[i] << "' is invalid in entry '" << currentParticipant << "'.\n";
        }
    }
}

void compareList(vector<int>&correct, vector<string>&winnerList, vector<string>&participantEntry)
{
	for(int i=0; i<winnerList.size(); i++)
	{
		if(winnerList[i] == participantEntry[i]) 
        {
            correct[i] = 1;
        }
	}
}

int tally(vector<int>&correct)
{
	int number=0;
	for(int i=0; i<correct.size(); i++)
	{
		if(correct[i]==1)number+=addition(i);
	}
	return number;
}

int addition(int input)
{
	if(input<=31) return 1;
	else if(input<=47) return 2;
	else if(input<=55) return 3;
	else if(input<=59) return 4;
	else if(input<=61) return 6;
	else return 10;
}

void printResult(ofstream &output, int score, string currentParticipant, vector<string>&winnerList, vector<string>&participantEntry, vector<string>&gameList)
{
     int scoreSoFar=0;
     output << "-------------------------------------------------------------\n\n";
     output << "Name: " << currentParticipant << "\n\n\n";
     for(int i=0; i<winnerList.size(); i++)
     {
         output << gameList[i] << "\n\n";
         output << "     Winner: " << winnerList[i] << "    Your Pick: " << participantEntry[i] << "\n";
         output << "     Points At Stake: " << addition(i) << "    Points Awarded: ";
         if(winnerList[i] == participantEntry[i])
         {
             output << addition(i);
             scoreSoFar+=addition(i);
         }
         else output << 0;
         output << "     Cumulative Score: " << scoreSoFar << "\n\n\n";
     }
     
     output << "FINAL SCORE: " << score << "\n\n\n";
     output << "-------------------------------------------------------------\n\n";
}

void printNullResult(ofstream &output, int score, string currentParticipant, vector<string>&winnerList, vector<string>&gameList)
{
     int scoreSoFar=0;
     output << "-------------------------------------------------------------\n\n";
     output << "Name: " << currentParticipant << "\n\n\n";
     for(int i=0; i<winnerList.size(); i++)
     {
         output << gameList[i] << "\n\n";
         output << "     Winner: " << winnerList[i] << "    Your Pick: NULL" << "\n";
         output << "     Points At Stake: " << addition(i) << "    Points Awarded: 0";
         output << "     Cumulative Score: 0" << "\n\n\n";
     }
     output << "FINAL SCORE: " << score << "\n\n\n";
     output << "-------------------------------------------------------------\n\n";
}

void sortPlaces(vector<string>&participantList, vector<int>&participantScores)
{
     int tempint;
     string tempstring;
     for(int i=0; i<participantList.size()-1; i++)
     {
         if(participantScores[i] < participantScores[i+1])
         {
             tempint = participantScores[i];
             participantScores[i] = participantScores[i+1];
             participantScores[i+1] = tempint;
             tempstring = participantList[i];
             participantList[i] = participantList[i+1];
             participantList[i+1] = tempstring;
             
             sortPlaces(participantList, participantScores);
         }
     }
}

void printPlaces(ofstream &places, int index, vector<string>&participantList, vector<int>&participantScores)
{
     for(int i=0; i<participantList.size(); i++)
     {
         places << participantList[index] << ": " << participantScores[index] << "\n";
     }
}
