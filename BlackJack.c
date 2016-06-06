#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>

char* stack[100];
int counter = 0;

int getCardNum(char * str){
    
    switch(str[0]){
        case '2': return 2;
        case '3': return 3;
        case '4': return 4;
        case '5': return 5;
        case '6': return 6;
        case '7': return 7;
        case '8': return 8;
        case '9': return 9;
        case '1': return 10;
        case 'J': return 10;
        case 'Q': return 10;
        case 'K': return 10;
        case 'A': return 11;
        case 'a': return 1;
    }
}

char* pop(){ 
    srand(time(NULL));
    //printf("counter is %i\n", counter);
    counter--;
    int deckIndex; 
    deckIndex = rand() % 52;
    while(strcmp(stack[deckIndex] , "0\0") == 0){
        deckIndex = rand() % 52;
    }
    char * result = malloc(5);
    strcpy(result, stack[deckIndex]);
    stack[deckIndex] = "0\0";
    if(counter == 0){
        createDeck();
    }
    return result;
}


void push(char * n){
    stack[counter] = n;
    counter++;
}

void printTop(){
    printf("%s\n", *(stack + counter - 1));
}

void createSuit(char c){
    int i;

    for(i = 2; i < 11; i++){
        char * current = malloc(4);
        sprintf(current, "%i%c", i, c);
        push(current);
    }
    char * current = malloc(4);
    sprintf(current, "J%c", c);
    push(current);
    char * current2 = malloc(4);
    sprintf(current2, "Q%c", c);
    push(current2);
    char * current3 = malloc(4);
    sprintf(current3, "K%c", c);
    push(current3);
    char * current4 = malloc(4);
    sprintf(current4, "A%c", c);
    push(current4);
}
 
void createDeck(){
    createSuit('S');
    createSuit('H');
    createSuit('D');
    createSuit('C');

}

int askForBet(int * money){
    printf("How much do you want to bet? Minimum is 1\n");
    int bet = 1;
    scanf("%d", &bet);
    if(bet < 1){
        bet = 1;
    }
    if(bet > *money){
        bet = *money;
    }
    getchar(); 
    return bet;
}

void printHand(char **hand, int handCounter){
    int i;
    printf("Your hand: ");
    for(i = 0; i < handCounter; i++){
        printf("%s ", hand[i]);
    }
    printf("\n");
}

void printDHand(char **hand, int handCounter){
    int i;
    printf("Dealer hand: ");
    for(i = 0; i < handCounter; i++){
        printf("%s ", hand[i]);
    }
    printf("\n");
}

int aceCheck(int * currentValue, char ** hand, int * handCounter){
    int i;
    for( i = 0; i < *handCounter; i++){
        if(hand[i][0] == 'A'){
            *currentValue-=10;
            hand[i][0] = 'a';
            return 1;
        }
    }
    return 0; 
}

int winConditions(int * currentValue, char ** hand, int * handCounter, char ** dHand){
    if (*currentValue == 21){
        return checkDealer(currentValue, dHand);;
    }
    if (*currentValue > 21){
        if(!aceCheck(currentValue, hand, handCounter)){
            printf("value: %i bust\n", *currentValue);
            return 0;
        }
    }
    

    return 2;
}

int checkDealer(int * currentValue, char ** hand){
    int dealerValue = 0, handCounter = 1;
    dealerValue += getCardNum(hand[0]);
    hand[handCounter++] = pop(); dealerValue += getCardNum(hand[1]);    
    while(dealerValue < 17){
        hand[handCounter] = pop();
        dealerValue += getCardNum(hand[handCounter]);
        if(dealerValue > 21){
            aceCheck(&dealerValue, hand, &handCounter);
        }
        handCounter++;
    }
    printDHand(hand, handCounter);
    printf("Dealer Value: %i\n", dealerValue);
    if(dealerValue > 21){
        return 1;
    }
    if(dealerValue > *currentValue){
        return 0;
    }
    if(dealerValue == *currentValue){
        return 2;
    }
}

int hitOrStay(int * handCounter, int * currentValue, char ** hand, char ** dHand){
    while(1){
        //printHand(hand, *handCounter);
            
        char c;
        //printf("value: %i, [h]it or [s]tay?\n", *currentValue);
        c = getchar(); getchar();
        if(c != 'h'){ return checkDealer(currentValue, dHand);}
        hand[*handCounter] = pop();
        *currentValue += getCardNum(hand[*handCounter]);
        (*handCounter)++;
        printHand(hand, *handCounter);
        int winCond = winConditions(currentValue, hand, handCounter, dHand);
        if( winCond == 0 ){
            printf("\n");
            return winCond;
        }
        if( winCond == 1){
            return checkDealer(currentValue, dHand);
        }
        printf("value: %i", *currentValue);
        printf(", [h]it or [s]tay?\n");
    }
}

void checkWinner(int winner, int * money, int * bet){
    if(winner){
        *money += *bet;
        printf("You won! Money: %i\n", *money);
    }
    else if(winner == 0){
        *money -= *bet;
        printf("You lost! Money: %i\n", *money);
    }
    else{
        printf("Tie! Money: %i\n", *money);
    }

}

void play(int * money){
    char * hand[10];
    char * dHand[10];
    dHand[0] = pop();
    int currentValue = 0, handCounter = 0, winner;
    hand[handCounter++] = pop(); currentValue += getCardNum(hand[0]);
    hand[handCounter++] = pop(); currentValue += getCardNum(hand[1]);  
    int bet = askForBet(money);
    printf("\n\nDealer Hand: %s\n", dHand[0]);
    printHand(hand, handCounter);
    if( winConditions(&currentValue, hand, &handCounter, dHand) == 1 ){
        printf("Blackjack!\n");
        *money += bet;
        printf("You won! Money: %i\n", *money);
        return;
    }
    printf("value: %i, [h]it or [s]tay?\n", currentValue);
    winner = hitOrStay(&handCounter, &currentValue, hand, dHand);
    checkWinner(winner, money, &bet);
    
}


int main(){
    createDeck();
    int money = 20;
    while(money > 0){
        play(&money);
    }
}
