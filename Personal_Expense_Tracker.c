#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define constants for categories
#define MAX_CATEGORIES 100
#define MAX_ENTRIES 100

char categories[MAX_CATEGORIES][20] = {"Food", "Transport", "Entertainment", "Bills", "Others"};
int categoryCount = 5;

// Define a structure to hold expense data
typedef struct {
    char category[20];
    float amount;
} Expense;

// Function prototypes
void addCategory();
void addExpense(Expense expenses[], int *count, const char *filename);
void displayExpenses(const char *filename);
void displayTotalByCategory(const char *filename);
void saveExpenseToFile(Expense expense, const char *filename);
void removeExpense(const char *filename);
void removeAllExpenses(const char *filename);
void editExpense(const char *filename);

int main() {
    Expense expenses[MAX_ENTRIES];
    int expenseCount = 0;
    int choice;
    const char *filename = "expenses.txt";

    do {
        printf("\n--- Personal Expense Tracker ---\n");
        printf("1. Add Category\n");
        printf("2. Add Expense\n");
        printf("3. Display All Expenses\n");
        printf("4. Display Total by Category\n");
        printf("5. Remove an Expense\n");
        printf("6. Edit an Expense\n");
        printf("7. Remove All Expenses\n");
        printf("8. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                addCategory();
                break;
            case 2:
                addExpense(expenses, &expenseCount, filename);
                break;
            case 3:
                displayExpenses(filename);
                break;
            case 4:
                displayTotalByCategory(filename);
                break;
            case 5:
                removeExpense(filename);
                break;
            case 6:
                editExpense(filename);
                break;
            case 7:
                removeAllExpenses(filename);
                break;
            case 8:
                printf("Exiting the program. Goodbye!\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 8);

    return 0;
}

// Function to add a new category
void addCategory() {
    if (categoryCount >= MAX_CATEGORIES) {
        printf("Error: Maximum number of categories reached!\n");
        return;
    }

    printf("Enter the name of the new category: ");
    scanf("%s", categories[categoryCount]);
    categoryCount++;
    printf("Category added successfully!\n");
}

// Function to add a new expense
void addExpense(Expense expenses[], int *count, const char *filename) {
    if (*count >= MAX_ENTRIES) {
        printf("Error: Maximum number of entries reached!\n");
        return;
    }

    printf("\nAvailable Categories:\n");
    for (int i = 0; i < categoryCount; i++) {
        printf("%d. %s\n", i + 1, categories[i]);
    }

    int categoryChoice;
    printf("Enter category number: ");
    scanf("%d", &categoryChoice);

    if (categoryChoice < 1 || categoryChoice > categoryCount) {
        printf("Invalid category. Expense not added.\n");
        return;
    }

    Expense newExpense;
    strcpy(newExpense.category, categories[categoryChoice - 1]);

    printf("Enter amount: ");
    scanf("%f", &newExpense.amount);

    expenses[*count] = newExpense;
    (*count)++;

    saveExpenseToFile(newExpense, filename);

    printf("Expense added successfully!\n");
}

// Function to save an expense to a file
void saveExpenseToFile(Expense expense, const char *filename) {
    FILE *file = fopen(filename, "a");
    if (file == NULL) {
        printf("Error: Could not open file for writing.\n");
        return;
    }

    fprintf(file, "%s %.2f\n", expense.category, expense.amount);
    fclose(file);
}

// Function to display all expenses
void displayExpenses(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("No expenses to display.\n");
        return;
    }

    printf("\nAll Expenses:\n");
    char category[20];
    float amount;
    int count = 1;

    while (fscanf(file, "%s %f", category, &amount) != EOF) {
        printf("%d. Category: %s, Amount: %.2f\n", count++, category, amount);
    }

    fclose(file);
}

// Function to display total expenses by category
void displayTotalByCategory(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("No expenses to display.\n");
        return;
    }

    float totals[MAX_CATEGORIES] = {0};
    char category[20];
    float amount;

    while (fscanf(file, "%s %f", category, &amount) != EOF) {
        for (int i = 0; i < categoryCount; i++) {
            if (strcmp(category, categories[i]) == 0) {
                totals[i] += amount;
                break;
            }
        }
    }

    fclose(file);

    printf("\nTotal Expenses by Category:\n");
    for (int i = 0; i < categoryCount; i++) {
        printf("%s: %.2f\n", categories[i], totals[i]);
    }
}

// Function to remove an expense
void removeExpense(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("No expenses to remove.\n");
        return;
    }

    Expense expenses[MAX_ENTRIES];
    int count = 0;

    while (fscanf(file, "%s %f", expenses[count].category, &expenses[count].amount) != EOF) {
        count++;
    }
    fclose(file);

    if (count == 0) {
        printf("No expenses to remove.\n");
        return;
    }

    displayExpenses(filename);
    int index;
    printf("Enter the number of the expense to remove: ");
    printf("(Enter 0 to remove all expenses)\n");
    scanf("%d", &index);

    if (index == 0) {
        removeAllExpenses(filename);
        return;
    }

    if (index < 1 || index > count) {
        printf("Invalid entry.\n");
        return;
    }

    file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error: Could not open file for writing.\n");
        return;
    }

    for (int i = 0; i < count; i++) {
        if (i != index - 1) {
            fprintf(file, "%s %.2f\n", expenses[i].category, expenses[i].amount);
        }
    }
    fclose(file);

    printf("Expense removed successfully!\n");
}

// Function to remove all expenses
void removeAllExpenses(const char *filename) {
    // Check if the file exists and has content
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("No expenses to remove.\n");
        return;
    }

    // Check if the file is empty
    fseek(file, 0, SEEK_END);
    if (ftell(file) == 0) {
        printf("No expenses to remove.\n");
        fclose(file);
        return;
    }
    fclose(file);

    // Overwrite the file to remove all data
    file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error: Could not open file for writing.\n");
        return;
    }
    fclose(file);

    // Check if the file is now empty and delete it
    file = fopen(filename, "r");
    fseek(file, 0, SEEK_END);
    if (ftell(file) == 0) {
        fclose(file);
        if (remove(filename) == 0) {
            printf("All expenses removed, and the file has been deleted successfully!\n");
        } else {
            printf("All expenses removed, but the file could not be deleted.\n");
        }
    } else {
        fclose(file);
        printf("All expenses removed successfully!\n");
    }
}


// Function to edit an expense
void editExpense(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("No expenses to edit.\n");
        return;
    }

    Expense expenses[MAX_ENTRIES];
    int count = 0;

    while (fscanf(file, "%s %f", expenses[count].category, &expenses[count].amount) != EOF) {
        count++;
    }
    fclose(file);

    if (count == 0) {
        printf("No expenses to edit.\n");
        return;
    }

    displayExpenses(filename);
    int index;
    printf("Enter the number of the expense to edit: ");
    scanf("%d", &index);

    if (index < 1 || index > count) {
        printf("Invalid entry.\n");
        return;
    }

    printf("Current Category: %s, Amount: %.2f\n", expenses[index - 1].category, expenses[index - 1].amount);

    printf("Enter new category: \n");
    for (int i = 0; i < categoryCount; i++) {
        printf("%d. %s\n", i + 1, categories[i]);
    }

    int categoryChoice;
    printf("Enter category number: ");
    scanf("%d", &categoryChoice);

    if (categoryChoice < 1 || categoryChoice > categoryCount) {
        printf("Invalid category. Expense not edited.\n");
        return;
    }

    strcpy(expenses[index - 1].category, categories[categoryChoice - 1]);

    printf("Enter new amount: ");
    scanf("%f", &expenses[index - 1].amount);

    file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error: Could not open file for writing.\n");
        return;
    }

    for (int i = 0; i < count; i++) {
        fprintf(file, "%s %.2f\n", expenses[i].category, expenses[i].amount);
    }
    fclose(file);

    printf("Expense edited successfully!\n");
}
