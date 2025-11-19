#include <iostream>
#include <string>
#include <fstream>

using namespace std;

const int MAX_USERS = 10;
const int LEVELS = 3;
const int Q_PER_LEVEL = 4;

int totalUsers = 0;
string users[MAX_USERS];
string passes[MAX_USERS];
int userScores[MAX_USERS];
int userPerLevelScores[MAX_USERS][LEVELS] = {{0}};

string levelName[LEVELS] = {"Easy", "Moderate", "Difficult"};

string qText[LEVELS][Q_PER_LEVEL] = {
    {
        "2+2 = ?",
        "Color of the sky on a clear day?",
        "5-3 = ?",
        "Which number is even?"
    },
    {
        "What is 12 * 12?",
        "Binary representation: 5 is ?",
        "Square root of 81?",
        "Binary representation: 10 is ?"
    },
    {
        "Derivative of x^2 ?",
        "Time complexity of binary search?",
        "Which algorithm uses backtracking?",
        "Time complexity of merge sort?"
    }
};

string qOpt[LEVELS][Q_PER_LEVEL][4] = {
    {
        {"1) 3","2) 4","3) 5","4) 6"},
        {"1) Blue","2) Green","3) Red","4) Black"},
        {"1) 1","2) 2","3) 3","4) 4"},
        {"1) 3","2) 5","3) 8","4) 7"}
    },
    {
        {"1) 132","2) 144","3) 154","4) 124"},
        {"1) 101","2) 110","3) 011","4) 100"},
        {"1) 7","2) 8","3) 9","4) 6"},
        {"1) 1010","2) 0101","3) 1110","4) 1001"}
    },
    {
        {"1) x","2) 2x","3) x^2","4) 1"},
        {"1) O(n)","2) O(log n)","3) O(n^2)","4) O(1)"},
        {"1) Dijkstra","2) BFS","3) Rat in a Maze","4) Merge Sort"},
        {"1) O(n)","2) O(n log n)","3) O(n^2)","4) O(log n)"}
    }
};

int correctAns[LEVELS][Q_PER_LEVEL] = {
    {2,1,2,1},
    {2,1,3,1},
    {2,2,3,2}
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

void rebuildLeaderboards() {
    lbCountOverall = 0;
    for (int l = 0; l < LEVELS; ++l) lbCountPerLevel[l] = 0;
    for (int i = 0; i < totalUsers; ++i) {
        addOverallLeaderboard(users[i], userScores[i]);
        for (int l = 0; l < LEVELS; ++l)
            addLevelLeaderboard(l, users[i], userPerLevelScores[i][l]);
    }
}

void saveData() {
    ofstream out("users.txt");
    if (!out) return;
    for (int i = 0; i < totalUsers; ++i) {
        out << users[i] << " "
            << passes[i] << " "
            << userScores[i] << " "
            << userPerLevelScores[i][0] << " "
            << userPerLevelScores[i][1] << " "
            << userPerLevelScores[i][2] << "\n";
    }
    out.close();
}

void loadData() {
    ifstream in("users.txt");
    if (!in) return;
    totalUsers = 0;
    string name, pass;
    int total, p0, p1, p2;
    while (in >> name >> pass >> total >> p0 >> p1 >> p2) {
        if (totalUsers >= MAX_USERS) break;
        users[totalUsers] = name;
        passes[totalUsers] = pass;
        userScores[totalUsers] = total;
        userPerLevelScores[totalUsers][0] = p0;
        userPerLevelScores[totalUsers][1] = p1;
        userPerLevelScores[totalUsers][2] = p2;
        totalUsers++;
    }
    in.close();
    rebuildLeaderboards();
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
    cout << "\n[" << levelName[lvl] << "]\n";
    cout << qText[lvl][qIdx] << "\n";
    for (int i = 0; i < 4; ++i)
        cout << qOpt[lvl][qIdx][i] << endl;
    cout << "Enter option number: ";
    int ans = askInt();
    return ans;
}

void showOverallLeaderboard() {
    if (lbCountOverall == 0) {
        cout << "Leaderboard empty\n";
        return;
    }

    cout << "\nNAME\t\tSCORE\n";
    cout << "---------------------------\n";

    for (int i = 0; i < lbCountOverall; i++) {
        cout << lbNamesOverall[i] << "\t\t" << lbScoresOverall[i] << "\n";
    }
}

int main() {

    loadData();

    int running = 1;
    while (running) {
        cout << "\n=========================\n";
        cout << "     STUDENT DASHBOARD    \n";
        cout << "=========================\n";
        cout << "\n Main Menu: \n";
        cout << "1) Student Sign Up\n";
        cout << "2) Student Log In\n";
        cout << "3) Student Leaderboard\n";
        cout << "4) Exit\n";
        cout << "Choose: ";

        int choice = askInt();
        if (choice == 4) {
            running = 0;
        } 
        else if (choice == 3) {
            cout << "\n--- Overall Leaderboard ---\n";
            showOverallLeaderboard();
        } 
        else if (choice == 1) {
            if (totalUsers < MAX_USERS) {
                cout << "Username: ";
                cin >> users[totalUsers];
                cout << "Password: ";
                cin >> passes[totalUsers];
                userScores[totalUsers] = 0;
                for (int l = 0; l < LEVELS; ++l) userPerLevelScores[totalUsers][l] = 0;
                totalUsers++;
                rebuildLeaderboards();
                saveData();
            } else {
                cout << "User limit reached\n";
            }
        } 
        else if (choice == 2) {
            string name, pass;
            cout << "Username: ";
            cin >> name;
            cout << "Password: ";
            cin >> pass;

            int idx = findUser(name, pass);
            if (idx == -1) {
                cout << "Invalid login\n";
            } 
            else {
                int innerMenu = 1;
                while (innerMenu) {

                    cout << "\n=========================\n";
                    cout << "     STUDENT DASHBOARD    \n";
                    cout << "=========================\n";
                    cout << "1) View Profile\n";
                    cout << "2) Start Quiz\n";
                    cout << "3) Log Out\n";
                    cout << "4) View Leaderboard\n";
                    cout << "Choose: ";

                    int opt = askInt();

                    if (opt == 3) {
                        innerMenu = 0;
                    }
                    else if (opt == 1) {
                        cout << "\n----- PROFILE -----\n";
                        cout << "Username: " << users[idx] << "\n";
                        cout << "Total Score: " << userScores[idx] << "\n";
                    }
                    else if (opt == 4) {
                        cout << "\n--- Overall Leaderboard ---\n";
                        showOverallLeaderboard();
                    }
                    else if (opt == 2) {

                        int qIndex[LEVELS] = {0,0,0};
                        int level = 0;
                        int wrongStreak = 0;
                        int perLevelScore[LEVELS] = {0,0,0};
                        int penalty = 5;

                        while (level < LEVELS) {
                            if (qIndex[level] >= Q_PER_LEVEL) { 
                                level++; 
                                wrongStreak = 0; 
                                continue; 
                            }

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
                                int correctIndex = correctAns[level][qIndex[level]-1] - 1;
                                cout << "Correct answer: " << qOpt[level][qIndex[level]-1][correctIndex] << "\n";
                                if (userScores[idx] <= penalty) userScores[idx] = 0;
                                else userScores[idx] -= penalty;
                                if (perLevelScore[level] <= penalty) perLevelScore[level] = 0;
                                else perLevelScore[level] -= penalty;
                                wrongStreak++;
                                if (wrongStreak >= 2) {
                                    cout << "Backtrack to easier level\n";
                                    wrongStreak = 0;
                                    level--;
                                    if (level < 0) level = 0;
                                }
                            }

                            if (level < LEVELS) {
                                int rem = Q_PER_LEVEL - qIndex[level];
                                if (rem < 0) rem = 0;
                                cout << "Questions remaining in " << levelName[level] << ": " << rem << "\n";
                            }
                        }

                        int totalCorrect = 0;
                        int totalWrong = 0;
                        for (int l = 0; l < LEVELS; l++) {
                            for (int q = 0; q < Q_PER_LEVEL; q++) {
                                if (corrects[l][q] > 0) totalCorrect++;
                                else if (attempts[l][q] > 0) totalWrong++;
                            }
                        }

                        cout << "\n=========================\n";
                        cout << "       QUIZ REPORT       \n";
                        cout << "=========================\n";
                        cout << "Username: " << users[idx] << "\n";
                        cout << "Total Score: " << userScores[idx] << "\n\n";
                        cout << "Correct Answers: " << totalCorrect << "\n";
                        cout << "Wrong Answers: " << totalWrong << "\n";
                        cout << "=========================\n";

                        for (int l = 0; l < LEVELS; ++l)
                            userPerLevelScores[idx][l] += perLevelScore[l];

                        rebuildLeaderboards();
                        saveData();
                    }
                    else {
                        cout << "Invalid choice\n";
                    }
                }
            }
        } 
        else {
            cout << "Invalid\n";
        }
    }
    return 0;
}
