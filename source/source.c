/*

Final commit - 23/11/2025

Team: Agriim Rastogi, Akshat Gaur, Amrit Mathur, Hemant Tyagi, Dhruv Singh

Section ND

Project: Hospital Patient Record Management System

Project Completion Date: Nov 15

Project Description: This project is a hospital patient record management system.
It allows the user to manage the patient records, doctor records, and appointment records.

Project Features:
- Patient Management
- Doctor Management
- Appointment Management
- Report Generation
- User Management`
- Login System
- Registration System
- Password Management
- Data Backup and Recovery
- Data Encryption / Deletion / Retrieval / Update

- Binary File Handling
- Dynamic Memory Allocation

*/

// Program started as the end-term stress 😭

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

struct Patient
{
    int id;
    char name[50];
    char medical_notes[500];
    int age;
    int gender; // 1 = Male, 2 = Female, 3 = Other (we have to respect everyone)
};

struct Doctor
{
    int id;
    char name[50];
    char specialisation[50];
    float charge;
    int appointments; // Doctor’s personal stress counter
};

struct Time
{
    int date;
    int month;  // 1–12
    float hour; // Using float even though we store integers
};

struct Appointment
{
    int patient_id;
    int doctor_id;
    int date;
    int month;
    float hour;
    char specialisation[50];
    float charge;
};

/* GLOBAL VARIABLES */
struct Doctor *doctor_list = NULL;
struct Patient *patient_list = NULL;
struct Appointment *appointment_list = NULL;
struct Time *time_list = NULL;

int num_doctors = 0, num_patient = 0, num_time = 0, num_appointments = 0;
int sor_t = 0, speciaaa = 1;
char pass[50];

/* 
   SAFE INTEGER INPUT:
   - Returns an int if user typed a valid integer.
   - If user types ANYTHING invalid (letters, symbols, etc.), returns -1.
   - Also flushes the invalid input from the buffer.
   We use -1 as "go one step back".
*/

int get_int_back()
{
    int x;
    int r = scanf("%d", &x);
    if (r != 1)
    {
        int c;
        while ((c = getchar()) != '\n' && c != EOF)
            ;
        return -1; // signal "back"
    }
    return x;
}

// SAFE REALLOC
void *safe_realloc(void *ptr, size_t size)
{
    void *tmp = realloc(ptr, size);
    if (size != 0 && tmp == NULL)
    {
        printf("Memory allocation failed. Goodbye cruel world...\n");
        exit(1);
    }
    return tmp;
}

/*
   DATA LOADING
*/
void load_data()
{
    FILE *fp;
    struct Doctor doc;
    struct Patient pat;
    struct Appointment app;

    time_t t = time(NULL);
    struct tm tm_now = *localtime(&t);

    /* LOAD DOCTOR FILE */
    fp = fopen("doctor.dat", "rb");
    if (fp == NULL)
    {
        fp = fopen("doctor.dat", "wb");
        if (fp) fclose(fp);
        fp = fopen("doctor.dat", "rb");
    }

    if (fp)
    {
        while (fread(&doc, sizeof(struct Doctor), 1, fp))
        {
            num_doctors++;
            doctor_list = safe_realloc(doctor_list, num_doctors * sizeof(struct Doctor));
            doctor_list[num_doctors - 1] = doc;
        }
        fclose(fp);
    }

    /* LOAD PATIENT FILE */
    fp = fopen("patient.dat", "rb");
    if (fp == NULL)
    {
        fp = fopen("patient.dat", "wb");
        if (fp) fclose(fp);
        fp = fopen("patient.dat", "rb");
    }

    if (fp)
    {
        while (fread(&pat, sizeof(struct Patient), 1, fp))
        {
            num_patient++;
            patient_list = safe_realloc(patient_list, num_patient * sizeof(struct Patient));
            patient_list[num_patient - 1] = pat;
        }
        fclose(fp);
    }

    /* LOAD APPOINTMENT FILE */
    fp = fopen("appointment.dat", "rb");
    if (fp == NULL)
    {
        fp = fopen("appointment.dat", "wb");
        if (fp) fclose(fp);
        fp = fopen("appointment.dat", "rb");
    }

    if (fp)
    {
        while (fread(&app, sizeof(struct Appointment), 1, fp))
        {
            num_appointments++;
            appointment_list = safe_realloc(appointment_list,
                                            num_appointments * sizeof(struct Appointment));
            appointment_list[num_appointments - 1] = app;
        }
        fclose(fp);
    }

    // TIME SLOT GENERATION
    for (int dd = 1; dd <= 2; dd++)
    {
        for (int hour = 9; hour < 24; hour++)
        {
            time_t future = t + dd * 86400 + hour * 3600;
            struct tm ft = *localtime(&future);

            num_time++;
            time_list = safe_realloc(time_list, num_time * sizeof(struct Time));
            time_list[num_time - 1].date = ft.tm_mday;
            time_list[num_time - 1].month = ft.tm_mon + 1;
            time_list[num_time - 1].hour = hour;
        }
    }
}


/*
    DATA SAVING (PERSISTENT)
*/
void upload_data()
{
    FILE *fp;

    fp = fopen("doctor.dat", "wb");
    if (fp)
    {
        if (doctor_list && num_doctors > 0)
            fwrite(doctor_list, sizeof(struct Doctor), num_doctors, fp);
        fclose(fp);
    }

    fp = fopen("patient.dat", "wb");
    if (fp)
    {
        if (patient_list && num_patient > 0)
            fwrite(patient_list, sizeof(struct Patient), num_patient, fp);
        fclose(fp);
    }

    fp = fopen("appointment.dat", "wb");
    if (fp)
    {
        if (appointment_list && num_appointments > 0)
            fwrite(appointment_list, sizeof(struct Appointment), num_appointments, fp);
        fclose(fp);
    }
}


/*
    SAFE CLEAN FUNCTION
*/
void clean(int which)
{
    FILE *fp;

    switch (which)
    {
    case 1: // Clean doctor file
        fp = fopen("doctor.dat", "wb");
        if (fp) fclose(fp);

        free(doctor_list);
        doctor_list = NULL;
        num_doctors = 0;
        break;

    case 2: // Clean patient file
        fp = fopen("patient.dat", "wb");
        if (fp) fclose(fp);

        free(patient_list);
        patient_list = NULL;
        num_patient = 0;
        break;

    case 3: // Clean appointments file
        fp = fopen("appointment.dat", "wb");
        if (fp) fclose(fp);

        free(appointment_list);
        appointment_list = NULL;
        num_appointments = 0;
        break;
    }
}

/* SORT DOCTORS BY SPECIALISATION */
void sorrt()
{
    for (int j = 0; j < num_doctors - 1; j++)
    {
        for (int i = 0; i < num_doctors - j - 1; i++)
        {
            if (strcmp(doctor_list[i].specialisation, doctor_list[i + 1].specialisation) > 0)
            {
                struct Doctor temp = doctor_list[i];
                doctor_list[i] = doctor_list[i + 1];
                doctor_list[i + 1] = temp;
            }
        }
    }
    sor_t = 1;
}

/* DOCTOR MANAGEMENT */
void Doctor(int mode)
{
    int id, ind;
    struct Doctor doct;

    switch (mode)
    {
    case 1:
    {
        int ch;
        printf("\n1: Add Doctor\n2: Edit Doctor\n3: Remove Doctor\nAnything else: Back\n");
        ch = get_int_back();
        if (ch == -1) return;  // invalid → back

        switch (ch)
        {
        case 1: // ADD DOCTOR
        {
            num_doctors++;
            doctor_list = safe_realloc(doctor_list, num_doctors * sizeof(struct Doctor));

            printf("Enter Doctor ID: ");
            int unique = 0;
            while (!unique)
            {
                id = get_int_back();
                if (id == -1)
                {
                    // cancel add, rollback size
                    num_doctors--;
                    doctor_list = safe_realloc(doctor_list, num_doctors * sizeof(struct Doctor));
                    return;
                }
                doct.id = abs(id);
                unique = 1;
                for (int i = 0; i < num_doctors - 1; i++)
                    if (doctor_list[i].id == doct.id)
                    {
                        printf("Duplicate ID! Try again: ");
                        unique = 0;
                        break;
                    }
            }

            printf("Enter Name: ");
            scanf(" %49[^\n]", doct.name);

            printf("Enter Specialisation: ");
            scanf(" %49[^\n]", doct.specialisation);

            printf("Enter Charge: ");
            // Here we keep normal scanf; user is expected to give a number
            while (scanf("%f", &doct.charge) != 1)
            {
                int c;
                while ((c = getchar()) != '\n' && c != EOF)
                    ;
                printf("Invalid charge. Enter again: ");
            }

            doct.appointments = 0;

            doctor_list[num_doctors - 1] = doct;
            sor_t = 0;
            break;
        }

        case 2: // EDIT DOCTOR
        {
            printf("Enter Doctor ID to edit: ");
            id = get_int_back();
            if (id == -1) return;  // back

            id = abs(id);

            ind = -1;
            for (int i = 0; i < num_doctors; i++)
                if (doctor_list[i].id == id)
                    ind = i;

            if (ind == -1)
            {
                printf("Doctor not found.\n");
                break;
            }

            printf("Editing doctor...\n");

            int old_appt = doctor_list[ind].appointments;

            doct.id = id;
            printf("Enter Name: ");
            scanf(" %49[^\n]", doct.name);

            printf("Enter Specialisation: ");
            scanf(" %49[^\n]", doct.specialisation);

            printf("Enter Charge: ");
            while (scanf("%f", &doct.charge) != 1)
            {
                int c;
                while ((c = getchar()) != '\n' && c != EOF)
                    ;
                printf("Invalid charge. Enter again: ");
            }

            doct.appointments = old_appt;
            doctor_list[ind] = doct;

            sor_t = 0;
            break;
        }

        case 3: // REMOVE DOCTOR
        {
            printf("Enter ID to remove: ");
            id = get_int_back();
            if (id == -1) return;

            id = abs(id);

            ind = -1;
            for (int i = 0; i < num_doctors; i++)
                if (doctor_list[i].id == id)
                    ind = i;

            if (ind == -1)
            {
                printf("Doctor not found.\n");
                break;
            }

            for (int i = ind; i < num_doctors - 1; i++)
                doctor_list[i] = doctor_list[i + 1];

            num_doctors--;
            doctor_list = safe_realloc(doctor_list, num_doctors * sizeof(struct Doctor));

            printf("Doctor removed successfully.\n");
            break;
        }

        default:
            return;
        }

        // Remove appointments belonging to removed/edited doctor
        for (int i = 0; i < num_appointments; i++)
        {
            if (appointment_list[i].doctor_id == id)
            {
                for (int j = i; j < num_appointments - 1; j++)
                    appointment_list[j] = appointment_list[j + 1];
                num_appointments--;
                i--;
            }
        }

        if (num_appointments > 0)
            appointment_list = safe_realloc(appointment_list,
                                            num_appointments * sizeof(struct Appointment));
        else
        {
            free(appointment_list);
            appointment_list = NULL;
        }

        printf("More doctor operations? Enter 1 to continue: ");
        int again = get_int_back();
        if (again == 1)
            Doctor(1);

        break;
    }

    case 2:
    {
        if (doctor_list == NULL || num_doctors == 0)
        {
            printf("Doctor list is emptier than my wallet.\n");
            return;
        }

        if (sor_t == 0)
            sorrt();

        printf("\n=== Doctors Sorted By Specialisation ===\n");

        char current[50];
        strcpy(current, doctor_list[0].specialisation);
        printf("%s:\n", current);

        for (int i = 0; i < num_doctors; i++)
        {
            if (strcmp(doctor_list[i].specialisation, current) == 0)
            {
                printf("\tID:%d  Name:%s  Charge:%.2f\n",
                       doctor_list[i].id, doctor_list[i].name, doctor_list[i].charge);
            }
            else
            {
                strcpy(current, doctor_list[i].specialisation);
                printf("%s:\n", current);
                i--;
            }
        }
        break;
    }
    }
}

/* PATIENT REGISTRATION / LOGIN / APPOINTMENT */
void Patient()
{
    int choice;
    int id = -1;

    printf("\n1: Register\n2: Login\nAnything else: Back\n");
    choice = get_int_back();
    if (choice == -1) return;  // invalid → back to main

    if (choice == 1)
    {
        num_patient++;
        patient_list = safe_realloc(patient_list,
                                    num_patient * sizeof(struct Patient));

        struct Patient *p = &patient_list[num_patient - 1];

        printf("Enter ID: ");
        int ok = 0;
        while (!ok)
        {
            int temp_id = get_int_back();
            if (temp_id == -1)
            {
                // cancel registration, rollback
                num_patient--;
                patient_list = safe_realloc(patient_list,
                                            num_patient * sizeof(struct Patient));
                return;
            }

            p->id = abs(temp_id);
            ok = 1;
            for (int i = 0; i < num_patient - 1; i++)
                if (patient_list[i].id == p->id)
                {
                    printf("ID already exists. Try again: ");
                    ok = 0;
                }
        }
        id = p->id;

        printf("Name: ");
        scanf(" %49[^\n]", p->name);

        printf("Age: ");
        while (scanf("%d", &p->age) != 1)
        {
            int c;
            while ((c = getchar()) != '\n' && c != EOF)
                ;
            printf("Invalid age. Enter again: ");
        }

        do
        {
            printf("Gender (1-Male, 2-Female, 3-Other): ");
            p->gender = get_int_back();
            if (p->gender == -1) return;  // back
        } while (p->gender < 1 || p->gender > 3);

        printf("Medical Notes: ");
        scanf(" %499[^\n]", p->medical_notes);
    }
    else if (choice == 2)
    {
        if (num_patient == 0)
        {
            printf("No patients registered yet.\n");
            return;
        }

        printf("Enter ID (or -1 to go back): ");
        while (1)
        {
            int temp = get_int_back();
            if (temp == -1) return; // invalid or -1 both go back

            id = temp;
            if (id == -1)
                return;
            id = abs(id);

            int found = 0;
            for (int i = 0; i < num_patient; i++)
                if (patient_list[i].id == id)
                    found = 1;

            if (found)
                break;

            printf("Wrong ID. Try again or enter -1 to exit: ");
        }
    }
    else
    {
        return;
    }

    /* Check if already has appointment */
    for (int i = 0; i < num_appointments; i++)
    {
        if (appointment_list[i].patient_id == id)
        {
            printf("\nYou already have an appointment:\n");
            printf("DoctorID:%d  Spec:%s  Time:%.2f, %d/%d  Charge:%.2f\n",
                   appointment_list[i].doctor_id,
                   appointment_list[i].specialisation,
                   appointment_list[i].hour,
                   appointment_list[i].date,
                   appointment_list[i].month,
                   appointment_list[i].charge);
            return;
        }
    }

    if (num_doctors == 0)
    {
        printf("No doctors available. This hospital is basically a building.\n");
        return;
    }

    /* Choose specialisation */
    if (sor_t == 0)
        sorrt();

    char **specs = NULL;
    int count = 1;
    specs = safe_realloc(NULL, sizeof(char *));
    specs[0] = malloc(50);
    strcpy(specs[0], doctor_list[0].specialisation);

    for (int i = 1; i < num_doctors; i++)
    {
        if (strcmp(doctor_list[i].specialisation, specs[count - 1]) != 0)
        {
            count++;
            specs = safe_realloc(specs, count * sizeof(char *));
            specs[count - 1] = malloc(50);
            strcpy(specs[count - 1], doctor_list[i].specialisation);
        }
    }

    printf("\nChoose Specialisation:\n");
    for (int i = 0; i < count; i++)
        printf("%d: %s\n", i + 1, specs[i]);

    int pick;
    while (1)
    {
        pick = get_int_back();
        if (pick == -1)
        {
            printf("Invalid choice. Try again: ");
            continue;
        }
        if (pick >= 1 && pick <= count)
            break;
        printf("Invalid choice. Try again: ");
    }

    /* Find doctor with minimum appointments in selected speciality */
    char chosen[50];
    strcpy(chosen, specs[pick - 1]);

    for (int i = 0; i < count; i++)
        free(specs[i]);
    free(specs);

    int first = -1;
    for (int i = 0; i < num_doctors; i++)
        if (strcmp(doctor_list[i].specialisation, chosen) == 0)
        {
            first = i;
            break;
        }

    int min = first;
    for (int i = first + 1; i < num_doctors &&
                           strcmp(doctor_list[i].specialisation, chosen) == 0;
         i++)
    {
        if (doctor_list[i].appointments < doctor_list[min].appointments)
            min = i;
    }

    doctor_list[min].appointments++;

    /* Choose time slot */
    printf("\nAvailable Time Slots:\n");
    for (int i = 0; i < num_time; i++)
    {
        int taken = 0;
        for (int j = 0; j < num_appointments; j++)
        {
            if (appointment_list[j].doctor_id == doctor_list[min].id &&
                appointment_list[j].date == time_list[i].date &&
                appointment_list[j].month == time_list[i].month &&
                appointment_list[j].hour == time_list[i].hour)
                taken = 1;
        }
        if (!taken)
            printf("%d: %.2f, %d/%d\n", i + 1,
                   time_list[i].hour, time_list[i].date, time_list[i].month);
    }

    int slot;
    while (1)
    {
        slot = get_int_back();
        if (slot == -1)
        {
            printf("Try again: ");
            continue;
        }
        if (slot >= 1 && slot <= num_time)
            break;
        printf("Try again: ");
    }

    num_appointments++;
    appointment_list = safe_realloc(appointment_list,
                                    num_appointments * sizeof(struct Appointment));

    struct Appointment *ap = &appointment_list[num_appointments - 1];
    ap->patient_id = id;
    ap->doctor_id = doctor_list[min].id;
    strcpy(ap->specialisation, doctor_list[min].specialisation);
    ap->charge = doctor_list[min].charge;

    ap->date = time_list[slot - 1].date;
    ap->month = time_list[slot - 1].month;
    ap->hour = time_list[slot - 1].hour;

    printf("\nAppointment Booked!\nDoctor: %s\nTime: %.2f, %d/%d\nCharge: %.2f\n",
           doctor_list[min].name, ap->hour, ap->date, ap->month, ap->charge);
}

/* MAIN FUNCTION */
int main()
{
    load_data();

    while (1)
    {
        int choice;
        printf("\n1: Patient\n2: Admin\nAnything else: Exit\n");
        choice = get_int_back();
        if (choice == -1)
            break;  // invalid input → exit program

        if (choice == 1)
        {
            Patient();
        }
        else if (choice == 2)
        {
            char input[50];
            FILE *fp = fopen("pass.txt", "r");
            if (!fp)
            {
                fp = fopen("pass.txt", "w");
                if (fp)
                {
                    fprintf(fp, "1071"); // gear 5 baby
                    fclose(fp);
                }
                fp = fopen("pass.txt", "r");
            }
            if (fp)
            {
                fscanf(fp, " %49[^\n]", pass);
                fclose(fp);
            }

            printf("Enter Admin Password: ");
            scanf(" %49[^\n]", input);

            if (strcmp(pass, input) != 0)
            {
                printf("Wrong password. Even Luffy does better.\n");
                continue;
            }

            while (1)
            {
                printf("\n--- ADMIN MENU ---\n");
                printf("1. Add/Edit/Delete Doctor\n");
                printf("2. View Patients\n");
                printf("3. View Doctors by Specialisation\n");
                printf("4. View Appointments\n");
                printf("5. Change Password\n");
                printf("6. Clean Files\n");
                printf("Anything else: Back\n");

                int a = get_int_back();
                if (a == -1) break;  // invalid input → back to main

                if (a == 1)
                    Doctor(1);
                else if (a == 2)
                {
                    printf("\nPatient List:\n");
                    for (int i = 0; i < num_patient; i++)
                    {
                        printf("%d  %s  Age:%d  Gender:%d\nNotes:%s\n\n",
                               patient_list[i].id,
                               patient_list[i].name,
                               patient_list[i].age,
                               patient_list[i].gender,
                               patient_list[i].medical_notes);
                    }
                }
                else if (a == 3)
                    Doctor(2);
                else if (a == 4)
                {
                    if (num_appointments == 0)
                        printf("No appointments yet.\n");
                    else
                        for (int i = 0; i < num_appointments; i++)
                            printf("Doc:%d  Pat:%d  %s  Time:%.2f %d/%d  %.2f\n",
                                   appointment_list[i].doctor_id,
                                   appointment_list[i].patient_id,
                                   appointment_list[i].specialisation,
                                   appointment_list[i].hour,
                                   appointment_list[i].date,
                                   appointment_list[i].month,
                                   appointment_list[i].charge);
                }
                else if (a == 5)
                {
                    printf("Enter new password: ");
                    scanf(" %49[^\n]", pass);
                    fp = fopen("pass.txt", "w");
                    if (fp)
                    {
                        fprintf(fp, "%s", pass);
                        fclose(fp);
                    }
                }
                else if (a == 6)
                {
                    printf("1: Doctors\n2: Patients\n3: Appointments\nAnything else: Back\n");
                    int w = get_int_back();
                    if (w == -1) continue; // back to admin menu

                    printf("Sure? 1=yes (anything else: no): ");
                    int c = get_int_back();
                    if (c == 1)
                        clean(w);
                }
                else
                    break;  // any other number → back
            }
        }
        else
        {
            break;
        }
    }

    upload_data();

    free(doctor_list);
    free(patient_list);
    free(appointment_list);
    free(time_list);

    printf("\nExiting... Your data is safe. Probably.\n");
    return 0;
}

// Program Finished before end-term stress 😭