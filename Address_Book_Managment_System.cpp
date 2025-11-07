#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Define the structure for a contact
typedef struct Contact {
    char name[100];
    char phone[20];
    char email[100];
    char address[200];
    struct Contact *next;
} Contact;

// Function prototypes
void addContact(Contact **head);
void deleteContact(Contact **head);
void editContact(Contact *head);
void searchContact(Contact *head);
void displayAllContacts(Contact *head);
void sortContacts(Contact **head);
void saveToFile(Contact *head);
void loadFromFile(Contact **head);
void freeContacts(Contact **head);
int isValidPhone(const char *phone);
int isValidEmail(const char *email);

// Main function
int main() {
    Contact *head = NULL;
    int choice;
    char filename[] = "addressbook.txt"; // Default file for saving/loading

    // Load existing contacts from file if available
    loadFromFile(&head);

    do {
        printf("\n--- Advanced Address Book Management System ---\n");
        printf("1. Add Contact\n");
        printf("2. Delete Contact\n");
        printf("3. Edit Contact\n");
        printf("4. Search Contact\n");
        printf("5. Display All Contacts\n");
        printf("6. Sort Contacts by Name\n");
        printf("7. Save to File\n");
        printf("8. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar(); // Consume newline

        switch (choice) {
            case 1:
                addContact(&head);
                break;
            case 2:
                deleteContact(&head);
                break;
            case 3:
                editContact(head);
                break;
            case 4:
                searchContact(head);
                break;
            case 5:
                displayAllContacts(head);
                break;
            case 6:
                sortContacts(&head);
                printf("Contacts sorted by name.\n");
                break;
            case 7:
                saveToFile(head);
                printf("Contacts saved to %s.\n", filename);
                break;
            case 8:
                saveToFile(head); // Auto-save on exit
                freeContacts(&head);
                printf("Exiting and saving changes.\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 8);

    return 0;
}

// Function to add a new contact
void addContact(Contact **head) {
    Contact *newContact = (Contact *)malloc(sizeof(Contact));
    if (!newContact) {
        printf("Memory allocation failed.\n");
        return;
    }

    printf("Enter name: ");
    fgets(newContact->name, sizeof(newContact->name), stdin);
    newContact->name[strcspn(newContact->name, "\n")] = 0; // Remove newline

    do {
        printf("Enter phone number: ");
        fgets(newContact->phone, sizeof(newContact->phone), stdin);
        newContact->phone[strcspn(newContact->phone, "\n")] = 0;
        if (!isValidPhone(newContact->phone)) {
            printf("Invalid phone number. Please enter a valid format (e.g., 123-456-7890 or 1234567890).\n");
        }
    } while (!isValidPhone(newContact->phone));

    do {
        printf("Enter email: ");
        fgets(newContact->email, sizeof(newContact->email), stdin);
        newContact->email[strcspn(newContact->email, "\n")] = 0;
        if (!isValidEmail(newContact->email)) {
            printf("Invalid email format. Please enter a valid email.\n");
        }
    } while (!isValidEmail(newContact->email));

    printf("Enter address: ");
    fgets(newContact->address, sizeof(newContact->address), stdin);
    newContact->address[strcspn(newContact->address, "\n")] = 0;

    newContact->next = *head;
    *head = newContact;

    printf("Contact added successfully.\n");
}

// Function to delete a contact by name
void deleteContact(Contact **head) {
    char name[100];
    printf("Enter name to delete: ");
    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")] = 0;

    Contact *current = *head;
    Contact *previous = NULL;

    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            if (previous == NULL) {
                *head = current->next;
            } else {
                previous->next = current->next;
            }
            free(current);
            printf("Contact deleted successfully.\n");
            return;
        }
        previous = current;
        current = current->next;
    }
    printf("Contact not found.\n");
}

// Function to edit a contact by name
void editContact(Contact *head) {
    char name[100];
    printf("Enter name to edit: ");
    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")] = 0;

    Contact *current = head;
    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            printf("Editing contact: %s\n", current->name);

            printf("Enter new name (leave blank to keep current): ");
            char temp[100];
            fgets(temp, sizeof(temp), stdin);
            temp[strcspn(temp, "\n")] = 0;
            if (strlen(temp) > 0) strcpy(current->name, temp);

            do {
                printf("Enter new phone (leave blank to keep current): ");
                fgets(temp, sizeof(temp), stdin);
                temp[strcspn(temp, "\n")] = 0;
                if (strlen(temp) > 0) {
                    if (isValidPhone(temp)) {
                        strcpy(current->phone, temp);
                    } else {
                        printf("Invalid phone number. Keeping current.\n");
                    }
                } else {
                    break;
                }
            } while (!isValidPhone(temp));

            do {
                printf("Enter new email (leave blank to keep current): ");
                fgets(temp, sizeof(temp), stdin);
                temp[strcspn(temp, "\n")] = 0;
                if (strlen(temp) > 0) {
                    if (isValidEmail(temp)) {
                        strcpy(current->email, temp);
                    } else {
                        printf("Invalid email. Keeping current.\n");
                    }
                } else {
                    break;
                }
            } while (!isValidEmail(temp));

            printf("Enter new address (leave blank to keep current): ");
            fgets(temp, sizeof(temp), stdin);
            temp[strcspn(temp, "\n")] = 0;
            if (strlen(temp) > 0) strcpy(current->address, temp);

            printf("Contact updated successfully.\n");
            return;
        }
        current = current->next;
    }
    printf("Contact not found.\n");
}

// Function to search for a contact by name, phone, or email
void searchContact(Contact *head) {
    char query[100];
    printf("Enter search query (name, phone, or email): ");
    fgets(query, sizeof(query), stdin);
    query[strcspn(query, "\n")] = 0;

    Contact *current = head;
    int found = 0;
    while (current != NULL) {
        if (strstr(current->name, query) || strstr(current->phone, query) || strstr(current->email, query)) {
            printf("\nName: %s\nPhone: %s\nEmail: %s\nAddress: %s\n", 
                   current->name, current->phone, current->email, current->address);
            found = 1;
        }
        current = current->next;
    }
    if (!found) {
        printf("No contacts found.\n");
    }
}

// Function to display all contacts
void displayAllContacts(Contact *head) {
    if (head == NULL) {
        printf("No contacts to display.\n");
        return;
    }
    Contact *current = head;
    while (current != NULL) {
        printf("\nName: %s\nPhone: %s\nEmail: %s\nAddress: %s\n", 
               current->name, current->phone, current->email, current->address);
        current = current->next;
    }
}

// Function to sort contacts by name (bubble sort on linked list)
void sortContacts(Contact **head) {
    if (*head == NULL) return;

    int swapped;
    Contact *ptr1;
    Contact *lptr = NULL;

    do {
        swapped = 0;
        ptr1 = *head;

        while (ptr1->next != lptr) {
            if (strcmp(ptr1->name, ptr1->next->name) > 0) {
                // Swap nodes
                Contact *temp = ptr1->next;
                ptr1->next = temp->next;
                temp->next = ptr1;
                if (ptr1 == *head) {
                    *head = temp;
                } else {
                    Contact *prev = *head;
                    while (prev->next != ptr1) prev = prev->next;
                    prev->next = temp;
                }
                swapped = 1;
            } else {
                ptr1 = ptr1->next;
            }
        }
        lptr = ptr1;
    } while (swapped);
}

// Function to save contacts to a file
void saveToFile(Contact *head) {
    FILE *file = fopen("addressbook.txt", "w");
    if (!file) {
        printf("Error opening file for writing.\n");
        return;
    }
    Contact *current = head;
    while (current != NULL) {
        fprintf(file, "%s|%s|%s|%s\n", current->name, current->phone, current->email, current->address);
        current = current->next;
    }
    fclose(file);
}

// Function to load contacts from a file
void loadFromFile(Contact **head) {
    FILE *file = fopen("addressbook.txt", "r");
    if (!file) {
        // File doesn't exist yet, no error
        return;
    }
    char line[500];
    while (fgets(line, sizeof(line), file)) {
        Contact *newContact = (Contact *)malloc(sizeof(Contact));
        if (!newContact) {
            printf("Memory allocation failed during load.\n");
            fclose(file);
            return;
        }
        char *token = strtok(line, "|");
        if (token) strcpy(newContact->name, token);
        token = strtok(NULL, "|");
        if (token) strcpy(newContact->phone, token);
        token = strtok(NULL, "|");
        if (token) strcpy(newContact->email, token);
        token = strtok(NULL, "|");
        if (token) {
            strcpy(newContact->address, token);
            newContact->address[strcspn(newContact->address, "\n")] = 0;
        }
        newContact->next = *head;
        *head = newContact;
    }
    fclose(file);
    // Sort after loading
    sortContacts(head);
}

// Function to free all contacts
void freeContacts(Contact **head) {
    Contact *current = *head;
    while (current != NULL) {
        Contact *temp = current;
        current = current->next;
        free(temp);
    }
    *head = NULL;
}

// Simple phone validation (allows digits, dashes, parentheses)
int isValidPhone(const char *phone) {
    int len = strlen(phone);
    if (len < 7 || len > 15) return 0;
    for (int i = 0; i < len; i++) {
        if (!isdigit(phone[i]) && phone[i] != '-' && phone[i] != '(' && phone[i] != ')' && phone[i] != ' ') {
            return 0;
        }
    }
    return 1;
}

// Simple email validation (checks for @ and .)
int isValidEmail(const char *email) {
    const char *at = strchr(email, '@');
    if (!at) return 0;
    const char *dot = strchr(at, '.');
    if (!dot || dot == at + 1) return 0;
    return 1;
}