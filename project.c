#pragma region basics
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <locale.h>
#include <ctype.h>
#pragma region design
#define MAX_TODO 100
#define MAX_PLAN 20
#define COLOR_RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN "\033[36m"
#define GRAS "\033[1m"
#pragma endregion
#pragma region RECORDS
typedef struct
{
    char todo[MAX_TODO];
} todolist;

typedef struct
{ // time
    int min, hour;
} TIME;

typedef struct
{ // date
    int day, month, year;
} MyDATE;

typedef struct
{
    char plan[MAX_PLAN];
    char module[11];
    int priorite;
    char note[200];
    MyDATE date;
    TIME starttime;
    TIME duration;
} planning;

typedef enum
{
    LANG_FR,
    LANG_EN
} Language;
Language currentLang = LANG_EN;
#define TR_FR_EN(fr, en) (currentLang == LANG_FR ? (fr) : (en))

#pragma endregion
#pragma region CONTROL ACTIONS

void design(int p)
{
    if (p == 1)
    {
        system("cls");
        printf(RED "\t\t\t_                 _ _                   \n" COLOR_RESET);
        printf(YELLOW "\t\t\t| | ___   __ _  __| (_)_ __   __ _       \n" COLOR_RESET);
        printf(MAGENTA "\t\t\t| |/ _ \\ / _` |/ _` | | '_ \\ / _` |      \n" COLOR_RESET);
        printf(BLUE "\t\t\t| | (_) | (_| | (_| | | | | | (_| |_ _ _ \n" COLOR_RESET);
        printf(GREEN "\t\t\t|_|\\___/ \\__,_|\\__,_|_|_| |_|\\__, (_|_|_)\n" COLOR_RESET);
        printf(CYAN "\t\t\t                             |___/       \n" COLOR_RESET);
        sleep(2);
    }
}

void chooseLanguage()
{
    printf(MAGENTA GRAS "\tChoisissez la langue /Choose language :\n" COLOR_RESET);
    printf(YELLOW "\t=============================================\n" COLOR_RESET);
    printf(BLUE "\t\t[1].English\n" COLOR_RESET);
    printf("\t\t \n");
    printf(BLUE "\t\t[2].Francais\n" COLOR_RESET);
    printf("\t\t \n");
    printf(YELLOW "\t=============================================\n" COLOR_RESET);
    printf(GREEN GRAS "\tvotre choix / your choice :" COLOR_RESET);
    int language;
    scanf("%d", &language);
    while (getchar() != '\n')
        ;

    if (language == 2)
    {
        currentLang = LANG_FR;
    }
    else
    {
        currentLang = LANG_EN;
    }
}

bool bisextuelle(int h)
{
    if ((h % 4 == 0 && h % 100 != 0) || (h % 400 == 0))
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool sameDate(MyDATE *a, MyDATE *b)
{
    return ((a->day == b->day) && (a->month == b->month) && (a->year == b->year));
}

int compareDate(MyDATE d1, MyDATE d2)
{
    if (d1.year != d2.year)
        return d1.year - d2.year;
    if (d1.month != d2.month)
        return d1.month - d2.month;
    return d1.day - d2.day;
}

planning *file_to_vec(const char *filename, int *vectorSize) // file to vec
{
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        printf(YELLOW GRAS "\t%s (-_-)\n" COLOR_RESET, TR_FR_EN("echec d'allocation memoire", "Memory allocation failed"));
        return NULL;
    }

    // First, count the number of lines in the file
    int size = 0;
    while (!feof(file))
    {
        char line[1024];
        if (fgets(line, 1024, file))
        {
            size++;
        }
    }

    // Allocate memory for the vector (planning structs)
    *vectorSize = size;

    planning *vec = (planning *)malloc(*vectorSize * sizeof(planning));
    if (!vec)
    {
        printf(YELLOW GRAS "\t%s (-_-)\n" COLOR_RESET, TR_FR_EN("Échec d'allocation mémoire", "Memory allocation failed"));
        fclose(file);
        return NULL;
    }

    // Reset the file pointer to the beginning for reading data
    fseek(file, 0, SEEK_SET);

    // Read the data into the vector
    int i = 0;
    while (!feof(file))
    {
        char *line = (char *)malloc(1024 * sizeof(char)); // Dynamically allocate memory for each line
        if (fgets(line, 1024, file))
        {
            sscanf(line, "%s %s %d %s %d %d %d %d %d %d %d",
                   vec[i].plan, vec[i].module, &vec[i].priorite, vec[i].note,
                   &vec[i].date.day, &vec[i].date.month, &vec[i].date.year,
                   &vec[i].starttime.hour, &vec[i].starttime.min,
                   &vec[i].duration.hour, &vec[i].duration.min);

            i++;
        }
        free(line); // Free the allocated memory for each line
    }

    fclose(file);
    return vec;
}

void vec_to_file(const char *filename, planning *vec, int vectorSize)
{
    // Open the file in write mode to overwrite previous content
    FILE *file = fopen(filename, "w");
    if (!file)
    {
        printf(YELLOW GRAS "\t%s (-_-)\n" COLOR_RESET, TR_FR_EN("Échec d'ouverture en écriture", "Failed to open file for writing"));
        return;
    }

    // Write each element of the vector to the file
    for (int i = 0; i < vectorSize; i++)
    {
        fprintf(file, "%s %s %d %s %d %d %d %d %d %d\n",
                vec[i].plan, vec[i].module, vec[i].priorite, vec[i].note,
                vec[i].date.day, vec[i].date.month, vec[i].date.year,
                vec[i].starttime.hour, vec[i].starttime.min,
                vec[i].duration.hour, vec[i].duration.min);
    }

    // Close the file after writing
    fclose(file);
}

int CharToInt(int *val)
{
    char buffer[100]; // Buffer to hold the input string
    int valid;
    do
    {
        fgets(buffer, sizeof(buffer), stdin);

        // Remove newline if present
        buffer[strcspn(buffer, "\n")] = '\0';

        valid = 1; // Assume it's valid
        for (int i = 0; buffer[i] != '\0'; i++)
        {
            if (!isdigit((unsigned char)buffer[i]) && !((i == 0) && (buffer[i] == '-')))
            {
                valid = 0;
                break;
            }
        }

        if (valid && strlen(buffer) > 0)
        {
            *val = atoi(buffer); // Convert the string to an int
            return *val;
        }
        else
        {
            printf(RED GRAS "\t%s\n" COLOR_RESET,
                   TR_FR_EN("Veuillez entrer un nombre valide: ", "Please enter a valid number: "));
        }

    } while (1);
}

int RepeatUntil(int num, int from, int to, int *result)
{
    while ((num < from) || (num > to))
    {
        printf(RED GRAS "\t%s (-_-)\n" COLOR_RESET, TR_FR_EN("Choix invalide", "Invalid choice"));
        CharToInt(&num);
    }
    *result = num;
    return 0;
}

#pragma endregion
#pragma region MAIN FUNCS

void deleteTodoByIndex(const char *filename, int index)
{
    FILE *original = fopen(filename, "r");
    if (original == NULL)
    {
        perror(RED GRAS "\tError opening file !!" COLOR_RESET);
        return;
    }

    FILE *temp = fopen("temp.txt", "w");
    if (temp == NULL)
    {
        perror(RED GRAS "\tError creating temporary file !!" COLOR_RESET);
        fclose(original);
        return;
    }

    char line[100];
    int i = 1;

    while (fgets(line, sizeof(line), original))
    {
        if (i != index)
        {
            fputs(line, temp);
        }
        i++;
    }
    fclose(original);
    fclose(temp);
    remove(filename);             // Delete the original file
    rename("temp.txt", filename); // Rename the temp file to the original file name
}

void addTodo(const char *filename, const char *newTodo)
{
    FILE *file = fopen(filename, "a"); // Open file in append mode
    if (file == NULL)
    {
        perror(RED GRAS "\tError opening file !!" COLOR_RESET);
        return;
    }

    fprintf(file, "%s\n", newTodo); // Write the new todo item with a newline
    fclose(file);
}

void remplicagedeseance(planning *q, char *name)
{
    strcpy(q->plan, name);

    printf(CYAN "\t%s : " COLOR_RESET, TR_FR_EN("Nom du module", "Module name"));
    scanf(" %[^\n]s", q->module);

    // priority
    printf(CYAN "\t%s : " COLOR_RESET, TR_FR_EN("Priorité du module", "Module priority")); // Fixed
    CharToInt(&q->priorite);

    // note
    printf(CYAN "\t%s : " COLOR_RESET, TR_FR_EN("Note pour cette séance", "Session note"));
    scanf(" %[^\n]s", q->note);

    // year
    printf(CYAN "\t%s : " COLOR_RESET, TR_FR_EN("Annee", "Year"));
    CharToInt(&q->date.year);
    while (q->date.year < 2025)
    {
        printf(MAGENTA "\t%s (*-*), %s : " COLOR_RESET,
               TR_FR_EN("Nos plans concernent le future", "Our plans are for future"),
               TR_FR_EN("ressaisissez l'année", "re-enter the year"));
        CharToInt(&q->date.year);
    }

    // month
    printf(CYAN "\t%s : " COLOR_RESET, TR_FR_EN("Mois", "Month"));
    CharToInt(&q->date.month);
    // Ensure this `do` statement is inside a valid function or block
    while (q->date.month > 12 || q->date.month <= 0)
    {
        printf(MAGENTA "\t%s [1-12] : " COLOR_RESET, TR_FR_EN("Le mois doit être entre", "Month must be between"));
        CharToInt(&q->date.month);
    }

    // day
    printf(CYAN "\t%s : " COLOR_RESET, TR_FR_EN("Jour", "Day"));
    CharToInt(&q->date.day);
    if ((q->date.month == 2) && bisextuelle(q->date.year) == true && q->date.day > 29)
    {
        do
        {
            printf(MAGENTA "\t%s [1-29] : " COLOR_RESET,
                   TR_FR_EN("Février : jours entre", "February days between"));
            CharToInt(&q->date.day);
        } while (q->date.day > 29 || q->date.day <= 0);
    }
    else
    {
        if ((q->date.month) == 2 && bisextuelle(q->date.year) == false && q->date.day > 28)
        {
            do
            {
                printf(MAGENTA "\t%s [1-28] : " COLOR_RESET,
                       TR_FR_EN("Février compte 28 jours maximum", "February has maximum 28 days"));
                CharToInt(&q->date.day);
            } while (q->date.day > 28 || q->date.day <= 0);
        }
        if (q->date.day > 31 || q->date.day <= 0)
        {
            do
            {
                printf(MAGENTA "\t%s [1-31] : " COLOR_RESET,
                       TR_FR_EN("Le jour doit être entre", "Day must be between"));

                CharToInt(&q->date.day);
            } while (q->date.day > 31 || q->date.day <= 0);
        }
    }

    // START
    //  hours (start)
    printf(CYAN "\t%s : " COLOR_RESET, TR_FR_EN("Heure de début", "Start time"));
    CharToInt(&q->starttime.hour);
    if (q->starttime.hour < 0 || q->starttime.hour > 24)
    {
        do
        {
            printf(MAGENTA "\t%s : " COLOR_RESET, TR_FR_EN("Entrez à nouveau le nombre d'heures", "Enter the number of hours again"));
            CharToInt(&q->starttime.hour);
        } while (q->starttime.hour < 0);
    }

    // mins
    printf(CYAN "\t%s : " COLOR_RESET, TR_FR_EN("Minutes de l'heure de début", "Starting time minutes"));
    CharToInt(&q->starttime.min);
    if (q->starttime.min < 0 || q->starttime.min > 60)
    {
        do
        {
            printf(MAGENTA "\t%s : " COLOR_RESET, TR_FR_EN("Ressaisir le nombre de minutes", "Enter the number of minutes again"));
            CharToInt(&q->starttime.min);
        } while (q->starttime.min < 0 || q->starttime.min > 60);
    }

    // DURATION
    //  hours
    printf(CYAN "\t%s : " COLOR_RESET, TR_FR_EN("Nombre d'heures", "Number of hours"));
    CharToInt(&q->duration.hour);
    if (q->duration.hour < 0 || q->duration.hour > 24)
    {
        do
        {
            printf(MAGENTA "\t%s : " COLOR_RESET, TR_FR_EN("Ressaisissez le nombre d'heures", "Enter the number of hours again"));
            CharToInt(&q->duration.hour);
        } while (q->duration.hour < 0);
    }

    // mins
    printf(CYAN "\t%s : " COLOR_RESET, TR_FR_EN("Minutes", "Minutes"));
    CharToInt(&q->duration.min);
    while (q->duration.min < 0 || q->duration.min > 60)
    {
        printf(MAGENTA "\t%s : " COLOR_RESET, TR_FR_EN("Ressaisissez le nombre de minutes", "Enter the number of minutes again"));
        CharToInt(&q->duration.min);
    }
}

void displayPlanning(planning p)
{
    printf(BLUE "\t\t%s " YELLOW "%s\n" COLOR_RESET, TR_FR_EN("Module :", "Module:"), p.module);
    printf(BLUE "\t\t%s " YELLOW "%d\n" COLOR_RESET, TR_FR_EN("Priorité :", "Priority:"), p.priorite);
    printf(BLUE "\t\t%s " YELLOW "%s\n" COLOR_RESET, TR_FR_EN("Note :", "Note:"), p.note);
    printf(BLUE "\t\t%s " YELLOW "%02d/%02d/%04d\n" COLOR_RESET, TR_FR_EN("Date :", "Date:"), p.date.day, p.date.month, p.date.year);
    printf(BLUE "\t\t%s " YELLOW "%02d:%02d\n" COLOR_RESET, TR_FR_EN("Heure de début :", "Start Time:"), p.starttime.hour, p.starttime.min);
    printf(BLUE "\t\t%s " YELLOW "%02d:%02d\n" COLOR_RESET, TR_FR_EN("Durée :", "Duration:"), p.duration.hour, p.duration.min);
    printf(YELLOW "\t\t=============================================\n" COLOR_RESET);
    printf("\n");
}

void showSorted(planning plans[], int n, int dir, int sort, char *name_of_plan)
{
    // Sort the array
    int i;
    i = 0;
    int j;
    for (i = 0; i < n - 1; i++)
    {
        j = i;
        for (j = i + 1; j < n; j++)
        {
            bool condition = false;

            // Sorting logic
            if (sort == 1)
            { // Sort by priority
                if (dir == 1)
                { // Ascending
                    condition = (plans[i].priorite > plans[j].priorite);
                }
                else
                { // Descending
                    condition = (plans[i].priorite < plans[j].priorite);
                }
            }
            else
            { // Sort by date
                int cmp = compareDate(plans[i].date, plans[j].date);
                if (dir == 1)
                { // Ascending
                    condition = (cmp > 0);
                }
                else
                { // Descending
                    condition = (cmp < 0);
                }
            }

            // Swap if the condition is met
            if (condition)
            {
                planning temp = plans[i];
                plans[i] = plans[j];
                plans[j] = temp;
            }
        }
    }

    // Display all the sorted planning entries
    for (int i = 0; i < n; i++)
    {
        if (strcmp(plans[i].plan, name_of_plan) == 0)
        {
            displayPlanning(plans[i]);
        }
    }
}

void obtenirDateDuJour()
{
    int day, month, year;
    time_t temps_actuel;
    struct tm *info_date;
    char buffer[80];

    // Obtenir le temps actuel
    time(&temps_actuel);

    // Convertir en structure tm (local time)
    info_date = localtime(&temps_actuel);

    // Vérification de la conversion
    if (info_date == NULL)
    {
        perror("Erreur de conversion du temps");
        return;
    }

    // Formater la date selon le standard français
    strftime(buffer, sizeof(buffer), MAGENTA "%A %d" COLOR_RESET YELLOW " %B %Y" COLOR_RESET, info_date);
    printf("%s\n", buffer);
}

#pragma endregion
#pragma endregion

int main()
{
#pragma region VARS
    bool c;
    int b9a;
    int pass;
    int enter;
    FILE *code;
    int actual_index;
    FILE *file;
    char new_plan[10];
    int search_matching[100];
    int i, j, n, l, a, b, ns, modif_index, match_count;
    bool EXIST;
    char newTodo[100];
    char modulle[11];
    todolist atodo;
    int direction = 0;   // 0 for descending, 1 for ascending
    int sortBy = 0;      // 0 for date, 1 for priority
    char shown[100][10]; // To store unique plan names
    int shown_count = 0;
    planning k; // to add a session in the file
    int count;  // pour parcourir le tableau pour acceder au case pressie pour recherche //
    int is_unique, search_pr;
    MyDATE datte;
    i = 0;
    n = 1;
    char name_of_plan[10];
    planning *vec;
    int size;
    int path = 7;
    int ch, choice;
#pragma endregion
#pragma region password
    code = fopen("code.txt", "r");
    if (code == NULL)
    {

        code = fopen("code.txt", "w");
        printf("enter your first code=");
        CharToInt(&pass);
        fprintf(code, "%d", pass);
        fclose(code);
    }
    else
    {
        printf("enter the code=");
        fscanf(code, "%d", &enter);
        fclose(code);
        b9a = 1;
        CharToInt(&pass);
        while (b9a < 3)
        {
            if (pass != enter)
            {
                printf("left %d tries ;enter the code again=", 3 - b9a);
                CharToInt(&pass);
                b9a++;
            }
            else
            {
                break;
            }
        }
        if (pass != enter)
        {
            printf("you are out");
            sleep(2);
            return 0;
        }
    }
#pragma endregion
#pragma region HELLO
    system("cls");
    obtenirDateDuJour();
    printf(RED "\t\tHHHHHHHHH     HHHHHHHHHEEEEEEEEEEEEEEEEEEEEEELLLLLLLLLLL             LLLLLLLLLLL                  OOOOOOOOO     \n" COLOR_RESET);
    printf(YELLOW "\t\tH:::::::H     H:::::::HE::::::::::::::::::::EL:::::::::L             L:::::::::L                OO:::::::::OO   \n" COLOR_RESET);
    printf(MAGENTA "\t\tH:::::::H     H:::::::HE::::::::::::::::::::EL:::::::::L             L:::::::::L              OO:::::::::::::OO \n" COLOR_RESET);
    printf(BLUE "\t\tHH::::::H     H::::::HHEE::::::EEEEEEEEE::::ELL:::::::LL             LL:::::::LL             O:::::::OOO:::::::O\n" COLOR_RESET);
    printf(GREEN "\t\t  H:::::H     H:::::H    E:::::E       EEEEEE  L:::::L                 L:::::L               O::::::O   O::::::O\n" COLOR_RESET);
    printf(CYAN "  \t\t  H:::::H     H:::::H    E:::::E               L:::::L                 L:::::L               O:::::O     O:::::O\n" COLOR_RESET);
    printf(RED "\t\t  H::::::HHHHH::::::H    E::::::EEEEEEEEEE     L:::::L                 L:::::L               O:::::O     O:::::O\n" COLOR_RESET);
    printf(YELLOW "\t\t  H:::::::::::::::::H    E:::::::::::::::E     L:::::L                 L:::::L               O:::::O     O:::::O\n" COLOR_RESET);
    printf(MAGENTA "\t\t  H:::::::::::::::::H    E:::::::::::::::E     L:::::L                 L:::::L               O:::::O     O:::::O\n" COLOR_RESET);
    printf(BLUE "\t\t  H::::::HHHHH::::::H    E::::::EEEEEEEEEE     L:::::L                 L:::::L               O:::::O     O:::::O\n" COLOR_RESET);
    printf(GREEN "\t\t  H:::::H     H:::::H    E:::::E               L:::::L                 L:::::L               O:::::O     O:::::O\n" COLOR_RESET);
    printf(CYAN "\t\t  H:::::H     H:::::H    E:::::E       EEEEEE  L:::::L         LLLLLL  L:::::L         LLLLLLO::::::O   O::::::O\n" COLOR_RESET);
    printf(RED "\t\tHH::::::H     H::::::HHEE::::::EEEEEEEE:::::ELL:::::::LLLLLLLLL:::::LLL:::::::LLLLLLLLL:::::LO:::::::OOO:::::::O\n" COLOR_RESET);
    printf(YELLOW "\t\tH:::::::H     H:::::::HE::::::::::::::::::::EL::::::::::::::::::::::LL::::::::::::::::::::::L OO:::::::::::::OO \n" COLOR_RESET);
    printf(MAGENTA "\t\tH:::::::H     H:::::::HE::::::::::::::::::::EL::::::::::::::::::::::LL::::::::::::::::::::::L   OO:::::::::OO   \n" COLOR_RESET);
    printf(BLUE "\t\tHHHHHHHHH     HHHHHHHHHEEEEEEEEEEEEEEEEEEEEEELLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL     OOOOOOOOO     \n" COLOR_RESET);
    sleep(5);
#pragma endregion
    system("cls");
    while (path != 0 && path <= 7)
    /*EXPLANATION:
        each path is a statement for a large destination:
            1) main menu
            2) plan menu
            3) search for
            4) todo list
            5) modify (after changing)
            6) change language
    */
    {
        switch (path)
        {
        case 1: // main menu
#pragma region path1
#pragma region main menu
            system("cls");
            printf(MAGENTA GRAS "\t%s\n" COLOR_RESET, TR_FR_EN("BIENVENUE UTILISATEUR (^-^)", "WELCOME USER (^-^)"));
            printf(YELLOW "\t=============================================\n" COLOR_RESET);
            printf(BLUE "\t\t[1].%s\n" COLOR_RESET, TR_FR_EN("Choisir un plan", "Choose plan"));
            printf("\t\t \n");
            printf(BLUE "\t\t[2].%s\n" COLOR_RESET, TR_FR_EN("Ajouter un plan", "Add plan"));
            printf("\t\t \n");
            printf(BLUE "\t\t[3].%s\n" COLOR_RESET, TR_FR_EN("Liste de taches", "To-do list"));
            printf("\t\t \n");
            printf(BLUE "\t\t[4].%s\n" COLOR_RESET, TR_FR_EN("Afficher les credits", "Show credits "));
            printf("\t\t \n");
            printf(BLUE "\t\t[5].%s\n" COLOR_RESET, TR_FR_EN("Choisissez la langue ", " Choose language"));
            printf("\t\t \n");
            printf(BLUE "\t\t[6].%s\n" COLOR_RESET, TR_FR_EN("changer le code ", " change the code"));
            printf("\t\t \n");
            printf(RED "\t\t[0].%s\n" COLOR_RESET, TR_FR_EN("QUITTER", "QUIT"));
            printf("\t\t \n");
            printf(YELLOW "\t=============================================\n" COLOR_RESET);
            printf(GREEN GRAS "\t%s: " COLOR_RESET, TR_FR_EN("VOTRE CHOIX", "YOUR CHOICE"));
#pragma endregion
            CharToInt(&ch);
            RepeatUntil(ch, 0, 6, &ch);
            switch (ch)
            {
            case 0: // Quit the app
#pragma region quit the app
                design(1);
                printf(MAGENTA GRAS "\t%s\n" COLOR_RESET, TR_FR_EN("AU REVOIR! MERCI D'AVOIR UTILISE NOTRE APPLICATION (^-^)", "GOODBYE! THANK YOU FOR USING OUR APP (^-^)"));
                sleep(2);
                return 0;
#pragma endregion
                break;
            case 1: // choose a plan
#pragma region show all plan
                system("cls");
                shown_count = 0; // Reset shown_count for each new session
                a = 0;           // Reset a for each plan
                vec = file_to_vec("sample.txt", &size);
                printf(GREEN GRAS "\t%s\n" COLOR_RESET, TR_FR_EN("Plans disponibles:", "Available Plans:"));
                for (a = 0; a < size; a++)
                {
                    is_unique = 1;
                    b = 0; // Reset b for each plan
                    // Check if this plan has already been shown
                    for (b = 0; b < shown_count; b++)
                    {
                        if (strcmp(vec[a].plan, shown[b]) == 0)
                        {
                            is_unique = 0;
                            break;
                        }
                    }

                    // If it's unique, show it and store it in the shown list
                    if (is_unique)
                    {
                        strcpy(shown[shown_count], vec[a].plan);
                        printf(YELLOW "[%d] " MAGENTA "%s\n" COLOR_RESET, shown_count + 1, vec[a].plan);
                        shown_count++;
                    }
                }
                vec_to_file("sample.txt", vec, size); // Save the updated vector to the file
                free(vec);                            // Free the allocated memory for the vector
#pragma endregion
#pragma region choose the plan
                printf(CYAN "%s" COLOR_RESET, TR_FR_EN("Entrez votre choix (numero): ", "Enter your choice (number): "));
                do
                {
                    CharToInt(&choice);
                    // Store the selected name in name_of_plan
                    if (choice >= 1 && choice <= shown_count)
                    {
                        strcpy(name_of_plan, shown[choice - 1]);
                    }
                    else
                    {
                        printf(RED GRAS "\t%s\n" COLOR_RESET, TR_FR_EN("Choix invalide (-_-)", "Invalid choice (-_-)"));
                    }
                } while (choice < 1 || choice > shown_count);
#pragma endregion
                design(1);
                path = 2; // next window after choosing the plan
                break;
            case 2: // add plan
#pragma region add plan + first session
                design(1);
                system("cls");
                printf(CYAN "%s" COLOR_RESET, TR_FR_EN("Entrez le nom du plan a ajouter:", "Enter the name of plan to add:"));
                scanf(" %[^\n]s", new_plan);
                printf(YELLOW "%s\n" COLOR_RESET,
                       TR_FR_EN("Maintenant ajoutons votre premiere session a ce plan (*-*)", "Now let's add your first session to this plan (*-*)"));
                do
                {
                    file = fopen("sample.txt", "a+");
                    remplicagedeseance(&k, new_plan);
                    fprintf(file, "%s %s %d %s %d %d %d %d %d %d %d\n",
                            k.plan, k.module, k.priorite, k.note,
                            k.date.day, k.date.month, k.date.year,
                            k.starttime.hour, k.starttime.min,
                            k.duration.hour, k.duration.min);
                    fclose(file);
                    printf(MAGENTA GRAS "\t%s\n" COLOR_RESET, TR_FR_EN("Prenez votre temps pour ajouter un plan (^-^)", "Take your time to add a plan (^-^)"));
                    printf(YELLOW "\t=============================================\n" COLOR_RESET);
                    printf(BLUE "\t\t[1].%s\n" COLOR_RESET, TR_FR_EN("Terminer", "Finish"));
                    printf("\t\t \n");
                    printf(BLUE "\t\t[2].%s\n" COLOR_RESET, TR_FR_EN("Ajouter plus", "Add more"));
                    printf("\t\t \n");
                    printf(YELLOW "\t=============================================\n" COLOR_RESET);
                    printf(GREEN "\t%s: " COLOR_RESET, TR_FR_EN("VOTRE CHOIX", "YOUR CHOICE"));
                    CharToInt(&ch);
                } while (ch == 2);
                fclose(file);
#pragma endregion
                break;
            case 3:       // todo list
                path = 4; // go to the todo list
                break;
            case 4: // credit
                design(1);
                system("cls");
                printf(RED GRAS "\t%s\n" COLOR_RESET, TR_FR_EN("Voici nos informations :", "Here is our information:"));
                printf(YELLOW "\t============================================================\n" COLOR_RESET);
                printf(MAGENTA "\t\t%s\n" COLOR_RESET, TR_FR_EN("nom : Sara Tayebi\t section: B\t groupe: 4\t matricule: 242431421418", "name: Sara Tayebi\t section: B\t group: 4\t ID: 242431421418"));
                printf("\t\t \n");
                printf(BLUE "\t\t%s\n" COLOR_RESET, TR_FR_EN("nom : Sabri Nada\t section: B\t groupe: 3\t matricule: 232431860002", "name: Sabri Nada\t section: B\t group: 3\t ID: 232431860002"));
                printf("\t\t \n");
                printf(MAGENTA "\t\t%s\n" COLOR_RESET, TR_FR_EN("nom : Maouchi Manar\t section: B\t groupe: 4\t matricule: 242431621810", "name: Maouchi Manar\t section: B\t group: 4\t ID: 242431621810"));
                printf("\t\t \n");
                printf(BLUE "\t\t%s\n" COLOR_RESET, TR_FR_EN("nom : Makhlouf Lina\t section: B\t groupe: 4\t matricule: 242431621413", "name: Makhlouf Lina\t section: B\t group: 4\t ID: 242431621413"));
                while (getchar() != '\n')
                    ;
                while (getchar() != '\n')
                    ;
                path = 1; // go back to the main menu
                break;
            case 5:
                path = 7;
                break;
            case 6:
                code = fopen("code.txt", "w");
                printf("enter the code that you want=");
                CharToInt(&pass);
                fprintf(code, "%d", pass);
                fclose(code);

            default:
                printf(RED GRAS "\t%s\n" COLOR_RESET, TR_FR_EN("Choix invalide (-_-)", "Invalid choice (-_-)"));
                sleep(1);
                break;
            }
            break;
#pragma endregion
        case 2: // plan menu
#pragma region path2
            printf(MAGENTA GRAS "\t%s %s! %s (^-^)\n" COLOR_RESET, TR_FR_EN("VOUS AVEZ CHOISI", "YOU CHOSE"), name_of_plan, TR_FR_EN("PRENEZ VOTRE TEMPS", "TAKE YOUR TIME"));
            printf(YELLOW "\t=============================================\n" COLOR_RESET);
            printf(BLUE "\t\t[1].%s\n" COLOR_RESET, TR_FR_EN("Supprimer le planning", "Delete planner"));
            printf("\t\t \n");
            printf(BLUE "\t\t[2].%s\n" COLOR_RESET, TR_FR_EN("Ajouter une seance", "Add session"));
            printf("\t\t \n");
            printf(BLUE "\t\t[3].%s\n" COLOR_RESET, TR_FR_EN("Tout afficher", "Show all"));
            printf("\t\t \n");
            printf(BLUE "\t\t[4].%s\n" COLOR_RESET, TR_FR_EN("Rechercher", "Search")); // Correction de "Serch" à "Search"
            printf("\t\t \n");
            printf(YELLOW "\t\t[5].%s\n" COLOR_RESET, TR_FR_EN("Retour", "Return"));
            printf("\t\t \n");
            printf(YELLOW "\t=============================================\n" COLOR_RESET);
            printf(GREEN GRAS "\n%s:" COLOR_RESET, TR_FR_EN("VOTRE CHOIX", "YOUR CHOICE"));
            CharToInt(&ch);
            RepeatUntil(ch, 1, 5, &ch);
            switch (ch)
            {
            case 1: // delete the planner
#pragma region delete a plan
                i = 0;
                vec = file_to_vec("sample.txt", &size);
                n = size;
                while (i < size)
                {
                    if (strcmp(vec[i].plan, name_of_plan) == 0)
                    {
                        l = i;
                        for (l = i; l < n - 1; l++)
                        {
                            vec[l] = vec[l + 1];
                        }
                        n = n - 1;
                    }
                    else
                    {
                        i = i + 1;
                    }
                }
                vec_to_file("sample.txt", vec, n);
                free(vec); // Free the allocated memory for the vector
#pragma endregion
                path = 1; // return to the main menu
                break;
            case 2: // add session
#pragma region add session
                design(1);
                do
                {
                    file = fopen("sample.txt", "a+");
                    remplicagedeseance(&k, name_of_plan);
                    fprintf(file, "%s %s %d %s %d %d %d %d %d %d %d\n",
                            k.plan, k.module, k.priorite, k.note,
                            k.date.day, k.date.month, k.date.year,
                            k.starttime.hour, k.starttime.min,
                            k.duration.hour, k.duration.min);
                    fclose(file);
                    system("cls");
                    printf(MAGENTA GRAS "\t%s (^-^)\n" COLOR_RESET, TR_FR_EN("PRENEZ VOTRE TEMPS POUR AJOUTER UNE SESSION", "TAKE YOUR TIME AND ADD SESSION"));
                    printf(YELLOW "\t=============================================\n" COLOR_RESET);
                    printf(BLUE "\t\t[2].%s\n" COLOR_RESET, TR_FR_EN("Ajouter plus", "Add more"));
                    printf("\t\t \n");
                    printf(BLUE "\t\t[1].%s\n" COLOR_RESET, TR_FR_EN("Terminer", "Finish"));
                    printf("\t\t \n");
                    printf(YELLOW "\t=============================================\n" COLOR_RESET);
                    printf(GREEN "\t%s:" COLOR_RESET, TR_FR_EN("VOTRE CHOIX", "YOUR CHOICE"));
                    CharToInt(&ch);
                    RepeatUntil(ch, 1, 2, &ch);
                } while (ch == 2);
                fclose(file);
#pragma endregion
                break;
            case 3: // show all
#pragma region show all
                do
                {
                    vec = file_to_vec("sample.txt", &size);
                    showSorted(vec, size, direction, sortBy, name_of_plan);
                    vec_to_file("sample.txt", vec, size);                                  // Save the updated vector to the file
                    free(vec);                                                             // Free the allocated memory for the vector
                    printf(BLUE "\t[1].%s\n" COLOR_RESET, TR_FR_EN("Options", "Options")); // Options de tri
                    printf(RED "\t[0].%s\n" COLOR_RESET, TR_FR_EN("Retour", "Return"));    // Retour
                    CharToInt(&i);
                    RepeatUntil(i, 0, 1, &i);
                    switch (i)
                    {
                    case 1:
                        do
                        {
#pragma region design
                            printf(RED "\t[0].%s\n" COLOR_RESET, TR_FR_EN("Quitter", "Quit"));
                            printf(BLUE "\t[1]" COLOR_RESET); // wech kayen hna
                            if (direction == 0)
                            {
                                printf("\033[1m");
                                printf("\033[35m");
                            }
                            printf(BLUE "\t%s\n" COLOR_RESET, TR_FR_EN("Tri decroissant :", "Descending:"));
                            printf("\033[0m");
                            if (direction == 1)
                            {
                                printf("\033[1m");
                                printf("\033[35m");
                            }
                            printf(BLUE "\t%s\n" COLOR_RESET, TR_FR_EN("Tri croissant :", "Ascending:"));
                            printf("\033[0m");

                            printf(BLUE "\t[2]" COLOR_RESET);
                            if (sortBy == 0)
                            {
                                printf("\033[1m");
                                printf("\033[35m");
                            }
                            printf(BLUE "\t%s\n" COLOR_RESET, TR_FR_EN("Date :", "Date:"));
                            printf("\033[0m");
                            if (sortBy == 1)
                            {
                                printf("\033[1m");
                                printf("\033[35m");
                            }
                            printf(BLUE "\t%s\n" COLOR_RESET, TR_FR_EN("Priorite :", "Priority:"));
                            printf("\033[0m");
                            printf(GREEN "\n%s " COLOR_RESET, TR_FR_EN("Selection :", "Select:"));
                            CharToInt(&j);
                            RepeatUntil(j, 0, 2, &j);
#pragma endregion
                            switch (j)
                            {
                            case 1:
                                direction = !direction;
                                break;
                            case 2:
                                sortBy = !sortBy;
                                break;
                            case 0:
                                printf(GREEN GRAS "%s\n" COLOR_RESET, TR_FR_EN("Sortie en cours...", "Exiting..."));
                                sleep(1);
                                system("cls");
                                break;
                            }

                        } while (j != 0);
                        break;
                    case 0:
                        printf(GREEN GRAS "%s\n" COLOR_RESET, TR_FR_EN("Sortie en cours...", "Exiting..."));
                        sleep(1);
                        system("cls");
                        break;
                    }
                } while (i != 0);

#pragma endregion // return to the main menu
                break;
            case 4: // serch
                path = 3;
                break;
            case 5:
                path = 1; // return
                break;
            default:
                printf(RED GRAS "\t%s (-_-)\n" COLOR_RESET, TR_FR_EN("Choix invalide", "Invalid choice"));
                sleep(1);
            }
            break;
#pragma endregion
        case 3: // serch menu
#pragma region path3
            design(1);
            system("cls");
            printf(RED GRAS "\t\t[1].%s \n" COLOR_RESET, TR_FR_EN("Retour", "Return"));
            printf("\t\t \n");
            printf(MAGENTA "\t%s \n" COLOR_RESET, TR_FR_EN("Rechercher par :", "Search by:"));
            printf(YELLOW "\t=============================================\n" COLOR_RESET);
            printf(BLUE "\t\t[2].%s\n" COLOR_RESET, TR_FR_EN("Date", "Date"));
            printf("\t\t \n");
            printf(BLUE "\t\t[3].%s\n" COLOR_RESET, TR_FR_EN("Module", "Module"));
            printf("\t\t \n");
            printf(BLUE "\t\t[4].%s\n" COLOR_RESET, TR_FR_EN("Priorite", "Priority"));
            printf("\t\t \n");
            printf(YELLOW "\t=============================================\n" COLOR_RESET);
            printf(GREEN "\t%s:" COLOR_RESET, TR_FR_EN("VOTRE CHOIX", "YOUR CHOICE"));
            CharToInt(&ch);
            vec = file_to_vec("sample.txt", &size);
#pragma region serch by
            RepeatUntil(ch, 1, 4, &ch);
            switch (ch)
            {
            case 1:
                path = 2; // return to the plan menu
                break;
            case 2: // DATE
                design(1);
                system("cls");
                printf(CYAN "\t%s :" COLOR_RESET, TR_FR_EN("Entrez la date que vous recherchez", "Enter the date you are looking for"));

                // year
                printf(CYAN "\t%s :" COLOR_RESET, TR_FR_EN("Entrez l'annee", "Enter the year"));
                CharToInt(&datte.year);
                while (datte.year < 2025)
                {
                    printf(MAGENTA "%s" COLOR_RESET, TR_FR_EN("Nos plans concernent le future (*-*), veuillez ressaisir l'année :",
                                                              "Our plans are for the future (*-*), please re-enter the year:"));
                    CharToInt(&datte.year);
                }

                // month
                printf(CYAN "\t%s :" COLOR_RESET, TR_FR_EN("Entrez le mois", "Enter the month"));
                CharToInt(&datte.month);
                // Ensure this `do` statement is inside a valid function or block
                while (datte.month > 12 || datte.month <= 0)
                {
                    printf(MAGENTA "\t%s :" COLOR_RESET, TR_FR_EN("Le numero du mois doit etre entre 1 et 12", "Month number must be between 1 and 12"));
                    CharToInt(&datte.month);
                }

                // day
                printf(CYAN "\t%s :" COLOR_RESET, TR_FR_EN("Entrez le jour", "Enter day"));
                CharToInt(&datte.day);
                if ((datte.month == 2) && bisextuelle(datte.year) == true && datte.day > 29)
                {
                    do
                    {
                        printf(MAGENTA "\t%s :" COLOR_RESET, TR_FR_EN("Fevrier compte entre 1 et 29 jours", "February has between 1 and 29 days"));
                        CharToInt(&datte.day);
                    } while (datte.day > 29 || datte.day <= 0);
                }
                else
                {
                    if ((datte.month) == 2 && bisextuelle(datte.year) == false && datte.day > 28)
                    {
                        do
                        {
                            printf(MAGENTA "\t%s :" COLOR_RESET, TR_FR_EN("Les jours de fevrier sont entre 1 et 28", "February days are between 1 and 28"));
                            CharToInt(&datte.day);
                        } while (datte.day > 28 || datte.day <= 0);
                    }
                    if (datte.day > 31 || datte.day <= 0)
                    {
                        do
                        {
                            printf(MAGENTA "\t%s :" COLOR_RESET, TR_FR_EN("Le jour doit etre entre 1 et 31", "Day must be between 1 and 31"));
                            CharToInt(&datte.day);
                        } while (datte.day > 31 || datte.day <= 0);
                    }
                }

                EXIST = false;
                i = 0;
                match_count = 0;

                while (i < size)
                {
                    if (strcmp(vec[i].plan, name_of_plan) == 0 && sameDate(&vec[i].date, &datte) == true)
                    {
                        search_matching[match_count] = i; // Stocke l'index de la séance correspondante
                        match_count++;
                        EXIST = true;
                    }
                    i++;
                }
                system("cls");
                if (!EXIST)
                {
                    printf(RED "\n\tAucune seance trouvee pour cette date.\n\n" COLOR_RESET);
                }
                else
                {
                    printf(MAGENTA "\n\t===============================================================================================================\n" COLOR_RESET);
                    printf(CYAN "\t" MAGENTA "|" CYAN " %-8s   " MAGENTA "|" CYAN " %-20s " MAGENTA "|" CYAN " %-8s " MAGENTA "|" CYAN " %-20s " MAGENTA "|" CYAN " %-10s " MAGENTA "|" CYAN " %-12s " MAGENTA "|" CYAN " %-9s " MAGENTA "|\n" COLOR_RESET,
                           "Numero", "Module", "Priorite", "Note", "Date", "Heure Debut", "Duree");
                    printf(MAGENTA "\t===============================================================================================================\n" COLOR_RESET);

                    for (int j = 0; j < match_count; j++)
                    {
                        int idx = search_matching[j];
                        printf(YELLOW "\t" MAGENTA "|" GREEN "    [%d]     " MAGENTA "|" YELLOW " %-20s " MAGENTA "|" YELLOW " %-8d " MAGENTA "|" YELLOW " %-20s " MAGENTA "|" YELLOW " %02d/%02d/%04d " MAGENTA "|" YELLOW " %02d:%02d        " MAGENTA "|" YELLOW " %02dh %02dmin " MAGENTA "|\n" COLOR_RESET,
                               j + 1, // Numéro de position (commence à 1)
                               vec[idx].module,
                               vec[idx].priorite,
                               vec[idx].note,
                               vec[idx].date.day, vec[idx].date.month, vec[idx].date.year,
                               vec[idx].starttime.hour, vec[idx].starttime.min,
                               vec[idx].duration.hour, vec[idx].duration.min);
                    }

                    printf(MAGENTA "\t===============================================================================================================\n\n" COLOR_RESET);
                }
                break;
            case 3:
                design(1);
                system("cls");
                printf(YELLOW "\tEnter the name of module that you are looking for :" COLOR_RESET);
                scanf(" %s", modulle);
                EXIST = false;
                i = 0;
                match_count = 0;

                while (i < size)
                {
                    if (strcmp(name_of_plan, vec[i].plan) == 0 && strcmp(vec[i].module, modulle) == 0)
                    {
                        search_matching[match_count] = i; // Store the index of the matching session
                        match_count++;
                        EXIST = true;
                    }
                    i++;
                }
                system("cls");
                if (!EXIST)
                {
                    printf(RED "\n\tAucune seance trouvee pour ce module.\n\n" COLOR_RESET);
                }
                else
                {
                    printf(MAGENTA "\n\t===============================================================================================================\n" COLOR_RESET);
                    printf(CYAN "\t" MAGENTA "|" CYAN " %-8s   " MAGENTA "|" CYAN " %-20s " MAGENTA "|" CYAN " %-8s " MAGENTA "|" CYAN " %-20s " MAGENTA "|" CYAN " %-10s " MAGENTA "|" CYAN " %-12s " MAGENTA "|" CYAN " %-9s " MAGENTA "|\n" COLOR_RESET,
                           "Numero", "Module", "Priorite", "Note", "Date", "Heure Debut", "Duree");
                    printf(MAGENTA "\t===============================================================================================================\n" COLOR_RESET);

                    for (int j = 0; j < match_count; j++)
                    {
                        int idx = search_matching[j];
                        printf(YELLOW "\t" MAGENTA "|" GREEN "    [%d]     " MAGENTA "|" YELLOW " %-20s " MAGENTA "|" YELLOW " %-8d " MAGENTA "|" YELLOW " %-20s " MAGENTA "|" YELLOW " %02d/%02d/%04d " MAGENTA "|" YELLOW " %02d:%02d        " MAGENTA "|" YELLOW " %02dh %02dmin " MAGENTA "|\n" COLOR_RESET,
                               j + 1, // Numéro de position (commence à 1)
                               vec[idx].module,
                               vec[idx].priorite,
                               vec[idx].note,
                               vec[idx].date.day, vec[idx].date.month, vec[idx].date.year,
                               vec[idx].starttime.hour, vec[idx].starttime.min,
                               vec[idx].duration.hour, vec[idx].duration.min);
                    }

                    printf(MAGENTA "\t===============================================================================================================\n\n" COLOR_RESET);
                }
                break;
            case 4:
                design(1);
                system("cls");
                printf(BLUE "\tEnter the priority that you are looking for :" COLOR_RESET);
                scanf(" %d", &search_pr);
                EXIST = false;
                i = 0;
                ns = 0;
                match_count = 0;

                while (i < size)
                {
                    if (strcmp(name_of_plan, vec[i].plan) == 0 && search_pr == vec[i].priorite)
                    {
                        search_matching[match_count] = i; // Stocke l'index de la séance correspondante
                        match_count++;
                        ns++;
                        EXIST = true;
                    }
                    i++;
                }
                system("cls");
                if (!EXIST)
                {
                    printf(RED "\n\tAucune seance trouvee avec cette priorite.\n\n" COLOR_RESET);
                }
                else
                {
                    printf(MAGENTA "\n\t===============================================================================================================\n" COLOR_RESET);
                    printf(CYAN "\t" MAGENTA "|" CYAN " %-8s   " MAGENTA "|" CYAN " %-20s " MAGENTA "|" CYAN " %-8s " MAGENTA "|" CYAN " %-20s " MAGENTA "|" CYAN " %-10s " MAGENTA "|" CYAN " %-12s " MAGENTA "|" CYAN " %-9s " MAGENTA "|\n" COLOR_RESET,
                           "Numero", "Module", "Priorite", "Note", "Date", "Heure Debut", "Duree");
                    printf(MAGENTA "\t===============================================================================================================\n" COLOR_RESET);

                    for (int j = 0; j < match_count; j++)
                    {
                        int idx = search_matching[j];
                        printf(YELLOW "\t" MAGENTA "|" GREEN "    [%d]     " MAGENTA "|" YELLOW " %-20s " MAGENTA "|" YELLOW " %-8d " MAGENTA "|" YELLOW " %-20s " MAGENTA "|" YELLOW " %02d/%02d/%04d " MAGENTA "|" YELLOW " %02d:%02d        " MAGENTA "|" YELLOW " %02dh %02dmin " MAGENTA "|\n" COLOR_RESET,
                               j + 1,
                               vec[idx].module,
                               vec[idx].priorite,
                               vec[idx].note,
                               vec[idx].date.day, vec[idx].date.month, vec[idx].date.year,
                               vec[idx].starttime.hour, vec[idx].starttime.min,
                               vec[idx].duration.hour, vec[idx].duration.min);
                    }

                    printf(MAGENTA "\t===============================================================================================================\n\n" COLOR_RESET);
                }
                break;
            default:
                printf(RED GRAS "\t%s (-_-)\n" COLOR_RESET, TR_FR_EN("Choix invalide", "Invalid choice"));
                sleep(1);
                break;
            }
#pragma endregion
            if (ch == 1)
            {
                path = 2;
                break;
            }
            printf(BLUE "\t\t[1].%s\n" COLOR_RESET, TR_FR_EN("Modifier", "Modify"));
            printf("\t\t \n");
            printf(RED GRAS "\t\t[2].%s\n" COLOR_RESET, TR_FR_EN("Retour", "Return"));
            printf("\t\t \n");
            printf(YELLOW "\t=============================================\n" COLOR_RESET);
            printf(GREEN "\t%s:" COLOR_RESET, TR_FR_EN("VOTRE CHOIX", "YOUR CHOICE"));
            CharToInt(&ch);
            RepeatUntil(ch, 1, 2, &ch);
            switch (ch)
            {
            case 1:
                path = 5; // go to modify menu
                break;
            case 2: // return
                system("cls");
                path = 2;
                break;
            default:
                printf(RED GRAS "\t%s (-_-)\n" COLOR_RESET, TR_FR_EN("Choix invalide", "Invalid choice"));
                sleep(1);
                break;
            }
            break;
#pragma endregion
        case 4: // todo list menu
#pragma region path4
#pragma region todo list
            do
            {
                system("cls");
                i = 1;
                printf(MAGENTA GRAS "\t\t\t %s\n" COLOR_RESET, TR_FR_EN("Liste de tâches :", "To-Do List:"));
                file = fopen("example.txt", "r"); // Open the file in text read mode
                while (fgets(atodo.todo, sizeof(atodo.todo), file))
                {
                    // Remove the trailing newline character, if present
                    atodo.todo[strcspn(atodo.todo, "\n")] = '\0';
                    printf("[%d] %s\n", i, atodo.todo);
                    i++;
                }
                printf(BLUE "\n\n%s\n" COLOR_RESET, TR_FR_EN("Entrez le numéro associé pour supprimer la tâche", "Enter the number associated to delete the task"));
                printf(BLUE "%s\n" COLOR_RESET, TR_FR_EN("Entrez 0 pour quitter", "Enter 0 to exit"));
                printf(BLUE "\t%s\n" COLOR_RESET, TR_FR_EN("Entrez -1 pour ajouter une tâche", "Enter -1 to add a task"));
                fclose(file);
                CharToInt(&j);
                RepeatUntil(j, -1, i, &j);
                if ((j != 0) && (j <= i) && (j != -1))
                {
                    printf(YELLOW "\t%s %d\n" COLOR_RESET, TR_FR_EN("Suppression de la tâche n°", "Deleting todo item at index"), j);
                    deleteTodoByIndex("example.txt", j);
                }
                else if (j == 0)
                {
                    printf(GREEN "%s\n" COLOR_RESET, TR_FR_EN("Fermeture en cours...", "Exiting..."));

                    break;
                }
                if (j == -1)
                {
                    printf(CYAN "\t%s " COLOR_RESET, TR_FR_EN("Nouvelle tâche :", "Enter the new todo item:"));
                    scanf(" %[^\n]", newTodo); // Read a line of text including spaces
                    addTodo("example.txt", newTodo);
                }
                else
                {
                    printf(RED GRAS "\t%s (-_-)\n" COLOR_RESET, TR_FR_EN("Index invalide, veuillez réessayer", "Invalid index, please try again"));
                    sleep(1);
                }
            } while (j != 0);
#pragma endregion
            path = 1; // return
            break;
#pragma endregion
        case 5:
#pragma region path5

            // hna makach clear bach to9e3d tban la list
            printf(MAGENTA GRAS "%s (^-^)\n" COLOR_RESET,
                   TR_FR_EN("Prenez votre temps pour choisir la séance à modifier",
                            "Take your time to choose the session you want to modify"));
            CharToInt(&modif_index);
            system("cls");
            printf(YELLOW "\t=============================================\n" COLOR_RESET);
            printf(BLUE "\t\t[1].%s\n" COLOR_RESET, TR_FR_EN("Modifier", "Modify"));
            printf("\t\t \n");
            printf(BLUE "\t\t[2].%s\n" COLOR_RESET, TR_FR_EN("Supprimer", "Delete")); // Fixed "delet"
            printf("\t\t \n");
            printf(RED "\t\t[3].%s\n" COLOR_RESET, TR_FR_EN("Retour", "Return"));
            printf("\t\t \n");
            printf(YELLOW "\t=============================================\n" COLOR_RESET);
            printf(GREEN "\t%s:\n" COLOR_RESET, TR_FR_EN("VOTRE CHOIX", "YOUR CHOICE"));
            CharToInt(&ch);
            RepeatUntil(ch, 1, 3, &ch);
            switch (ch)
            {
            case 1:
                path = 6;
                break;

            case 2:
                i = 0;
                for (i = search_matching[modif_index - 1]; i < size - 1; i++)
                {
                    vec[i] = vec[i + 1];
                }
                size = size - 1;
                vec_to_file("sample.txt", vec, size);
                free(vec); // Free the allocated memory for the vector
                match_count = 0;
                break;
            case 3:
                path = 3; // return to search menu
                break;
            default:
                printf(RED GRAS "\t%s (-_-)\n" COLOR_RESET, TR_FR_EN("Choix invalide", "Invalid choice"));
                sleep(1);
                break;
            }
            break;
#pragma endregion
        case 6:
#pragma region path6
            design(1);

            system("cls");
            printf(MAGENTA GRAS "\t%s (^-^)\n" COLOR_RESET,
                   TR_FR_EN("Prenez votre temps pour choisir ce que vous voulez modifier", "Take your time to choose what you want to modify"));
            printf(YELLOW "\t=============================================\n" COLOR_RESET);
            printf(BLUE "\t\t[1].%s\n" COLOR_RESET, TR_FR_EN("Module", "Module"));
            printf("\t\t \n");
            printf(BLUE "\t\t[2].%s\n" COLOR_RESET, TR_FR_EN("Date", "Date"));
            printf("\t\t \n");
            printf(BLUE "\t\t[3].%s\n" COLOR_RESET, TR_FR_EN("Durée", "Duration")); // Fixed "dure"
            printf("\t\t \n");
            printf(BLUE "\t\t[4].%s\n" COLOR_RESET, TR_FR_EN("Heure de début", "Start time"));
            printf("\t\t \n");
            printf(BLUE "\t\t[5].%s\n" COLOR_RESET, TR_FR_EN("Priorité", "Priority"));
            printf("\t\t \n");
            printf(RED "\t\t[6].%s\n" COLOR_RESET, TR_FR_EN("Retour", "Return"));
            printf("\t\t \n");
            printf(YELLOW "\t=============================================\n" COLOR_RESET);
            printf(GREEN "\t%s:" COLOR_RESET, TR_FR_EN("VOTRE CHOIX", "YOUR CHOICE"));
            CharToInt(&ch);
            RepeatUntil(ch, 1, 6, &ch);
            vec = file_to_vec("sample.txt", &size);
            actual_index = search_matching[modif_index - 1];
            switch (ch) // Modification de la session
            {
            case 1:
                printf(CYAN "\t%s : " COLOR_RESET, TR_FR_EN("Nouveau nom du module", "New module name"));
                scanf(" %[^\n]s", vec[actual_index].module);
                break;

            case 2:
                printf(CYAN "\t%s : " COLOR_RESET, TR_FR_EN("Nouvelle année", "New year"));
                CharToInt(&vec[actual_index].date.year);
                while (vec[actual_index].date.year < 2025)
                {
                    printf(MAGENTA "\t%s (*-*), %s : " COLOR_RESET, TR_FR_EN("Nos plans concernent le futur", "Our plans are for future"), TR_FR_EN("ressaisissez l'année", "re-enter the year"));
                    CharToInt(&vec[actual_index].date.year);
                }

                printf(CYAN "\t%s : " COLOR_RESET, TR_FR_EN("Nouveau mois", "New month"));
                CharToInt(&vec[actual_index].date.month);
                while (vec[actual_index].date.month > 12 || vec[actual_index].date.month <= 0)
                {
                    printf(MAGENTA "\t%s [1-12] : " COLOR_RESET, TR_FR_EN("Le mois doit être entre", "Month must be between"));
                    CharToInt(&vec[actual_index].date.month);
                }

                printf(CYAN "\t%s : " COLOR_RESET, TR_FR_EN("Nouveau jour", "New day"));
                CharToInt(&vec[actual_index].date.day);

                if ((vec[actual_index].date.month == 2) && bisextuelle(vec[actual_index].date.year) == true && vec[actual_index].date.day > 29)
                {
                    do
                    {
                        printf(MAGENTA "\t%s [1-29] : " COLOR_RESET, TR_FR_EN("Février compte jusqu'à 29 jours", "February has up to 29 days"));
                        CharToInt(&vec[actual_index].date.day);
                    } while (vec[actual_index].date.day > 29 || vec[actual_index].date.day <= 0);
                }
                else if ((vec[actual_index].date.month == 2) && bisextuelle(vec[actual_index].date.year) == false && vec[actual_index].date.day > 28)
                {
                    do
                    {
                        printf(MAGENTA "\t%s [1-28] : " COLOR_RESET, TR_FR_EN("Février compte 28 jours", "February has 28 days"));
                        CharToInt(&vec[actual_index].date.day);
                    } while (vec[actual_index].date.day > 28 || vec[actual_index].date.day <= 0);
                }
                else if (vec[actual_index].date.day > 31 || vec[actual_index].date.day <= 0)
                {
                    do
                    {
                        printf(MAGENTA "\t%s [1-31] : " COLOR_RESET, TR_FR_EN("Le jour doit être entre", "Day must be between"));
                        CharToInt(&vec[actual_index].date.day);
                    } while (vec[actual_index].date.day > 31 || vec[actual_index].date.day <= 0);
                }
                break;

            case 3:
                printf(CYAN "\t%s : " COLOR_RESET, TR_FR_EN("Nouvelles heures d'étude", "New study hours"));
                CharToInt(&vec[actual_index].duration.hour);
                if (vec[actual_index].duration.hour < 0)
                {
                    do
                    {
                        printf(MAGENTA "\t%s : " COLOR_RESET, TR_FR_EN("Ressaisissez le nombre d'heures", "Re-enter hours number"));
                        CharToInt(&vec[actual_index].duration.hour);
                    } while (vec[actual_index].duration.hour < 0);
                }

                printf(CYAN "\t%s : " COLOR_RESET, TR_FR_EN("Nouvelles minutes d'étude", "New study minutes"));
                CharToInt(&vec[actual_index].duration.min);
                if (vec[actual_index].duration.min < 0 || vec[actual_index].duration.min > 60)
                {
                    do
                    {
                        printf(MAGENTA "\t%s [0-60] : " COLOR_RESET, TR_FR_EN("Ressaisissez les minutes", "Re-enter minutes"));
                        CharToInt(&vec[actual_index].duration.min);
                    } while (vec[actual_index].duration.min < 0 || vec[actual_index].duration.min > 60);
                }
                break;

            case 4:
                printf(CYAN "\t%s : " COLOR_RESET, TR_FR_EN("Nouvelle heure de début", "New start hour"));
                CharToInt(&vec[actual_index].starttime.hour);
                if (vec[actual_index].starttime.hour < 0)
                {
                    do
                    {
                        printf(MAGENTA "\t%s : " COLOR_RESET, TR_FR_EN("Heure invalide, ressaisissez", "Invalid hour, re-enter"));
                        CharToInt(&vec[actual_index].starttime.hour);
                    } while (vec[actual_index].starttime.hour < 0);
                }

                printf(CYAN "\t%s : " COLOR_RESET, TR_FR_EN("Nouvelles minutes de début", "New start minutes"));
                CharToInt(&vec[actual_index].starttime.min);
                if (vec[actual_index].starttime.min < 0 || vec[actual_index].starttime.min > 60)
                {
                    do
                    {
                        printf(MAGENTA "\t%s [0-60] : " COLOR_RESET, TR_FR_EN("Minutes invalides, ressaisissez", "Invalid minutes, re-enter"));
                        CharToInt(&vec[actual_index].starttime.min);
                    } while (vec[actual_index].starttime.min < 0 || vec[actual_index].starttime.min > 60);
                }
                break;
            case 6:
                design(1);
                system("cls");
                path = 3;
                break;
            case 5:
                design(1);
                system("cls");
                printf(CYAN "\t%s : " COLOR_RESET, TR_FR_EN("Nouvelle priorité", "New priority"));
                CharToInt(&vec[actual_index].priorite);
                if (vec[actual_index].priorite < 0)
                {
                    do
                    {
                        printf(RED GRAS "\t%s\n" COLOR_RESET, TR_FR_EN("ERREUR", "ERROR"));
                        printf(MAGENTA "\t%s : " COLOR_RESET, TR_FR_EN("Saisissez à nouveau la priorité", "Enter the priority again"));
                        CharToInt(&vec[actual_index].priorite);
                    } while (vec[actual_index].priorite < 0);
                }
                break;
            }
            vec_to_file("sample.txt", vec, size);
            free(vec);
            match_count = 0;
            break;
#pragma endregion
        case 7:
#pragma region path7
            chooseLanguage();
            path = 1;
            break;
#pragma endregion
        default:
            printf(RED GRAS "\t%s (-_-)\n" COLOR_RESET, TR_FR_EN("Choix invalide", "Invalid choice"));
            sleep(1);
            break;
        }
    }
    return 0;
}