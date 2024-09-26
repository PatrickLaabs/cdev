#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME 100

typedef struct {
    int id;
    char name[MAX_NAME];
    int age;
} User;

void addUser(FILE *db) {
    User user;

    printf("Enter User ID: ");
    scanf("%d", &user.id);
    printf("Enter User Name: ");
    scanf("%s", user.name);
    printf("Enter User Age: ");
    scanf("%d", &user.age);

    fseek(db, 0, SEEK_END);  // Move to the end of the file
    fwrite(&user, sizeof(User), 1, db);  // Write user record to file
    printf("User added successfully!\n");
}

void listUsers(FILE *db) {
    User user;
    fseek(db, 0, SEEK_SET);  // Go to the start of the file

    printf("\nList of Users:\n");
    while (fread(&user, sizeof(User), 1, db)) {
        printf("ID: %d, Name: %s, Age: %d\n", user.id, user.name, user.age);
    }
}

void findUser(FILE *db, int id) {
    User user;
    fseek(db, 0, SEEK_SET);  // Go to the start of the file

    while (fread(&user, sizeof(User), 1, db)) {
        if (user.id == id) {
            printf("Found User: ID: %d, Name: %s, Age: %d\n", user.id, user.name, user.age);
            return;
        }
    }
    printf("User with ID %d not found.\n", id);
}

void updateUser(FILE *db, int id) {
    User user;
    fseek(db, 0, SEEK_SET);  // Go to the start of the file

    long pos;
    while (fread(&user, sizeof(User), 1, db)) {
        if (user.id == id) {
            printf("Enter new name: ");
            scanf("%s", user.name);
            printf("Enter new age: ");
            scanf("%d", &user.age);

            // Move file pointer back to the beginning of this record
            pos = ftell(db) - sizeof(User);
            fseek(db, pos, SEEK_SET);

            fwrite(&user, sizeof(User), 1, db);  // Overwrite the record
            printf("User updated successfully!\n");
            return;
        }
    }
    printf("User with ID %d not found.\n", id);
}

void deleteUser(FILE *db, int id) {
    FILE *tempDb = fopen("temp.bin", "wb");
    User user;
    fseek(db, 0, SEEK_SET);  // Go to the start of the file

    int found = 0;
    while (fread(&user, sizeof(User), 1, db)) {
        if (user.id != id) {
            fwrite(&user, sizeof(User), 1, tempDb);  // Write all except the one to delete
        } else {
            found = 1;
        }
    }

    fclose(db);
    fclose(tempDb);

    remove("users.bin");
    rename("temp.bin", "users.bin");

    if (found) {
        printf("User deleted successfully!\n");
    } else {
        printf("User with ID %d not found.\n", id);
    }
}

int main() {
    FILE *db = fopen("users.bin", "rb+");

    if (db == NULL) {
        db = fopen("users.bin", "wb+");
        if (db == NULL) {
            printf("Failed to open database file.\n");
            return 1;
        }
    }

    int choice, id;

    while (1) {
        printf("\nMenu:\n");
        printf("1. Add User\n");
        printf("2. List Users\n");
        printf("3. Find User\n");
        printf("4. Update User\n");
        printf("5. Delete User\n");
        printf("6. Exit\n");
        printf("Choose an option: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                addUser(db);
                break;
            case 2:
                listUsers(db);
                break;
            case 3:
                printf("Enter User ID to find: ");
                scanf("%d", &id);
                findUser(db, id);
                break;
            case 4:
                printf("Enter User ID to update: ");
                scanf("%d", &id);
                updateUser(db, id);
                break;
            case 5:
                printf("Enter User ID to delete: ");
                scanf("%d", &id);
                deleteUser(db, id);
                // Re-open the file after deleting to ensure correct file pointer
                db = fopen("users.bin", "rb+");
                break;
            case 6:
                fclose(db);
                return 0;
            default:
                printf("Invalid option.\n");
                break;
        }
    }

    return 0;
}
