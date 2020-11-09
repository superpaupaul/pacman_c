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
		Point position = {0,0};
		for(int i = 0; i < p.L; i++)
		{
			position.x = 0;
			for (int j = 0; j < p.C; j++)
			{
				char obj = p.plateau[i][j];	// l'objet peut être un mur, un bonus, rien ou un fantome
				switch (obj)
					{
						case '*':
							dessiner_rectangle(position,p.taillex,p.tailley,noir);
							break;
						case ' ':
							dessiner_rectangle(position,p.taillex,p.tailley,blanc);
							break;
						case 'B':
							dessiner_rectangle(position,p.taillex,p.tailley,jaune);
							break;
						case 'F':
							dessiner_rectangle(position,p.taillex,p.tailley,rouge);
							break;
						case 'P':
							dessiner_rectangle(position,p.taillex,p.tailley,bleu);
							break;
						default:
							break;

					}
				position.x += p.taillex;
			}
			position.y += p.tailley;
		}
	}

Partie get_size(Partie p) 
	{
		p.taillex = SIZEX;
		p.tailley = SIZEY;
		return p;
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
			attente(400);
		}
	}
	
Partie deplacement_joueur(Partie p, int touche)
	{
		Pos pac = Get_Pacman_Pos(p);
		printf("Position pacman: ligne %d, colonne %d\n",pac.l,pac.c);
		switch(touche)
			{
				case SDLK_LEFT:
					
					if (pac.c - 1 >= 0) // on ne sort pas des indexs du plateau
					{ 
						char char_dest = p.plateau[pac.l][pac.c - 1]; // le char du plateau là où le joueur veut aller
						if (char_dest == ' ')
						{
							printf("gauche\n");
							p.plateau[pac.l][pac.c] = ' '; // remplace le 'P' dans le plateau par un ' '
							dessiner_sprite(p,pac); // redessine le ' ' à l'ancienne place de pacman
							p.plateau[pac.l][pac.c - 1] = 'P'; // ajoute nouvelle pos de pacman au plateau
							pac.c--; // update nouvelle pos de pacman
							dessiner_sprite(p,pac); // dessine pacman à sa nouvelle pos
						}
					}
					break;
				case SDLK_UP:
				
					if (pac.l - 1 >= 0)
					{ 
						char char_dest = p.plateau[pac.l - 1][pac.c];
						if (char_dest == ' ')
						{
							printf("haut\n");
							p.plateau[pac.l][pac.c] = ' ';
							dessiner_sprite(p,pac);
							p.plateau[pac.l - 1][pac.c] = 'P';
							pac.l--;
							dessiner_sprite(p,pac);
						}
					}
					break;
				case SDLK_RIGHT:
					
					if (pac.c + 1 < p.C)
					{ 
						char char_dest = p.plateau[pac.l][pac.c + 1];
						if (char_dest == ' ')
						{
							printf("droite\n");
							p.plateau[pac.l][pac.c] = ' ';
							dessiner_sprite(p,pac);
							p.plateau[pac.l][pac.c + 1] = 'P';
							pac.c++;
							dessiner_sprite(p,pac);
						}
					}
					break;
				case SDLK_DOWN:
					
					if (pac.l + 1 < p.L)
					{ 
						char char_dest = p.plateau[pac.l + 1][pac.c];
						if (char_dest == ' ')
						{
							printf("bas\n");
							p.plateau[pac.l][pac.c] = ' ';
							dessiner_sprite(p,pac);
							p.plateau[pac.l + 1][pac.c] = 'P';
							pac.l++;
							dessiner_sprite(p,pac);
						}
					}
					break;
				default:
					printf("Touche inconnue\n");
					break;
			}
		return p;
	}

void dessiner_sprite(Partie p, Pos pos)
	{
		Point point = Pos_to_Point(p,pos);
		char c = p.plateau[pos.l][pos.c];
		switch (c)
			{
				case ' ':
					dessiner_rectangle(point,p.taillex,p.tailley,blanc);
					break;
				case '*':
					dessiner_rectangle(point,p.taillex,p.tailley,noir);
					break;
				case 'B':
					dessiner_rectangle(point,p.taillex,p.tailley,jaune);
					break;
				case 'F':
					dessiner_rectangle(point,p.taillex,p.tailley,rouge);
					break;
				case 'P':
					dessiner_rectangle(point,p.taillex,p.tailley,bleu);
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
Point Pos_to_Point(Partie p, Pos pos)
	{
		Point res = {pos.c*p.taillex,pos.l*p.tailley};
		return res;
	}