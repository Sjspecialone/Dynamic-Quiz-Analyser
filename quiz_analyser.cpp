#include <bits/stdc++.h>
using namespace std;

const int MAX_USERS = 20;
const int LEVELS = 3;
const int Q_PER_LEVEL = 6;

int totalUsers = 0;
string users[MAX_USERS];
string passes[MAX_USERS];
int userScores[MAX_USERS];

string levelName[LEVELS] = {"Easy", "Moderate", "Difficult"};

string qText[LEVELS][Q_PER_LEVEL] = {
    {
        "2+2 = ?",
        "Which is a mammal?",
        "Color of the sky on a clear day?",
        "5-3 = ?",
        "Which planet is known as the Red Planet?",
        "Which number is even?"
    },
    {
        "What is 12 * 12?",
        "Which data structure uses LIFO?",
        "Binary representation: 5 is ?",
        "Square root of 81?",
        "Which data structure uses FIFO?",
        "Binary representation: 10 is ?"
    },
    {
        "Derivative of x^2 ?",
        "Time complexity of binary search?",
        "Which algorithm uses backtracking?",
        "Integral of 2x dx?",
        "Time complexity of merge sort?",
        "Which is an NP-complete problem?"
    }
};

string qOpt[LEVELS][Q_PER_LEVEL][4] = {
    {
        {"1) 3","2) 4","3) 5","4) 6"},
        {"1) Shark","2) Salmon","3) Dolphin","4) Trout"},
        {"1) Blue","2) Green","3) Red","4) Black"},
        {"1) 1","2) 2","3) 3","4) 4"},
        {"1) Earth","2) Mars","3) Venus","4) Jupiter"},
        {"1) 3","2) 5","3) 8","4) 7"}
    },
    {
        {"1) 132","2) 144","3) 154","4) 124"},
        {"1) Queue","2) Stack","3) Tree","4) Graph"},
        {"1) 101","2) 110","3) 011","4) 100"},
        {"1) 7","2) 8","3) 9","4) 6"},
        {"1) Stack","2) Queue","3) Tree","4) Graph"},
        {"1) 1010","2) 0101","3) 1110","4) 1001"}
    },
    {
        {"1) x","2) 2x","3) x^2","4) 1"},
        {"1) O(n)","2) O(log n)","3) O(n^2)","4) O(1)"},
        {"1) Dijkstra","2) BFS","3) Rat in a Maze","4) Merge Sort"},
        {"1) x^2 + C","2) x^3 + C","3) 2x + C","4) ln|x| + C"},
        {"1) O(n)","2) O(n log n)","3) O(n^2)","4) O(log n)"},
        {"1) Sorting","2) Subset Sum","3) Searching","4) Matrix Multiplication"}
    }
};

int correctAns[LEVELS][Q_PER_LEVEL] = {
    {2,3,1,2,2,1},
    {2,2,1,3,2,1},
    {2,2,3,2,2,2}
};

int attempts[LEVELS][Q_PER_LEVEL] = {{0}};
int corrects[LEVELS][Q_PER_LEVEL] = {{0}};

string lbNamesOverall[MAX_USERS];
int lbScoresOverall[MAX_USERS];
int lbCountOverall = 0;

string lbNamesPerLevel[LEVELS][MAX_USERS];
int lbScoresPerLevel[LEVELS][MAX_USERS];
int lbCountPerLevel[LEVELS] = {0};

int askInt() {
    int x;
    while (!(cin >> x)) {
        cin.clear();
        string dummy;
        getline(cin, dummy);
    }
    return x;
}

void sortLeaderboard(string names[], int scores[], int &count) {
    int i = 0;
    while (i < count) {
        int j = i + 1;
        int best = i;
        while (j < count) {
            if (scores[j] > scores[best]) best = j;
            j++;
        }
        if (best != i) {
            swap(scores[i], scores[best]);
            swap(names[i], names[best]);
        }
        i++;
    }
}

void addOverallLeaderboard(string name, int score) {
    if (lbCountOverall < MAX_USERS) {
        lbNamesOverall[lbCountOverall] = name;
        lbScoresOverall[lbCountOverall] = score;
        lbCountOverall++;
    } else {
        int m = 0;
        int i = 1;
        while (i < lbCountOverall) {
            if (lbScoresOverall[i] < lbScoresOverall[m]) m = i;
            i++;
        }
        if (score > lbScoresOverall[m]) {
            lbNamesOverall[m] = name;
            lbScoresOverall[m] = score;
        }
    }
    sortLeaderboard(lbNamesOverall, lbScoresOverall, lbCountOverall);
}

void addLevelLeaderboard(int lvl, string name, int score) {
    if (lbCountPerLevel[lvl] < MAX_USERS) {
        lbNamesPerLevel[lvl][lbCountPerLevel[lvl]] = name;
        lbScoresPerLevel[lvl][lbCountPerLevel[lvl]] = score;
        lbCountPerLevel[lvl]++;
    } else {
        int m = 0;
        int i = 1;
        while (i < lbCountPerLevel[lvl]) {
            if (lbScoresPerLevel[lvl][i] < lbScoresPerLevel[lvl][m]) m = i;
            i++;
        }
        if (score > lbScoresPerLevel[lvl][m]) {
            lbNamesPerLevel[lvl][m] = name;
            lbScoresPerLevel[lvl][m] = score;
        }
    }
    sortLeaderboard(lbNamesPerLevel[lvl], lbScoresPerLevel[lvl], lbCountPerLevel[lvl]);
}

int findUser(string name, string pass) {
    int i = 0;
    while (i < totalUsers) {
        if (users[i] == name && passes[i] == pass) return i;
        i++;
    }
    return -1;
}

int askQuestionInteractive(int lvl, int qIdx) {
    cout << "\n[" << levelName[lvl] << "] " << qText[lvl][qIdx] << "\n";
    for (int i = 0; i < 4; ++i) cout << qOpt[lvl][qIdx][i] << "\n";
    cout << "Enter option number: ";
    int ans = askInt();
    return ans;
}

void showOverallLeaderboard() {
    if (lbCountOverall == 0) {
        cout << "Leaderboard empty\n";
        return;
    }
    int i = 0;
    while (i < lbCountOverall) {
        cout << lbNamesOverall[i] << " : " << lbScoresOverall[i] << "\n";
        i++;
    }
}

void showLevelLeaderboard() {
    cout << "Choose level: 1) Easy 2) Moderate 3) Difficult\nChoose: ";
    int c = askInt();
    if (c < 1 || c > 3) {
        cout << "Invalid\n";
        return;
    }
    int lvl = c - 1;
    if (lbCountPerLevel[lvl] == 0) {
        cout << "No scores for " << levelName[lvl] << "\n";
        return;
    }
    int i = 0;
    while (i < lbCountPerLevel[lvl]) {
        cout << lbNamesPerLevel[lvl][i] << " : " << lbScoresPerLevel[lvl][i] << "\n";
        i++;
    }
}

int main() {

    int running = 1;
    while (running) {
        cout << "\n1) Sign Up  2) Log In  3) Leaderboard  4) Exit\nChoose: ";
        int choice = askInt();
        if (choice == 4) {
            running = 0;
        } else if (choice == 3) {
            cout << "1) Overall  2) Per Level\nChoose: ";
            int c = askInt();
            if (c == 1) showOverallLeaderboard();
            else if (c == 2) showLevelLeaderboard();
            else cout << "Invalid\n";
        } else if (choice == 1) {
            if (totalUsers < MAX_USERS) {
                cout << "Username: ";
                cin >> users[totalUsers];
                cout << "Password: ";
                cin >> passes[totalUsers];
                userScores[totalUsers] = 0;
                totalUsers++;
            } else {
                cout << "User limit reached\n";
            }
        } else if (choice == 2) {
            string name, pass;
            cout << "Username: ";
            cin >> name;
            cout << "Password: ";
            cin >> pass;
            int idx = findUser(name, pass);
            if (idx == -1) {
                cout << "Invalid login\n";
            } else {
                int qIndex[LEVELS] = {0,0,0};
                int level = 0;
                int wrongStreak = 0;
                int perLevelScore[LEVELS] = {0,0,0};
                userScores[idx] = 0;

                while (level < LEVELS) {
                    if (qIndex[level] >= Q_PER_LEVEL) { level++; wrongStreak = 0; continue; }
                    int ans = askQuestionInteractive(level, qIndex[level]);
                    attempts[level][qIndex[level]]++;
                    qIndex[level]++;

                    if (ans == correctAns[level][qIndex[level]-1]) {
                        corrects[level][qIndex[level]-1]++;
                        cout << "Correct answer\n";
                        int pts = 10 * (level + 1);
                        userScores[idx] += pts;
                        perLevelScore[level] += pts;
                        wrongStreak = 0;
                        level++;
                    } else {
                        cout << "Wrong answer\n";
                        wrongStreak++;
                        if (wrongStreak >= 2) {
                            cout << "Backtrack to easier level\n";
                            wrongStreak = 0;
                            level--;
                            if (level < 0) level = 0;
                        }
                    }

                    cout << "Current Score: " << userScores[idx] << "\n";
                    if (level < LEVELS) {
                        int rem = Q_PER_LEVEL - qIndex[level];
                        if (rem < 0) rem = 0;
                        cout << "Questions remaining in " << levelName[level] << ": " << rem << "\n";
                    }
                }

                cout << "\nFinal Score: " << userScores[idx] << "\n";
                addOverallLeaderboard(name, userScores[idx]);
                for (int l = 0; l < LEVELS; ++l) addLevelLeaderboard(l, name, perLevelScore[l]);
            }
        } else {
            cout << "Invalid\n";
        }
    }
    return 0;
}
