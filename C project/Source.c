#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Patients
{
    int id;
    char name[50];
    int age;
    int gender;
};
struct Doctor
{
    int id;
    char name[50];
    char specialisation[50];
    float charge;
};

struct Doctor *doctor_list = NULL;
int num_doctors = 0, sor_t = 0, speciaaa = 1;
char pass[50];

void load_dataaaaaaa()
{
    FILE *fp;
    struct Doctor doc;

    fp = fopen("doctor.dat", "rb");

    if (doctor_list != NULL)
    {
        free(doctor_list);
    }

    while (fread(&doc, sizeof(struct Doctor), 1, fp) == 1)
    {
        num_doctors++;
        doctor_list = realloc(doctor_list, num_doctors * sizeof(struct Doctor));
        doctor_list[num_doctors - 1] = doc;
    }

    fclose(fp);
}
void upload_dataaaaaaa()
{
    FILE *fp;
    fp = fopen("doctor.dat", "wb");

    fwrite(doctor_list, sizeof(struct Doctor), num_doctors, fp);

    fclose(fp);
}
void clean()
{
    FILE *fp;
    fp = fopen("doctor.dat", "wb");
    fclose(fp);
    free(doctor_list);
    doctor_list = NULL;
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
    int id, ind;
    char str[50];
    switch (i)
    {
    case 1:
        int a;
        printf("\n\nEnter 1 for: Add Doctor\nEnter 2 for: Edit Doctor\nEnter 3 for: Remove Doctor\nEnter anything else for: Previous Selection");
        scanf("%d", &a);
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
                foo = 0;
                for (int i = 0; i < num_doctors - 1; i++)
                {
                    if (doctor_list[i].id == doct.id)
                    {
                        printf("\n\nWrong Input, Try again");
                        foo = 1;
                        break;
                    }
                }
            }

            printf("Enter Name: ");
            scanf("%s", doct.name);

            printf("Enter Specialisation: ");
            scanf("%s", doct.specialisation);

            printf("Enter charge: ");
            scanf("%f", &doct.charge);
            doctor_list[num_doctors - 1] = doct;

            sor_t = 0;
            break;
        case 2:
            ind = -1;
            printf("Enter Id to edit doctor");
            scanf("%d", &id);

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
                printf("\n\n%d\t%s\t%s\t%f\n\n", doctor_list[i].id, doctor_list[i].name, doctor_list[i].specialisation, doctor_list[i].charge);

                doct.id = id;

                printf("Enter Name: ");
                scanf("%s", doct.name);

                printf("Enter Specialisation: ");
                scanf("%s", doct.specialisation);

                printf("Enter charge: ");
                scanf("%f", &doct.charge);
                doctor_list[ind] = doct;
            }
            else
            {
                printf("Not Found");
            }
            sor_t = 0;
            break;
        case 3:
            ind = -1;
            printf("Enter Id to remove doctor");
            scanf("%d", &id);

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
                    clean();
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
        printf("\n\nAdd/Edit/Remove Again?\n1:yes\nanything else:no");
        scanf("%d", &i);
        if (i == 1)
            Doctor(1);
        break;

    case 2:
        if (doctor_list != NULL)
        {
            if (sor_t == 0)
                sorrt();
            strcpy(str, doctor_list[0].specialisation);
            printf("%s:\n", str);
            speciaaa = 1;
            for (int i = 0; i < num_doctors; i++)
            {
                if (strcmp(str, doctor_list[i].specialisation) == 0)
                {

                    printf("\t%d\t%s\t%f\n", doctor_list[i].id, doctor_list[i].name, doctor_list[i].charge);
                }
                else
                {
                    strcpy(str, doctor_list[i].specialisation);
                    printf("%s:\n", str);
                    i--;
                    speciaaa++;
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
    int b;
    printf("\n\nEnter 1 for: Patient Register\nEnter 2 for: Patient Login\nEnter anything else: To go back");

    scanf("%d", &b);
    if (b == 1)
    {
        struct Patients s;

        FILE *fp;
        fp = fopen("patients.dat", "ab");
        if (fp == NULL)
        {
            printf("Error opening file!");
            return;
        }

        printf("Enter ID: ");
        scanf("%d", &s.id);

        printf("Enter Name: ");
        scanf("%s", s.name);

        printf("Enter Age: ");
        scanf("%d", &s.age);

        while (1)
        {

            printf("Enter Gender:\n1: Male\n2: Female\n3: Other");
            scanf("%d", &s.gender);

            if (s.gender != 1 && s.gender != 2 && s.gender != 3)
            {
                printf("\nwrong input\n");
                continue;
            }

            break;
        }
        fwrite(&s, sizeof(s), 1, fp);
        fclose(fp);
    }
    else if (b == 2)
    {
        printf("to be done");
    }
    else
    {
        printf("Wrong Input");
    }
}

int main()
{
    load_dataaaaaaa();

    int a, flag;
    while (1)
    {
        printf("\n\nEnter 1 for: Patient Features\nEnter 2 for: Admin Features\nEnter anything else for: Exit");
        scanf("%d", &a);

        if (a == 1)
        {
            Patient();
        }
        else if (a == 2)
        {
            char p[50];
            FILE *fp = fopen("pass.txt", "r");
            fscanf(fp, "%49s", pass);
            printf("Enter Password: ");
            scanf("%s", p);
            if (strcmp(p, pass) != 0)
            {
                printf("Wrong Password, going back");
                continue;
            }
            int c;

            do
            {
                flag = 0;
                printf("\n\nEnter 1 for: Add/Edit/Delete doctor\nEnter 2 for: View all patients\nEnter 3 for: Maintain doctor specialization wise list\nEnter 4 for: Password reset\nEnter 5 for: Clean Files\nEnter anything else for: Go to previous options");

                scanf("%d", &c);
                switch (c)
                {
                case 1:
                    Doctor(1);
                    break;
                case 2:
                    struct Patients p;

                    FILE *fp;
                    fp = fopen("patients.dat", "rb");
                    if (fp == NULL)
                    {
                        printf("Error opening file!");
                        return 0;
                    }
                    printf("\n\nID      Name    Age     Gender\n");
                    while (fread(&p, sizeof(struct Patients), 1, fp))
                    {
                        printf("%d\t%s\t%d\t%s\n", p.id, p.name, p.age, (p.gender == 1 ? "Male" : p.gender == 2 ? "Female"
                                                                                                                : "Other"));
                    }
                    fclose(fp);
                    break;

                case 3:
                    Doctor(2);
                    break;

                case 4:
                    printf("\n\nEnter new password");
                    scanf("%s", pass);
                    fp = fopen("pass.txt", "w");
                    fprintf(fp, "%s", pass);
                    fclose(fp);
                    printf("Password Updated");

                    break;
                case 5:
                    int conf = 0;
                    printf("\n\nAre You Sure?\n1:yes\nanything else:no");
                    scanf("%d", &conf);
                    if (!(conf - 1))
                        clean();
                    break;

                default:
                    flag = 1;
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

    upload_dataaaaaaa();

    return 0;
}