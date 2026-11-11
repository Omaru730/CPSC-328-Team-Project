#include <iostream>
#include <string>
#include <vector>
#include <sys/socket.h>
#include <netinet/in.h>
using namespace std;

void Menue(int Guesses, string CurrentWord, string GuessedWord);
void CorrectnessCheck(string GuessedWord, string CurrentWord);


int main()
{
    int Guesses = 5;
    string CurrentWord = "black";
    string GuessedWord;
    int Socket;

    Socket = socket(AF_INET, SOCK_STREAM, 0);
    cout << "Success" << endl;

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
    vector<char> CorrectLetters;
    for (int i = 0; i < CurrentWord.length(); i++)
    {
        if(CurrentWord[i] == GuessedWord[i])
        {
            Dummy[i] = GuessedWord[i];
            CorrectLetters.push_back(GuessedWord[i]);
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
}
