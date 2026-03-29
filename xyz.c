#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define KEY 123

// ENCRYPT / DECRYPT
void encrypt(char *str)
{
    for (int i = 0; str[i] != '\0'; i++)
        str[i] ^= KEY;
}

// CLEAR BUFFER
void clearBuffer()
{
    while (getchar() != '\n');
}

// MASKED PASSWORD INPUT
void inputPassword(char *pass)
{
    char ch;
    int i = 0;

    while ((ch = getchar()) != '\n')
    {
        pass[i++] = ch;
        printf("*");
    }
    pass[i] = '\0';
    printf("\n");
}

// PASSWORD VALIDATION
void getValidPassword(char *password)
{
    while (1)
    {
        int upper = 0, lower = 0, digit = 0, special = 0;

        printf("\nEnter Strong Password: ");
        inputPassword(password);

        if (strlen(password) < 8)
        {
            printf("Minimum 8 characters required\n");
            continue;
        }

        for (int i = 0; password[i]; i++)
        {
            if (isupper(password[i])) upper = 1;
            else if (islower(password[i])) lower = 1;
            else if (isdigit(password[i])) digit = 1;
            else special = 1;
        }

        if (upper && lower && digit && special)
        {
            printf("Strong Password Accepted\n");
            break;
        }
        else
        {
            printf("\n⚠ Weak Password! Suggestions:\n");
            if (!upper) printf("- Add uppercase\n");
            if (!lower) printf("- Add lowercase\n");
            if (!digit) printf("- Add digit\n");
            if (!special) printf("- Add special character\n");
        }
    }
}

// SIGNUP
void signup()
{
    char name[50], id[20], password[100];
    char file_name[50], file_id[20], file_pass[100];
    char line[200];
    char choice;
    int found = 0;

    FILE *fp = fopen("users.txt", "r");

    printf("\n===== SIGNUP =====\n");

    printf("\nEnter Your Name: ");
    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")] = 0;

    printf("\nEnter Your ID: ");
    fgets(id, sizeof(id), stdin);
    id[strcspn(id, "\n")] = 0;

    // DUPLICATE CHECK
    if (fp != NULL)
    {
        while (fgets(line, sizeof(line), fp))
        {
            sscanf(line, "%[^|]|%[^|]|%[^\n]", file_name, file_id, file_pass);

            if (strcmp(name, file_name) == 0 &&
                strcmp(id, file_id) == 0)
            {
                found = 1;
                break;
            }
        }
        fclose(fp);
    }

    // IF USER EXISTS
    if (found)
    {
        printf("\n⚠ User already exists!");
        printf("\nUpdate password? (y/n): ");
        scanf(" %c", &choice);
        clearBuffer();

        if (choice == 'y' || choice == 'Y')
        {
            FILE *temp = fopen("temp.txt", "w");
            fp = fopen("users.txt", "r");

            getValidPassword(password);
            encrypt(password);

            while (fgets(line, sizeof(line), fp))
            {
                sscanf(line, "%[^|]|%[^|]|%[^\n]", file_name, file_id, file_pass);

                if (strcmp(name, file_name) == 0 &&
                    strcmp(id, file_id) == 0)
                {
                    fprintf(temp, "%s|%s|%s\n", name, id, password);
                }
                else
                {
                    fprintf(temp, "%s|%s|%s\n", file_name, file_id, file_pass);
                }
            }

            fclose(fp);
            fclose(temp);

            remove("users.txt");
            rename("temp.txt", "users.txt");

            printf("\nPassword Updated Successfully\n");
        }
        else
        {
            printf("\nSignup Cancelled\n");
        }
        return;
    }

    // NEW USER
    getValidPassword(password);
    encrypt(password);

    fp = fopen("users.txt", "a");
    fprintf(fp, "%s|%s|%s\n", name, id, password);
    fclose(fp);

    printf("\nSignup Successful\n");
}

// LOGIN
void login()
{
    char id[20], password[100];
    char file_name[50], file_id[20], file_pass[100];
    char line[200];
    int found = 0;

    FILE *fp = fopen("users.txt", "r");

    if (!fp)
    {
        printf("\nNo users found\n");
        return;
    }

    printf("\n===== LOGIN =====\n");

    printf("\nEnter Your ID: ");
    fgets(id, sizeof(id), stdin);
    id[strcspn(id, "\n")] = 0;

    printf("\nEnter Your Password: ");
    inputPassword(password);
    encrypt(password);

    while (fgets(line, sizeof(line), fp))
    {
        sscanf(line, "%[^|]|%[^|]|%[^\n]", file_name, file_id, file_pass);

        if (strcmp(id, file_id) == 0 &&
            strcmp(password, file_pass) == 0)
        {
            found = 1;
            printf("\nLogin Successful! Welcome %s\n", file_name);
            break;
        }
    }

    if (!found)
        printf("\nInvalid ID or Password\n");

    fclose(fp);
}

// MAIN
int main()
{
    int choice;

    while (1)
    {
        printf("\n========================\n");
        printf("   USER MANAGEMENT APP\n");
        printf("========================\n");

        printf("\n1. Signup\n");
        printf("2. Login\n");
        printf("3. Exit\n");

        printf("\nEnter choice: ");

        if (scanf("%d", &choice) != 1)
        {
            clearBuffer();
            printf("Invalid input\n");
            continue;
        }

        clearBuffer();

        switch (choice)
        {
        case 1:
            signup();
            break;

        case 2:
            login();
            break;

        case 3:
            printf("\nExiting...\n");
            return 0;

        default:
            printf("\nInvalid choice\n");
        }
    }

    return 0;
}