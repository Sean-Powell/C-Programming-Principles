#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

#define MAX_INPUT 255
#define FIXED_ARRAY_FILE_LOCATION "Fixed2DList.txt"
#define VAR_ARRAY_FILE_LOCATION "VarArray.txt"
#define LINKED_LIST_FILE_LOCATION "LinkedList.txt"
#define FIXED_ARRAY_SIZE 5
#define NULL_LINE "NULL"

#define LOWEST_VALUE_UPPERCASE 65
#define HIGHEST_VALUE_UPPERCASE 90
#define UPPER_TO_LOWERCASE_DIFFERENCE 32

//global variables
char DataInput[MAX_INPUT], KeyInput[MAX_INPUT], GenericInput[MAX_INPUT];


//Struct Decelerations
typedef struct bucket{
    char data[MAX_INPUT];
    char key[MAX_INPUT];
    bool inUse;
}BUCKET;

typedef struct node{
    struct node * previous;
    BUCKET bucket;
    struct node * next;
}NODE;

typedef struct head{
    NODE * node;
    int length;
}HEAD;

typedef struct variable_array{
    int length;
    BUCKET * array;
}VARIABLE_ARRAY;

//--------------
//Global Functions
void flushBuffer(void){
    while(getchar() != '\n');
}

void clearInputs(void){
    for(int i = 0; i < MAX_INPUT; i++){
        DataInput[i] = '\0';
        KeyInput[i] = '\0';
    }
}

char changeToLowerCase(char _c){
    int x;
    char newC;
    if(_c >= LOWEST_VALUE_UPPERCASE && _c <= HIGHEST_VALUE_UPPERCASE){
        x = _c + UPPER_TO_LOWERCASE_DIFFERENCE;
        newC = (char) x;
    }else{
        newC = _c;
    }
    return newC;
}


int calculateHash(const char _data[], int _dataLength, const char _key[], int _keyLength, int _width){
    int keyTotal = 0, dataTotal = 0;

    for(int i = 0; i < _dataLength; i++){
        dataTotal += _data[i];
    }

    for(int i  = 0; i < _keyLength; i++){
        keyTotal +=  _key[i];
    }

    return (keyTotal * dataTotal) % _width;
}

void getInputs(void){
    printf("Please input the data\n");
    fgets(DataInput, MAX_INPUT, stdin);
    DataInput[strlen(DataInput) - 1] = '\0';
    printf("Please input the key\n");
    fgets(KeyInput, MAX_INPUT, stdin);
    KeyInput[strlen(KeyInput) - 1] = '\0';
}

int checkForInvalidInput(char _invalidInput[]){
    if(strcmp(DataInput, _invalidInput) == 0 || strcmp(KeyInput, _invalidInput) == 0){
        return 1;
    }
    return 0;
}

//GLOBAL DECLARATIONS FOR QUESTION A
BUCKET fixedHashMap2D[FIXED_ARRAY_SIZE][FIXED_ARRAY_SIZE];
//----------------------------------

//FUNCTIONS FOR QUESTION A
int addNodeA(int _hash){
    BUCKET buk;
    int index = -1;
    for(int i = 0; i < FIXED_ARRAY_SIZE; i++){
        if(fixedHashMap2D[_hash][i].inUse == false && index == -1){
            index = i;
        }
    }
    if(index == -1){
        printf("The hash space for that hash is full, please remove a node before adding this one\n");
        return -1;
    }else{
        buk = fixedHashMap2D[_hash][index];
        buk.inUse = true;
        strcpy(buk.data, DataInput);
        strcpy(buk.key, KeyInput);
        fixedHashMap2D[_hash][index] = buk;
        return 0;
    }
}

int findNodeA(int _hash){
    BUCKET buk;
    int index = -1;
    for(int i = 0; i < FIXED_ARRAY_SIZE; i++){
        buk = fixedHashMap2D[_hash][i];
        if(strcmp(buk.data, DataInput) == 0 && strcmp(buk.key, KeyInput) == 0 && index == -1
           && buk.inUse == true){
            index = i;
        }
    }
    if(index != -1){
        printf("-----------\n");
        printf("NODE FOUND:\n");
        printf("Hash: %d\n", _hash);
        printf("Index: %d\n", index);
        printf("-----------\n");
        return 0;
    }else{
        printf("A node could not be found with that data\n");
        return 1;
    }
}

void resetDataA(int _hash, int _index){
    BUCKET buk = fixedHashMap2D[_hash][_index];
    for(int  i = 0; i < strlen(buk.data); i++){
        buk.data[i] = '\0';
    }
    for(int i = 0; i < strlen(buk.key); i++){
        buk.key[i] = '\0';
    }
    buk.inUse = false;
    fixedHashMap2D[_hash][_index] = buk;
}

int removeNodeA(int _hash, int _index){
    BUCKET buk = fixedHashMap2D[_hash][_index];
    if(buk.inUse == true){//checks if node has any data
        resetDataA(_hash, _index);
        for(int i = _index + 1; i < FIXED_ARRAY_SIZE; i++){//moves any nodes if there is any down by 1
            buk = fixedHashMap2D[_hash][i];
            if(buk.inUse == true){
                fixedHashMap2D[_hash][i - 1] = buk;
                resetDataA(_hash, i);
            }
        }
        return 0;
    }else{
        return 1;
    }
}

int insertNodeA(int _hash, int _index){
    BUCKET buk = fixedHashMap2D[_hash][_index];
    int position = -1;
    if(buk.inUse == false){
        addNodeA(_hash);
    }else{
        for(int i = _index + 1; i < FIXED_ARRAY_SIZE; i++){ //checks if there is enough space to move things up one
            buk = fixedHashMap2D[_hash][i];
            if(buk.inUse == false && position == -1){
                position = i;
            }
        }

        if(position == -1){
            return 1;
        }else{
            for(int i = position; i >= _index; i--){//moves all nodes from the index up, up one space
                fixedHashMap2D[_hash][i] = fixedHashMap2D[_hash][i - 1];
            }
            resetDataA(_hash, _index);
            addNodeA(_hash);
            return 0;
        }
    }
}

int saveMapA(void){
    printf("Saving...");
    FILE * f = fopen(FIXED_ARRAY_FILE_LOCATION, "w");
    for(int i = 0; i < FIXED_ARRAY_SIZE; i++){
        for(int j = 0; j < FIXED_ARRAY_SIZE; j++){
            BUCKET buk = fixedHashMap2D[i][j];
            if(buk.inUse == true){
                fprintf(f, "%s\n", buk.data);
                fprintf(f, "%s\n", buk.key);
            }else{
                fprintf(f, "%s\n", NULL_LINE);
            }
        }
    }
    fclose(f);
    printf("Saved\n");
    return 0;
}

int loadMapA(void){
    printf("Loading...");
    FILE * f = fopen(FIXED_ARRAY_FILE_LOCATION, "r");
    for(int i = 0; i < FIXED_ARRAY_SIZE; i++){
        for(int j = 0; j< FIXED_ARRAY_SIZE; j++){
            resetDataA(i, j);
            fgets(GenericInput, MAX_INPUT, f);
            GenericInput[strlen(GenericInput) - 1] = '\0';
            if(strcmp(GenericInput, NULL_LINE) != 0) {
                strcpy(fixedHashMap2D[i][j].data, GenericInput);
                fgets(GenericInput, MAX_INPUT, f);
                GenericInput[strlen(GenericInput) - 1] = '\0';
                strcpy(fixedHashMap2D[i][j].key, GenericInput);
                fixedHashMap2D[i][j].inUse = true;
            }
        }
    }
    printf("Loaded\n");
    return 0;
}
//------------------------

//Manager for running question A
void runA(void){
    bool loop = true;
    char input, *c;
    int hash, index, returnValue;
    for(int i = 0; i < FIXED_ARRAY_SIZE; i++){
        for(int j = 0; j < FIXED_ARRAY_SIZE; j++){
            fixedHashMap2D[FIXED_ARRAY_SIZE][FIXED_ARRAY_SIZE].inUse = false;
        }
    }
    while(loop){
        printf("Please input a to add a node, f to find a node, i to insert a node, r to remove a node, s to save and l to load\n");
        printf("Enter q to quit\n");
        input = (char) getchar();
        flushBuffer();
        input = changeToLowerCase(input);
        switch (input){
            case 'a'://adding a node
                getInputs();
                if(checkForInvalidInput(NULL_LINE) == 1){
                    printf("%s is an invalid input\n", NULL_LINE);
                    break;
                }
                hash = calculateHash(DataInput, (int) strlen(DataInput), KeyInput, (int) strlen(KeyInput), FIXED_ARRAY_SIZE);
                addNodeA(hash);
                clearInputs();
                break;
            case 'f'://finding a node
                getInputs();
                if(checkForInvalidInput(NULL_LINE) == 1){
                    printf("%s is an invalid input\n", NULL_LINE);
                    break;
                }
                hash = calculateHash(DataInput, (int) strlen(DataInput), KeyInput, (int) strlen(KeyInput), FIXED_ARRAY_SIZE);
                findNodeA(hash);
                clearInputs();
                break;
            case 'i'://inserting a node
                getInputs();
                if(checkForInvalidInput(NULL_LINE) == 1){
                    printf("%s is an invalid input\n", NULL_LINE);
                    break;
                }
                hash = calculateHash(DataInput, (int) strlen(DataInput), KeyInput, (int) strlen(KeyInput), FIXED_ARRAY_SIZE);
                printf("Please input the index you would like to insert this at. Please enter a value between 0 and %d\n", FIXED_ARRAY_SIZE - 1);
                fgets(GenericInput, MAX_INPUT, stdin);
                index = (int) strtol(GenericInput, &c, 10);
                if(index >= 0 && index <= FIXED_ARRAY_SIZE - 1){
                    returnValue = insertNodeA(hash, index);
                    if(returnValue == 0){
                        printf("Node inserted\n");
                    }else if(returnValue == 1){
                        printf("The hash space does not have enough space to insert this node please remove a node to insert this node\n");
                    }
                }else{
                    printf("Invalid index\n");
                }
                clearInputs();
                break;
            case 'r'://remove a node
                printf("Please input the hash\n");
                fgets(GenericInput, MAX_INPUT, stdin);
                hash = (int) strtol(GenericInput, &c, 10);
                printf("Please enter the index\n");
                fgets(GenericInput, MAX_INPUT, stdin);
                index = (int) strtol(GenericInput, &c, 10);
                if(hash <= FIXED_ARRAY_SIZE - 1 && index <= FIXED_ARRAY_SIZE - 1 && index >= 0 && hash >=0){
                    if(removeNodeA(hash, index) == 0){
                        printf("Node removed\n");
                    }else{
                        printf("The node was not in use\n");
                    }

                }else{
                    printf("Inputs are out of bounds\n");
                }
                clearInputs();
                break;
            case 's'://save the map
                if(saveMapA() != 0){
                    printf("An error occurred while saving\n");
                }
                break;
            case 'l'://load the map
                if(loadMapA() != 0){
                    printf("An error occurred while loading from file\n");
                }
                break;
            case  'q'://exit this section back to the main menu
                printf("Quitting\n");
                loop = false;
                break;
            default:
                printf("INVALID INPUT\n");
                break;
        }
    }
}

//--------------------

//Global Declerations for B
VARIABLE_ARRAY * varArray;
//--------------------

//Global Methods for B

int addNodeB(int _hash){
    BUCKET * temp;
    if(varArray[_hash].length == 0){
        varArray[_hash].length++;
        varArray[_hash].array = malloc(varArray[_hash].length * sizeof(BUCKET));
    }else{
        varArray[_hash].length++;
        temp = realloc(varArray[_hash].array, varArray[_hash].length * sizeof(BUCKET));
        varArray[_hash].array = temp;
    }
    strcpy(varArray[_hash].array[varArray[_hash].length - 1].data, DataInput);
    strcpy(varArray[_hash].array[varArray[_hash].length - 1].key, KeyInput);
    varArray[_hash].array[varArray[_hash].length - 1].inUse = true;
    return 0;
}

int findNodeB(int _hash){
    for(int i = 0; i < varArray[_hash].length; i++){
        if(strcmp(varArray[_hash].array[i].key, KeyInput) == 0 && strcmp(varArray[_hash].array[i].data, DataInput) == 0){
            return i;
        }
    }
    return -1;
}

int insertNodeB(int _hash, int _index){
    BUCKET * temp;
    if(varArray[_hash].length == 0){
        addNodeB(_hash);
        return 0;
    }else{
        varArray[_hash].length++;
        temp = realloc(varArray[_hash].array, varArray[_hash].length * sizeof(BUCKET));
        varArray[_hash].array = temp;
    }

    for(int i = varArray[_hash].length - 1; i >= _index; i--){ //moves everything in the hash up one until the index
        strcpy(varArray[_hash].array[i].data, varArray[_hash].array[i - 1].data);
        strcpy(varArray[_hash].array[i].key, varArray[_hash].array[i - 1].key);
        varArray[_hash].array[i].inUse = varArray[_hash].array[i - 1].inUse;
    }

    strcpy(varArray[_hash].array[_index].data, DataInput);
    strcpy(varArray[_hash].array[_index].key, KeyInput);
    varArray[_hash].array[_index].inUse = true;
    return 0;
}

int removeNodeB(int _hash, int _index){
    BUCKET * temp;
    varArray[_hash].length--;
    temp = malloc(varArray[_hash].length * sizeof(BUCKET));

    for(int  i = 0; i < _index; i++){
        strcpy(temp[i].data, varArray[_hash].array[i].data);
        strcpy(temp[i].key, varArray[_hash].array[i].key);
        temp[i].inUse = varArray[_hash].array[i].inUse;
    }

    for(int i = _index; i < varArray[_hash].length; i++){
        strcpy(temp[i].key, varArray[_hash].array[i + 1].key);
        strcpy(temp[i].data, varArray[_hash].array[i + 1].data);
        temp[i].inUse = varArray[_hash].array[i + 1].inUse;
    }

    free(varArray[_hash].array);
    varArray[_hash].array = temp;
    return 0;
}
//--------------------

//Manager for running question B
void runB(void){
    bool loop = true, load = false, validInput = false;
    char input, *c;
    int width = 0, hash, returnValue, index;
    FILE *f = NULL;


    while (!validInput) {
        printf("Do you want to load a file? Enter y for yes and n for no\n");
        input = (char) getchar();
        input = changeToLowerCase(input);
        flushBuffer();
        switch(input){
            case 'y':
                printf("Loading...");
                validInput = true;
                load =  true;
                f = fopen(VAR_ARRAY_FILE_LOCATION, "r");
                fgets(GenericInput, MAX_INPUT, f);
                width = (int) strtol(GenericInput, &c, 10);
                break;
            case 'n':
                validInput = true;
                do {
                    printf("Please input a hash map width\n");
                    fgets(GenericInput, MAX_INPUT, stdin);
                    width = (int) strtol(GenericInput, &c, 10);
                }while(width == 0);
                break;
            default:
                printf("INVALID INPUT\n");
                break;
        }
    }

    varArray = malloc(width * sizeof(VARIABLE_ARRAY));
    for(int i = 0; i < width; i++){
        varArray[i].length = 0;
    }
    if(load == true){
        for(int i = 0; i < width; i++){
            fgets(GenericInput, MAX_INPUT, f);
            varArray[i].length = (int) strtol(GenericInput, &c, 10);
            if(varArray[i].length != 0) {
                varArray[i].array = malloc(varArray[i].length * sizeof(BUCKET));
                for (int j = 0; j < varArray[i].length; j++) {
                    fgets(GenericInput, MAX_INPUT, f);
                    for(int k = 0; k < strlen(GenericInput); k++){
                        if(GenericInput[k] == '\r'){
                            GenericInput[k] = '\0';
                            break;
                        }
                    }
                    strcpy(varArray[i].array[j].data, GenericInput);
                    fgets(GenericInput, MAX_INPUT, f);
                    for(int k = 0; k < strlen(GenericInput); k++){
                        if(GenericInput[k] == '\r'){
                            GenericInput[k] = '\0';
                            break;
                        }
                    }
                    strcpy(varArray[i].array[j].key, GenericInput);
                }
            }
        }
        fclose(f);
        printf("Loaded\n");
    }

    while(loop){
        printf("Please input a to add a node, f to find a node, i to insert a node, r to remove a node, s to save and l to load\n");
        printf("Enter q to quit\n");
        input = (char) getchar();
        flushBuffer();
        switch(input){
            case 'a':
                getInputs();
                if(checkForInvalidInput(NULL_LINE) == 1){
                    printf("%s is and invalid input\n", NULL_LINE);
                    break;
                }
                hash = calculateHash(DataInput, (int) sizeof(DataInput), KeyInput, (int) sizeof(KeyInput), width);
                if(addNodeB(hash) == 0){
                    printf("Node added\n");
                }else{
                    printf("A error occurred\n");
                }
                clearInputs();
                break;
            case 'f':
                printf("Enter the information to find the node\n");
                getInputs();
                if(checkForInvalidInput(NULL_LINE) == 1){
                    printf("%s is and invalid input\n", NULL_LINE);
                    break;
                }
                hash = calculateHash(DataInput, (int) sizeof(DataInput), KeyInput, (int) sizeof(KeyInput), width);

                returnValue = findNodeB(hash);
                if(returnValue == -1){
                    printf("A node could not be found with that information\n");
                }else{
                    printf("--Node Found--\n");
                    printf("Hash: %d\n", hash);
                    printf("Index: %d\n", returnValue);
                    printf("Data: %s\n", DataInput);
                    printf("Key: %s\n", KeyInput);
                    printf("--------------\n");
                }
                break;
            case 'i':
                getInputs();
                if(checkForInvalidInput(NULL_LINE) == 1){
                    printf("%s is and invalid input\n", NULL_LINE);
                    break;
                }
                hash = calculateHash(DataInput, (int) sizeof(DataInput), KeyInput, (int) sizeof(KeyInput), width);
                do {
                    printf("Please input the index you would like to insert the data at\n");
                    printf("The index has to be between 0 and %d\n", varArray[hash].length);
                    fgets(GenericInput, MAX_INPUT, stdin);
                    index = (int) strtol(GenericInput, &c, 10);
                    if(index < 0 && index > varArray[hash].length){
                        printf("Invalid index\n");
                    }
                }while(index < 0 && index > varArray[hash].length);
                returnValue = insertNodeB(hash, index);
                if(returnValue == 0){
                    printf("Node inserted\n");
                }else{
                    printf("A error occurred\n");
                }
                clearInputs();
                break;
            case 'r':
                printf("Please input the hash from which you want to remove a node\n");
                fgets(GenericInput, MAX_INPUT, stdin);
                hash = (int) strtol(GenericInput, &c, 10);
                printf("Please input the nodes index that you want to remove\n");
                fgets(GenericInput, MAX_INPUT, stdin);
                index = (int) strtol(GenericInput, &c, 10);

                if(index >= 0 && index <= varArray[hash].length){//TODO test this untested if statement
                    returnValue = removeNodeB(hash, index);
                    if(returnValue == 0){
                        printf("The node was removed\n");
                    }else{
                        printf("A error occurred\n");
                    }
                    clearInputs();
                }else{
                    break;
                }

                break;
            case 's':
                printf("Saving...");
                f = fopen(VAR_ARRAY_FILE_LOCATION, "w");
                fprintf(f, "%d\n", width);
                for(int i = 0; i < width; i++){
                    fprintf(f, "%d\n", varArray[i].length);
                    for(int j = 0; j < varArray[i].length; j++){
                        fprintf(f, "%s\n", varArray[i].array[j].data);
                        fprintf(f, "%s\n", varArray[i].array[j].key);
                    }
                }
                fclose(f);
                printf("Saved\n");
                break;
            case 'q':
                printf("Quitting\n");
                loop = false;
                break;
            default:
                printf("INVALID INPUT\n");
                break;
        }
    }
}

//--------------------

//GLOBAL Decelerations for C

HEAD * linkedHashMap;

//--------------------

//Global methods for C

int addNodeC(NODE * _currentNode, char _DataInput[], char _KeyInput[]){
    NODE * nextNode = malloc(sizeof(NODE));
    _currentNode->next = nextNode;
    nextNode->previous = _currentNode;
    strcpy(nextNode->bucket.data, _DataInput);
    strcpy(nextNode->bucket.key, _KeyInput);
    nextNode->bucket.inUse = true;
    nextNode->next = NULL;
    return 0;
}

int findNodeC(int _hash){
    bool found = false;
    int index = 0;
    NODE * currentNode = linkedHashMap[_hash].node;

    while(currentNode != NULL && found == false){
        index++;
        if(strcmp(currentNode->bucket.data, DataInput) == 0 && strcmp(currentNode->bucket.key, KeyInput) == 0){
            found = true;
        }
        currentNode = currentNode->next;
    }

    if(found == true){
        return index;
    }else{
        return -1;
    }
}

int insertNodeC( int _hash, int _index){
    NODE * nextNode, * previousNode;
    nextNode = linkedHashMap[_hash].node;

    for(int i = 0; i < _index; i++){
        if(nextNode->next != NULL){
            nextNode = nextNode->next;
        }else{
            return 1;
        }
    }
    previousNode = nextNode->previous;
    NODE * newNode = malloc(sizeof(NODE));


    if(previousNode){
        previousNode->next = newNode;
        newNode->previous = previousNode;
    }else{
        newNode->previous = NULL;
    }

    nextNode->previous = newNode;
    newNode->next = nextNode;
    strcpy(newNode->bucket.data, DataInput);
    strcpy(newNode->bucket.key, KeyInput);
    newNode->bucket.inUse = true;
    return 0;
}

int removeNodeC(int _hash, int _index){
    NODE * currentNode = linkedHashMap[_hash].node;

    for(int i = 0; i < _index; i++){
        if(currentNode->next){
            currentNode = currentNode->next;
        }else{
            return 1;
        }
    }

    NODE * previousNode, * nextNode;
    if(linkedHashMap[_hash].length > 0 ) {
        previousNode = currentNode->previous;

        if (currentNode->next) {
            nextNode = currentNode->next;
            previousNode->next = nextNode;
            nextNode->previous = previousNode;
        } else {
            previousNode->next = NULL;
        }
        currentNode->bucket.data[0] = '\0';
        currentNode->bucket.key[0] = '\0';
        free(currentNode);
        linkedHashMap[_hash].length--;
    }else if(linkedHashMap[_hash].length == -1){
        return 2;
    }else{
        linkedHashMap[_hash].length = -1;
        free(linkedHashMap[_hash].node);
    }

    return 0;
}

int saveMapC(int _width){
    printf("Saving...");
    FILE *f = fopen(LINKED_LIST_FILE_LOCATION, "w");
    NODE *currentNode;
    if(f){
        fprintf(f, "%d\n", _width);
        for(int i = 0; i < _width; i++){
            if(linkedHashMap[i].length != -1){
                fprintf(f, "%d\n", linkedHashMap[i].length + 1);
                currentNode = linkedHashMap[i].node;
                while(currentNode != NULL){
                    fprintf(f, "%s\n", currentNode->bucket.data);
                    fprintf(f, "%s\n", currentNode->bucket.key);
                    currentNode = currentNode->next;
                }
            }else{
                fprintf(f, "%s\n", NULL_LINE);
            }
        }
        fclose(f);
        return 0;
    }else{
        fclose(f);
        return 1;
    }
}

int loadMapC(void) {
    FILE *f = fopen(LINKED_LIST_FILE_LOCATION, "r");
    int size, width;
    char *c;
    NODE *currentNode, *previousNode = NULL;
    bool first;

    free(linkedHashMap);
    if(f == NULL){
        return 1;
    }
    fgets(GenericInput, MAX_INPUT, f);
    width = (int) strtol(GenericInput, &c, 10);
    linkedHashMap = malloc(sizeof(HEAD) * width);
    for(int i = 0; i < width; i++){
        first = true;
        fgets(GenericInput, MAX_INPUT, f);
        GenericInput[strlen(GenericInput) - 1] = '\0';
        if(strcmp(GenericInput, NULL_LINE) == 0){
            linkedHashMap[i].node = NULL;
            linkedHashMap[i].length = -1;
        }else{
            size = (int) strtol(GenericInput, &c, 10);
            for(int j = 0; j < size; j++){
                if(first){
                    linkedHashMap[i].node = malloc(sizeof(NODE));
                    linkedHashMap[i].node->previous = NULL;
                    linkedHashMap[i].node->bucket.inUse = true;
                    fgets(GenericInput, MAX_INPUT, f);
                    GenericInput[strlen(GenericInput) - 1] = '\0';
                    strcpy(linkedHashMap[i].node->bucket.data, GenericInput);
                    fgets(GenericInput, MAX_INPUT, f);
                    GenericInput[strlen(GenericInput) - 1] = '\0';
                    strcpy(linkedHashMap[i].node->bucket.key, GenericInput);
                    linkedHashMap[i].node->next = NULL;
                    linkedHashMap[i].length = 0;
                    first = false;
                }else{
                    currentNode = linkedHashMap[i].node;
                    while(currentNode){
                        previousNode = currentNode;
                        currentNode = currentNode->next;
                    }

                    fgets(DataInput, MAX_INPUT, f);
                    DataInput[strlen(DataInput) - 1] = '\0';
                    fgets(KeyInput, MAX_INPUT,f);
                    KeyInput[strlen(KeyInput) - 1] = '\0';

                    NODE * nextNode = malloc(sizeof(NODE));
                    nextNode->previous = currentNode;
                    if(previousNode){
                        previousNode->next = nextNode;
                    }
                    strcpy(nextNode->bucket.data, DataInput);
                    strcpy(nextNode->bucket.key, KeyInput);
                    nextNode->bucket.inUse = true;
                    nextNode->next = NULL;
                    linkedHashMap[i].length++;
                }
            }
        }
    }
    return 0;
}
//--------------------

//METHOD FOR RUNNING C
void runC(void){
    bool loop = true;
    char *c, input;
    int width, hash, index, returnValue;
    NODE * currentNode,* nextNode;

    do {
        printf("Please input the width of the hash map\n");
        fgets(GenericInput, MAX_INPUT, stdin);
        width = (int) strtol(GenericInput, &c, 10);
        if(width == 0){
            printf("Invalid input\n");
        }
    }while(width == 0);

    linkedHashMap = malloc(sizeof(HEAD) * width);
    for(int  i = 0; i < width; i++){
        linkedHashMap[i].length = -1;
    }
    while(loop) {
        printf("Please input a to add a node, f to find a node, i to insert a node, r to remove a node, s to save and l to load\n");
        printf("Enter q to quit\n");
        input = (char) getchar();
        flushBuffer();
        input = changeToLowerCase(input);
        switch (input) {
            case 'a':
                getInputs();
                if(checkForInvalidInput(NULL_LINE) == 1){
                    printf("%s is and invalid input\n", NULL_LINE);
                    break;
                }
                hash = calculateHash(DataInput, (int) strlen(DataInput), KeyInput, (int) strlen(KeyInput), width);
                if(linkedHashMap[hash].length == -1){
                    linkedHashMap[hash].node = malloc(sizeof(NODE));
                    linkedHashMap[hash].node->previous = NULL;
                    linkedHashMap[hash].node->bucket.inUse = true;
                    strcpy(linkedHashMap[hash].node->bucket.data, DataInput);
                    strcpy(linkedHashMap[hash].node->bucket.key, KeyInput);
                    linkedHashMap[hash].node->next = NULL;
                    linkedHashMap[hash].length = 0;
                }else{
                    currentNode = linkedHashMap[hash].node;
                    while(currentNode->next != NULL){
                        currentNode = currentNode->next;
                    }
                    if(addNodeC(currentNode, DataInput, KeyInput) == 0){
                        printf("Added node\n");
                        linkedHashMap[hash].length++;
                    }else{
                        printf("A error occurred adding the node\n");
                    }
                }
                clearInputs();
                break;
            case 'f':
                getInputs();
                if(checkForInvalidInput(NULL_LINE) == 1){
                    printf("%s is and invalid input\n", NULL_LINE);
                    break;
                }
                hash = calculateHash(DataInput, (int) strlen(DataInput), KeyInput, (int) strlen(KeyInput), width);
                index = findNodeC(hash);
                if(index != -1){
                    printf("---Node Found---\n");
                    printf("Hash: %d\n", hash + 1);
                    printf("Index: %d\n", index);
                    printf("----------------\n");
                }else{
                    printf("The node could not be found\n");
                }
                clearInputs();
                break;
            case 'i':
                getInputs();
                if(checkForInvalidInput(NULL_LINE) == 1){
                    printf("%s is and invalid input\n", NULL_LINE);
                    break;
                }
                hash = calculateHash(DataInput, (int) strlen(DataInput), KeyInput, (int) strlen(KeyInput), width);
                do {
                    printf("Please input the index you would like to insert into\n");
                    fgets(GenericInput, MAX_INPUT, stdin);
                    index = (int) strtol(GenericInput, &c, 10);
                    if(index == 0){
                        printf("Invalid input\n");
                    }
                }while(index == 0);
                index--;
                returnValue = insertNodeC(hash, index);
                if(returnValue == 0){
                    printf("The node was inserted successfully\n");
                }else if(returnValue == 1){
                    printf("The node was out of range\n");
                }else{
                    printf("A error has occurred while inserting the node\n");
                }
                clearInputs();
                break;
            case 'r':
                do {
                    printf("Please input the hash you would like to remove from\n");
                    fgets(GenericInput, MAX_INPUT, stdin);
                    hash = (int) strtol(GenericInput, &c, 10);
                    if(hash == 0){
                        printf("Invalid Input\n");
                    }
                }while(hash == 0);
                hash--;

                do {
                    printf("Please input the index you would like to insert into\n");
                    fgets(GenericInput, MAX_INPUT, stdin);
                    index = (int) strtol(GenericInput, &c, 10);
                    if(index == 0){
                        printf("Invalid input\n");
                    }
                }while(index == 0);
                index--;
                printf("Length: %d\n", linkedHashMap[hash].length);
                returnValue = removeNodeC(hash, index);
                if(returnValue == 0){
                    printf("Node remove\n");
                }else if(returnValue == 1){
                    printf("The node was out of bounds\n");
                }else if(returnValue == 2){
                    printf("There is nothing in the list to remove\n");
                }else{
                    printf("An error occurred\n");
                }
                break;
            case 's':
                returnValue = saveMapC(width);
                if(returnValue == 0){
                    printf("File saved\n");
                }else if(returnValue == 1){
                    printf("File could not be created\n");
                }else{
                    printf("A error occured\n");
                }
                break;
            case 'l':
                returnValue = loadMapC();
                if(returnValue == 0){
                    printf("Loading complete\n");
                }else{
                    printf("An error occurred during loading\n");
                }
                break;
            case 'q':
                for(int i = 0; i < width; i++){
                    if(linkedHashMap[i].length != -1){
                        currentNode = linkedHashMap[i].node;
                        while(currentNode->next != NULL){
                            nextNode = currentNode->next;
                            free(currentNode);
                            currentNode = nextNode;
                        }
                        free(currentNode);
                    }
                }
                printf("Quitting\n");
                loop = false;
                break;
            default:
                printf("INVALID INPUT\n");
                break;
        }
    }
}


int main() {
    bool mainLoop = true;
    char input;

    while(mainLoop){
        printf("Enter a, b, c or d to run the respective question or enter q to quit\n");
        input = (char) getchar();
        input = changeToLowerCase(input);
        flushBuffer();
        switch (input){
            case 'a':
                runA();
                break;
            case 'b':
                runB();
                break;
            case 'c':
                runC();
                break;
            case 'd':
                break;
            case 'q':
                mainLoop = false;
                break;
            default:
                printf("INVALID INPUT\n");
                break;
        }
    }
    return 0;
}