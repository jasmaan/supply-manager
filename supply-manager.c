#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// --------------------------------------------
//  Supply Manager
//  Completed during a University course
// --------------------------------------------

// Constant used in ShipmentStats struct
#define MAX_SUPPLIERS 50

// Struct for holding shipment info
struct Shipment {
    int type;          
    int amount;      
    char date[11];     
    int supplier;   
};

// Struct for managing inventory
struct Inventory {
    struct Shipment* shipments;  
    int count;         
    int max;      
};

// Struct for maintaining info of reprts
struct ShipmentStats {
    int typeTotal[10];
    int suppliers[MAX_SUPPLIERS];
    int supplierTotal[MAX_SUPPLIERS];
    int supplierCount;
    float supplierPercentage[MAX_SUPPLIERS];
    int top[3];
    int amountTotal;
};

// Helper function to reallocate memory for inv in case max is reached
void reallocInventory(struct Inventory* inv) {
    int newMax = inv->max * 2;
    inv->max = newMax;
    inv->shipments = realloc(inv->shipments, newMax * sizeof(struct Shipment));
}

// Helper function to check if file contains proper date format (YYYY-MM-DD)
int verifyDate(char* date) {
    if (strlen(date) != 10) {
        return 0;
    }
    if (date[4] != '-' || date[7] != '-') {
        return 0;
    }
    for (int i = 0; i < 10; i++) {
        if (i == 4 || i == 7) {
            continue;
        }
        if (date[i] < '0' || date[i] > '9') {
            return 0;
        }
    }

    return 1;
}

// Reads each line of shipments.txt, stores in dynamic array
struct Inventory* readShipments(char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Could not open the file\n");
        return NULL;
    }
    
    struct Inventory* inv = malloc(sizeof(struct Inventory));
    inv->max = 7;
    inv->shipments = malloc(inv->max * sizeof(struct Shipment));
    inv->count = 0;

    int type, amount, supplier;
    char date[11];

    while (fscanf(file, "%d %d %s %d", &type, &amount, date, &supplier) == 4) {
        if (type < 0 || type > 9 || amount <= 0 || verifyDate(date) == 0 || supplier < 0) {          // skip invalid data lines 
            continue;
        }
         if (inv->count == inv->max) {
            reallocInventory(inv);
        }

        inv->shipments[inv->count].type = type;
        inv->shipments[inv->count].amount = amount;
        strcpy(inv->shipments[inv->count].date, date);
        inv->shipments[inv->count].supplier = supplier; 
        inv->count++;
    }

    fclose(file);
    printf("\nThe shipments have been loaded, Mochi.\n");
    printf("---> Loaded %d valid shipments (ignored any with incorrect data)\n", inv->count);
    return inv;
}

// Frees all memory used by inventory
void freeInventory(struct Inventory* inv) {
    if (inv != NULL) {
        if (inv->shipments != NULL) {
            free(inv->shipments);
        }
        free(inv);
    }
}

// Prints single shipment details
void printShipment(struct Shipment* s) {
    printf("TYPE: %d, AMOUNT: %d, DATE: %s, SUPPLIER: %d\n",
           s->type, s->amount, s->date, s->supplier);
}

// Displays all shipments in inventory
void printAllShipments(struct Inventory* inv) {
    printf("\nHere are all shipments in inventory...\n");
    struct Shipment* ptr = inv->shipments;
    for (int i = 0; i < inv->count; i++, ptr++) {
        printf("[%d] ", i);
        printShipment(ptr);
    }
    printf("---> Total shipments: %d\n", inv->count);
}

// Adds new shipment to inventory
void addShipment(struct Inventory* inv, int type, int amount, char* date, int supplier) {
     if (type < 0 || type > 9) {
         printf("Invalid bamboo type. Not adding..\n");
         return;
    }
    if (amount <= 0) {
        printf("Invalid shipment amount. Not adding...\n");
        return;
    }
    if (supplier < 0) {
         printf("Invalid supplier ID. Not adding...\n");
         return;
    }
    if (verifyDate(date) == 0) {
        printf("Invalid date. Not adding...\n");
        return;
    }
    if (inv->count == inv->max) {
        reallocInventory(inv);
    }
    inv->shipments[inv->count].type = type;
    inv->shipments[inv->count].amount = amount;
    strcpy(inv->shipments[inv->count].date, date);
    inv->shipments[inv->count].supplier = supplier;
    inv->count++;

    printf("Shipment was added successfully.\n");
    printShipment(&inv->shipments[inv->count - 1]);
}

// Removes shipment at given index
void removeShipment(struct Inventory* inv, int index) {
    if (inv->count == 0) {
        printf("There are currently no shipments in inventory.\n");
        return;
    }
    if (index < 0 || index >= inv->count) {
        printf("This shipment does not exist. Exiting removal process.\n");
        return;
    }

    for (int i = index; i < inv->count - 1; i++) { 
        inv->shipments[i] = inv->shipments[i + 1];         // from the index you want to remove, shift to left to overwrite it
    }
    inv->count--;
    printf("Shipment was removed successfully.\n");
}

// Searches and prints shipments matching bamboo type
void searchByType(struct Inventory* inv, int type) {
    if (type < 0 || type > 9) {
        printf("This bamboo type does not exist. Exiting search by type process.\n");
        return;
    }

    printf("\nHere are all shipments by bamboo type %d...\n", type);
    int found = 0;
    struct Shipment* ptr = inv->shipments;
    for (int i = 0; i < inv->count; i++, ptr++) {
        if (type == ptr->type) {
            printShipment(ptr);
            found++;
        }
    }
    if (found == 0) {
        printf("The selected bamboo type is not currently in inventory.");
    }
}

// Searches and prints shipments matching supplier ID
void searchBySupplier(struct Inventory* inv, int supplier) {
    if (supplier < 0) {
        printf("Supplier IDs are only positive integers. Exiting search by type process.\n");
        return;
    }

    printf("\nHere are all shipments by supplier %d...\n", supplier);
    int found = 0;
    struct Shipment* ptr = inv->shipments;
    for (int i = 0; i < inv->count; i++, ptr++) {
        if (supplier == ptr->supplier) {
            printShipment(ptr);
            found++;
        }
    }
    if (found == 0) {
        printf("The selected supplier ID is not in the system.\n");
    }
}

// Searches and prints shipments within date range
void searchByRange(struct Inventory* inv, char* startDate, char* endDate) {
    int found = 0;
    struct Shipment* ptr = inv->shipments;

    printf("\nHere are all shipments between %s and %s...\n", startDate, endDate);
    for (int i = 0; i < inv->count; i++, ptr++) {
        if (strcmp(ptr->date, startDate) >= 0 && strcmp(ptr->date, endDate) <= 0) {               // sstrcmp(a,b) returns 0 if a==b, <0 if a comes before b, >0 if a comes after b
            printShipment(ptr);                                                                   // so if date comes after start date, >= 0 must satifsy
            found++;                                                                               // and if date comes before end date, <= must satisfy
        }                                                                                         // = is possible because date may match exact start/end date
    }

    if (found == 0) {
        printf("No shipments were found between that date range.\n");
    }
}

// Compares two shipments by amount for qsort
int compareAmount(const void* a, const void* b) {
    struct Shipment* s1 = (struct Shipment*)a;
    struct Shipment* s2 = (struct Shipment*)b;
    return s2->amount - s1->amount;                   // by largest to smallest
}

// Sorts shipments by amount (descending)
void sortByAmount(struct Inventory* inv) {
    printf("\nSorted shipments by amount...\n");
    qsort(inv->shipments, inv->count, sizeof(struct Shipment), compareAmount);
    printAllShipments(inv);
}

// Compares two shipments by bamboo type for qosrt
int compareType(const void* a, const void* b) {
    struct Shipment* s1 = (struct Shipment*)a;
    struct Shipment* s2 = (struct Shipment*)b;
    return s1->type - s2->type;                       // by types [0-9]
}

// Sorts shipments by bamboo type (ascending)
void sortByType(struct Inventory* inv) {
    printf("\nSorted shipments by bamboo type...\n");
    qsort(inv->shipments, inv->count, sizeof(struct Shipment), compareType);
    printAllShipments(inv);
}

// Compares two shipments by date for qsort
int compareDate(const void* a, const void* b) {
    struct Shipment* s1 = (struct Shipment*)a;
    struct Shipment* s2 = (struct Shipment*)b;
    return strcmp(s2->date, s1->date);                // newest to oldest 
}

// Sorts shipments by date (ascending)
void sortByDate(struct Inventory* inv) {
    printf("\nSorted shipments by date...\n");
    qsort(inv->shipments, inv->count, sizeof(struct Shipment), compareDate);
    printAllShipments(inv);
}

// Creates the statistics required for the final report:
// Total amount for each type, how much each supplier sent, top 3 most common types
void computeStats(struct Inventory* inv, struct ShipmentStats* stats) {
    for (int i = 0; i < 10; i++) {
        stats->typeTotal[i] = 0;
    }
    stats->supplierCount = 0;
    stats->amountTotal = 0;

    struct Shipment* ptr = inv->shipments;
    for (int i = 0; i < inv->count; i++, ptr++) {
        int type = ptr->type;                                     
        int amount = ptr->amount;
        int supplier = ptr->supplier;
        stats->typeTotal[type] = stats->typeTotal[type] + amount;     // add to bamboo type total
        stats->amountTotal = stats->amountTotal + amount;             // add to full total (for percentage calc later)

        int found = 0;
        for (int j = 0; j < stats->supplierCount; j++) {
            if (stats->suppliers[j] == supplier) {
                stats->supplierTotal[j] = stats->supplierTotal[j] + amount;  // add amount to existing supplier
                found = 1;
                break;
            }
        }
        if (found == 0) {
            int index = stats->supplierCount;
            stats->suppliers[index] = supplier;      // when supplier new, add amount (then will become existing supplier)
            stats->supplierTotal[index] = amount;
            stats->supplierCount++;
        }
    }
    for (int i = 0; i < stats->supplierCount; i++) {
        float supplierTotal = (float)stats->supplierTotal[i];
        stats->supplierPercentage[i] = (supplierTotal/ stats->amountTotal) * 100;
    }

    for (int i = 0; i < 3; i++) {
        int currentMax = 0;
        int currentType = 0;
        for (int j = 0; j < 10; j++) {
            int alreadyUsed = 0;
            for (int k = 0; k < i; k++) {
                if (j == stats->top[k]) {  // skip already selected
                    alreadyUsed = 1;
                    break;
                }
            }
            if (alreadyUsed == 0 && stats->typeTotal[j] > currentMax) {   
                currentMax = stats->typeTotal[j];
                currentType = j;
            }
        }
        stats->top[i] = currentType;
    }
}

void generateReport(struct ShipmentStats* stats, char* filename) {
    FILE* file = fopen(filename, "w");
    if (!file) {
        printf("Could not create report.txt file\n");
        return;
    }

    fprintf(file, "Total bamboo stock:\n");
    for (int i = 0; i < 10; i++) {
        fprintf(file, "Type %d: %d\n", i, stats->typeTotal[i]);
    }
    int top1 = stats->top[0];
    int top2 = stats->top[1];
    int top3 = stats->top[2];
    fprintf(file, "Top 3 bamboo types: %d, %d, %d\n", top1, top2, top3);
    fprintf(file, "Supplier statistics:\n");
    for (int i = 0; i < stats->supplierCount; i++) {
        fprintf(file, "Supplier %d: %.0f%%\n", stats->suppliers[i], stats->supplierPercentage[i]);
    }

    fclose(file);
    printf("Report generated and saved to report.txt.\n");
}

// Displays menu options
void displayMenu() {
    printf("\n-------------------------------------------------------------------------\n");
    printf("                      Mochi's Bamboo Supply Manager\n");
    printf("\n1. Not sure what's in inventory? View all shipments\n");
    printf("2. Add new shipment\n");
    printf("3. Remove old/spoiled shipment\n");
    printf("4. Search by bamboo type\n");
    printf("5. Search by supplier ID\n");
    printf("6. Search by date range\n");
    printf("7. Sort by amount (biggest to smallest)\n");
    printf("8. Sort by bamboo type (0 to 9)\n");
    printf("9. Sort by date (newest to oldest)\n");
    printf("10. Generate a report for the Forest Council\n");
    printf("0. Exit\n");
    printf("\n-------------------------------------------------------------------------\n");
}

// Handles user interaction
// Checks if user's inputs are correct type
void menu(struct Inventory* inv) {
    int choice;
    while (1) {
        displayMenu();
        printf("\nChoice: ");
        if (scanf("%d", &choice) != 1) {
            printf("Mochi entered an invalid input. Try again.\n");
            while (getchar() != '\n');
            continue;
        }

        switch (choice) {
            case 1:
                printAllShipments(inv);
                break;

            case 2: {
                int type, amount, supplier;
                char date[11];
                printf("Enter bamboo type [0-9]: \n");
                if (scanf("%d", &type) != 1) {
                    printf("Invalid bamboo type. Inteeger needed.\n");
                    while (getchar() != '\n');
                    break;
                }
                printf("Enter amount: \n");
                if (scanf("%d", &amount) != 1) {
                    printf("Invalid amount. Integer needed.\n");
                    while (getchar() != '\n');
                    break;
                }
                printf("Enter date (YYYY-MM-DD): \n");
                scanf("%s", &date);
                if (verifyDate(date) == 0) {
                    printf("Invalid date. (YYYY-MM-DD) needed.\n");
                    break;
                }
                printf("Enter supplier's ID: \n");
                if (scanf("%d", &supplier) != 1) {
                    printf("Invalid supplier ID. Integer needed.\n");
                    while (getchar() != '\n');
                    break;
                }
                addShipment(inv, type, amount, date, supplier);
                break;
            }

            case 3: {
                int index;
                printAllShipments(inv);
                printf("\nSelect an index to remove: \n");
                if (scanf("%d", &index) != 1) {
                    printf("Invalid index. Integer required.\n");
                    while (getchar() != '\n');
                    break;
                }
                removeShipment(inv, index);
                break; 
            }    

            case 4: {
                int type;
                printf("Enter bamboo type to search by [0-9]: \n");
                if (scanf("%d", &type) != 1) {
                    printf("Invalid type. Integer needed.\n");
                    while (getchar() != '\n');
                    break;
                }
                searchByType(inv, type);        
                break;
            }

            case 5: {
                int supplier;
                printf("Enter supplier's ID: \n");
                if (scanf("%d", &supplier) != 1) {
                    printf("Invalid supplier ID. Integer needed.\n");
                    while (getchar() != '\n');
                    break;
                }
                searchBySupplier(inv, supplier);
                break;
            }

            case 6: { 
                char startDate[11];
                char endDate[11];
                printf("Enter your start date (YYYY-MM-DD): \n");
                scanf("%s", startDate); 
                if (verifyDate(startDate) == 0) {
                    printf("Invalid start date. (YYYY-MM-DD) needed.\n");
                    break;
                }
                printf("Enter your end date (YYYY-MM-DD): \n");
                scanf("%s", endDate);
                if (verifyDate(endDate) == 0) {
                    printf("Invalid end date. (YYYY-MM-DD) needed.\n");
                    break;
                }
                searchByRange(inv, startDate, endDate);
                break;
            }

            case 7:
                sortByAmount(inv);
                break;

            case 8:
                sortByType(inv);
                break;

            case 9:
                sortByDate(inv);
                break;

            case 10: {
                struct ShipmentStats stats;
                computeStats(inv, &stats);
                generateReport(&stats, "report.txt");
                break;
            }

            case 0:
                printf("Exiting...\n \n");
                return;

            default:
                printf("Mochi entered an invalid input. Try again.\n");
                break;
        }
    }
}

int main() {
    struct Inventory* inv = readShipments("shipments.txt");
    if (inv == NULL) {
        printf("Could not load shipments\n");
        return 1;
    }
    menu(inv);
    freeInventory(inv);
    return 0;
}
