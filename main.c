#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

//works out the interest that will be received based on the current value and the interest rate
double calculateInterest(double value, float interest){
    return (value * interest);
}

//manages the calls and logic for question 1_A
void question1_A(void){
    //decelerations
    //Tom, 15% normal interest
    //Joan, 10% compounded interest
    const float TOMS_INTEREST = 0.15;
    const float JOANS_INTEREST = 0.10;

    //starting investment at 200 euros
    double tomsBalance = 200;
    double joansBalance = 200;
    //variables for holding the values of the interest
    double tomsInterest, joansInterest;

    int numberOfYears = 0;

    //toms interest is worked here as his interest is not compounding so is going to be a flat rate
    tomsInterest = calculateInterest(tomsBalance, TOMS_INTEREST);
    while(tomsBalance >= joansBalance){
        //joan's interest is worked here as her interest is compounding so must be worked out each time
        joansInterest = calculateInterest(joansBalance, JOANS_INTEREST);
        //adds the interest to their balances respectively
        joansBalance = joansBalance + joansInterest;
        tomsBalance = tomsBalance + tomsInterest;
        //increments the number of years
        numberOfYears++;
        //displays the year and the current balances
        printf("After %d years, tom's balance is %.2lf€ and joan's is %.2lf€ \n", numberOfYears, tomsBalance, joansBalance);
    }
    //prints how long it took for joan's balance to become greater than tom's
    printf("It took %d years for joan's balance of %.2lf€ to over take tom's balance %.2lf€ \n", numberOfYears, joansBalance, tomsBalance);
}
//empties the rest of the buffer of unneeded data
void clearBuffer(void){
    char ch;
    while((ch = getchar()) != '\n');
}

//checks to see if the expected return value was returned
void checkInput(int returned, int expectedValue){
    if(returned != expectedValue){
        printf("Invalid input \n");
    }
}
//manages the logic and calls for question 1_B
void question1_B(void){
    /*
     * artichokes (a)- 2.05/KG
     * onions (b)- 1.15/kg
     * carrots (c)- 1.09/kg
     * 5% discount on orders over 100€
     * 6.50€ shipping on orders <=5kg
     * 14.00€  shipping on orders 5kg < & < 20kg
     * 14.00€  + 0.50€ /kg when >=20kg
     *
     * q goes to checkout
     */

    //stores the value of each required;
    float a = 0;
    float b = 0;
    float c = 0;
    //cost of each item per KG
    const float aCost = 2.05;
    const float bCost = 1.15;
    const float cCost = 1.09;
    //variables that are needed
    float totalWeight = 0;
    double totalCost = 0;
    double shippingCost = 0;
    const float discountPercentage = 0.05;
    double discountAmount = 0;
    int tempCounter;
    //input variables
    char input, temp;
    double amountWanted;
    //loop control values
    bool loop = true;
    bool validInput = false;

    while(loop){
        printf("Input q to go to checkout, a to buy artichokes, b to buy onions, c to buy carrots\n");
        input = getchar();
        tempCounter = 0;
        while((temp = getchar()) != '\0' && temp != '\n'){
            tempCounter++;
        }
        clearBuffer();
        if(tempCounter > 0){
            printf("Invalid input\n");
        }else {
            switch (input) {
                case 'q'://processing the checkout
                    printf("q\n");
                    loop = false;//ends the main loop
                    //works out the weights and cost of items
                    totalWeight = a + b + c;
                    printf("%.2fKG of artichokes costing %.2lf€\n", a, a * aCost);
                    printf("%.2fKG of onions costing %.2lf€\n", b, b * bCost);
                    printf("%.2fKG of carrots costing %.2lf€\n", c, c * cCost);
                    totalCost = (a * aCost) + (b * bCost) + (c * cCost);
                    if (totalCost >= 100) {//checks if a discount needs to be applied
                        discountAmount = totalCost * discountPercentage;
                        printf("discount of %.2lf€ applied\n", discountAmount);
                    }
                    //works out shipping cost
                    if (totalWeight <= 5) {
                        shippingCost = 6.5;
                    } else if (totalWeight < 20) {
                        shippingCost = 14;
                    } else {
                        shippingCost = 14 + (totalWeight * 0.5);
                    }
                    printf("Shipping of %.2lfkg costing %.2lf€\n", totalWeight, shippingCost);
                    totalCost = totalCost - discountAmount +
                                shippingCost;//works out the final cost after discounts and shipping
                    printf("Total cost of %.2lf€\n", totalCost);
                    printf("Would you like to save this receipt to file? Y - Yes, N - No\n");
                    while (!validInput) { //loop waiting for Y or N
                        input = getchar();
                        clearBuffer();
                        switch (input) {
                            case 'Y':
                                validInput = true;
                                printf("Saving\n");

                                //creates a file or opens to it if it already exists and writes to it
                                FILE *f = fopen("receipt.txt", "w");
                                if (f == NULL) {
                                    printf("ERROR OPENING FIlE");
                                    exit(EXIT_FAILURE);
                                } else {//writes to the file
                                    fprintf(f, "%.2fKG of artichokes costing %.2lf€\n", a, a * aCost);
                                    fprintf(f, "%.2fKG of onions costing %.2lf€\n", b, b * bCost);
                                    fprintf(f, "%.2fKG of carrots costing %.2lf€\n", c, c * cCost);
                                    if (totalCost >= 100) {
                                        fprintf(f, "discount of %.2lf€ applied\n", discountAmount);
                                    }
                                    fprintf(f, "Shipping of %.2lfKG costing %.2lf€\n", totalWeight, shippingCost);
                                    fprintf(f, "Total cost of %.2lf€", totalCost);
                                }
                                fclose(f);
                                printf("Saving Successful\n");
                                break;
                            case 'N':
                                validInput = true;
                                printf("Not saving to file. \n");
                                break;
                            default:
                                printf("Invalid input\n");
                                printf("Please input Y for Yes or N for No\n");
                                break;
                        }
                    }
                    break;
                case 'a'://if a is entered gets the amount of artichokes required
                    printf("Please input the amount of artichokes required\n");
                    checkInput(scanf("%lf", &amountWanted), 1);
                    clearBuffer();
                    if (amountWanted < 0) {
                        printf("Invalid Input \n");
                        break;
                    }
                    a += amountWanted;
                    break;
                case 'b'://if b is entered get teh amount of onions required
                    printf("Please input the amount of onions required\n");
                    checkInput(scanf("%lf", &amountWanted), 1);
                    clearBuffer();
                    if (amountWanted < 0) {
                        printf("Invalid Input \n");
                        break;
                    }
                    b += amountWanted;
                    break;
                case 'c'://if c is entered gets the amount of carrots required
                    printf("Please input the amount of carrots required\n");
                    checkInput(scanf("%lf", &amountWanted), 1);
                    clearBuffer();
                    if (amountWanted < 0) {
                        printf("Invalid Input \n");
                        break;
                    }
                    c += amountWanted;
                    break;
                default:
                    printf("Invalid input \n");
            }
        }
    }
}

//returns true if a match is found.
bool checkString(int charNum , const char str1[], const char str2[],bool htmlCheck){
    int x = 0; //x + 1 also serves as to check if the match was successful as it will only be the length of the string if it matches
    for(int i = charNum; i < (int) sizeof(str2) - 1; i++){
        if(str1[i] == str2[x]){
            x++;
            //does a case insensitive check if htmlCheck is true and str2[i] is a letter and not a symbol
        }else if(str1[i] == (str2[x] + 32) && htmlCheck && (((str2[i] + 32) > 65 && (str2[i] + 32) < 90 ) || (((str2[i] + 32) > 97) && (str2[i] + 32) < 122))){
            x++;
        }
    }
    if(x == ((int) sizeof(str2) - 1)){
        return true;
    }else{
        return false;
    }
}

//manages the logic and calls for question 1_C
void question1_C(void){
    //reads the file input.txt
    //works out if the file is a C file or if it is a HTML file.
    //first line == #include or <HTML>
    //last line in case of </HTML>
    //required decelerations
    FILE *f = fopen("input.txt", "r");
    char * line = NULL;
    size_t length  = 0;
    char currentChar = NULL;
    int charNum = 0;
    bool htmlMatch = false;
    bool cMatch = false;

    //checks if there has been an error opening the file
    if(f == NULL){
        exit(EXIT_FAILURE);
    }else {
        while ((getline(&line, &length, f)) != -1) {
            printf("%s\n", line);
            currentChar = line[charNum];

            //removes the end of line characters that are not needed for the process
            for (int i = 0; i < length; i++) {
                if (line[i] == '\r' || line[i] == '\n') {
                    line[i] = '\0';
                }
            }

            //makes a note of any tabs and spaces in front of the start of a line
            while ((currentChar == 32 || currentChar == 9) && charNum <= (int) length) {
                charNum++;
                currentChar = line[charNum];
            }
            //checks to see if <HTML> is in the file
            if (checkString(charNum, line, "<HTML>", true)) {//checks for HTML Header
                printf("HTML file type\n");
                htmlMatch = true;
            }
            //checks to see if </HTML> is in the file
            if (checkString(charNum, line, "</HTML>", true)) { //checks for HTML Footer
                printf("HTML file type\n");
                htmlMatch = true;
            }
            //checks to see if #include is in the file
            if (checkString(charNum, line, "#include", false)) {//Checks for #include
                printf("C file type\n");
                cMatch = true;
            }
        }
    }

    //checks to see if the file has been flagged as a C file or HTML file, if not says "other file type"
    if(cMatch == false && htmlMatch == false){
        printf("Other file type\n");
    }
    fclose(f);
}

char* removeChar(int id, const char oldLine[], size_t size){
    int offset = 0;

    char* newLine = malloc(size * ((int) size) - 1);
    for(int i = 0; i < ((int) size + 1); i++){
        if(i == id){
            offset = 1;
        }else{
            newLine[i - offset] = oldLine[i];
        }
    }
    return newLine;
}


void question1_D(void){
    FILE *f = fopen("error_text.txt", "r");
    //TODO locate cause of unxpected end of line chars '\247\346VQ]\001'
    char sinceSpace[255], previousChar, currentChar;
    int charSinceSpace = 0, charSinceSpacePointer = 0;
    char* line = NULL, newLine;
    size_t size;
    bool issues = false;

    long pos = ftell(f);
    fseek(f,0,SEEK_END);
    long length = ftell(f);
    fseek(f,pos,SEEK_SET);
    line = malloc((size_t) length);
    fread(line, (size_t) length, 1, f);
    printf("%s", line);

    printf("%d", (int) strlen(line));
    for(int i = 0; i < (int) strlen(line); i++){

    }
}

int main(void) {
    question1_D();
}