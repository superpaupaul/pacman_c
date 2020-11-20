/******************************************************************************/
/* CHARGEMENT.h                                                               */
/******************************************************************************/
#ifndef CHARGEMENT_H
#define CHARGEMENT_H

#include <stdio.h>
#include <stdlib.h>

#define SIZEX 25 // longueur d'une case en pixels
#define SIZEY 25 // hauteur d'une case
#define TAILLETEXTE 20 // taille de la police du texte affiché sur le menu
#define TAILLENOM 10 // taille du nombre de caractères maximum pour le nom du joueur
#define TAILLESCORE 20+TAILLENOM // Taille du score maximal atteignable + TAILLE du Nom du joueur carles deux sont sur une même ligne
#define NBFANTOMES 4 // nombres de fantômes dans les plateaux chargés
#define MUR '*'
#define POINT '.'
#define PACMAN 'P'
#define FANTOME 'F'
#define BONUS 'B'
#define VIDE ' '
#include "../lib/libgraphique.h" // j'ai ajouté
// STRUCTURES
/* Structure Pos: permet de stocker un couple ligne/colonne                   */
typedef struct pos {
    int     l;
    int     c;
    } Pos;

/* Structure Partie:  permet de stocker les paramètres d'une partie           */
typedef struct partie {
    char ** plateau; // le tableau de caractères contenant le plateau de jeu
    int     L; // le nb de lignes du plateau de jeu
    int     C; // le nb de colonne du plateau de jeu
    Pos     pacman; // la position de Pacman dans le plateau
    Pos     fantomes[NBFANTOMES]; // les positions de chaque fantôme
    int     nbbonus; // le nombre de bonus restants à manger
    int     LONGUEUR; // longueur fenêtre graphique
    int     HAUTEUR; // hauteur fenêtre graphique
    int		isdead; // 1 si pacman est mort, 0 sinon
    } Partie;

// PROTOTYPES
/* charge_plan : lit un fichier contenant un plateau de jeu et le charge
                    en mémoire, dans le champ 'plateau' d'une Partie */
Partie charge_plan(char *fichier);

void affiche_plan(Partie p); // affiche le plan graphiquement

void debut_graphique(Partie p); // lance la fenêtre graphique avec les dimensions p.taillex et p.tailley calculées dans get_size()

void fin_graphique(); // attend un clic puis termine la session graphique

void lancer_partie(Partie p); // gère la partie

Partie deplacement_joueur(Partie p, int touche); // déplacements de pacman

void dessiner_sprite(Partie p, Pos pos); // dessine graphiquement en fonction de se qui se trouve sur le plateau à la position i,j

Pos Get_Pacman_Pos(Partie p); // renvoie la position de pacman dans le plateau

Point Pos_to_Point(Pos pos); // Convertit pos du plateau en point graphique(pixels)

void Start_Menu(Partie p); // dessine le menu et gère les choix du joueur, avec les flèches et le bouton entrée

void write_score(int score); // demande le nom du joueur sur le bash et écrit son nom + son score dans le fichier score
#endif

Partie deplacement_fantomes(Partie p);

int check_fantome(int c,int l);

int check_case_fantome(Partie p,int i,char a);

void check_case_libre(Partie p,int i);

void effacement_fantomes(Partie p,int i);

void nouveau_fantomes(Partie p,int i);

int check_case_droite_fantomes(Partie p,int i);

int check_case_gauche_fantomes(Partie p,int i);

int check_case_haut_fantomes(Partie p,int i);

int check_case_bas_fantomes(Partie p,int i);

int Get_gum_number(Partie p); // obtient le nombre de gum restants