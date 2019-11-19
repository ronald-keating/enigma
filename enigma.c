/*  
    Ronald Keating
    Enigma Machine C Code Implementation
    MATH 415 Project
    December 6, 2019
*/
//---------LIBRARIES---------------//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>                                             //strncpy, not strcpy

//---------GLOBAL VARIABLES----------//
const int ALPHA = 26;                                           //obviously 26 will be used a lot
const int MAX_SIZE = 1000;                                      //max size of string input

//----------STATIC ROTORS-------------//
char *rand_alpha[27] = {"ABCDEFGHIJKLMNOPQRSTUVWXYZ", 
                        "QRUKOVHTAICFEBSWPYDZGNMJXL", 
                        "JZSGXMUCELIPAVYDWQOKTNHRFB", 
                        "PEXVWARSNCBDQYLIOZUJTHFGMK", 
                        "FJOZDGUNTHBEXCSAKPMIVQWRYL"};          //used randomizer to get these

char reflector[26] =    "ABCDEFGHIJKLMNOPQRSTUVWXYZ";           //reflector
char refl_pairs[2][13] = {"KQCHIGDFWJAOR", "XLZEPVUSMTNBY"};    //^swap values (pairs) for reflector array

//----------ITEM ROTORS-----------//
struct rotors{
    char alpha[27];                                             //27 to put NULL character at end, strncpy put some weird characters a few times
    int start;                                                  //keep track of where the rotor values are at in order to increment next one
};

//---------------FUNCTIONS----------------//
void inc_rotor(char *arr)                                       //moves the characters in the rotor array down one
{                                                               //like enigma when the rotors turn
    int i;
    char temp = arr[ALPHA-1];                                   //temp set to end value, this will be replaced with the first value
    for(i = ALPHA-1; i > 0; i--)                                //note that the for loop decrements through the array backwards
        arr[i] = arr[i-1];                                      //swap value with previous value
    arr[0] = temp;                                              //set first value to end value, make sure to do this at end of for loop
}

void plugboard(char plugs[][10])                                //this just fills the plugboard
{                                                               //really tedious so maybe an input file
    int i, j;                                                   //which i did
    char c;                                                     //temp character
    FILE *fp;                                                   //file pointer
    fp = fopen("plugboard.txt", "r");                           //follow plugboard format for editing
    for(i = 0; i < 2; i++)
    {
        for(j = 0; j < 10; j++)
        {
            c = fgetc(fp);
            if(c == '\n')                                       //dont want endline
                c = fgetc(fp);
            else if(c == ' ')                                   //dont want space
                c = fgetc(fp);
            plugs[i][j] = c;                                    //fortunately we know the txt file format
        }                                                       //dont need to continuously check
    }
    fclose(fp);                                                 //CLOSE!!
}

void swap_char(char *string, char plugs[][10])                  //swaps the value of the beginning input string with the plugboard
{                                                               //also swaps the decrpyted string afterwards as well
    int i, j;
    for(i = 0; string[i] != '\0'; i++)
    {
        for(j = 0; j < 10; j++)
        {
            if(string[i] == plugs[0][j])                        //here is where it doesnt matter what order they go in
                string[i] = plugs[1][j];                        //checks both sides of plugboard
            else if(string[i] == plugs[1][j])
                string[i] = plugs[0][j];
        }
    }
}

void fillEnigmaMachine(struct rotors rotor[])                   //fills the rotor's alpha and start value
{
    int input[3], init_val[3], i, j;                            //input and init_value arrays, i decided to make it confusing
    printf("Enter rotor values:\n");
    for(i = 0; i < 3; i++)
    {
        scanf("%d", &input[i]);
        if(input[i] > 4 || input[i] < 0)
        {
            printf("Wrong input.\n"); 
            i--;
        }
    }
    printf("Enter starting values:\n");
    for(i = 0; i < 3; i++)
    {
        scanf("%d", &init_val[i]);
        if(init_val[i] > 25 || init_val[i] < 0)
        {
            printf("Wrong input");
            i--;
        }
    }
    for(i = 0; i < 3; i++)
    {
        strncpy(rotor[i].alpha, rand_alpha[input[i]], ALPHA);   //strncpy the randomized alphabet arrays into struct rotors
        rotor[i].start = init_val[i];                           //sets the initial value (the start numbers for enigma)
        rotor[i].alpha[26] = '\0';                              //the ever so convenient null character
    }
    for(i = 0; i < 3; i++)                                      //forgot this at first, shift the rotor arrays to their specified values
    {                                                           //not 100% sure this is nessecary but i think it is
        for(j = 0; j < init_val[i]; j++)
            inc_rotor(rotor[i].alpha);
    }
}

void iterateRotors(struct rotors *rotor, int i)                 //increments rotor 0 and decideds whether or not to increment the next rotor
{                                                               //recursive so pay attention
    rotor[i].start++;
    inc_rotor(rotor[i].alpha);
    if(rotor[i].start == 26)
    {   
        i++; 
        rotor[i].start = 0;
        if(i == 2)
            return;
        iterateRotors(rotor, i++);                              //recursion
    }
}

char enigmate(char c, struct rotors *rotor)                     //enigmate: to encrpyt using enigma
{                                                               //this is where basically the magic happens
    int i, j, temp;                                             //will comment later because this even confuses me.
    for(i = 0; i < 3; i++)                                      //takes value of c, subtract 65, and set equal to value of next rotor where c-65
        c = rotor[i].alpha[(c - 65)%26];                        //decimal value of A (65)
    c = reflector[(c-65)];                                      //path to reflector given value of c
    for(i = 0; i < ALPHA/2; i++)                                //this is entirely for the reflector function
    {                                                           //taegan's idea, set values in reflector with pairs and
        if(c == refl_pairs[0][i])                               //swap values. this is what refl_pairs is for
        {
            temp = i;
            c = refl_pairs[1][temp];
            break;
        }
        else if(c == refl_pairs[1][i])
        {
            temp = i;
            c = refl_pairs[0][temp];
            break;
        }
    }
    c = reflector[c - 65];                                      //need this in order to traverse back through rotors
    for(i = 0; i < ALPHA; i++)                                  //hardest part of the program
    {                                                           //basically i need a path to, given input lets say A->X,
        if(c == reflector[i])                                   //for A to X and then X to A on different, but same setup.
            temp = i;                                           //Since the path is constant during the intial set up,
    }                                                           //I need to work backwards from the reflector to X, given A.
    c = temp + 65;                                              //This is what the temp + 65 is for. Undoes what was previously done
    for(i = 2; i >=0; i--)
    {
        for(j = 0; j < ALPHA; j++)
        {
            if(c == rotor[i].alpha[j])
                temp = j;
        }
        c = temp + 65;
    }   
    return c;
}


//---------------MAIN---------------//
int main()
{
    int input[3], init_val[3], i, j;                            //input array for rotor #, init_val array for starting #, i & j for loops
    struct rotors rotor[3];                                     //array of structs, easy to iterate through rotors and pass to functions
    char plugs[2][10];                                          //plugboard values, again i should use an extern file
    plugboard(plugs);                                           //initialize plugboard values
    fillEnigmaMachine(rotor);                                   //initialize enigma machine
    printf("Enter string (UPPERCASE and no spaces):\n");
    char string[MAX_SIZE];
    scanf(" %s", string);
    swap_char(string, plugs);                                   //swap with plugboard before encrpytion                                   
    i = 0;
    while(string[i] != '\0')                                    //originally used fgets, got random chars
    {                                                           //no need for spaces anyways
        string[i] = enigmate(string[i], rotor);                 //encrpyts/decrypts char i in string
        iterateRotors(rotor, 0);                                //move rotors
        i++;                                                    //move one forward in string
    }
    swap_char(string, plugs);                                   //swap with plugboard after encryption
    printf("%s\n", string);
    return 0;
}
