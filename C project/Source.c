#include <stdio.h>

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

void Doctor(int i)
{
    switch (i)
    {
    case 1:

        struct Doctor d;

        FILE *fp;
        fp = fopen("doctor.dat", "ab");
        if (fp == NULL)
        {
            printf("Error opening file!");
            return;
        }

        int a;
        printf("\n\nEnter 1 for: Add Doctor\nEnter 2 for: Edit Doctor\nEnter 3 for: Remove Doctor\nEnter anything else for: Previous Selection");
        scanf("%d", &a);
        switch (a)
        {
        case 1:
            printf("Enter ID: ");
            scanf("%d", &d.id);

            printf("Enter Name: ");
            scanf("%s", d.name);

            printf("Enter Specialisation: ");
            scanf("%s", d.specialisation);

            printf("Enter charge: ");
            scanf("%f", &d.charge);
            fwrite(&d, sizeof(d), 1, fp);

            fclose(fp);
            break;
        case 2:

            break;
        case 3:

            break;
        default:
            return;
        }
        break;

    case 2:

        break;
    default:
        printf("Wrong input bro");
        break;
    }

    printf("Enter Again?\n1:yes\nanything else:no");
    scanf("%d", &i);
    if (i == 1)
        Doctor(1);
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
    int a;
    while (1)
    {
        printf("\n\nEnter 1 for: Patient Features\nEnter 2 for: Admin Features");
        scanf("%d", &a);

        if (a == 1)
        {
            Patient();
        }
        else if (a == 2)
        {
            int p, pass = 1071;
            printf("Enter Password: ");
            scanf("%d", &p);
            if (p != pass)
            {
                printf("Wrong Password, going back");
                continue;
            }
            int c;

            do
            {

                printf("\n\nEnter 1 for: Add/Edit/Delete doctor\nEnter 2 for: View all patients\nEnter 3 for: Maintain doctor specialization wise list\nEnter 4 for: Password reset\nEnter anything else for: Go to previous options");

                scanf("%d", &c);
                switch (c)
                {
                case 1:
                    Doctor(1);
                    break;

                case 3:
                    Doctor(2);
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
printf("ID      Name    Age     Gender\n");
                    while (fread(&p, sizeof(struct Patients), 1, fp))
                    {
                        printf("%d\t%s\t%d\t%s\n", p.id, p.name, p.age, (p.gender == 1 ? "Male" : p.gender == 2 ? "Female"
                                                                                                                : "Other"));
                    }
                    fclose(fp);
                    break;

                case 4:

                    break;

                default:
                    main();
                }

            } while (1);
        }
        else
        {
            printf("Wrong Input");
            continue;
        }
    }
    return 0;
}