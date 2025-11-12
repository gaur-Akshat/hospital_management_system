#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Patient
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
struct Patient *patient_list = NULL;
int num_doctors = 0, num_patient = 0, sor_t = 0, speciaaa = 1;
char pass[50];

void load_dataaaaaaa()
{
    FILE *fp;
    struct Doctor doc;
    struct Patient pat;

    fp = fopen("doctor.dat", "rb");

    while (fread(&doc, sizeof(struct Doctor), 1, fp) == 1)
    {
        num_doctors++;
        doctor_list = realloc(doctor_list, num_doctors * sizeof(struct Doctor));
        doctor_list[num_doctors - 1] = doc;
    }

    fclose(fp);

    fp = fopen("patient.dat", "rb");

    while (fread(&pat, sizeof(struct Patient), 1, fp) == 1)
    {
        num_patient++;
        patient_list = realloc(patient_list, num_patient * sizeof(struct Patient));
        patient_list[num_patient - 1] = pat;
    }

    fclose(fp);
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
        num_doctors=0;
    }
    else if (which == 2)
    {
        fp = fopen("patient.dat", "wb");
        fclose(fp);
        free(patient_list);
        patient_list = NULL;
        num_patient=0;
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
                printf("\n\n%d\t%s\t%s\t%f\n\n", doctor_list[ind].id, doctor_list[ind].name, doctor_list[ind].specialisation, doctor_list[ind].charge);

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
    int b, id;
    printf("\n\nEnter 1 for: Patient Register\nEnter 2 for: Patient Login\nEnter anything else: To go back");

    scanf("%d", &b);
    if (b == 1)
    {
        struct Patient s;
        patient_list = realloc(patient_list, sizeof(struct Patient) * ++num_patient);

        printf("Enter ID: ");
        int foo = 1;
        while (foo)
        {
            scanf("%d", &patient_list[num_patient - 1].id);
            foo = 0;
            for (int i = 0; i < num_patient - 1; i++)
            {
                if (patient_list[i].id == patient_list[num_patient - 1].id)
                {
                    printf("\n\nWrong Input, Try again");
                    foo = 1;
                    break;
                }
            }
        }

        printf("Enter Name: ");
        scanf("%s", patient_list[num_patient - 1].name);

        printf("Enter Age: ");
        scanf("%d", &patient_list[num_patient - 1].age);

        while (1)
        {

            printf("Enter Gender:\n1: Male\n2: Female\n3: Other");
            scanf("%d", &patient_list[num_patient - 1].gender);

            if (patient_list[num_patient - 1].gender != 1 && patient_list[num_patient - 1].gender != 2 && patient_list[num_patient - 1].gender != 3)
            {
                printf("\nwrong input\n");
                continue;
            }

            break;
        }
    }
    else if (b == 2)
    {
        printf("to be done");
    }
    else
    {
        return;
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
                printf("\n\nEnter 1 for: Add/Edit/Delete doctor\nEnter 2 for: View all Patient\nEnter 3 for: Maintain doctor specialization wise list\nEnter 4 for: Password reset\nEnter 5 for: Clean Files\nEnter anything else for: Go to previous options");

                scanf("%d", &c);
                switch (c)
                {
                case 1:
                    Doctor(1);
                    break;
                case 2:
                    printf("\n\nID      Name    Age     Gender\n");
                    
                    for (int i =0; i< num_patient;i++)
                    {
                                            printf("\t%d\t%s\t%d\t%s\n", patient_list[i].id, patient_list[i].name, patient_list[i].age,patient_list[i].gender==1?"Male":patient_list[i].gender==2? "Female":"Other");

                                                                                                               
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
                    int conf = 0, which;

                    printf("\n\nWhich data?\n1: Doctor Data\n2: Patient Data\nanything else:none");
                    scanf("%d", &which);

                    if (which == 1 || which == 2)
                    {
                        printf("\n\nAre You Sure?\n1:yes\nanything else:no");
                        scanf("%d", &conf);
                        if (!(conf - 1))
                            clean(which);
                    }
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