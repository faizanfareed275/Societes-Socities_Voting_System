
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <memory>

using namespace std;

/*

*************************************************
*                                               *
*                  VOTING SYSTEM                *
*                                               *
*  Developed by:                                *
*                Faizan Fareed (BSE-24S-030)    *
*                Nabiha Raja (BSE-24S-036)      *
*                Zubia Ghulamullah (BSE-24S-042)*
*                Amna Usmani (BSE-24S-015)      *
*************************************************

*/

void heading();

// Abstract Base Class (Abstraction)
class User
{
protected:
    string id, password;

public:
    virtual void login() = 0;
    string getId() const
    {
        return id;
    }
};

// Derived Class (Inheritance)
class Voter : public User
{
public:
    void login() override
    {
        int log = 0;
        cout << "Enter 1 for Login\n";
        cin >> log;

        cin.ignore();
        if (log == 1)
        {
            cout << "Enter ID: ";
            getline(cin, id);
            cout << "Enter password: ";
            getline(cin, password);
        }
        else
        {
            exit(0);
        }

        ifstream voted("voted.txt");
        string line;
        while (getline(voted, line))
        {
            if (line == id)
            {
                cout << "You have already voted.\n";
                exit(0);
            }
        }
        voted.close();

        ifstream logFile("login.txt");
        string storedId, storedPassword;
        while (logFile >> storedId >> storedPassword)
        {
            if (id == storedId && password == storedPassword)
            {
                cout << "\t\t\t\n Login successful.\n";
                //  cout << "Logged in user ID: " << this->id << endl;
                return;
            }
        }
        cout << "Invalid ID or password.\n";
        exit(0);
    }
};

// Base Class for Society (Abstraction + Encapsulation)
class Society
{
protected:
    string name;
    vector<string> candidates;
    vector<int> votes;

public:
    Society(string societyName) : name(societyName) {}

    virtual void loadCandidates()
    {
        ifstream file(name + "_candidates.txt");
        if (!file)
        {
            cout << "Error opening file for candidates in " << name << " society.\n";
            return;
        }
        string candidate;
        while (getline(file, candidate))
        {
            candidates.push_back(candidate);
            votes.push_back(0);
        }
        file.close();
    }

    virtual void displayCandidates() const
    {
        for (size_t i = 0; i < candidates.size(); i++)
        {
            cout << i + 1 << ". " << candidates[i] << endl;
        }
    }

    virtual void vote(int choice)
    {
        if (choice < 1 || choice > candidates.size())
        {
            cout << "Invalid choice.\n";
            return;
        }
        votes[choice - 1]++;
        saveVotes();
        cout << "Your vote has been recorded successfully.\n";
    }

   virtual void liveCount() const 
{
    vector<int> currentVotes(votes.size(), 0);

    // Reload votes from file
    ifstream fileIn(name + "_votes.txt");
    if (fileIn)
    {
        for (size_t i = 0; i < votes.size() && fileIn; ++i)
        {
            fileIn >> currentVotes[i];
        }
        fileIn.close();
    }
    else
    {
        cout << "Error: Could not open " << name << "_votes.txt for live count.\n";
        return;
    }

    // Display the updated votes
    // cout << "Live Count for " << name << " Society:\n";
    for (size_t i = 0; i < candidates.size(); i++)
    {
        cout << candidates[i] << " - Votes: " << currentVotes[i] << endl;
    }
}


    string getName() const
    {
        return name;
    }

protected:
    void saveVotes() const
    {
        vector<int> currentVotes;

        // Read the existing votes from the file
        ifstream fileIn(name + "_votes.txt");
        if (fileIn)
        {
            int voteCount;
            while (fileIn >> voteCount)
            {
                currentVotes.push_back(voteCount);
            }
            fileIn.close();
        }

        // Update votes by incrementing the relevant candidate's count
        for (size_t i = 0; i < votes.size(); ++i)
        {
            if (i < currentVotes.size())
            {
                currentVotes[i] += votes[i]; // Increment existing votes
            }
            else
            {
                currentVotes.push_back(votes[i]); // Add new votes
            }
        }

        // Write the updated votes back to the file
        ofstream fileOut(name + "_votes.txt");
        for (const auto &vote : currentVotes)
        {
            fileOut << vote << endl;
        }
        fileOut.close();
    }
};

// Derived Class for a Specialized Society (Inheritance + Polymorphism)
class ScienceSociety : public Society
{
public:
    ScienceSociety() : Society("Science") {}

    void liveCount() const override
    {
        cout << "Live Count for Science Society:\n";
        Society::liveCount();
    }
};

class ArtSociety : public Society
{l
public:
    ArtSociety() : Society("Art") {}

    void liveCount() const override
    {
        cout << "Live Count for Art Society:\n";
        Society::liveCount();
    }
};

class SportSociety : public Society
{
public:
    SportSociety() : Society("Sport") {}

    void liveCount() const override
    {
        cout << "Live Count for Sport Society:\n";
        Society::liveCount();
    }
};

// Manager Class to Handle Voting System (Encapsulation)
class VotingSystem
{
    unique_ptr<User> voter; // Polymorphic object
    vector<shared_ptr<Society> > societies;

public:
    VotingSystem()
    {
        societies.push_back(make_shared<ScienceSociety>());
        societies.push_back(make_shared<ArtSociety>());
        societies.push_back(make_shared<SportSociety>());
    }

    void loadSocieties()
    {
        for (const auto &society : societies)
        {
            society->loadCandidates();
        }
    }

    void start()
    {
        voter = make_unique<Voter>(); // Sabse pehle, ek Voter object banaya jata hai using make_unique<Voter>()
        voter->login(); // Phir voter->login(); line execute hoti hai jo Voter class ka login() function call karti hai.

        int choice;
        while (true)
        {
            system("pause");
            system("cls");

            heading();

            cout << "Rule Book:\n\n";
            cout << "1. You can vote only once.\n2. Enter names as per registered in SMIU.\n";
            cout << "3. Student from any branch can participate.\n4. Please be fair and impartial in voting.\n\n\n";

            system("pause");

            displayMenu();
            cout << "\n Enter your choice: ";
            cin >> choice;

            switch (choice)
            {
            case 1:
            {
                voteMenu();
                liveCountMenu();

                int forExt = 0;
                cout << "\n\nEnter 1 to exit the program: ";
                cin >> forExt;
                if (forExt == 1)
                {
                    exit(1);
                }
                break;
            }
            case 2:
                liveCountMenu();
                break;
            case 3:
                exit(0);
            default:
                cout << "Invalid choice. Please try again.\n";
            }
        }
    }

private:
    void displayMenu()
    {
        cout << "\nVoting System Menu:\n"
             << "1. Vote\n"
             << "2. Live Count\n"
             << "3. Exit\n";
    }

    void voteMenu()
    {
        // cout << "for debugging: Voter ID at voteMenu: " << voter->getId() << endl;

        if (voter->getId().empty())
        {
            cout << "Error: Voter ID is empty. Ensure you are logged in.\n";
            return;
        }

        cout << "\n\t\t In which society do you want to enroll yourself\n\n";

        for (size_t i = 0; i < societies.size(); i++)
        {
            cout << i + 1 << ". " << societies[i]->getName() << endl;
        }
        int societyChoice;
        cin >> societyChoice;

        if (societyChoice < 1 || societyChoice > societies.size())
        {
            cout << "Invalid choice. Returning to menu.\n";
            return;
        }
        cout << "You have successfully enrolled" << endl;

        shared_ptr<Society> selectedSociety = societies[societyChoice - 1];
        cout << "\nCandidates in " << selectedSociety->getName() << " Society:\n";
        selectedSociety->displayCandidates();
        cout << "Enter the number of your chosen candidate: ";
        int candidateChoice;
        cin >> candidateChoice;

        selectedSociety->vote(candidateChoice);

        // Debug logs
        // cout << "Attempting to record voter ID: " << voter->getId() << endl;
        markVoter(voter->getId());
        // cout << "markVoter() executed.\n";
    }

    void liveCountMenu()
    {
        cout << "\nSelect a society to view live counts:\n";
        for (size_t i = 0; i < societies.size(); i++)
        {
            cout << i + 1 << ". " << societies[i]->getName() << endl;
        }
        int societyChoice;
        cin >> societyChoice;

        if (societyChoice < 1 || societyChoice > societies.size())
        {
            cout << "Invalid choice. Returning to menu.\n";
            return;
        }

        shared_ptr<Society> selectedSociety = societies[societyChoice - 1];
        selectedSociety->liveCount();
    }

    void markVoter(const string &voterId)
    {
        ofstream votedFile("voted.txt", ios::app);
        if (!votedFile)
        {
            cout << "Error opening voted file for appending.\n";
            return;
        }
        votedFile << voterId << endl;
        votedFile.flush();
        // cout << "Voter ID " << voterId << " has been recorded.\n";
        votedFile.close();
    }
};

int main()
{
    VotingSystem system;

    cout << "\n\t\t\tWelcome to the Login_System\n";
    cout << "\n\t\t-------------------------------------------\n";

    system.loadSocieties();
    system.start();

    cin.get();
    return 0;
}

void heading() // We can also use scope resolution operator for this function (from class Society)
{
    cout << "\n\t\t   SINDH MADRESSATUL ISLAM UNIVERSITY\n";
    cout << "------------------------------------------------------------------------------------\n\n";
    cout << "\t\t  WELCOME TO THE SMIU STUDENT ELECTION 2024\n\n";
    cout << "\t\t\t   **********\n";
    cout << "\n\n\n";
}
