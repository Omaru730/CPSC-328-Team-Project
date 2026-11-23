/************************************************************/
/* Author: Keith Groves */
/* Major: Computer Science */
/* Creation Date: 11/11/2026 */
/* Due Date:  11/23/2026 */
/* Course: CSC 328 010 */
/* Professor Name: Walther Reiley */
/* Assignment: Group Project */
/* Filename: Client.cpp */
/* Purpose: This program reads in a server address and a given port
/* number and uses them to connect to a listening server. the program 
/* then initiates a Wordle game.*/
/************************************************************/

#include <iostream>
#include <string>
#include "wordlelib.h"
#include <vector>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
using namespace std;

void Menue(int Guesses, int Socket, string CurrentWord, string GuessedWord);
void CorrectnessCheck(string GuessedWord, string CurrentWord);
void Output(string Dummy, vector<char> CorrectLetters, vector<char> YellowLetters);

/*************************************************************************/
/* */
/* Function name: main */
/* Description: Takes in command line arguments from the terminal. After validating them, 
/* the program connects to a listening server and initiates a game of Wordle. */
/* Parameters: int argc: the number of command line arguments put into the terminal
/*             char *argv[]: stores the name of each Command line argument */
/* Return Value: int*/
/* */
/*************************************************************************/
int main(int argc, char *argv[])
{
    int Guesses = 6;
    string GuessedWord;
    string CurrentWord;
    int Socket;
    int Port = 50400;

    
    if (argc != 2)
    {
        cout << "Usage" << argv[0] << ": Failed to provide the required CLAs" << endl;
        exit(1);
    }
    
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;

    if( inet_pton(AF_INET, argv[1], &addr.sin_addr) <= 0)
    {
        perror("invalid address");
        exit(1);
    }
    
    if (argc > 2)
    {
        addr.sin_port = htons(atoi(argv[2]));
    }
    else
    {
        addr.sin_port = htons(Port);
    }

    // creates the client socket
    if ((Socket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("Failed to create a socket.");
        exit(1); 
    }
    // connects the client to the server with the given address
    if (connect(Socket, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("Failed to create a server connection.");
        exit(1);  
    }
    
    char request[10];
    // waits for the server to send "HELLO" message
    if (receiveMessage(Socket, request, 10) != 1)
    {
        perror("Failed to recieve message");
        exit(1);
    }
    // send "READY" to the server after verifying 
    // communication.
    if (sendMessage(Socket, "READY\n") != 1)
    {
        perror("Failed to send message");
        exit(1);
    }
    char receive[20];
    // waits for the server to send a random word.
    if (receiveMessage(Socket, receive, 20) != 1)
    {
        perror("Failed to recieve message");
        exit(1);
    }
    
    CurrentWord = receive;
    Menue(Guesses, Socket, CurrentWord, GuessedWord);
}


/*************************************************************************/
/* */
/* Function name: Menue */
/* Description: Simulates a standard wordle application. the user has six
/* attempts to guess a hiden word. if they get it right, the program notifies
/* the user that they won and prompts them if they want to start again. if not,
/* the program outputs any letters that the user correctly guessed and keeps 
/* prompting for new words until the user runs out of guesses or correctly 
/* guesses the given word.*/
/* Parameters: string CorrectWord: The given word being compared against
/*             string GuessedWord: the proposed word being tested
               int Guesses: the number of guesses to find the given word
               int Socket: Socket used to communicate with the server.*/
/* Return Value: none*/
/* */
/*************************************************************************/
void Menue(int Guesses, int Socket, string CurrentWord, string GuessedWord)
{
    cout << "Welcome to Wordle!" << endl;
    string response;
    while(response != "no")
    {
        cout << "Please guess a word!" << endl;
        while (Guesses != 0)
        {
            Guesses--;
            cin >> GuessedWord;
            if (GuessedWord.length() != 5)
            {
                cout << "wrong word length" << endl;
                continue;
            }
            // if the player correctly guesses the hiden word, notify the player
            // and reset their guesses
            if (GuessedWord == CurrentWord)
            {
                cout << "You Guessed the word!" << endl;
                Guesses = 6;
                break;
            }
            // if the player didn't guess the word but still has guesses, show
            // them the letters they got right and their correct positions
            else if (Guesses != 0 && GuessedWord != CurrentWord)
            {
                CorrectnessCheck(GuessedWord, CurrentWord);
                cout << "You have " << Guesses << " guesses left." << endl; 
            }
            // if the player runs out of guesses and failed to get the word,
            // notify the player, tell them which word it was, and reset their 
            // guesses.
            else
            {
                cout << "You ran out of guesses!" << endl;
                cout << "The word was: " << CurrentWord << endl;
                Guesses = 6;
                break;
            }

        }
        cout << "Would you like to start the game again (yes/no)." << endl;
        cin >> response;
        // if the player wants to play again, request a new word from the server
        if (response == "yes")
        {
            if (sendMessage(Socket, "READY\n") != 1)
            {
                perror("Failed to send message");
                exit(1);
            }
            char receive[20];
            if (receiveMessage(Socket, receive, 20) != 1)
            {
                perror("Failed to recieve message");
                exit(1);
            }
            CurrentWord = receive;
        }
        // if the player wants to quit, send a request to the server to terminate
        // the connection.
        else
        {
            if (sendMessage(Socket, "QUIT\n") != 1)
            {
                perror("Failed to send message");
                exit(1);
            }
            exit(1);
        }
    }
}



/*************************************************************************/
/* */
/* Function name: CorrectnessCheck */
/* Description: takes a given word and a proposed word and determines if 
/* they have the same letters. The function then organizes these letters
/* based on if they are in the correct posion or not. */
/* Parameters: string CorrectWord: The given word being compared against
/*             string GuessedWord: the proposed word being tested*/
/* Return Value: none*/
/* */
/*************************************************************************/
void CorrectnessCheck(string GuessedWord, string CurrentWord)
{
    // a placeholder string to track the posions of correct letters that the.
    // user guessed.
    string Dummy = "_____";
    // a tracker string meant track both green and yellow letters that the user guessed.
    string tracker = CurrentWord;
    // a tracker string meant track only green letters that the user guessed.
    string ytracker = CurrentWord;
    // integers meant to track specific positions in the trackers
    int trackerpos;
    int ytrackerpos;
    // two character vectors meant to track both green and yellow words.
    vector<char> CorrectLetters;
    vector<char> YellowLetters;

    for (size_t i = 0; i < GuessedWord.length(); i++)
    {
        // if a letter is in the correct position, add it to the green letters
        // and update the dummy string with the correct letter positions.
        // Also remove the green letter from trackers if it exists inside the string.
        if(CurrentWord[i] == GuessedWord[i])
        {
            Dummy[i] = GuessedWord[i];
            CorrectLetters.push_back(GuessedWord[i]);
            
            if(tracker.find(GuessedWord[i]) != std::string::npos)
            {
                trackerpos = tracker.find(GuessedWord[i]);
                tracker.erase(trackerpos, 1);
            }
            if (ytracker.find(GuessedWord[i]) != std::string::npos)
            {
                ytrackerpos = ytracker.find(GuessedWord[i]);
                ytracker.erase(ytrackerpos, 1);
            }
        }
        // if the letter isn't int correct position but exists in the tracker
        // then the letter is saved in in the yellow words.
        // Also remove the yellow letter from only the main tracker
        else if(tracker.find(GuessedWord[i]) != std::string::npos)
        {
            Dummy[i] = GuessedWord[i];
            trackerpos = tracker.find(GuessedWord[i]);
            tracker.erase(trackerpos, 1);
            YellowLetters.push_back(GuessedWord[i]);
        }
    }
    // remove all yellow leters if the player has found their green positions.
    for (size_t i = 0; i < YellowLetters.size(); i++)
    {
        if (ytracker.find(YellowLetters[i]) == std::string::npos)
        {
            Dummy[Dummy.find(YellowLetters[i])] = '_';
            YellowLetters.erase(YellowLetters.begin() + i);
        }
    }

    Output(Dummy, CorrectLetters, YellowLetters);
}


/*************************************************************************/
/* */
/* Function name: Output */
/* Description: Outputs green letters, yellow letters, and their correct 
/* positions. */
/* Parameters: string Dummy: a string representing the positions of each found 
/*             character from a guess. */
/*             vector<char> CorrectLetters: a vector storing green letters
/*             vector <char> YellowLetters: a vector storing yellow letters*/
/*             
/* Return Value: none */
/* */
/*************************************************************************/
void Output(string Dummy, vector<char> CorrectLetters, vector<char> YellowLetters)
{
    for (size_t i = 0; i < Dummy.length(); i++)
    {
        cout << Dummy[i] << " ";
    }
    cout << endl;
    cout << "( GREEN: ";
    for (size_t i = 0; i < CorrectLetters.size(); i++)
    {
        cout << CorrectLetters[i] << " ";
    }
    cout << " )" << endl;

    cout << "( Yellow: ";
    for (size_t i = 0; i < YellowLetters.size(); i++)
    {
        cout << YellowLetters[i] << " ";
    }
    cout << " )";
    cout << endl;
}
