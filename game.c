#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/sem.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <signal.h>

#define ILEMIODUDOSTARCZAJA 2
#define ILEJEDZA 1
#define KOSZTROBOTNICY 7
#define KOSZTWOJOWNIKA 10
#define CZASROBOTNICY 3
#define CZASJEDZENIA 14
#define CZASMISIA 10
#define CZASTWORZENIAROBOTNICY 3
#define CZASTWORZENIAWOJOWNIKA 5
#define SZANSAZAWOLANIAMISKA 5
#define GLODMISKA 1
#define PSZCZOLYPOCZATEK 10
#define KOSZTKROLOWEJ 500

typedef struct{
  pid_t pid;
  int rodzaj;
} pszczola;

pid_t cScr(int* bufor){
  pid_t pid;
  if((pid = fork())==0){
  while(1){
    printf("\033[2J"); // Czyści ekran
    printf("\033[0;0f"); // Ustawia kursor w lewym, górnym rogu
    printf("Punkty zwyciestwa: %d\n",bufor[5] );
    printf("Ile jest w Ulu pszczol: %d\n",bufor[3] );
    printf("Ile mamy miodu: %d\n", bufor[0]);
    printf("Ile misow: %d\n", bufor[6]);
    printf("-------------------------\n");
    printf("9. Start gry\n");
    printf("0. Produkcja robotnicy\n");
    printf("1. Produkcja wojownika\n");
    printf("2. Produkcja królowej\n");
    printf("3. Tworz Misia\n");
    printf("4.Wyjście z gry\n");
    printf("Wpisz co chcesz zrobic: ");
    sleep(5);
  }
}else{//nic
}

return pid;

}


//dodawanie liczby miejsc
int tworzenieSemaphoraMagzynLiczbaMiejsc(){
  key_t key=25;
  int semafor;
  semafor=semget(key,1,IPC_CREAT|0666);
  return semafor;
}

int tworzenieSemaphoraDoBuffora(){
  key_t key=13;
  int semafor;
  semafor=semget(key,1,IPC_CREAT|0666);
  return semafor;
}
//dodawanie Miodu
int tworzenieSemaphoraMiodDod(){
  key_t key=39;
  int semafor;
  semafor=semget(key,1,IPC_CREAT|0666);
  return semafor;
}

int tworzenieSemaphoraMis(){
  key_t key=71;
  int semafor;
  semafor=semget(key,1,IPC_CREAT|0666);
  return semafor;
}
//dodawanie Pszczol
int tworzenieSemaphoraDodawanie(){
  key_t key=39;
  int semafor;
  semafor=semget(key,1,IPC_CREAT|0666);
  return semafor;
}

int *tworzeniePamieci(){
  int *bufor;
  int id = shmget(34, 8, IPC_CREAT|0666);
  bufor = (int*) shmat(id,NULL, 0);
  return bufor;
}

pszczola *tworzeniePamieciPszczol(){
  pszczola *bufor;
  int id = shmget(28, 1024, IPC_CREAT|0666);
  bufor = (pszczola*) shmat(id,NULL, 0);
  return bufor;
}

void sekcjaKrytycznaMagazyn(int *bufor, int groupID, int miodSemaphore, int coRobimy){


  struct sembuf operacjaP;
  operacjaP.sem_op=-1;
  operacjaP.sem_num=0;
  operacjaP.sem_flg=0;

  struct sembuf operacjaV;
  operacjaV.sem_op=1;
  operacjaV.sem_num=0;
  operacjaV.sem_flg=0;

  while(bufor[4]!=0){
    //nic
  }
  semop(groupID,&operacjaP,1);
  semop(miodSemaphore,&operacjaP,1);
  if(coRobimy == 0){
    bufor[0] = bufor[0] +ILEMIODUDOSTARCZAJA;
    //printf("%d\n",bufor[0]);
  }else{
    bufor[0] = bufor[0] -ILEJEDZA;
    //printf("%d\n",bufor[0]);
  }
  semop(miodSemaphore,&operacjaV,1);
  semop(groupID, &operacjaV, 1);
}


void tworzPszole(int* bufor, int magazynSemaphore, int miodSemaphore,pszczola* pszczoly, int bufforSemaphore){
  //potrzebne do semafora
  struct sembuf operacjaP;
  operacjaP.sem_op=-1;
  operacjaP.sem_num=0;
  operacjaP.sem_flg=0;

  struct sembuf operacjaV;
  operacjaV.sem_op=1;
  operacjaV.sem_num=0;
  operacjaV.sem_flg=0;
  semop(miodSemaphore,&operacjaP,1);
  bufor[0] = bufor[0] - KOSZTROBOTNICY;
  semop(miodSemaphore,&operacjaV,1);

  pid_t pid = 0;
  int coRobimy = 0;
  if((pid = fork())==0){
    sleep(CZASTWORZENIAROBOTNICY);
    int objad = 0;
    while(1){
      sleep(CZASROBOTNICY-1);
      if(objad == 4){
        objad = 0;
        coRobimy = 1;
        sekcjaKrytycznaMagazyn(bufor, magazynSemaphore, miodSemaphore, coRobimy);
        coRobimy = 0;
      }
      sleep(CZASROBOTNICY-2);
      sekcjaKrytycznaMagazyn(bufor, magazynSemaphore, miodSemaphore, coRobimy);
      objad++;

    }
  }else{
    semop(bufforSemaphore,&operacjaP,1);
    pszczoly[bufor[3]].pid = pid;
    pszczoly[bufor[3]].rodzaj = 0;
    bufor[1]++;
    bufor[3]++;
    semop(bufforSemaphore,&operacjaV,1);
    //printf("%d\n", pid);
  }
}

void tworzWojownika(int* bufor, int magazynSemaphore, int miodSemaphore, pszczola* pszczoly, int bufforSemaphore){
  //potrzebne do semafora
  struct sembuf operacjaP;

  operacjaP.sem_op=-1;
  operacjaP.sem_num=0;
  operacjaP.sem_flg=0;

  struct sembuf operacjaV;
  operacjaV.sem_op=1;
  operacjaV.sem_num=0;
  operacjaV.sem_flg=0;
  semop(miodSemaphore,&operacjaP,1);
  bufor[0] = bufor[0] - KOSZTWOJOWNIKA;
  semop(miodSemaphore,&operacjaV,1);


  pid_t pid = 0;
  if((pid = fork())==0){
    sleep(CZASTWORZENIAWOJOWNIKA);
    while(1){
      sleep(CZASJEDZENIA);
      sekcjaKrytycznaMagazyn(bufor, magazynSemaphore, miodSemaphore, 1);
    }
  }else{
    semop(bufforSemaphore,&operacjaP,1);
    pszczoly[bufor[3]].pid = pid;
    pszczoly[bufor[3]].rodzaj = 1;
    bufor[2]++;
    bufor[3]++;
    semop(bufforSemaphore,&operacjaV,1);
    //printf("%d\n", pid);
  }
}

void tworzMisia(int* bufor, int magazynSemaphore, int miodSemaphore, pszczola* pszczoly, int bufforSemaphore, int misSemaphore){
  bufor[6]++;
  if(fork()==0){
    sleep(5);
    srand(time(NULL));
    struct sembuf operacjaP;

    operacjaP.sem_op=-1;
    operacjaP.sem_num=0;
    operacjaP.sem_flg=0;

    struct sembuf operacjaV;
    operacjaV.sem_op=1;
    operacjaV.sem_num=0;
    operacjaV.sem_flg=0;

    int licznikglodu = GLODMISKA;
    int szansazawolaniaMiska = SZANSAZAWOLANIAMISKA;

    while(1){
      sleep(CZASMISIA);
      int glod = rand()%10;
      if(licznikglodu>= glod){
        glod = GLODMISKA;

        bufor[4]++;
        semop(misSemaphore,&operacjaP,1);
        semop(bufforSemaphore,&operacjaP,1);
        semop(magazynSemaphore,&operacjaP,10);
        if(bufor[2]<10){
          semop(miodSemaphore,&operacjaP,1);
            if(bufor[0]<50){
              bufor[0] = 0;
            }else{
              bufor[0] = bufor[0] - 50;
            }
            //printf("%d\n",bufor[0]);
          semop(miodSemaphore,&operacjaV,1);
        }

        //zabija
        if(bufor[3]<10){
          for(int i=0; i<bufor[3]; i++){
            pid_t pid = pszczoly[i].pid;
            pszczoly[i].rodzaj = 0;
            kill(pid,9);
            //printf("ZAbite pszczoly\n");

          }
          bufor[1]=0;
          bufor[2]=0;
          bufor[3]=0;
        }
        else{
          for(int i=bufor[3]; i>(bufor[3]-11); i--){
            pid_t pid = pszczoly[i].pid;
            if(pszczoly[i].rodzaj==1){
              bufor[2]--;
            }else{
              bufor[1]--;
            }
            pszczoly[i].rodzaj = 0;

            kill(pid,9);
            //printf("%d ZAbite pszczoly\n", i);
          }
          bufor[3]=bufor[3]-10;
        }
        semop(magazynSemaphore, &operacjaV, 10);
        semop(bufforSemaphore,&operacjaV,1);
        semop(misSemaphore,&operacjaV,1);
        bufor[4]--;

      }else{
        glod++;
      }
      int misiekNowy = rand()%100;
      if(szansazawolaniaMiska >= misiekNowy){
        tworzMisia(bufor, magazynSemaphore, miodSemaphore, pszczoly, bufforSemaphore,misSemaphore);
      }
      else{
        szansazawolaniaMiska= szansazawolaniaMiska+ SZANSAZAWOLANIAMISKA;
      }
    }
  }else{ //nic
  }
}

int main(void)
{

  int magazynSemaphore = tworzenieSemaphoraMagzynLiczbaMiejsc();
  int miodSemaphore = tworzenieSemaphoraMiodDod();
  int bufforSemaphore = tworzenieSemaphoraDoBuffora();
  int misSemaphore = tworzenieSemaphoraMis();
  int* bufor = tworzeniePamieci();
  pszczola* pszczoly = tworzeniePamieciPszczol();


  int i=0;
  semctl(magazynSemaphore,0,SETVAL,100);
  semctl(miodSemaphore,0,SETVAL,1);
  semctl(bufforSemaphore,0,SETVAL,1);
  semctl(misSemaphore,0,SETVAL,1);
  struct sembuf operacjaP;
  operacjaP.sem_op=-1;
  operacjaP.sem_num=0;
  operacjaP.sem_flg=0;

  struct sembuf operacjaV;
  operacjaV.sem_op=1;
  operacjaV.sem_num=0;
  operacjaV.sem_flg=0;
  //inicjalizacja programu

  bufor[0] = 4000;//ile jest w ulu miodu
  bufor[1] = 0;// ile jest pszczol robotnic
  bufor[2] = 0;// ile jest pszczol wojonikow
  bufor[3] = 0;// ile jest ogolnie pszczol
  bufor[4] = 0; //misie
  bufor[5] = 0;
  bufor[6] = 0;// ilosc misiow
  pid_t pid = 0;
  int liczba = 3;
  //bool screen = true;

  while(1){
    pid_t pidConsol;
    pidConsol = cScr(bufor);

    //scanf ("%d", &liczba);
    while(scanf("%d",&liczba) != 1){
                printf("podano nie prawidlowa wartosc\n ");
                while(getchar() != '\n');
    }
    if(liczba == 9){
      for(int i=0; i<PSZCZOLYPOCZATEK; i++){
        tworzPszole(bufor, magazynSemaphore, miodSemaphore, pszczoly, bufforSemaphore);
      }
      tworzMisia(bufor, magazynSemaphore, miodSemaphore, pszczoly, bufforSemaphore,misSemaphore);

      while (1){
        while(scanf("%d",&liczba) != 1){
                    printf("podano nie prawidlowa wartosc\n ");
                    while(getchar() != '\n');
        }
         if(liczba == 0){
           if(bufor[0]>=KOSZTROBOTNICY){
             tworzPszole(bufor, magazynSemaphore, miodSemaphore, pszczoly, bufforSemaphore);
           }
           else{
             printf("Nie mamy tyle miodu\n");
           }
         }
         else if(liczba == 1 ){
           if(bufor[0]>=KOSZTWOJOWNIKA){
             tworzWojownika(bufor, magazynSemaphore, miodSemaphore, pszczoly, bufforSemaphore);
           }else{
             printf("Nie mamy tyle miodu\n");
           }
         }
         else if(liczba == 3 ){
           tworzMisia(bufor, magazynSemaphore, miodSemaphore, pszczoly, bufforSemaphore,misSemaphore);
         }
         else if(liczba == 2){
           srand(time(NULL));
           int szansaNaKrolowa = rand()%4;
           if(bufor[0]>=KOSZTKROLOWEJ){
             semop(miodSemaphore,&operacjaP,1);
             bufor[0]=bufor[0]-500;
             semop(miodSemaphore,&operacjaV,1);
             sleep(9);
             if(szansaNaKrolowa == 1 || szansaNaKrolowa == 3){
               bufor[5]++;
               if(bufor[5]==3){
                 printf("Koniec GRY WYGRALISMY\n");
                 kill(pidConsol,9);
                 exit(1);
               }
             }
             else if(szansaNaKrolowa == 2){
               tworzPszole(bufor, magazynSemaphore, miodSemaphore, pszczoly, bufforSemaphore);
               //printf("Stworzyles Robotnice beka wydales na nia 500 goldow\n");
             }
             else if(szansaNaKrolowa == 0){
               tworzWojownika(bufor, magazynSemaphore, miodSemaphore, pszczoly, bufforSemaphore);

                //printf("Stworzyles Wojownika beka wydales na niego 500 goldow\n");
             }
           }else{
             printf("Nie masz tyle miodu\n");
           }

         }
         else if(liczba == 4){
           printf("Koniec GRY\n");
           kill(pidConsol,9);
           exit(1);
         }
       }
    }
    else if(liczba == 4){
      printf("Koniec GRY\n");
      kill(pidConsol,9);
      exit(1);
    }
    //else{
      //printf("Podaj inna liczbe\n");
    //}
  }

}
