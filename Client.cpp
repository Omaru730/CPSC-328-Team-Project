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

int main(int argc, char *argv[])
{
    int Guesses = 6;
    string GuessedWord;
    string CurrentWord;
    int Socket;
    int Port = 50400;

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

    Socket = socket(AF_INET, SOCK_STREAM, 0);
    connect(Socket, (struct sockaddr *)&addr, sizeof(addr));
    

    char request[10];
    if (receiveMessage(Socket, request, 10) != 1)
    {
        cout << "failed to recieve h" << endl;
    }
    if (sendMessage(Socket, "READY\n") != 1)
    {
        cout << "failed to send" << endl;
    }
    char receive[20];
    if (receiveMessage(Socket, receive, 20) != 1)
    {
        cout << "failed to receive b" << endl;
    }
    
    CurrentWord = receive;
    Menue(Guesses, Socket, CurrentWord, GuessedWord);

}

void Menue(int Guesses, int Socket, string CurrentWord, string GuessedWord)
{
    cout << "Welcome to Wordle!" << endl;
    string response;
    while(response != "no")
    {
        cout << "Please guess a word!" << endl;
        while (Guesses != 0)
        {
            cin >> GuessedWord;
            if (GuessedWord.length() != 5)
            {
                cout << "wrong word length" << endl;
                continue;
            }
            if (GuessedWord == CurrentWord)
            {
                cout << "You Guessed the word!" << endl;
                Guesses = 6;
                break;
            }
            else
            {
                CorrectnessCheck(GuessedWord, CurrentWord);
                Guesses--;
                cout << "You have " << Guesses << " guesses left." << endl; 
            }

        }
        cout << "Would you like to start the game again (yes/no)." << endl;
        cin >> response;
        if (response == "yes")
        {
            if (sendMessage(Socket, "READY\n") != 1)
            {
                cout << "failed to send" << endl;
            }
            char receive[20];
            if (receiveMessage(Socket, receive, 20) != 1)
            {
                cout << "failed to receive b" << endl;
            }
            CurrentWord = receive;
        }
        else
        {
            sendMessage(Socket, "QUIT\n");
        }
    }
}

void CorrectnessCheck(string GuessedWord, string CurrentWord)
{
    string Dummy = "_____";
    string tracker = CurrentWord;
    string ytracker = CurrentWord;
    int trackerpos;
    int ytrackerpos;
    vector<char> CorrectLetters;
    vector<char> YellowLetters;

    for (size_t i = 0; i < GuessedWord.length(); i++)
    {
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
        else if(tracker.find(GuessedWord[i]) != std::string::npos)
        {
            Dummy[i] = GuessedWord[i];
            trackerpos = tracker.find(GuessedWord[i]);
            tracker.erase(trackerpos, 1);
            YellowLetters.push_back(GuessedWord[i]);
        }
    }

    for (size_t i = 0; i < YellowLetters.size(); i++)
    {
        if (ytracker.find(YellowLetters[i]) == std::string::npos)
        {
            YellowLetters.erase(YellowLetters.begin() + i);
            Dummy[Dummy.find(YellowLetters[i])] = '_';
        }
    }


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
