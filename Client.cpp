#include <iostream>
#include <string>
#include <vector>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
using namespace std;

void Menue(int Guesses, string CurrentWord, string GuessedWord);
void CorrectnessCheck(string GuessedWord, string CurrentWord);

int main(int argc, char *argv[])
{
    int Guesses = 5;
    string CurrentWord = "apple";
    string GuessedWord;
    int Socket;
    int Port = 50300;

    struct sockaddr_in addr = {0};
    addr.sin_family = AF_INET;
    if( inet_pton(AF_INET, argv[1], &addr.sin_addr) <= 0)
    {
        perror("invalid address");
        exit(1);
    }

    if (argc > 2)
    {
        addr.sin_port = htons(atoi(argv[2]));
        cout << addr.sin_port << endl;
    }
    else
    {
        addr.sin_port = htons(Port);
        cout << addr.sin_port << endl;
    }

    Socket = socket(AF_INET, SOCK_STREAM, 0);
    int result = connect(Socket, (struct sockaddr *)&addr, sizeof(addr));
    cout << result << endl;

    Menue(Guesses, CurrentWord, GuessedWord);
}

void Menue(int Guesses, string CurrentWord, string GuessedWord)
{
    cout << "Welcome to Wordle!" << endl;
    cout << "Please guess your first word!" << endl;
    {
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
                Guesses = 5;
                break;
            }
            else
            {
                CorrectnessCheck(GuessedWord, CurrentWord);
                Guesses--;
                cout << "You have " << Guesses << " guesses left." << endl; 
            }

        }
    }
}

void CorrectnessCheck(string GuessedWord, string CurrentWord)
{
    string Dummy = "_____";
    string tracker = CurrentWord;
    int trackerpos;
    vector<char> CorrectLetters;
    vector<char> YellowLetters;
    for (int i = 0; i < GuessedWord.length(); i++)
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
        }
        else if(tracker.find(GuessedWord[i]) != std::string::npos)
        {
            Dummy[i] = GuessedWord[i];
            trackerpos = tracker.find(GuessedWord[i]);
            tracker.erase(trackerpos, 1);
            YellowLetters.push_back(GuessedWord[i]);
        }
    }


    for (int i = 0; i < Dummy.length(); i++)
    {
        cout << Dummy[i] << " ";
    }
    cout << endl;
    cout << "( GREEN: ";
    for (int i = 0; i < CorrectLetters.size(); i++)
    {
        cout << CorrectLetters[i] << " ";
    }
    cout << " )";
    cout << endl;

    cout << endl;
    cout << "( Yellow: ";
    for (int i = 0; i < YellowLetters.size(); i++)
    {
        cout << YellowLetters[i] << " ";
    }
    cout << " )";
    cout << endl;
}
