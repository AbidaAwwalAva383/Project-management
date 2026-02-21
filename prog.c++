#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
using namespace std;

class Meme {
private:
    string memeName;
    int originYear;
    int humorScore;
    bool isStillViral;

public:
    Meme() {
        memeName = "";
        originYear = 2010;
        humorScore = 50;
        isStillViral = false;
    }

    // as you can't get me read this okay ?
    // step 1: initialize: git init
    // step 2: git add . (all the files that will be commited)
    // step 3: 



    Meme(string name, int year, int score, bool viral) {
        memeName = name;
        setOriginYear(year);
        setHumorScore(score);
        isStillViral = viral;
    }

    string getMemeName() { 
        return memeName; 
    }
    
    int getOriginYear() { 
        return originYear; 
    }
    
    int getHumorScore() { 
        return humorScore; 
    }
    
    bool getIsStillViral() { 
        return isStillViral; 
    }

    void setMemeName(string name) { 
        memeName = name; 
    }
    
    void setOriginYear(int year) {
        if (year >= 2000 && year <= 2025) {
            originYear = year;
        } else {
            originYear = 2010;
        }
    }
    
    void setHumorScore(int score) {
        if (score >= 1 && score <= 100) {
            humorScore = score;
        } else {
            humorScore = 50;
        }
    }
    
    void setIsStillViral(bool viral) { 
        isStillViral = viral; 
    }

    void display() {
        cout << "Meme: " << memeName << " | Year: " << originYear 
             << " | Humor: " << humorScore << " | Viral: " 
             << (isStillViral ? "Yes" : "No") << endl;
    }
};

void archiveMemes(Meme collection[], int size) {
    for (int i = 0; i < 5; i++) {
        string name;
        int year, score;
        bool viral;
        
        cout << "\nMeme " << (i + 1) << " details:" << endl;
        cout << "Name: ";
        cin.ignore();
        getline(cin, name);
        cout << "Year: ";
        cin >> year;
        cout << "Humor Score: ";
        cin >> score;
        cout << "Viral (1=Yes, 0=No): ";
        cin >> viral;
        
        collection[i].setMemeName(name);
        collection[i].setOriginYear(year);
        collection[i].setHumorScore(score);
        collection[i].setIsStillViral(viral);
    }
    
    srand(time(0));
    
    for (int i = 5; i < size; i++) {
        collection[i].setMemeName("Lost Meme");
        collection[i].setOriginYear(2005 + rand() % 11);
        collection[i].setHumorScore(1 + rand() % 100);
        collection[i].setIsStillViral(false);
    }
}

int main() {
    Meme collection[40];
    
    archiveMemes(collection, 40);
    
    cout << "\n=== Memes before 2010 with Humor > 70 ===" << endl;
    for (int i = 0; i < 40; i++) {
        if (collection[i].getOriginYear() < 2010 && collection[i].getHumorScore() > 70) {
            collection[i].display();
        }
    }
    
    return 0;
}