#include <iostream>
#include <cstdlib>
#include <time.h>
#include <cassert>
#include <assert.h>
// uncomment to disable assert():
//#define NDEBUG

using namespace std;

struct Spielwelt {
    int twod_world[5][5];
};

struct Charakter {
    int Lebenspunkte;
    int Reliktpunkte;
    int x;
    int y;
};

struct Gegner {
    int Lebenspunkte;
    int x;
    int y;
};


struct Spielwelt generateField(){
    // generate a game field with the condition that there has to be at least one relic hidden on it
    struct Spielwelt field;
    int relic_count = 0;
    do{
        // fill the game 5 by 5 field with randomly generated integers between 1 and 10
        for(int i = 0; i < 5; i++){
            for(int j = 0; j < 5; j++){
                field.twod_world[i][j] = rand() % 10 + 1;
            }
        }

        // check if there is at least one relic hidden on the field
        for(int i = 0; i < 5; i++){
            for(int j = 0; j < 5; j++){
                if(field.twod_world[i][j] == 10){
                    relic_count = 1;
                }
            }
        }
    }while(relic_count == 0);
    return field;
}

void printField(struct Spielwelt& field, struct Gegner& enemy_1, struct Gegner& enemy_2){
    // print the field
    for(int i = 0; i < 5; i++){
        for(int j = 0; j < 5; j++){
            // if the enemy should be displayed uncomment the following lines
            /*
            if(enemy_1.Lebenspunkte > 0 && i == enemy_1.y && j == enemy_1.x){
                cout << "G ";
                continue;
            }

            if(enemy_2.Lebenspunkte > 0 && i == enemy_2.y && j == enemy_2.x){
                cout << "G ";
                continue;
            }
            */
            assert(field.twod_world[i][j] >= 1 && field.twod_world[i][j] <= 10);

            if(field.twod_world[i][j] >= 1 && field.twod_world[i][j] <= 4){
                cout << "E ";
            } else if(field.twod_world[i][j] >= 5 && field.twod_world[i][j] <= 8){
                cout << "D ";
            } else if(field.twod_world[i][j] == 9){
                cout << "Q ";
            } else if(field.twod_world[i][j] == 10){
                cout << "R ";
            }
        }
        cout << endl;
    }
    cout << endl;
}

void printHero(struct Charakter& hero){
    assert(hero.x >= 0 && hero.x <= 4);
    assert(hero.y >= 0 && hero.y <= 4);

    cout << "Leben: " << hero.Lebenspunkte << endl;
    cout << "Relikte gefunden: " << hero.Reliktpunkte << endl;
    cout << "X-Koordinate: " << hero.x << endl;
    cout << "Y-Koordinate: " << hero.y << endl;
}

int countHiddenRelics(struct Spielwelt& field){
    int relic_hidden = 0;
    // count the number of relics present on the field
    for(int i = 0; i < 5; i++){
        for(int j = 0; j < 5; j++){
            assert(field.twod_world[i][j] >= 1 && field.twod_world[i][j] <= 10);
            if(field.twod_world[i][j] == 10){
                relic_hidden += 1;
            }
        }
    }
    return relic_hidden;
}

void enemyMove(struct Gegner& enemy){
    assert(enemy.x >= 0 && enemy.x <= 4);
    assert(enemy.y >= 0 && enemy.y <= 4);

    int enemy_move = rand() % 4 + 1;
    if(enemy_move == 1){
        if(enemy.y == 0){
            enemy.y = 4;
        } else {
            enemy.y -= 1;
        }
    } else if(enemy_move == 2){
        if(enemy.x == 0){
            enemy.x = 4;
        } else {
            enemy.x -= 1;
        }
    } else if(enemy_move == 3){
        if(enemy.y == 4){
            enemy.y = 0;
        } else {
            enemy.y += 1;
        }
    } else if(enemy_move == 4){
        if(enemy.x == 4){
            enemy.x = 0;
        } else {
            enemy.x += 1;
        }
    }
}

void enemyFight(struct Charakter& hero, struct Gegner& enemy, int& round, int& Sieg_counter, int& Gegner_counter,
                int (&Gegner_besiegt)[100], int (&Schaden_Gegner)[100]){

    // if hero and enemy meet let them both win with a probability of 50%
    if(enemy.y == hero.y && enemy.x == hero.x){
        int fight = rand() % 6 + 1;
        if(1 <= fight && fight <= 3){
            enemy.Lebenspunkte -= 1;
            Gegner_besiegt[Sieg_counter] = round;
            Sieg_counter++;
        } else {
            hero.Lebenspunkte -= 2;
            Schaden_Gegner[Gegner_counter] = round;
            Gegner_counter++;
        }
    }
}

void Spielverlauf(int Gefahr_counter, int Quelle_counter, int Relikt_counter, int Gegner_counter, int Sieg_counter,
                  int Schaden_Gefahr[100], int Quelle_Gefunden[100], int Relikt_Gefunden[100], int Schaden_Gegner[100], int Gegner_besiegt[100]){

    cout << "Spielverlauf: " << endl;
    cout << "Schaden durch Gefahr erlitten in Runde: ";
    for(int i = 0; i < Gefahr_counter; i++){
        cout << Schaden_Gefahr[i] << " ";
    }
    cout << endl;

    cout << "Quelle gefunden in Runde: ";
    for(int j = 0; j < Quelle_counter; j++){
        cout << Quelle_Gefunden[j] << " ";
    }
    cout << endl;

    cout << "Relikt gefunden in Runde: ";
    for(int k = 0; k < Relikt_counter; k++){
        cout << Relikt_Gefunden[k] << " ";
    }
    cout << endl;

    cout << "Schaden durch Gegner erlitten in Runde: ";
    for(int l = 0; l < Gegner_counter; l++){
        cout << Schaden_Gegner[l] << " ";
    }
    cout << endl;

    cout << "Gegner besiegt in Runde: ";
    for(int m = 0; m < Sieg_counter; m++){
        cout << Gegner_besiegt[m] << " ";
    }
    cout << endl;
}


int main(){

    /* GAME PREPARATION */

    // use the current time as seed for the random generator
    srand(time(NULL));

    // generate the game field
    struct Spielwelt field = generateField();

    // set the lives of the hero to 5 and the number of collectible items to 0
    // set the starting point of the hero to the coordinates x:0, y:0
    struct Charakter hero = {5, 0, 0, 0};

    // set the lives of the enemies to 1 and their starting point to the coordinates x:2, y:2
    struct Gegner enemy_1 = {1, 2, 2};
    struct Gegner enemy_2 = {1, 2, 2};

    int relic_hidden = countHiddenRelics(field);
    cout << "Relikte zu finden: " << relic_hidden << endl;

    printHero(hero);
    printField(field, enemy_1, enemy_2);

    /* GAME */

    // declare variables and arrays for summary stats at the end of the game
    int round = 0;
    int Gefahr_counter = 0;
    int Quelle_counter = 0;
    int Relikt_counter = 0;
    int Gegner_counter = 0;
    int Sieg_counter = 0;

    int Schaden_Gefahr[100] = {0};
    int Quelle_Gefunden[100] = {0};
    int Relikt_Gefunden[100] = {0};
    int Schaden_Gegner[100] = {0};
    int Gegner_besiegt[100] = {0};
    do{
        // read-in user input to move the character
        char user_input;
        cin >> user_input;
        // test user input for correctness: min requirements lowercase, alphabetic character
        assert(islower(user_input));
        assert(isalpha(user_input));

        // move the character according to the user's input
        if(user_input == 'w'){
            if(hero.y == 0){
                hero.y = 4;
            } else {
                hero.y -= 1;
            }
        } else if(user_input == 'a'){
            if(hero.x == 0){
                hero.x = 4;
            } else {
                hero.x -= 1;
            }
        } else if(user_input == 's'){
            if(hero.y == 4){
                hero.y = 0;
            } else {
                hero.y += 1;
            }
        } else if(user_input == 'd'){
            if(hero.x == 4){
                hero.x = 0;
            } else {
                hero.x += 1;
            }
        } else {
            cout << "Falscher Befehl!" << endl;
            continue;
        }

        // increase the number of rounds by 1 at the start of each round
        round += 1;
        // manipulate the fields, lives of the hero and the number of relics found and hidden
        // if the hero steps on an empty field nothing happens
        if(field.twod_world[hero.y][hero.x] >= 1 && field.twod_world[hero.y][hero.x] <= 4){
            field.twod_world[hero.y][hero.x] = 1;
        // if the hero steps on a dangerous field decrease his life points by 1 with a probability of 1/6
        } else if(field.twod_world[hero.y][hero.x] >= 5 && field.twod_world[hero.y][hero.x] <= 8){
            int prob_danger = rand() % 6 + 1;
            if(prob_danger == 1){
                hero.Lebenspunkte -= 1;
                Schaden_Gefahr[Gefahr_counter] = round;
                Gefahr_counter++;
            }
            field.twod_world[hero.y][hero.x] = 1;
        // if the hero steps on a field with a life-fountain increase his life points by 1
        } else if(field.twod_world[hero.y][hero.x] == 9){
            hero.Lebenspunkte += 1;
            Quelle_Gefunden[Quelle_counter] = round;
            Quelle_counter++;
            field.twod_world[hero.y][hero.x] = 1;
        // if the hero steps on a field with a relic increase number of found relics
        } else {
            relic_hidden -= 1;
            hero.Reliktpunkte += 1;
            Relikt_Gefunden[Relikt_counter] = round;
            Relikt_counter++;
            field.twod_world[hero.y][hero.x] = 1;
        }

        // move enemy 1 automatically every round after the hero moved
        // enemy's movement is based on random number generation
        if(enemy_1.Lebenspunkte > 0){
            enemyMove(enemy_1);
            enemyFight(hero, enemy_1, round, Sieg_counter, Gegner_counter, Gegner_besiegt, Schaden_Gegner);
        }

        // move enemy 2 automatically every round after the hero moved
        // enemy's movement is based on random number generation
        if(enemy_2.Lebenspunkte > 0){
            enemyMove(enemy_2);
            enemyFight(hero, enemy_2, round, Sieg_counter, Gegner_counter, Gegner_besiegt, Schaden_Gegner);
        }

        // print basic stats in each round
        cout << "Runde: " << round << endl;
        cout << "Relikte zu finden: " << countHiddenRelics(field) << endl;
        printHero(hero);
        printField(field, enemy_1, enemy_2);

    }while(hero.Lebenspunkte > 0 && relic_hidden > 0);

    if(relic_hidden == 0){
        cout << "Hurra, gewonnen!" << endl;
    } else if(hero.Lebenspunkte == 0){
        cout << "Leider verloren!" << endl;
    }

    // print summary stats at the end of the game
    Spielverlauf(Gefahr_counter, Quelle_counter, Relikt_counter, Gegner_counter, Sieg_counter,
                 Schaden_Gefahr, Quelle_Gefunden, Relikt_Gefunden, Schaden_Gegner, Gegner_besiegt);

    return 0;
}






