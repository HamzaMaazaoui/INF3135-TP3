/**
 *  Ce programme est un jeu de bataille navalle en C.
 */

#include<stdio.h>
#include<stdlib.h>
#include<time.h>

#define TAILLE_NAVIRE_MIN 2
#define TAILLE_NAVIRE_MAX 6
#define NOMBRE_NAVIRES 6
#define SENS_MIN 0
#define SENS_MAX 3
#define CASE_VIDE 0
#define CASE_OCCUPEE 1
#define CASE_JOUEE -1

typedef struct unechoix_case {
    int x; /* position de la case en x*/
    int y; /* position de la case en y*/
} Case;

typedef struct navire {
    int sens; /* 0 haut 1 droite 2 bas 3 gauche */
    Case premiere_case;
    int taille;
} Navire;
/**
 *Initialiser le générateur des nombres aléatoires.
 */
void init_nb_aleatoire();

/**
 * Renvoie un nombre, tiré au hasard, compris entre deux valeurs.
 * @param min la borne inférieure.
 * @param max la borne supérieure.
 * @return un entier au hasard dans [min, max].
 */
int nb_aleatoire_intervalle(int min, int max);
/**
 * Créer un navire d'une taille donnée.
 * @param taille la taille du navire.
 * @param taille_plateau la taille du plateau.
 * @return le navire créé.
 */
Navire creer_navire(int taille, int taille_plateau);

/**
 * Tester si un navire est valide.
 * @param plateau le plateau du jeu.
 * @param taille_plateau la taille du plateau.
 * @param nav le navire à tester.
 * @return 1 si le navire est valide, 0 sinon.
 */
int est_valide(int **plateau, int taille_plateau, struct navire *nav);

/**
 * Placer un navire sur le plateau.
 * @param plateau
 * @param taille_plateau
 * @param nav
 */
void placer_navire(int **plateau, int taille_plateau, struct navire* nav);

/**
 * Créer une matrice.
 * @param taille_matrice l'ordre de la matrice.
 * @return la matrice crée.
 */
int** creer_matrice(int taille_matrice);

/**
 * Initialisation du plateau.
 * @param plateau le plateau à initialiser.
 * @param taille_plateau la taille du plateau.
 * @param navires
 */
void initialisation_plateau(int **plateau, int taille_plateau, struct navire* navires);

/**
 * Initialisation de la matrice des propriétés du plateau.
 * @param prop matrice des propriétés.
 * @param plateau le plateau de jeu.
 * @param taille_plateau la taille du plateau.
 */
void initialisation_proprietes_plateau(int **prop, int **plateau, int taille_plateau);

/**
 * Tester si un navire est dans une case.
 * @param nav le navire à tester
 * @param choix_case la case à tester
 * @return 1 si le navire est dans la case, 0 sinon.
 */
int verifier_navire_case(struct navire nav, Case choix_case);

/**
 * mettre à jour la table des cases touchés des navires dans le plateau.
 * @param navires la liste des navires
 * @param choix_case la case joué
 * @param nb_touche_nav la table des cases touché des navires.
 */
void mettre_a_jour_navires(struct navire *navires, Case choix_case, int* nb_touche_nav);

/**
 * Demande à l’utilisateur de saisir une case (x,y) à jouer.
 * @param plateau le plateau du jeu.
 * @param prop la matrice de propriétés du plateau
 * @param nb_touche le nombre de cases touchées
 * @param nb_joue le nombre de tentatives
 * @param nb_touche_nav la table d'etats des cases d'un navire
 * @param taille_plateau la taille du plateau
 * @param navires la liste des navires dans le plateau
 */
void choix_joueur(int **plateau, int **prop, int *nb_touche, int *nb_joue, int *nb_touche_nav, int taille_plateau, struct navire* navires);

/**
 * Une fonction pour afficher le plateau.
 * @param plateau le plateau de jeu
 * @param taille_plateau taille du plateau
 * @param prop la matrice de propriétés
 */
void affichage_plateau(int **plateau, int taille_plateau, int **prop);

/**
 * fonction pour sauvegarder la partie dans un fichier.
 * @param nom_fichier le nom de fichier
 */
void sauvegarder_la_partie(const char* nom_fichier, int **plateau, int **prop, int nb_touche, int nb_joue, int *nb_touche_nav, int taille_plateau, struct navire* navires);

/**
 * methode pour afficher le menu
 */
void menu();

/**
 * fonction pour calculre le nombre des cases occupees par des navires
 *  @param navires la liste des navires dans le plateau
 */
int nbre_cases_occupees(struct navire* navires); 

void init_nb_aleatoire() {
   srand(time(NULL));
}

int nb_aleatoire_intervalle(int min, int max) {
   return rand()%(max - min + 1) + min;
}

Navire creer_navire(int taille, int taille_plateau) {
   Navire navire;
   navire.taille =taille;
   navire.sens = nb_aleatoire_intervalle(SENS_MIN, SENS_MAX);
   navire.premiere_case.x = nb_aleatoire_intervalle(0, taille_plateau - 1);
   navire.premiere_case.y = nb_aleatoire_intervalle(0, taille_plateau - 1);
   return navire;
}

int est_valide(int **plateau, int taille_plateau, struct navire *nav) {
   int valide = 1, cpt = 0;
   if(plateau[nav->premiere_case.x][nav->premiere_case.y] == CASE_VIDE) {
      switch(nav->sens) {
         case 0: if(abs(nav->taille - nav->premiere_case.x) >= 0) {
                    for(int i = nav->premiere_case.x; i > abs(nav->taille - nav->premiere_case.x) && valide == 1; --i) {
                       if(plateau[i][nav->premiere_case.y] == CASE_OCCUPEE) 
                          valide = 0;
                     ++cpt;
                     }
                  }
         break;
         case 1: if(nav->taille + nav->premiere_case.y <= taille_plateau) {
                    for(int j = nav->premiere_case.y; j < nav->taille + nav->premiere_case.y && valide == 1; ++j) {
                       if(plateau[nav->premiere_case.x][j] == CASE_OCCUPEE) 
                          valide = 0;
                    ++cpt;
                    }
                  }
         break;      
         case 2: if(nav->taille + nav->premiere_case.x <= taille_plateau) {
                    for(int i = nav->premiere_case.x; i < nav->taille + nav->premiere_case.x && valide == 1; ++i) {
                       if(plateau[i][nav->premiere_case.y] == CASE_OCCUPEE) 
                          valide = 0;
                    ++cpt;
                    }
                  }
         break;  
         case 3: if(abs(nav->taille - nav->premiere_case.y) >= 0) {
                    for(int j = nav->premiere_case.y; j > abs(nav->taille - nav->premiere_case.y)  && valide == 1; --j) {
                       if(plateau[nav->premiere_case.x][j] == CASE_OCCUPEE) 
                          valide = 0;          
                    ++cpt;
                    }
                  }
         break;           
      }
   } else {
      valide = 0;
   }
   return valide == 1 && cpt == nav->taille ? 1 : 0;
}

void placer_navire(int **plateau, int taille_plateau, struct navire* nav) {
   switch(nav->sens) {
      case 0: for(int i = nav->premiere_case.x; i > abs(nav->taille - nav->premiere_case.x); --i) 
                 plateau[i][nav->premiere_case.y] = CASE_OCCUPEE;
      break;
      case 1: for(int j = nav->premiere_case.y; j < nav->taille + nav->premiere_case.y; ++j) 
                 plateau[nav->premiere_case.x][j] = CASE_OCCUPEE;
      break;
      case 2: for(int i = nav->premiere_case.x; i < nav->taille + nav->premiere_case.x; ++i) 
                 plateau[i][nav->premiere_case.y] = CASE_OCCUPEE;
      break;
      case 3: for(int j = nav->premiere_case.y; j > abs(nav->taille - nav->premiere_case.y); --j) 
                 plateau[nav->premiere_case.x][j] = CASE_OCCUPEE;
      break;   
   }
}

int** creer_matrice(int taille_matrice) {
   int** matrice = (int**) malloc(taille_matrice*sizeof(int*));
   for(int i = 0; i < taille_matrice; i++) 
      matrice[i] = (int *) malloc(taille_matrice*sizeof(int));
   return matrice;
}

void initialisation_plateau(int **plateau, int taille_plateau, struct navire* navires) {
   Navire nav;
   int taille_navire;
   for(int i = 0; i < taille_plateau; i++) {
      for(int j = 0; j < taille_plateau; j++) 
         plateau[i][j] = CASE_VIDE;
   }
   for(int i = 0; i < NOMBRE_NAVIRES; i++) {
      do {
         taille_navire = nb_aleatoire_intervalle(TAILLE_NAVIRE_MIN, TAILLE_NAVIRE_MAX);
         nav = creer_navire(taille_navire, taille_plateau);
      } while(est_valide(plateau, taille_navire, &nav) == 0);
      placer_navire(plateau, taille_plateau, &nav);
      navires[i] = nav;
   }
}

void initialisation_proprietes_plateau(int **prop, int **plateau, int taille_plateau) {
   for(int i = 0; i < taille_plateau; i++) {
      for(int j = 0; j < taille_plateau; j++) 
         prop[i][j] = plateau[i][j];
   }
}

int verifier_navire_case(struct navire nav, Case choix_case) {
   switch(nav.sens) {
      case 0: for(int i = nav.premiere_case.x; i > abs(nav.taille - nav.premiere_case.x); i++) {
                 if(choix_case.y == nav.premiere_case.y && choix_case.x == i) 
                    return 1;          
               }
      break;
      case 1: for(int j = nav.premiere_case.y; j < nav.taille + nav.premiere_case.y; j++) {
                 if(choix_case.x == nav.premiere_case.x && choix_case.y == j) 
                    return 1;   
              }
      break;
      case 2: for(int i = nav.premiere_case.x; i < nav.taille + nav.premiere_case.x; i++) {
                 if(choix_case.y == nav.premiere_case.y && choix_case.x == i) 
                    return 1;            
              }
      break;
      case 3: for(int j = nav.premiere_case.y; j > abs(nav.taille - nav.premiere_case.y); j--) {
                 if(choix_case.x == nav.premiere_case.x && choix_case.y == j) 
                    return 1;
              }
      break;
   }
   return 0;
}

void mettre_a_jour_navires(struct navire *navires, Case choix_case, int* nb_touche_nav) {
   for(int i = 0; i < NOMBRE_NAVIRES; i++) {
      if(verifier_navire_case(navires[i], choix_case) == 1) {
         nb_touche_nav[i]++;
         if(nb_touche_nav[i] == navires[i].taille) {
            printf("le navire de taille %d a %ct%c coul%c\n", navires[i].taille, 130, 130, 130);
         }
      }
   }
}

void choix_joueur(int **plateau, int **prop, int *nb_touche, int *nb_joue, int *nb_touche_nav, int taille_plateau, struct navire* navires) {
   Case choix_case;
   do {
      printf("saisir la case: \n");
      printf("x (commencant par 0)= ");
      scanf("%d", &choix_case.x);
      printf("y (commencant par 0)= ");
      scanf("%d", &choix_case.y);
   } while(choix_case.x >= taille_plateau || choix_case.y >= taille_plateau || choix_case.x < 0 || choix_case.y < 0);

   if(prop[choix_case.x][choix_case.y] != CASE_JOUEE) {
      ++(*nb_joue);
      if(prop[choix_case.x][choix_case.y] == CASE_OCCUPEE) {
         ++(*nb_touche);
         mettre_a_jour_navires(navires, choix_case, nb_touche_nav);
      }
      prop[choix_case.x][choix_case.y] = CASE_JOUEE;
   }
}

void affichage_plateau(int **plateau, int taille_plateau, int **prop) {
   for(int i = 0; i < taille_plateau; i++) {
      for(int j = 0; j < taille_plateau; j++) {
         if(prop[i][j] == CASE_JOUEE) {
            if(plateau[i][j] == CASE_OCCUPEE) {
               printf("X ");
            } else {
               printf("O ");
            }
         } else {
            printf(". ");
         }
      }
      printf("\n");
   }
   printf("\n");
}

void afficher_navires_positions(struct navire* navires) {
   printf("navires: ");
   for(int i = 0; i < NOMBRE_NAVIRES; ++i) {
      printf("{case :(%d, %d) , sens: %d, taille: %d}", navires[i].premiere_case.x, navires[i].premiere_case.y, navires[i].sens, navires[i].taille);
   }
   printf("\n");
}

void sauvegarder_la_partie(const char* nom_fichier, int **plateau, int **prop, int nb_touche, int nb_joue, int *nb_touche_nav, int taille_plateau, struct navire* navires) {
   FILE* fichier = fopen(nom_fichier, "w");
   fprintf(fichier, "%d", taille_plateau); fprintf(fichier, " = taille plateau\n");
   fprintf(fichier, "%d", nb_touche);fprintf(fichier, " = nombre case touchee\n");
   fprintf(fichier, "%d",nb_joue);fprintf(fichier, " = nombre case jouee\n");
   for(int i = 0; i < taille_plateau; ++i) {
      for(int j = 0; j < taille_plateau; ++j) {
         fprintf(fichier, "%d;", plateau[i][j]);
      }
      fprintf(fichier, "\n");
   }
   for(int i = 0; i < taille_plateau; ++i) {
      for(int j = 0; j < taille_plateau; ++j) {
         fprintf(fichier, "%d;", prop[i][j]);
      }
      fprintf(fichier, "\n");
   }
   for(int i = 0; i < NOMBRE_NAVIRES; ++i) {
      fprintf(fichier, "%d;", nb_touche_nav[i]);
   }
   fprintf(fichier, "\n");
   for(int i = 0; i < NOMBRE_NAVIRES; ++i) {
      fprintf(fichier, "ligne: %d;colonne: %d;sens: %d;taille: %d;\n",navires[i].premiere_case.x, navires[i].premiere_case.y, navires[i].sens, navires[i].taille);

   }
   fclose(fichier);
}

int charger_la_taille(const char* nom_fichier, int* taille_plateau) {
   FILE* fichier = fopen(nom_fichier, "r");
   if(fichier == NULL) {
      return 0;
   }
   fscanf(fichier, "%d\n", taille_plateau);
   fclose(fichier);
   return 1;
}

int charger_la_partie(const char* nom_fichier, int **plateau, int **prop, int *nb_touche, int *nb_joue, int *nb_touche_nav, int* taille_plateau, struct navire* navires) {
   FILE* fichier = fopen(nom_fichier, "r");
   if(fichier == NULL) {
      return 0;
   }
   fscanf(fichier, "%d", taille_plateau); fscanf(fichier, " = taille plateau\n");
   fscanf(fichier, "%d", nb_touche);fscanf(fichier, " = nombre case touchee\n");
   fscanf(fichier, "%d",nb_joue);fscanf(fichier, " = nombre case jouee\n");
   for(int i = 0; i < *taille_plateau; ++i) {
      for(int j = 0; j < *taille_plateau; ++j) {
         fscanf(fichier, "%d;", &plateau[i][j]);
      }
      fscanf(fichier, "\n");
   }
   for(int i = 0; i < *taille_plateau; ++i) {
      for(int j = 0; j < *taille_plateau; ++j) {
         fscanf(fichier, "%d;", &prop[i][j]);
      }
      fprintf(fichier, "\n");
   }
   for(int i = 0; i < NOMBRE_NAVIRES; ++i) {
      fscanf(fichier, "%d;", &nb_touche_nav[i]);
   }
   fscanf(fichier, "\n");
   for(int i = 0; i < NOMBRE_NAVIRES; ++i) {
      fscanf(fichier, "ligne:%d; colonne:%d; sens:%d; taille:%d;\n",&navires[i].premiere_case.x, &navires[i].premiere_case.y, &navires[i].sens, &navires[i].taille);
   }
   fclose(fichier);
   return 1;
}

void menu() {
   printf("Bataille navale \n");
   printf("------------------------\n");
   printf("1 : Jouer\n");
   printf("2 : Chargement de partie en cours\n");
   printf("0 : Quitter\n");
}

int nbre_cases_occupees(struct navire* navires) {
   int cnt = 0;
   for(int i = 0; i < NOMBRE_NAVIRES; ++i) {
      cnt += navires[i].taille;
   }
   return cnt;
}

int main(int argc, char* argv[]) {
   int** plateau = NULL;
   int taille_plateau = 0;
   struct navire* navires = NULL;
   int** prop = NULL;
   int nb_touche = 0, nb_joue = 0;
   int nb_touche_nav[NOMBRE_NAVIRES] = {0};
   init_nb_aleatoire();
   int choix, cases_occupee = 0;
   char* nom_fichier = argc >= 2 ? nom_fichier = argv[1] : "sauvegarde.txt";
   navires = (struct navire*) malloc(NOMBRE_NAVIRES*sizeof(struct navire));
   cases_occupee = nbre_cases_occupees(navires);
   menu();
   do {
      printf("votre choix : ");
      scanf("%d", &choix);
   } while(choix > 2 || choix < 0 || (choix == 2 && charger_la_taille(nom_fichier, &taille_plateau) == 0));
   if(choix != 0) {
      if(choix == 1) {
         printf("taille_plateau : "); scanf("%d", &taille_plateau);
         plateau = creer_matrice(taille_plateau);
         prop = creer_matrice(taille_plateau);
         initialisation_plateau(plateau, taille_plateau, navires);
         initialisation_proprietes_plateau(prop, plateau, taille_plateau);
      } else if(choix == 2){
         charger_la_taille(nom_fichier, &taille_plateau);
         plateau = creer_matrice(taille_plateau);
         prop = creer_matrice(taille_plateau);
         charger_la_partie(nom_fichier, plateau, prop, &nb_touche, &nb_joue, nb_touche_nav, &taille_plateau, navires);
      }
      cases_occupee = nbre_cases_occupees(navires);
      while(nb_touche < cases_occupee) {
         printf("nb_touche = %d\ncases_occupee = %d\n", nb_touche, cases_occupee);
         affichage_plateau(plateau, taille_plateau, prop);
         choix_joueur(plateau, prop, &nb_touche, &nb_joue, nb_touche_nav, taille_plateau, navires);
         sauvegarder_la_partie(nom_fichier, plateau, prop, nb_touche, nb_joue, nb_touche_nav, taille_plateau, navires);
      }
   } else {
      printf("A la prochaine :) \n");
   }
}

