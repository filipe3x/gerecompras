#define PBSTR "||||||||||||||||||||||||||||||"
#define PBWIDTH 30

void printProgress (double percentage)
{
    int val = (int) (percentage * 100);
    int lpad = (int) (percentage * PBWIDTH);
    int rpad = PBWIDTH - lpad;
    printf ("\r%3d%% [%.*s%*s]", val, lpad, PBSTR, rpad, "");
    fflush (stdout);
}

void menu(){
    char option;
    bool isRunning = true;

    while(isRunning==true){
        system("clear");
        fflush(stdin);
        printf("\n[1]CATÁLOGOS"
             "\n[2]FATURAÇÃO"
             "\n[3]GESTÃO DE FILIAIS"
             "\n[x]Sair"
             "\nInserir opção > ");

        option = getchar();

        putchar('\n');

puts(
"\n┌─────┬────────┐"
"\n|  #  | Codigo |"
"\n├─────┼────────┤"
"\n│  1  │  100   │"
"\n│  2  │  500   │"
"\n└─────┴────────┘");

        putchar('\n');

        switch(option)
        {
            case '1':
                     //TO DO CODE
                     break;
            case '2':
                     //TO DO CODE
                     break;
            case '3':
                     //TO DO CODE
                     break;
            //...
            case 'x':
                     //Exits the system
                     isRunning = false;
                     return ;
            default :
                     //User enters wrong input
                     //TO DO CODE
                     isRunning = false;
                     break;
        }
    }
    return ;
}
