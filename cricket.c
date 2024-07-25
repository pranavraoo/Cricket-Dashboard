#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_PLAYERS 11

struct Players {
    char name[50];
    int runsScored;
    int ballsPlayed; 
};

struct batOrder {
    struct Players players[MAX_PLAYERS];
    int top;
};

struct bowlOrder {
    struct Players players[MAX_PLAYERS];
    int front, rear;
};

struct Team {
    char name[50];
    struct batOrder batting;
    struct bowlOrder bowling;
    int totalRuns;
    int wicketsLost;
    float oversBowled;
};

void initBattingOrder(struct batOrder *order) {
    order->top = -1;
}

void initBowlingOrder(struct bowlOrder *order) {
    order->front = order->rear = -1;
}

bool isBattingOrderFull(struct batOrder *order) {
    return (order->top == MAX_PLAYERS - 1);
}

bool isBowlingOrderFull(struct bowlOrder *order) {
    return (order->rear == MAX_PLAYERS - 1);
}

void pushBattingOrder(struct batOrder *order, struct Players players) {
    if (isBattingOrderFull(order)) {
        printf("Batting order is full. Cannot add more players.\n");
        return;
    }
    order->players[++(order->top)] = players;
}

void enqueueBowlingOrder(struct bowlOrder *order, struct Players players) {
    if (isBowlingOrderFull(order)) {
        printf("Bowling order is full. Cannot add more players.\n");
        return;
    }
    if (order->front == -1) {
        order->front = 0;
    }
    order->players[++(order->rear)] = players;
}

void displayTeamScore(struct Team team) {
    printf("\n----------------------- %s Scoreboard -----------------------\n", team.name);
    printf("Batsman\t\tRuns Scored\tBalls Played\tStrike Rate\n");
    for (int i = 0; i <= team.batting.top; i++) {
        struct Players currentPlayer = team.batting.players[i];
        float strikeRate = (float)currentPlayer.runsScored / (currentPlayer.ballsPlayed > 0 ? currentPlayer.ballsPlayed : 1);

        printf("%s\t\t%d\t\t%d\t\t%.2f\n", currentPlayer.name, currentPlayer.runsScored, currentPlayer.ballsPlayed, strikeRate);
    }
    printf("Total Runs: %d\nWickets Lost: %d\nOvers Bowled: %.1f\n", team.totalRuns, team.wicketsLost, team.oversBowled);
}

void enterTeamInformation(struct Team *team, int teamNumber) {
    printf("\nEnter details for Team %d\n", teamNumber);
    printf("Enter team name: ");
    fgets(team->name, sizeof(team->name), stdin);
    team->name[strcspn(team->name, "\n")] = 0;

    initBattingOrder(&team->batting);
    initBowlingOrder(&team->bowling);

    int totalRuns = 0;
    int totalBalls = 0;

    for (int i = 0; i < MAX_PLAYERS; i++) {
        struct Players newPlayers;
        printf("Enter name for player %d: ", i + 1);
        fgets(newPlayers.name, sizeof(newPlayers.name), stdin);
        newPlayers.name[strcspn(newPlayers.name, "\n")] = 0;
        printf("Enter runs scored for %s: ", newPlayers.name);
        scanf("%d", &newPlayers.runsScored);
        printf("Enter balls played for %s: ", newPlayers.name);
        scanf("%d", &newPlayers.ballsPlayed);

        totalRuns += newPlayers.runsScored;
        totalBalls += newPlayers.ballsPlayed;

        pushBattingOrder(&team->batting, newPlayers);
        enqueueBowlingOrder(&team->bowling, newPlayers);

        while (getchar() != '\n');
    }

    team->totalRuns = totalRuns;

    printf("Total Runs by players: %d\n", team->totalRuns);
    
    int totalOvers = totalBalls / 6;
    int remainingBalls = totalBalls % 6;
    team->oversBowled = totalOvers + (float)remainingBalls / 10;

    printf("Total Overs bowled: %.1f\n", team->oversBowled);

    int extras;
    printf("\nEnter extras provided in runs: ");
    scanf("%d", &extras);
    team->totalRuns += extras;

    printf("Extras added to total runs: %d\n", extras);
}

typedef struct {
    int matchesPlayed;
    int runsScored;
    float battingAverage;
    int fifties;
    int centuries;
} BattingStats;

typedef struct {
    int matchesPlayed;
    int wicketsTaken;
    float economy;
} BowlingStats;


typedef struct {
    char playerName[50];
    char playerType;
    BattingStats batting;
    BowlingStats bowling;
} CricketPlayerCard;

typedef struct Node {
    CricketPlayerCard data;
    struct Node* next;
} Node;

Node* initializeStack() {
    return NULL;
}
Node* push(Node* top, CricketPlayerCard player) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) {
        printf("Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }

    newNode->data = player;
    newNode->next = top;
    return newNode;
}

Node* pop(Node* top) {
    if (top == NULL) {
        printf("Stack is empty.\n");
        return NULL;
    }

    Node* temp = top;
    top = top->next;
    free(temp);
    return top;
}

typedef struct QueueNode {
    CricketPlayerCard data;
    struct QueueNode* next;
} QueueNode;

typedef struct {
    QueueNode* front;
    QueueNode* rear;
} Queue;

Queue* initializeQueue() {
    Queue* queue = (Queue*)malloc(sizeof(Queue));
    if (queue == NULL) {
        printf("Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }

    queue->front = queue->rear = NULL;
    return queue;
}

void enqueue(Queue* queue, CricketPlayerCard player) {
    QueueNode* newNode = (QueueNode*)malloc(sizeof(QueueNode));
    if (newNode == NULL) {
        printf("Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }

    newNode->data = player;
    newNode->next = NULL;

    if (queue->rear == NULL) {
        queue->front = queue->rear = newNode;
        return;
    }

    queue->rear->next = newNode;
    queue->rear = newNode;
}

Queue* dequeue(Queue* queue) {
    if (queue->front == NULL) {
        printf("Queue is empty.\n");
        return queue;
    }

    QueueNode* temp = queue->front;
    queue->front = queue->front->next;
    free(temp);

    if (queue->front == NULL) {
        queue->rear = NULL;
    }

    return queue;
}

typedef struct PointTable {
    char name[50];
    int wins;
    int losses;
    int draws;
    int points;
    struct PointTable *next;
} PointTable;


typedef struct MatchNode {
    int matchID;
    char teamName1[50];
    int runsTeam1;
    int wicketsTeam1;
    int oversTeam1;
    char teamName2[50];
    int runsTeam2;
    int wicketsTeam2;
    int oversTeam2;
    char dateTime[50];
    struct MatchNode* left;
    struct MatchNode* right;
} MatchNode;

int matchCounter = 1;

MatchNode* createMatchNode() {
    MatchNode* newMatch = (MatchNode*)malloc(sizeof(MatchNode));
    if (newMatch != NULL) {
        newMatch->matchID = matchCounter++;
        newMatch->left = NULL;
        newMatch->right = NULL;
        printf("Enter Team 1 name: ");
        scanf(" %[^\n]", newMatch->teamName1);
        printf("Enter Team 2 name: ");
        scanf(" %[^\n]", newMatch->teamName2);
    }
    return newMatch;
}

MatchNode* insertMatch(MatchNode* root, MatchNode* newMatch) {
    if (root == NULL) {
        return newMatch;
    }
    if (strcmp(newMatch->dateTime, root->dateTime) < 0) {
        root->left = insertMatch(root->left, newMatch);
    } else {
        root->right = insertMatch(root->right, newMatch);
    }
    return root;
}

void addMatch(MatchNode** tournamentBracket) {
    char choice;
    do {
        MatchNode* newMatch = createMatchNode();

        printf("Enter the number of runs for %s: ", newMatch->teamName1);
        scanf("%d", &newMatch->runsTeam1);

        printf("Enter the number of wickets for %s: ", newMatch->teamName1);
        scanf("%d", &newMatch->wicketsTeam1);

        printf("Enter the total overs played by Team 1: ");
        scanf("%d", &newMatch->oversTeam1);

        printf("Enter the number of runs for %s: ", newMatch->teamName2);
        scanf("%d", &newMatch->runsTeam2);

        printf("Enter the number of wickets for %s: ", newMatch->teamName2);
        scanf("%d", &newMatch->wicketsTeam2);

        printf("Enter the total overs played by Team 2: ");
        scanf("%d", &newMatch->oversTeam2);

        printf("Enter the date and time of the match (e.g., 'November 20, 2023 - 15:30'): ");
        scanf(" %[^\n]", newMatch->dateTime);

        *tournamentBracket = insertMatch(*tournamentBracket, newMatch);

        printf("Do you want to add another match? (y/n): ");
        scanf(" %c", &choice);
    } while (choice == 'y' || choice == 'Y');
}

void displayMatches(MatchNode* root) {
    if (root != NULL) {
        displayMatches(root->left);

        char* winningTeam;
        if (root->runsTeam1 > root->runsTeam2) {
            winningTeam = root->teamName1;
        } else if (root->runsTeam1 < root->runsTeam2) {
            winningTeam = root->teamName2;
        } else {
            winningTeam = "Match Tied";
        }

        printf("Match ID: %d\nMatch Details:\nTeam 1: %s\nRuns - %d, Wickets - %d, Overs - %d\nTeam 2: %s\nRuns - %d, Wickets - %d, Overs - %d\nDate & Time: %s\nWinner: %s\n\n",
               root->matchID, root->teamName1, root->runsTeam1, root->wicketsTeam1, root->oversTeam1, root->teamName2, root->runsTeam2, root->wicketsTeam2, root->oversTeam2, root->dateTime, winningTeam);
        
        displayMatches(root->right);
    }
}

PointTable* createTeam(char name[], int wins, int losses, int draws);
void insertTeam(PointTable **head, char name[], int wins, int losses, int draws);
void displayPointsTable(PointTable *head);

typedef struct {
    char playerName[50];
} Player;


typedef struct Noded {
    char teamName[50];
    Player players[11];
    struct Noded* next;
} TeamNode;


TeamNode* createTeamNode() {
    TeamNode* newNode = (TeamNode*)malloc(sizeof(TeamNode));
    newNode->next = NULL;
    return newNode;
}

void addTeam(TeamNode** head) {
    TeamNode* newTeam = createTeamNode();

    printf("Enter the name of the team: ");
    scanf("%s", newTeam->teamName);

    printf("Enter the names of 11 players:\n");
    for (int i = 0; i < 11; ++i) {
        printf("Player %d: ", i + 1);
        scanf("%s", newTeam->players[i].playerName);
    }

    
    newTeam->next = *head;
    *head = newTeam;
}


void displayTeams(TeamNode* head) {
    TeamNode* current = head;

    while (current != NULL) {
        printf("\nTeam: %s\n", current->teamName);
        printf("Players:\n");
        for (int i = 0; i < 11; ++i) {
            printf("  %d. %s\n", i + 1, current->players[i].playerName);
        }

        current = current->next;
    }
}

void displayTournamentBracket(MatchNode* root) {
    if (root != NULL) {
        displayTournamentBracket(root->left);
        printf("Match details: Match ID - %d, RunsTeam1 - %d, WicketsTeam1 - %d, RunsTeam2 - %d, WicketsTeam2 - %d\n",
               root->matchID, root->runsTeam1, root->wicketsTeam1, root->runsTeam2, root->wicketsTeam2);
        displayTournamentBracket(root->right);
    }
}

void displayBatsmen(CricketPlayerCard players[], int num) {
    printf("\nBatsmen Details:\n");
    for (int i = 0; i < num; i++) {
        if (players[i].playerType == 'B' || players[i].playerType == 'b') {
            printf("\nPlayer Name: %s\n", players[i].playerName);
            printf("Batting Stats:\n");
            printf("Matches Played: %d\n", players[i].batting.matchesPlayed);
            printf("Runs Scored: %d\n", players[i].batting.runsScored);
            printf("Batting Average: %.2f\n", players[i].batting.battingAverage);
            printf("Fifties: %d\n", players[i].batting.fifties);
            printf("Centuries: %d\n", players[i].batting.centuries);
        }
    }
}

void displayBowlers(CricketPlayerCard players[], int num) {
    printf("\nBowlers Details:\n");
    for (int i = 0; i < num; i++) {
        if (players[i].playerType == 'O' || players[i].playerType == 'o') {
            printf("\nPlayer Name: %s\n", players[i].playerName);
            printf("Bowling Stats:\n");
            printf("Matches Played: %d\n", players[i].bowling.matchesPlayed);
            printf("Wickets Taken: %d\n", players[i].bowling.wicketsTaken);
            printf("Economy: %.2f\n", players[i].bowling.economy);
        }
    }
}

struct Player {
    char name[50];
    char role[10];
    int statistics;
    struct Player* next;
};

void insertPlayer(struct Player** head, char name[], char role[], int statistics) {
    struct Player* newPlayer = (struct Player*)malloc(sizeof(struct Player));
    strcpy(newPlayer->name, name);
    strcpy(newPlayer->role, role);
    newPlayer->statistics = statistics;
    newPlayer->next = NULL;

    if (*head == NULL) {
        *head = newPlayer;
    } else {
        struct Player* current = *head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newPlayer;
    }
}

void sortPlayers(struct Player** head) {
    struct Player* current;
    struct Player* next;
    int tempStatistics;
    char tempName[50];
    char tempRole[10];

    if (*head == NULL) {
        return;
    }

    for (current = *head; current->next != NULL; current = current->next) {
        for (next = current->next; next != NULL; next = next->next) {
            if (current->statistics < next->statistics) {
                tempStatistics = current->statistics;
                strcpy(tempName, current->name);
                strcpy(tempRole, current->role);

                current->statistics = next->statistics;
                strcpy(current->name, next->name);
                strcpy(current->role, next->role);

                next->statistics = tempStatistics;
                strcpy(next->name, tempName);
                strcpy(next->role, tempRole);
            }
        }
    }
}

void printPlayers(struct Player* head) {
    struct Player* current = head;
    while (current != NULL) {
        printf("Name: %s, Role: %s, Statistics: %d\n", current->name, current->role, current->statistics);
        current = current->next;
    }
}

void inputBatsmanDetails(CricketPlayerCard *player) {
    player->playerType = 'B';

    printf("Enter Batting Stats for %s:\n", player->playerName);
    printf("Enter number of matches played: ");
    scanf("%d", &player->batting.matchesPlayed);

    printf("Enter total runs scored: ");
    scanf("%d", &player->batting.runsScored);

    printf("Enter batting average: ");
    scanf("%f", &player->batting.battingAverage);

    printf("Enter number of fifties: ");
    scanf("%d", &player->batting.fifties);

    printf("Enter number of centuries: ");
    scanf("%d", &player->batting.centuries);
}

void inputBowlerDetails(CricketPlayerCard *player) {
    player->playerType = 'O';

    printf("Enter Bowling Stats for %s:\n", player->playerName);
    printf("Enter number of matches played: ");
    scanf("%d", &player->bowling.matchesPlayed);

    printf("Enter number of wickets taken: ");
    scanf("%d", &player->bowling.wicketsTaken);

    printf("Enter economy rate: ");
    scanf("%f", &player->bowling.economy);
}

int main() {
    int numTeam,num;
    printf("Enter the number of teams (up to 10): ");
    scanf("%d", &numTeam);
    printf("Enter the number of players who's stats are to be checked:");
    scanf("%d",&num);
    PointTable *pointsTable = NULL;
    MatchNode* tournamentBracket = NULL;   
    TeamNode* head = NULL;
    Node* battingStack = initializeStack();
    Queue* bowlingQueue = initializeQueue();
    struct Player* batters = NULL;
    struct Player* bowlers = NULL;
    struct Team teamA, teamB;
    CricketPlayerCard players[num];
    int n;
    if(numTeam<=10){
    int choice;
    do {
        printf("\nChoose an option:\n");
        printf("1. View Points Table\n");
        printf("2. View Match details\n");
        printf("3. View Team information \n");
        printf("4. View Player Tournament statistics \n");
        printf("5. View Player All time statistics \n");
        printf("6. View Scoreboards \n");
        printf("7. Enter the Match details\n");
        printf("8. Enter Team information \n");
        printf("9. Enter Player tournament statistics \n");
        printf("10. Enter Player All time statistics \n");
        printf("11. Enter Scoreboard information \n");
        printf("12. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                for (int i = 1; i <= numTeam; i++) {
                    char teamName[50];
                    int teamWins, teamLosses, teamDraws;
                    printf("Enter Team %d name: ", i);
                    scanf("%s", teamName);
                    printf("Enter number of wins for Team %d: ", i);
                    scanf("%d", &teamWins);
                    printf("Enter number of losses for Team %d: ", i);
                    scanf("%d", &teamLosses);
                    printf("Enter number of draws for Team %d: ", i);
                    scanf("%d", &teamDraws);
                    insertTeam(&pointsTable, teamName, teamWins, teamLosses, teamDraws);
                    }
                displayPointsTable(pointsTable);
                break;
            case 2:
                displayMatches(tournamentBracket);
                break;
            
            case 3:
                displayTeams(head);
                break;
            case 4:
                char ch;
                printf("Do you want to view batting or bowling stats? (b/o)\n");
                scanf(" %c", &ch); 
                if (ch == 'B' || ch == 'b') {
                    sortPlayers(&batters);
                    printf("\nSorted Batters:\n");
                    printPlayers(batters);
                    break;
                } 
                else if (ch == 'O' || ch == 'o') {
                    sortPlayers(&bowlers);
                    printf("\nSorted Bowlers:\n");
                    printPlayers(bowlers);
                    break;
                }
                else {
                    printf("Invalid choice\n");
                    break;
                }
            case 5:
                printf("Displaying Player Cards:\n");
                char displaychoice;
                printf("Do you want to view batting or bowling stats? (b/o)\n");
                scanf(" %c", &displaychoice); 
                if (displaychoice == 'b' || displaychoice == 'B') {
                    displayBatsmen(players, num);
                }
                else if (displaychoice == 'o' || displaychoice == 'O') {
                    displayBowlers(players, num); 
                } 
                else {
                    printf("Invalid choice\n");
                }
                break;
            case 6:
                printf("\nDisplay Team Score:\n");
                printf("1. Team A\n");
                printf("2. Team B\n");
                printf("Enter your choice: ");
                scanf("%d", &choice);

                while (getchar() != '\n'); 

                if (choice == 1) {
                    printf("\nTeam A Score:\n");
                    displayTeamScore(teamA);
                } else if (choice == 2) {
                    printf("\nTeam B Score:\n");
                    displayTeamScore(teamB);
                } else {
                    printf("Invalid choice. Please enter 1 or 2.\n");
                }
                break;
            case 7:
                addMatch(&tournamentBracket);
                break;
            case 8:
                do {
                    addTeam(&head);
                    printf("Do you want to add another team? (y/n): ");
                    scanf(" %c", &choice); 
                } while (choice == 'y' || choice == 'Y');
                    break;
            case 9:
                
                for (int i = 0; i < n; i++) {
                    char name[50];
                    char role[10];
                    int statistics;

                    printf("Enter the name of player %d: ", i + 1);
                    scanf("%s", name);
                    printf("Enter the role of player %d (Batter/Bowler): ", i + 1);
                    scanf("%s", role);

                    if (strcmp(role, "Batter") == 0) {
                        printf("Enter the runs of the batter: ");
                        scanf("%d", &statistics);
                        insertPlayer(&batters, name, role, statistics);
                    } 
                    else if (strcmp(role, "Bowler") == 0) {
                        printf("Enter the wickets of the bowler: ");
                        scanf("%d", &statistics);
                        insertPlayer(&bowlers, name, role, statistics);
                    } 
                    else {
                        printf("Invalid role. Player not added.\n");
                    }
                }
                break;
            case 10:
                for (int i = 0; i < num; i++) {
                    printf("\nEnter details for Player %d:\n", i + 1);
                    printf("Enter Player's name: ");
                    scanf("%s", players[i].playerName);

                    char playerType;
                    printf("Enter player type - 'B' for Batsman, 'O' for Bowler: ");
                    scanf(" %c", &playerType);

                    if (playerType == 'B' || playerType == 'b') {
                        printf("Enter Batting Stats for Player %d:\n", i + 1);
                        inputBatsmanDetails(&players[i]);
                    } 
                    else if (playerType == 'O' || playerType == 'o') {
                        printf("Enter Bowling Stats for Player %d:\n", i + 1);
                        inputBowlerDetails(&players[i]);
                    } 
                    else {
                        printf("Invalid player type entered.\n");
                        i--;
                    }
                }
                break;
            case 11:
                printf("\nEnter details for Team:\n");
                printf("1. Team A\n");
                printf("2. Team B\n");
                printf("Enter your choice: ");
                scanf("%d", &choice);

                while (getchar() != '\n'); 

                if (choice == 1)
                    enterTeamInformation(&teamA, 1);
                else if (choice == 2)
                    enterTeamInformation(&teamB, 2);
                else
                    printf("Invalid choice. Please enter 1 or 2.\n");
                break;
            case 12:
                printf("Exiting the program.\n");
                break;
            default:
                printf("Invalid choice. Please enter a valid option.\n");
                break;
        }
    } while (choice != 12);
    }
    else{
        printf("Invalid amount");
         }
    return 0;
}

PointTable* createTeam(char name[], int wins, int losses, int draws) {
    PointTable *newTeam = (PointTable*)malloc(sizeof(PointTable));
    strcpy(newTeam->name, name);
    newTeam->wins = wins;
    newTeam->losses = losses;
    newTeam->draws = draws;
    newTeam->points = (wins * 2) + draws;
    newTeam->next = NULL;
    return newTeam;
}

void insertTeam(PointTable **head, char name[], int wins, int losses, int draws) {
    PointTable *newTeam = createTeam(name, wins, losses, draws);

    if (*head == NULL) {
        *head = newTeam;
    } else {
        PointTable *current = *head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newTeam;
    }
}

void sortPointsTable(PointTable *head) {
    int swapped;
    PointTable *ptr1;
    PointTable *lptr = NULL;
    if (head == NULL)
        return;

    do {
        swapped = 0;
        ptr1 = head;

        while (ptr1->next != lptr) {
            if (ptr1->points < ptr1->next->points) {
                int temp = ptr1->points;
                ptr1->points = ptr1->next->points;
                ptr1->next->points = temp;
                
                char tempName[100];
                strcpy(tempName, ptr1->name);
                strcpy(ptr1->name, ptr1->next->name);
                strcpy(ptr1->next->name, tempName);
                swapped = 1;
            }
            ptr1 = ptr1->next;
        }
        lptr = ptr1;
    } while (swapped);
}
void displayPointsTable(PointTable *head) {
    sortPointsTable(head);
    PointTable *current = head;
    printf("Cricket Points Table (Sorted in Descending Order):\n");
    printf("%-8s | Wins | Losses | Draws | Points\n", "Team");
    printf("---------------------------------------------\n");
    while (current != NULL) {
        printf("%-8s | %4d | %6d | %5d | %6d\n", current->name, current->wins, current->losses, current->draws, current->points);
        current = current->next;
    }
}

void addToTournamentBracket(MatchNode** tournamentBracket, MatchNode* newMatch) {
    if (*tournamentBracket == NULL) {
        *tournamentBracket = newMatch;
    } else {
        if (newMatch->matchID < (*tournamentBracket)->matchID) {
            addToTournamentBracket(&((*tournamentBracket)->left), newMatch);
        } else {
            addToTournamentBracket(&((*tournamentBracket)->right), newMatch);
        }
    }
}

