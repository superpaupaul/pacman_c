/******************************************************************************/
/* CHARGEMENT.c                                                               */
/******************************************************************************/
#include "./partie.h"
#include "../lib/libgraphique.h"

/******************************************************************************/
/* CHARGE PLAN                                                                */
/******************************************************************************/
Partie charge_plan(char *fichier)
    {
    Partie  p;
    FILE    *f = fopen(fichier,"r"); // Ouverture en lecture du fichier
    p.plateau = NULL;   // Le futur plan, à allouer dynamiquement.
    int     res, l, c;
    int     nbf;        // Nb de fantômes trouvés sur le plan
    int     nbb;        // Nb de bonus trouvés sur le plan
    char    ch;

    if(!f) // Si f== NULL, ouverture ratée
        {
        printf("Impossible d'ouvrir '%s'\n",fichier);
        exit(0);
        }
    
/* Lecture des dimensions du plan en en-tête                                  */
    res = fscanf(f,"%d %d\n",&p.L,&p.C); // Lecture de deux entiers

// Si on n'a pas pu lire deux entiers ou s'ils sont incorrects
    if(res !=2 || p.C<2 || p.L<2 || p.C >800 || p.L>600) 
        {
        printf("Dimensions du tableau lues dans '%s' incorrectes\n",fichier);
        fclose(f);
        exit(0);
        }
    printf("Dimensions lues: %d x %d\n",p.L,p.C);

/* ALLOCATION DYNAMIQUE                                                       */
/* Allocation du tableau de *L pointeurs sur lignes                           */
    p.plateau = (char **) malloc(p.L * sizeof(char *));
    if(p.plateau == NULL)
        {
        printf("Allocation dynamique impossible\n");
        fclose(f);
        exit(0);
        }
    
/* Allocation des tableaux de *C caractères                                   */
    for(l=0;l!=p.L;l++)
        {
        p.plateau[l] = (char *) malloc(p.C * sizeof(char));
        if(p.plateau[l] == NULL)
            {
            printf("Allocation dynamique impossible\n");
            fclose(f);
            exit(0);
            }
        }
    

/* LECTURE DES LIGNES DU PLAN                                                 */
    l = 0;
    res = 0;
    nbf = 0;
    nbb = 0;
    while(res != EOF) // Lecture de chaque ligne
        {
        c=0;
        while(1)
            {
            res = fscanf(f,"%c",&ch); // Lecture d'un caractère
            if (res == EOF) // Si fin de fichier
                break; // Quittons la boucle interne
    
            if(c>p.C) // Si trop de colonnes...
                {
                printf("Ligne %d colonne %d: trop de colonnes\n",l,c);
                fclose(f);
                exit(0);
                }
    
            if(c==p.C) // Si fin de ligne supposée...
                {
                if(ch=='\n') // Si fin de ligne réelle, on quitte la boucle
                    {
                    break;
                    }
                else // Sinon trop de caractères
                    {
                    printf("Ligne %d: trop de caractères\n",l);
                    fclose(f);
                    exit(0);
                    }
                }
/* ...sinon, nous ne sommes pas à la fin de la ligne.                         */
/* Si on lit un caractère interdit...                                         */
            if(ch!='.' && ch!=' ' && ch!= '*' && ch!='P' && ch!='F' && ch!='B' && ch!='C')
                {
                if(ch=='\n') // Si c'est un saut de ligne
                    printf("Ligne %d: trop peu de caractères\n",l);
                else
                    printf("Ligne %d: caractère '%c' incorrect\n",l,ch);
    
                fclose(f);
                exit(0);
                }
            
            if(ch=='P')
                {
                p.pacman.l = l;
                p.pacman.c = c;
                }
            else if(ch=='F')
                {
                if(nbf>NBFANTOMES)
                    {
                    printf("Ligne %d:  un fantôme de trop!\n",l);
                    fclose(f);
                    exit(0);
                    }
                p.fantomes[nbf].l = l;
                p.fantomes[nbf].c = c;
                nbf++;
                }
            else if(ch=='B')
                nbb++;

            p.plateau[l][c] = ch; // Ecriture dans le plan
    
            c++; // caractère suivant
            }
        l++; // ligne suivante
        }
    
    fclose(f); // Fermeture du flux de lecture du fichier
    
/* Si à la lecture de EOF on n'est pas sur la *V+1 ème ligne...               */
    if(l != p.L+1)
        {
        printf("Ligne %d: nb de lignes incorrect\n",l);
        exit(0);
        }

/* Si nb de fantômes incorrect...                                             */
    if(nbf!=NBFANTOMES)
        {
        printf("Nb de fantômes incorrect sur le plan\n");
        fclose(f);
        exit(0);
        }

/* Si pas de bonus....                                                       */
    if(nbb==0)
        {
        printf("Aucun bonus sur le plan!\n");
        fclose(f);
        exit(0);
        }
    p.nbbonus = nbb;
    
    return p;
    }


/*****************************************/
/*			affiche_plan				 */ 
/*****************************************/

void affiche_plan(Partie p)
	{
		for(int i = 0; i < p.L; i++)
		{
			for (int j = 0; j < p.C; j++)
			{
				Pos pos = {i,j};
				dessiner_sprite(p,pos);
			}
		}
	}


void debut_graphique(Partie p)
	{
		ouvrir_fenetre(p.LONGUEUR,p.HAUTEUR);
	}
void fin_graphique()
	{
		attendre_clic();
		fermer_fenetre();
	}

void lancer_partie(Partie p)
	{
		while(1)
		{
			int touche = attendre_touche();
			p = deplacement_joueur(p, touche);
			actualiser();
			attente(100);
		}
	}
	
Partie deplacement_joueur(Partie p, int touche)
	{
		int tunnel = 0;
		Pos pac = Get_Pacman_Pos(p);
		char char_dest = ' ';
		printf("Position pacman: ligne %d, colonne %d\n",pac.l,pac.c);
		switch(touche)
			{
				case SDLK_LEFT:
					
					if (pac.c == 0) // tunnel?
					{
						printf("TUNNELLLL\n");
						char_dest = p.plateau[pac.l][p.C - 1]; // si le joueur prend un tunnel à gauche, il se retrouve à droite, même ligne
						tunnel = 1;
						
					}
					else
					{
						char_dest = p.plateau[pac.l][pac.c - 1]; // le char du plateau là où le joueur veut aller
					}
	
					if (char_dest == VIDE || char_dest == POINT || char_dest == BONUS)
					{
						if (char_dest == BONUS)
						{
							/* à implémenter */
						}
						if (char_dest == POINT)
						{
							/* code */
						}

						p.plateau[pac.l][pac.c] = VIDE; // remplace le 'P' dans le plateau par un ' '
						dessiner_sprite(p,pac); // redessine le ' ' à l'ancienne place de pacman

						if (tunnel)
						{
							p.plateau[pac.l][p.C - 1] = PACMAN; //update les coordonnées de pacman tout à droite du plateau
							pac.c = p.C - 1; 
							printf("TUNNELLLL\n");
						}
						
						else
						{
							p.plateau[pac.l][pac.c - 1] = PACMAN; // ajoute nouvelle pos de pacman au plateau
							pac.c--; // update nouvelle pos de pacman
						}

						dessiner_sprite(p,pac); // dessine pacman à sa nouvelle pos
						
					}
						
						
					break;
				case SDLK_UP:
				
					if (pac.l == 0)
					{
						char_dest = p.plateau[p.L - 1][pac.c];
						tunnel = 1;
					}
					else
					{
						char_dest = p.plateau[pac.l - 1][pac.c];
					}
					if (char_dest == VIDE || char_dest == POINT || char_dest == BONUS)
					{
						if (char_dest == BONUS)
						{
							/* à implémenter */
						}
						if (char_dest == POINT)
						{
							/* code */
						}
						p.plateau[pac.l][pac.c] = VIDE;
						dessiner_sprite(p,pac);
						if (tunnel)
						{
							p.plateau[p.L - 1][pac.c] = PACMAN;
							pac.l = p.L - 1;
						}
						else
						{
							p.plateau[pac.l - 1][pac.c] = PACMAN;
							pac.l--;
						}
						dessiner_sprite(p,pac);
					}
					break;
				case SDLK_RIGHT:
					
					if (pac.c == p.C - 1)
					{
						char_dest = p.plateau[pac.l][0];
						tunnel = 1;
					}
					else
					{
						char_dest = p.plateau[pac.l][pac.c + 1];
					}
					
					if (char_dest == VIDE || char_dest == POINT || char_dest == BONUS)
					{
						if (char_dest == BONUS)
						{
							/* à implémenter */
						}
						if (char_dest == POINT)
						{
							/* code */
						}
						p.plateau[pac.l][pac.c] = VIDE;
						dessiner_sprite(p,pac);
						if (tunnel)
						{
							p.plateau[pac.l][0] = PACMAN;
							pac.c = 0;
						}
						else
						{
							p.plateau[pac.l][pac.c + 1] = PACMAN;
							pac.c++;
						}
						dessiner_sprite(p,pac);
					}
					break;
				case SDLK_DOWN:
					
					if (pac.l == p.L - 1)
					{
						char_dest = p.plateau[0][pac.c];
						tunnel = 1;
					}
					else
					{
						char_dest = p.plateau[pac.l + 1][pac.c];
					}
					
					if (char_dest == VIDE || char_dest == POINT || char_dest == BONUS)
					{
						if (char_dest == BONUS)
						{
							/* à implémenter */
						}
						if (char_dest == POINT)
						{
							/* code */
						}
						p.plateau[pac.l][pac.c] = VIDE;
						dessiner_sprite(p,pac);
						if (tunnel)
						{
							p.plateau[0][pac.c] = PACMAN;
							pac.l = 0;
						}
						else
						{
							p.plateau[pac.l + 1][pac.c] = PACMAN;
							pac.l++;
						}
						dessiner_sprite(p,pac);
					}
					break;
				default:
					printf("Touche inconnue\n");
					break;
			}
		p.pacman = pac;
		return p;
	}

void dessiner_sprite(Partie p, Pos pos)
	{
		Point point = Pos_to_Point(pos);
		char c = p.plateau[pos.l][pos.c];
		Point centre = {0,0};
		switch (c)
			{
				case VIDE:
					dessiner_rectangle(point,SIZEX,SIZEY,blanc);
					break;
				case POINT:
					dessiner_rectangle(point,SIZEX,SIZEY,blanc);
					centre.x = point.x + SIZEX/2;
					centre.y = point.y + SIZEY/2;
					dessiner_disque(centre,(int)SIZEX/4,jaune);
					break;
				case MUR:
					dessiner_rectangle(point,SIZEX,SIZEY,noir);
					break;
				case BONUS:
					dessiner_rectangle(point,SIZEX,SIZEY,blanc);
					centre.x = point.x + SIZEX/2;
					centre.y = point.y + SIZEY/2;
					dessiner_disque(centre,(int)SIZEX/2,jaune);
					break;
				case FANTOME:
					dessiner_rectangle(point,SIZEX,SIZEY,rouge);
					break;
				case PACMAN:
					dessiner_rectangle(point,SIZEX,SIZEY,bleu);
					break;
				default:
					break;
			}
	}

Pos Get_Pacman_Pos(Partie p)
	{
		int stop = 0;
		Pos res = {-1,-1};	
		for (int i = 0; i < p.L; i++)
		{
			for (int j = 0; j < p.C; j++)
			{
				if (p.plateau[i][j] == 'P')
				{
					res.l = i;
					res.c = j;
					stop = 1;
					break;
				}
			}
			if (stop == 1)
				break;
		}
		return res;
	}
Point Pos_to_Point(Pos pos)
	{
		Point res = {pos.c*SIZEX,pos.l*SIZEY};
		return res;
	}