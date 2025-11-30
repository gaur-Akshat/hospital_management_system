/*
Project Completion Date: Nov 15----But bug fixes, allignment changes, etc went on until previous commit

Helllo, This our End Semester Project on Hospital Patient Record Management System

Section ND;
Team: Agriim Rastogi, Akshat Gaur, Amrit Mathur, Hemant Tyagi, Dhruv Singh

In this code, we have Implemented, Structures, Dynamic Memory, File Handling, Authentication, Searching/Sorting, and String Handling

Link to the FlowChart: https://lucid.app/lucidchart/544529bb-770d-46ec-8313-6059b0461af5/edit?viewport_loc=-1502%2C-997%2C2587%2C1250%2C0_0&invitationId=inv_69ed15e9-9221-4eea-8d34-1d78aad465f6
*/

// important comment before 1st function

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
    int gender;
};
struct Doctor
{
    int id;
    char name[50];
    char specialisation[50];
    float charge;
    int appointments;
};
struct Time
{
    int date;
    int month;
    float hour;
};
struct Appointment
{
    int patient_id, doctor_id;
    int time;
    char specialisation[50];
    float charge;
};

struct Doctor *doctor_list = NULL;
struct Patient *patient_list = NULL;
struct Appointment *appointment_list = NULL;
struct Time *time_list = NULL;
// Used these for Dynamic Memory

int num_doctors = 0, num_patient = 0, num_time = 0, num_appointments = 0, sor_t = 0, speciaaa = 1;
char pass[50];

/*
After This goto Main, my comments will explain the whole code, Also make sure to read the printf strings ;)
*/

void load_dataaaaaaa()
{
    FILE *fp;
    struct Doctor doc;
    struct Patient pat;
    struct Appointment app;
    struct Time tim;

    time_t t = time(NULL);
    time_t ft;
    struct tm tm = *localtime(&t); // used it for Time Slots creation

    fp = fopen("doctor.dat", "rb");
    if (fp == NULL)
    {
        fp = fopen("doctor.dat", "wb");
        fclose(fp);
        fp = fopen("doctor.dat", "rb");
    } // open OR create then open

    while (fread(&doc, sizeof(struct Doctor), 1, fp) == 1)
    {
        num_doctors++;
        doctor_list = realloc(doctor_list, num_doctors * sizeof(struct Doctor));
        doctor_list[num_doctors - 1] = doc;
    } // Dynamic Memory, Yay!!

    fclose(fp);

    fp = fopen("patient.dat", "rb");
    if (fp == NULL)
    {
        fp = fopen("patient.dat", "wb");
        fclose(fp);
        fp = fopen("patient.dat", "rb");
    } // open OR create then open

    while (fread(&pat, sizeof(struct Patient), 1, fp) == 1)
    {
        num_patient++;
        patient_list = realloc(patient_list, num_patient * sizeof(struct Patient));
        patient_list[num_patient - 1] = pat;
    } // Dynamic Memory, Yay!!

    fclose(fp);
    fp = fopen("appointment.dat", "rb");
    if (fp == NULL)
    {
        fp = fopen("appointment.dat", "wb");
        fclose(fp);
        fp = fopen("appointment.dat", "rb");
    } // open OR create then open

    while (fread(&app, sizeof(struct Appointment), 1, fp) == 1)
    {
        num_appointments++;
        appointment_list = realloc(appointment_list, num_appointments * sizeof(struct Appointment));
        appointment_list[num_appointments - 1] = app;
    } // Dynamic Memory, Yay!!

    fclose(fp);

    for (int i = tm.tm_mday + 1; i < tm.tm_mday + 3; i++)
    {
        for (int j = 0; j < 24; j++)
        {
            ft = t + j * 3600 + (i - tm.tm_mday) * 24 * 3600;
            struct tm tmt = *localtime(&ft);

            if (j < 9)
                continue;
            num_time++;
            time_list = realloc(time_list, num_time * sizeof(struct Time));
            time_list[num_time - 1].hour = j;
            time_list[num_time - 1].date = tmt.tm_mday;
            time_list[num_time - 1].month = tmt.tm_mon;
        }
    } // This loop has created Time Slots for upto 3 next days
}
void upload_dataaaaaaa()
{
    FILE *fp;
    fp = fopen("doctor.dat", "wb");
    fwrite(doctor_list, sizeof(struct Doctor), num_doctors, fp);
    fclose(fp);

    fp = fopen("patient.dat", "wb");
    fwrite(patient_list, sizeof(struct Patient), num_patient, fp);
    fclose(fp);

    fp = fopen("appointment.dat", "wb");
    fwrite(appointment_list, sizeof(struct Appointment), num_appointments, fp);
    fclose(fp);
}
void clean(int which)
{
    FILE *fp;
    if (which == 1)
    {
        fp = fopen("doctor.dat", "wb");
        fclose(fp);
        free(doctor_list);
        doctor_list = NULL;
        num_doctors = 0;
        fp = fopen("appointment.dat", "wb");
        fclose(fp);
        free(appointment_list);
        appointment_list = NULL;
        num_appointments = 0;
    }
    else if (which == 2)
    {
        fp = fopen("patient.dat", "wb");
        fclose(fp);
        free(patient_list);
        patient_list = NULL;
        num_patient = 0;
        fp = fopen("appointment.dat", "wb");
        fclose(fp);
        free(appointment_list);
        appointment_list = NULL;
        num_appointments = 0;
    }
    else if (which == 3)
    {
        fp = fopen("appointment.dat", "wb");
        fclose(fp);
        free(appointment_list);
        appointment_list = NULL;
        num_appointments = 0;
    }
}

void sorrt()
{
    struct Doctor temp;

    for (int j = 0; j < num_doctors; j++)
    {
        for (int i = 0; i < num_doctors - j - 1; i++)
        {
            if (strcmp(doctor_list[i].specialisation, doctor_list[i + 1].specialisation) < 0)
            {
                temp = doctor_list[i];
                doctor_list[i] = doctor_list[i + 1];
                doctor_list[i + 1] = temp;
            }
        }
    }

    sor_t = 1;
}

void Doctor(int i)
{
    struct Doctor doct;
    int id = -1918, ind;
    char str[50];
    switch (i)
    {
    case 1:
        int a = 0;
        printf("\n\nEnter 1 for: Add Doctor\nEnter 2 for: Edit Doctor\nEnter 3 for: Remove Doctor\nEnter anything else for: Previous Selection\n");
        scanf("%d", &a);
        printf("\n");
        switch (a)
        {
        case 1:
            num_doctors++;
            doctor_list = realloc(doctor_list, num_doctors * sizeof(struct Doctor));

            printf("Enter ID: ");
            int foo = 1;
            while (foo)
            {
                scanf("%d", &doct.id);
                doct.id = abs(doct.id);
                foo = 0;
                for (int i = 0; i < num_doctors - 1; i++)
                {
                    if (doctor_list[i].id == doct.id)
                    {
                        printf("\n\nId Duplicate, Try again");
                        foo = 1;
                        break;
                    }
                }
            } // previously we have seen this logic

            printf("Enter Name: ");
            scanf(" %49[^\n]", doct.name);

            printf("Enter Specialisation: ");
            scanf(" %49[^\n]", doct.specialisation);

            printf("Enter charge: ");
            scanf("%f", &doct.charge);

            doct.appointments = 0;

            doctor_list[num_doctors - 1] = doct; // assign

            sor_t = 0;
            break;
        case 2:
            ind = -1;
            printf("Enter Id to edit doctor: ");
            scanf("%d", &id);
            id = abs(id);

            for (int i = 0; i < num_doctors; i++)
            {
                if (doctor_list[i].id == id)
                {
                    ind = i;
                    break;
                }
            } // ind: index of ID in doctor list
            if (ind != -1)
            {
                printf("\n\n%d\t%s\t%s\t%f\n\n", doctor_list[ind].id, doctor_list[ind].name, doctor_list[ind].specialisation, doctor_list[ind].charge);

                doct.id = id;

                printf("Enter Name: ");
                scanf(" %49[^\n]", doct.name);

                printf("Enter Specialisation: ");
                scanf(" %49[^\n]", doct.specialisation);

                printf("Enter charge: ");
                scanf("%f", &doct.charge);
                doctor_list[ind] = doct;

                doctor_list[ind].appointments = 0; // simple stuff
            }
            else
            {
                printf("Not Found");
            }
            sor_t = 0;
            break;
        case 3:
            printf("Enter Id to remove doctor: ");
            scanf("%d", &id);
            id = abs(id);

            ind = -1;
            for (int i = 0; i < num_doctors; i++)
            {
                if (doctor_list[i].id == id)
                {
                    ind = i;
                    break;
                }
            }
            if (ind != -1)
            {
                for (int i = ind; i < num_doctors - 1; i++)
                {
                    doctor_list[i] = doctor_list[i + 1];
                }
                num_doctors--;
                if (num_doctors != 0)
                    doctor_list = realloc(doctor_list, num_doctors * sizeof(struct Doctor));
                else
                    clean(1);
            }
            else
            {
                printf("not found");
            }
            sor_t = 0;
            break;
        default:
            return;
        }

        if (a == 2 || a == 3) // Adjusting Appointment_list according to changes(edit/remove)
        {
            for (int i = 0; i < num_appointments; i++)
            {
                if (appointment_list[i].doctor_id == id)
                {

                    for (int j = i; j < num_appointments - 1; j++)
                    {
                        appointment_list[j] = appointment_list[j + 1];
                    }
                    num_appointments--;
                    i--;
                    if (num_appointments != 0)
                        appointment_list = realloc(appointment_list, num_appointments * sizeof(struct Appointment));
                    else
                        clean(3);
                }
            }
        }

        printf("\n\nAdd/Edit/Remove Again?\n1:yes\nanything else:no\n");

        scanf("%d", &i);
        if (i == 1)
            Doctor(1);
        break;

    case 2:
        if (doctor_list != NULL)
        {
            if (sor_t == 0)
                sorrt(); // Sort list Speciation wise so we can easily print...
            strcpy(str, doctor_list[0].specialisation);
            printf("%s:\n", str);
            for (int i = 0; i < num_doctors; i++)
            {
                if (strcmp(str, doctor_list[i].specialisation) == 0)
                {

                    printf("\t%d\t%-20s\t%f\n", doctor_list[i].id, doctor_list[i].name, doctor_list[i].charge);
                }
                else
                {
                    strcpy(str, doctor_list[i].specialisation);
                    printf("%s:\n", str);
                    i--;
                }
            }
        }
        else
        {
            printf("\n\nDoctor List Empty");
        }

        break;
    default:
        break;
    }
}

void Patient()
{
    int b = 0, id = -1918;
    char str[50];
    char **ptr = NULL;

    printf("\n\nEnter 1 for: Patient Register\nEnter 2 for: Patient Login\nEnter anything else: To go back\n");

    scanf("%d", &b);
    printf("\n");
    if (b == 1)
    {
        struct Patient s;
        patient_list = realloc(patient_list, sizeof(struct Patient) * ++num_patient); // since patient choose 1(register)

        printf("Enter ID: ");
        int foo = 1;
        while (foo)
        {
            scanf("%d", &id);
            id = abs(id);                          // All ID's are abs() after input
            patient_list[num_patient - 1].id = id; // register
            foo = 0;
            for (int i = 0; i < num_patient - 1; i++)
            {
                if (patient_list[i].id == patient_list[num_patient - 1].id) // checking for dups
                {
                    printf("\n\nId Duplicate, Try again");
                    foo = 1;
                    break;
                }
            }
        }

        printf("Enter Name: ");
        scanf(" %49[^\n]", patient_list[num_patient - 1].name); // register

        printf("Enter Age: ");
        scanf("%d", &patient_list[num_patient - 1].age); // register

        while (1)
        {

            printf("Enter Gender:\n1: Male\n2: Female\n3: Other");
            scanf("%d", &patient_list[num_patient - 1].gender); // register

            if (patient_list[num_patient - 1].gender != 1 && patient_list[num_patient - 1].gender != 2 && patient_list[num_patient - 1].gender != 3)
            {
                printf("\nwrong input\n");
                continue;
            }

            break;
        }
        printf("Add Medical Notes: ");
        scanf(" %499[^\n]", patient_list[num_patient - 1].medical_notes); // register
    }
    else if (b == 2) // Login
    {
        if (patient_list != NULL)
        {
            printf("Enter Id: ");
            int foo = 1, bar;
            while (foo)
            {
                scanf("%d", &id);
                if (id == -1)
                    return;
                id = abs(id);
                foo = 1;
                for (int i = 0; i < num_patient; i++)
                {
                    if (patient_list[i].id == id)
                    {
                        foo = 0;
                        break;
                    }
                }
                if (foo == 1)
                    printf("\n\nID not Found, try again, or Press -1 to EXIT\n");
            } // We successfully got the login ID
        }
        else
        {
            printf("\n\nPatient list is Empty, Going back");
            return; // going back to main
        }
    }
    else
    {
        return; // going back to main
    }
    int bsd = 1;
    for (int i = 0; i < num_appointments; i++)
    {
        if (id == appointment_list[i].patient_id) // Check if user Already is registered
        {
            printf("\n\nYou have already been Registered here are your Appointment details:\n");
            printf("\nDoctorID        PatientID       Specialisation  TimeSlot           Charge\n");

            printf("%d\t\t%d\t\t%s\t\t%-5.2f, %d, %d    \t%f\n", appointment_list[i].doctor_id, appointment_list[i].patient_id, appointment_list[i].specialisation, time_list[appointment_list[i].time].hour, time_list[appointment_list[i].time].date, time_list[appointment_list[i].time].month, appointment_list[i].charge);

            bsd = 0;
        }
    }
    if (bsd)
    {

        if (doctor_list != NULL) // bsd checks if already registered
        {

            if (sor_t == 0)
                sorrt();

            speciaaa = 1;
            ptr = realloc(ptr, speciaaa * sizeof(char *));
            ptr[speciaaa - 1] = malloc(50);
            strcpy(ptr[speciaaa - 1], doctor_list[0].specialisation);
            strcpy(str, doctor_list[0].specialisation);

            for (int i = 0; i < num_doctors; i++)
            {
                if (strcmp(str, doctor_list[i].specialisation) != 0) // increament speciaaa as soon as we detect Specialisation change
                {
                    speciaaa++;
                    strcpy(str, doctor_list[i].specialisation);
                    ptr = realloc(ptr, speciaaa * sizeof(char *)); // capacity of ptr to hold n pointers
                    ptr[speciaaa - 1] = malloc(50);                // ptr's nth pointer's memory
                    strcpy(ptr[speciaaa - 1], doctor_list[i].specialisation);
                }
            }

            printf("Enter any of the number from 1 to %d to get assigned to that speciality Doctor\n", speciaaa);
            int ahh = -1;
            for (int i = 0; i < speciaaa; i++)
                printf("\n%d for:%s", i + 1, *(ptr + i)); // prints all specialisation to chose from
            while (1)
            {
                printf("\n");
                scanf("%d", &ahh);
                if (ahh > speciaaa || ahh < 1)
                {
                    printf("wrong input");
                    continue;
                }
                break;
            }

            for (int i = 0; i < speciaaa; i++)
                free(ptr[i]);
            free(ptr); // freeeeeeeeeeee
            int dae = 0, ooh = -1;
            int min, chck;

            int ND = 0;
            while (ahh - 1 > 0 && ND + 1 < num_doctors)
            {
                if (strcmp(doctor_list[ND].specialisation, doctor_list[ND + 1].specialisation) != 0)
                    ahh--;
                ND++; // This will hold the index for 1st index of nth specialisation
            }
            min = ND;
            for (; ND + 1 < num_doctors && strcmp(doctor_list[ND].specialisation, doctor_list[ND + 1].specialisation) == 0; ND++)
            {
                if (doctor_list[ND + 1].appointments < doctor_list[min].appointments)
                    min = ND + 1;
            } // this'll hold the Doctor index with min appointments
            doctor_list[min].appointments++; // simples enough right?

            printf("\n\nEnter any number to Choose from Available Time slots\n\n");
            for (int i = 0; i < num_time; i++)
            {
                chck = 1;
                for (int j = 0; j < num_appointments; j++)
                {
                    if (i == appointment_list[j].time && doctor_list[min].id == appointment_list[j].doctor_id)
                    { // checks if doctor already have appointement at cosen time
                        chck = 0;
                    }
                }
                if (chck)
                {
                    printf("%-3d for: %-5.2f, %d, %d            ", i + 1, time_list[i].hour, time_list[i].date, time_list[i].month);
                    dae++;
                    if (dae == 3)
                    {
                        printf("\n");
                        dae = 0;
                    }
                }
            }
            while (1)
            {
                printf("\n");
                scanf("%d", &ooh);
                chck = 0;
                for (int j = 0; j < num_appointments; j++)
                {
                    if (ooh - 1 == appointment_list[j].time && doctor_list[min].id == appointment_list[j].doctor_id)
                    { // similar to previous loop
                        chck = 1;
                    }
                }

                if (ooh > num_time || ooh < 1 || chck) // simple enough
                {
                    if (chck)
                        printf("Slot Alrady Booked");
                    else
                        printf("wrong input, try again");
                    continue;
                }
                break;
            }

            num_appointments++;

            appointment_list = realloc(appointment_list, num_appointments * sizeof(struct Appointment));
            appointment_list[num_appointments - 1].time = ooh - 1;
            appointment_list[num_appointments - 1].charge = doctor_list[min].charge;
            appointment_list[num_appointments - 1].patient_id = id; // LMAO I previously wrote: patient_list[min].id
            appointment_list[num_appointments - 1].doctor_id = doctor_list[min].id;
            strcpy(appointment_list[num_appointments - 1].specialisation, doctor_list[min].specialisation);

            // Here we assign Appointment info, using the inputs and calculations

            printf("\n\nDear Patient, thankyou for your patience :)\nYou have been Successfully Appointed with \"Mr/Mrs\"%s with Specialisation in %s at Time Slot: ", doctor_list[min].name, appointment_list[num_appointments - 1].specialisation);

            printf("%-5.2f, %d, %d", time_list[ooh - 1].hour, time_list[ooh - 1].date, time_list[ooh - 1].month);
            printf(" The Total charge would be %.2f BUCKS", appointment_list[num_appointments - 1].charge);
        }
        else
            printf("\nSorry, No Doctors currently Available, Try again in future with PATIENT LOGIN Option");
    }
}

int main()
{
    load_dataaaaaaa(); // Load the data from binary File and create if doesn't exist, (I'd suggest go there)

    int a = -1, flag; // both are for switch
    while (1)
    {
        printf("\n\nEnter 1 for: Patient Features\nEnter 2 for: Admin Features\nEnter anything else for: Exit");
        printf("\n");
        scanf("%d", &a);

        if (a == 1)
        {
            Patient(); // Everything Patient is there (Go 👆)
        }
        else if (a == 2)
        {
            char p[50];
            FILE *fp;
            fp = fopen("pass.txt", "r"); // PASSWORD
            if (fp == NULL)
            {
                fp = fopen("pass.txt", "w");
                fprintf(fp, "%s", "1071"); // GEAR 5TH !! (one piece)
                fclose(fp);
                fp = fopen("pass.txt", "r");
            }
            fscanf(fp, " %49[^\n]", pass);
            printf("Enter Password: ");
            scanf(" %49[^\n]", p);
            if (strcmp(p, pass) != 0)
            {
                printf("Wrong Password, going back");
                continue;
            }
            int c = -1;

            do
            {
                flag = 0;
                printf("\n\nEnter 1 for: Add/Edit/Delete doctor\nEnter 2 for: View all Patient\nEnter 3 for: Maintain doctor specialization wise list\nEnter 4 for: Show Appointment List\nEnter 5 for: Password reset\nEnter 6 for: Clean Files\nEnter anything else for: Go to previous options\n");

                scanf("%d", &c);
                switch (c)
                {
                case 1:
                    Doctor(1); // DOCTOR add, edit, delete
                    break;
                case 2: // print patients
                    if (patient_list != NULL)
                    {
                        printf("\n\nID      Name            Age     Gender   Medical_Notes\n");

                        for (int i = 0; i < num_patient; i++)
                        {
                            printf("%d\t%-16s%d\t%-9s%s\n", patient_list[i].id, patient_list[i].name, patient_list[i].age, patient_list[i].gender == 1 ? "Male" : patient_list[i].gender == 2 ? "Female"
                                                                                                                                                                                              : "Other",
                                   patient_list[i].medical_notes);
                        }
                    }
                    else
                    {
                        printf("Patient List Empty");
                    }
                    break;

                case 3:
                    Doctor(2); // check Case 2
                    break;
                case 4: // Print
                    if (appointment_list != NULL)
                    {
                        printf("\n\nDoctorID        PatientID       Specialisation         TimeSlot                  Charge\n");

                        for (int i = 0; i < num_appointments; i++)
                        {
                            printf("%d\t\t%d\t\t%-23s%-5.2f, %d, %-14d %f\n", appointment_list[i].doctor_id, appointment_list[i].patient_id, appointment_list[i].specialisation, time_list[appointment_list[i].time].hour, time_list[appointment_list[i].time].date, time_list[appointment_list[i].time].month, appointment_list[i].charge);
                        }
                    }
                    else
                    {
                        printf("Appointment List Empty");
                    }
                    break;
                    break;

                case 5: // Password Reset
                    printf("\n\nEnter new password: ");
                    scanf(" %49[^\n]", pass);
                    fp = fopen("pass.txt", "w");
                    fprintf(fp, "%s", pass);
                    fclose(fp);
                    printf("Password Updated");

                    break;
                case 6: // CLEAN!!!!
                    int conf = 0, which = -1;

                    printf("\n\nWhich data?\n1: Doctor Data\n2: Patient Data\n3: Appointment Data\nanything else:none\n");
                    scanf("%d", &which);

                    if (which == 1 || which == 2 || which == 3)
                    {
                        printf("\n\nAre You Sure?\n1:yes\nanything else:no\n");
                        scanf("%d", &conf);
                        if (!(conf - 1)) // Confirm
                            clean(which);
                    }
                    break;

                default:
                    flag = 1; // exit
                    break;
                }
                if (flag == 1)
                    break;

            } while (1);
        }
        else
        {
            printf("Files Saved, Exiting");
            break;
        }
    }

    upload_dataaaaaaa(); // dear user your data is safe😈
    return 0;
}