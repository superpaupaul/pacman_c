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
		p.bouche = 0;
		p.isdead = 0;
		p.bonus = 0;
		p.direction = 1;
		int rand = 1;
		int touche = 0;
		int ancienne_touche = 0;
		affiche_plan(p);
		actualiser();
		while(p.isdead == 0)
		{
			cleanNavbar(p);

			if(Get_gum_number(p) == 0) // il faut l'envoyer sur l'autre niveau
			{
				p.level++;
				switch(p.level)
				{
					Partie new;
					case 2:
						new = charge_plan("data/test.txt");
						new.level = 2;
						lancer_partie(new);
						break;
					case 3:
						new = charge_plan("pathtolevelthree");
						new.level = 3;
						lancer_partie(new);
						break;
					default:
						printf("Vous avez terminé le jeu");
						attendre_clic();
						Start_Menu(p);
						break;
				}
			}

			touche = attendre_touche_duree(180);

			if(touche == 0)	// si le joueur n'a pas appuyé sur une touche, on garde l'ancienne pour qu'il continue dans une direction
			{
				touche = ancienne_touche;
			}

			p = deplacement_joueur(p, touche);


			// Déplacement fantômes une fois sur deux pour ralentir les fantômes
			if(rand)
			{
				p = deplacement_fantomes(p);
				rand = 0;
			}
			else
				rand = 1;

			ancienne_touche = touche;

			// Alternance bouche pac-man ouverte/fermée
			if (p.bouche)
				p.bouche = 0;
			else
				p.bouche = 1;

			actualiser();

		}
		// fin de la partie, assignement du score à un nom
		p.level = 1;
		printf("Vous êtes mort :(\n");
		printf("Score: %d\n",p.score);
		write_score(p.score);
		Start_Menu(p);
	}

Partie deplacement_fantomes(Partie p)
	{
		if (p.bonus>0)
		{
			p.bonus-=1;
		}
		for (int i = 0; i < 4; ++i)
		{
			int delta_x = p.pacman.c-p.fantomes[i].c;//différence de position x entre pacman et le fantome
			int delta_y = p.pacman.l-p.fantomes[i].l;//différence de position y entre pacman et le fantome
			switch(abs(delta_x)>abs(delta_y))//on commence avec le cas où l'abcisse serait plus grande que l'ordonnée
				{
					case 1:
						if(delta_x>0)//ensuite on prend le cas où c>0
						{
							switch(check_case_fantome(p,i,'d'))//on check la disponibilité de la case adjacente
							{
								case 1:
									effacement_fantomes(p,i);//effacement de l'ancien fantome
									p.fantomes[i].c+=1;//changement de la position du fantome 
									nouveau_fantomes(p,i);//création du nouveau fantome à la position donné
									break;
								case 0:
									if(delta_y>=0 &&check_case_fantome(p,i,'b')==1)
									{
										effacement_fantomes(p,i);
										p.fantomes[i].l+=1;
										nouveau_fantomes(p,i);
									}
									else if(check_case_fantome(p,i,'h')==1)
									{
										effacement_fantomes(p,i);
										p.fantomes[i].l-=1;
										nouveau_fantomes(p,i);
									}
									else if(check_case_fantome(p,i,'g')==1)
									{
										effacement_fantomes(p,i);
										p.fantomes[i].c-=1;
										nouveau_fantomes(p,i);
									}
									else
									{
										check_case_libre(p,i);
									}
									break;

							}
						}
						else
						{
							switch(check_case_fantome(p,i,'g'))
							{
								case 1:
									effacement_fantomes(p,i);
									p.fantomes[i].c-=1;
									nouveau_fantomes(p,i);
									break;
								case 0:
									if(delta_y>=0 &&check_case_fantome(p,i,'b')==1)
									{;
										effacement_fantomes(p,i);
										p.fantomes[i].l+=1;
										nouveau_fantomes(p,i);
									}
									else if(check_case_fantome(p,i,'h')==1)
									{
										effacement_fantomes(p,i);
										p.fantomes[i].l-=1;
										nouveau_fantomes(p,i);
									}
									else if(check_case_fantome(p,i,'d')==1)
									{
										effacement_fantomes(p,i);
										p.fantomes[i].c+=1;
										nouveau_fantomes(p,i);
									}
									else
									{
										check_case_libre(p,i);
									}
									break;

							}
						}
						break;
					case 0:
						if(delta_y>0)
						{
							switch(check_case_fantome(p,i,'b'))
							{
								case 1:
									effacement_fantomes(p,i);
									p.fantomes[i].l+=1;
									nouveau_fantomes(p,i);
									break;
								case 0:
									if(delta_x>=0 &&check_case_fantome(p,i,'d')==1)
									{
										effacement_fantomes(p,i);
										p.fantomes[i].c+=1;
										nouveau_fantomes(p,i);
									}
									else if(check_case_fantome(p,i,'g')==1)
									{
										effacement_fantomes(p,i);
										p.fantomes[i].c-=1;
										nouveau_fantomes(p,i);
									}
									else if(check_case_fantome(p,i,'h')==1)
									{
										effacement_fantomes(p,i);
										p.fantomes[i].l-=1;
										nouveau_fantomes(p,i);
									}
									else
									{
										check_case_libre(p,i);
									}
									break;


							}
						}
						else
						{
							switch(check_case_fantome(p,i,'h'))
							{
								case 1:
									effacement_fantomes(p,i);
									p.fantomes[i].l-=1;
									nouveau_fantomes(p,i);
									break;
								case 0:
									if(delta_x>=0 &&check_case_fantome(p,i,'d')==1)
									{
										effacement_fantomes(p,i);
										p.fantomes[i].c+=1;
										nouveau_fantomes(p,i);
									}
									else if(check_case_fantome(p,i,'g')==1)
									{
										effacement_fantomes(p,i);
										p.fantomes[i].c-=1;
										nouveau_fantomes(p,i);
									}
									else if(check_case_fantome(p,i,'b')==1)
									{
										effacement_fantomes(p,i);
										p.fantomes[i].l+=1;
										nouveau_fantomes(p,i);
									}
									else
									{
										check_case_libre(p,i);	
									}
									break;
							}
						}
				}
			
			
			if((p.fantomes[i].c==p.pacman.c) && (p.fantomes[i].l==p.pacman.l) && (p.bonus==0))
			{
				p.isdead+=1;
			}
			else if(p.fantomes[i].c==p.pacman.c && p.fantomes[i].l==p.pacman.l && p.bonus>0)
			{
				effacement_fantomes(p,i);
				//printf("reapparition fantome %d en l=%d,c=%d\n",i,p.reapparition[i].l,p.reapparition[i].c);
				p.fantomes[i].l=p.reapparition[i].l;
				p.fantomes[i].c=p.reapparition[i].c;
				nouveau_fantomes(p,i);
				
			}
		}
	return p;
		
	}

void check_case_libre(Partie p,int i)
	{
		if (check_case_fantome(p,i,'h')==1)
		{
			effacement_fantomes(p,i);
			p.fantomes[i].l-=1;
			nouveau_fantomes(p,i);				
		}
		else if(check_case_fantome(p,i,'d')==1)
		{
			effacement_fantomes(p,i);
			p.fantomes[i].c+=1;
			nouveau_fantomes(p,i);
		}
		else if(check_case_fantome(p,i,'g')==1)
		{
			effacement_fantomes(p,i);
			p.fantomes[i].c-=1;
			nouveau_fantomes(p,i);
		}
		else if(check_case_fantome(p,i,'b')==1)
		{
			effacement_fantomes(p,i);
			p.fantomes[i].l+=1;
			nouveau_fantomes(p,i);
		}
		else
		{
			printf("Fantome %d ne bouge pas\n",i);
			//effacement_fantomes(p,i);
			//nouveau_fantomes(p,i);
		}
	}

void effacement_fantomes(Partie p,int i)//fonction chargé d'effacé le sprite de l'ancien fantome
	{
		char position_ancien_fantome=p.plateau[p.fantomes[i].l][p.fantomes[i].c];
		if(position_ancien_fantome==FANTOME)
			{
				p.reapparition[i].l=p.fantomes[i].l;
				p.reapparition[i].c=p.fantomes[i].c;
				printf("enregistrement reapparition fantome %d\np.reapparition[i].l= %d, p.reapparition[i].c= %d\n",i,p.reapparition[i].l,p.reapparition[i].c);
				p.plateau[p.fantomes[i].l][p.fantomes[i].c]=VIDE;
			}
		dessiner_sprite(p,p.fantomes[i]);
		//printf("reapparition fantome %d en l=%d,c=%d\n",i,p.reapparition[i].l,p.reapparition[i].c);
	}
void nouveau_fantomes(Partie p,int i)
	{

		int x=p.fantomes[i].c*SIZEX;
		int y=p.fantomes[i].l*SIZEY;
		Point position={x,y};
		if (p.bonus==0)
		{
			if(i==0)
			{
				afficher_image("BMP/fantome_bleu.bmp",position);
			}
			else if(i==1)
			{
				afficher_image("BMP/fantome_rose.bmp",position);
			}
			else if(i==2)
			{
				afficher_image("BMP/fantome_vert.bmp",position);
			}
			else
			{
				afficher_image("BMP/fantome_jaune.bmp",position);
			}
		}
		else
		{
			afficher_image("BMP/fantome_bonus.bmp",position);
		}
		actualiser();	
	}

int check_fantome(int c,int l)//check si la case est un fantome
	{
		int x=(c*SIZEX);
		int y=(l*SIZEY); 
		Point point_fantome={x,y};
		if(couleur_point(point_fantome)==65536)
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
int check_case_fantome(Partie p,int i,char a)
	{
		switch(a){
			case 'd': ;
				char position_nouveau_fantome_droit=p.plateau[p.fantomes[i].l][p.fantomes[i].c+1];
				if(position_nouveau_fantome_droit==MUR||check_fantome(p.fantomes[i].c+1,p.fantomes[i].l))
				{
					return 0;
				}
				else
				{
					return 1;
				}
				break;
			case 'g': ;
				char position_nouveau_fantome_gauche=p.plateau[p.fantomes[i].l][p.fantomes[i].c-1];
				if(position_nouveau_fantome_gauche==MUR||check_fantome(p.fantomes[i].c-1,p.fantomes[i].l))
				{
					return 0;
				}
				else
				{
					return 1;
				}
				break;
			case 'h': ;
				char position_nouveau_fantome_haut=p.plateau[p.fantomes[i].l-1][p.fantomes[i].c];
				if(position_nouveau_fantome_haut==MUR||check_fantome(p.fantomes[i].c,p.fantomes[i].l-1))
				{
					return 0;
				}
				else
				{
					return 1;
				}
				break;
			case 'b': ;
				char position_nouveau_fantome_bas=p.plateau[p.fantomes[i].l+1][p.fantomes[i].c];
				if(position_nouveau_fantome_bas==MUR||check_fantome(p.fantomes[i].c,p.fantomes[i].l+1))
				{
					return 0;
				}
				else
				{
					return 1;
				}
				break;
			default:
				return 0;
				break;
		}
	}
	
Partie deplacement_joueur(Partie p, int touche)
	{
		int tunnel = 0;
		Pos pac = Get_Pacman_Pos(p);
		char char_dest = ' ';
		//printf("Position pacman: ligne %d, colonne %d\n",pac.l,pac.c);
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
	
					if (char_dest == VIDE || char_dest == POINT || char_dest == BONUS || char_dest == FANTOME)
					{
						if (char_dest == BONUS)
						{
							p.score += 50;
							p.bonus=TPSBONUS;
							printf("p.bonus=%d",p.bonus);
							gotPoints(50);
						}
						if (char_dest == POINT)
						{
							p.score += 10;
						}
						/*if(char_dest == FANTOME)
						{
							if(p.bonus == 0) // si pacman va sur le fantôme et qu'il n'est pas sous l'emprise du bonus
							{
								
								p.isdead = 1;
							}
							else
							{
								p.score += 400;
								gotPoints(400);
								// fantome doit disparaitre
							}
						}*/

						p.plateau[pac.l][pac.c] = VIDE; // remplace le 'P' dans le plateau par un ' '
						dessiner_sprite(p,pac); // redessine le ' ' à l'ancienne place de pacman
						p.direction = 3; // on indique la direction pour dessiner_sprite dans le bon sens

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
					if (char_dest == VIDE || char_dest == POINT || char_dest == BONUS || char_dest == FANTOME)
					{
						if (char_dest == BONUS)
						{
							p.score += 50;
							p.bonus=TPSBONUS;
							printf("p.bonus=%d",p.bonus);
							gotPoints(50);
						}
						if (char_dest == POINT)
						{
							p.score += 10;
						}
						/*if(char_dest == FANTOME)
						{
							if(p.bonus == 0) // si pacman va sur le fantôme et qu'il n'est pas sous l'emprise du bonus
							{
								p.isdead = 1;
							}
							else
							{
								p.score += 400;
								gotPoints(400);
								// fantome doit disparaitre
							}
						}*/

						p.plateau[pac.l][pac.c] = VIDE;
						dessiner_sprite(p,pac);
						p.direction = 0;
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
					
					if (char_dest == VIDE || char_dest == POINT || char_dest == BONUS || char_dest == FANTOME)
					{
						if (char_dest == BONUS)
						{
							p.score += 50;
							p.bonus=TPSBONUS;
							printf("p.bonus=%d",p.bonus);
							gotPoints(50);
						}
						if (char_dest == POINT)
						{
							p.score += 10;
						}
						/*if(char_dest == FANTOME)
						{
							if(p.bonus == 0) // si pacman va sur le fantôme et qu'il n'est pas sous l'emprise du bonus
							{
								p.isdead = 1;
							}
							else
							{
								p.score += 400;
								gotPoints(400);
								// fantome doit disparaitre
							}
						}*/

						p.plateau[pac.l][pac.c] = VIDE;
						dessiner_sprite(p,pac);
						p.direction = 1;
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
					
					if (char_dest == VIDE || char_dest == POINT || char_dest == BONUS || char_dest == FANTOME)
					{
						if (char_dest == BONUS)
						{
							p.score += 50;
							p.bonus=TPSBONUS;
							printf("p.bonus=%d",p.bonus);
							gotPoints(50);
						}
						if (char_dest == POINT)
						{
							p.score += 10;
						}
						/*if(char_dest == FANTOME)
						{
							if(p.bonus == 0) // si pacman va sur le fantôme et qu'il n'est pas sous l'emprise du bonus
							{
								p.isdead = 1;
							}
							else
							{
								p.score += 400;
								gotPoints(400);
								// fantome doit disparaitre
							}
						}*/

						p.plateau[pac.l][pac.c] = VIDE;
						dessiner_sprite(p,pac);
						p.direction = 2;
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
				case SDLK_ESCAPE:
					fermer_fenetre();
					exit(0);
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
					dessiner_rectangle(point,SIZEX,SIZEY,noir);
					break;
				case POINT:
					dessiner_rectangle(point,SIZEX,SIZEY,noir);
					centre.x = point.x + SIZEX/2;
					centre.y = point.y + SIZEY/2;
					dessiner_disque(centre,(int)SIZEX/4,jaune);
					break;
				case MUR:
					//dessiner_rectangle(point,SIZEX,SIZEY,vert);
					afficher_image("BMP/MUR.bmp",point);
					break;
				case BONUS:
					dessiner_rectangle(point,SIZEX,SIZEY,noir);
					centre.x = point.x + SIZEX/2;
					centre.y = point.y + SIZEY/2;	
					dessiner_disque(centre,(int)SIZEX/2,jaune);
					break;
				case FANTOME:
					dessiner_rectangle(point,SIZEX,SIZEY,0x010000);
					break;
				case PACMAN:
					//dessiner_rectangle(point,SIZEX,SIZEY,bleu);
					switch(p.direction)
					{
						case 0: // UP
							if(p.bouche) // bouche ouverte
							{
								afficher_image("BMP/pacman_ouvert_haut.bmp",point);
								p.bouche = 0;
							}
							else
							{
								afficher_image("BMP/pacman_ferme_haut.bmp",point);
								p.bouche = 1;
							}
							break;
						case 1: // RIGHT
							if(p.bouche)
							{
								afficher_image("BMP/pacman_ouvert_droit.bmp",point);
								p.bouche = 0;
							}
							else
							{
								afficher_image("BMP/pacman_ferme_droit.bmp",point);
								p.bouche = 1;
							}
							break;
						case 2: // DOWN 
							if(p.bouche)
							{
								afficher_image("BMP/pacman_ouvert_bas.bmp",point);
								p.bouche = 0;
							}
							else
							{
								afficher_image("BMP/pacman_ferme_bas.bmp",point);
								p.bouche = 1;
							}
							break;
						case 3: // LEFT
							if(p.bouche)
							{
								afficher_image("BMP/pacman_ouvert_gauche.bmp",point);
								p.bouche = 0;
							}
							else
							{
								afficher_image("BMP/pacman_ferme_gauche.bmp",point);
								p.bouche = 1;
							}
							break;
						default: // weird..
							break;
					}
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
void Start_Menu(Partie p)
{
	int choice = 1; // le choix du joueur: 1 -> jouer, 2 -> scores, 3 -> Quitter
	int oldchoice = choice;
	int touche = 0;
	int couleur_texte = noir;
	char * jouer = "jouer";
	char * highscore = "scores";
	char * quitter = "quitter";
	int longueur = p.C * SIZEX;
	int hauteur = p.L * SIZEY;
	Point hd = {0,0};
	Point pjouer = {longueur/2 - longueur/10,hauteur/4};
	Point phighscore = {pjouer.x,pjouer.y+hauteur/6};
	Point pquitter = {pjouer.x,phighscore.y+hauteur/6};
	Point select = {pjouer.x - 10, pjouer.y}; 
	Point oldselect = select;
	// AFFICHAGE DU MENU
	dessiner_rectangle(hd,longueur,hauteur,blanc); // dessin de l'arrière-plan
	dessiner_rectangle(select,5,1.5*TAILLETEXTE,bleu); // dessin du rectangle de selection
	afficher_texte(jouer,TAILLETEXTE,pjouer,couleur_texte); // dessin des trois options
	afficher_texte(highscore,TAILLETEXTE,phighscore,couleur_texte);
	afficher_texte(quitter,TAILLETEXTE,pquitter,couleur_texte);
	actualiser();
	// AU TOUR DU JOUEUR DE FAIRE SON CHOIX
	while(touche != SDLK_RETURN)
	{
		attente(120);
		touche = attendre_touche();
		switch(touche)
		{
			case SDLK_DOWN:
				if(choice == 1)
				{
					choice = 2;
					select.y += hauteur/6;
				}
				else if(choice == 2)
				{
					choice = 3;
					select.y += hauteur/6;
				}
				break;
			case SDLK_UP:
				if(choice == 2)
				{
					choice = 1;
					select.y -= hauteur/6;
				}
				else if(choice == 3)
				{
					choice = 2;
					select.y -= hauteur/6;
				}
				break;
			case SDLK_ESCAPE:
				fin_graphique();
				break;
			default:
				break;
		}
		if (oldchoice != choice) // On efface l'ancienrectangle de selection si l'utilisateur se déplace dans le menu
		{
			dessiner_rectangle(select,5,1.5*TAILLETEXTE,bleu);
			dessiner_rectangle(oldselect,5,1.5*TAILLETEXTE,blanc);
		}
		oldchoice = choice;
		oldselect = select;
		actualiser();
	}

	if (choice == 1)
		lancer_partie(p);
	
	else if(choice == 2)
	{
		view_scores(p);
	}
	else if(choice == 3)
	{
		fermer_fenetre();
		exit(0);
	}
}

void write_score(int score)
{
	FILE *scores;
	char filePath[] = "data/scores";
	char nomJoueur[TAILLENOM];
	char buf[TAILLESCORE];
	scores = fopen(filePath,"a"); // ouverture du fichier scores en mode append pour ajouter

	if (scores == NULL)	// fopen renvoie NULL si il n'arrive pas à trouver le fichier
	{
		printf("Impossible d'ouvrir %s\n",filePath);
	}
	else
	{
		printf("Entrez votre nom :\n");
		scanf("%s",nomJoueur);
		sprintf(buf, "%s %d\n",nomJoueur,score);
		fprintf(scores,buf);
		fclose(scores);
	}
}

int Get_gum_number(Partie p)
{
	int res = 0;
	for(int i = 0; i < p.L; i++)
	{
		for(int j = 0; j < p.C; j++)
		{
			if (p.plateau[i][j] == POINT)
			{
				res++;
			}
		}
	}
	return res;
}
void view_scores(Partie p)
{
	printf("viewing scores:\n");
	int longueur = p.C * SIZEX;
	int hauteur = p.L * SIZEY;
	FILE *file;
	int i = 0;
	int score;
	char nomJoueur[TAILLENOM];
	Score scoretab[500]; // OMG pas plus de 500 scores !! Bug du 500ième niveau?????
	char filePath[] = "data/scores";
	file = fopen(filePath,"r");

	while(fscanf(file,"%s%d",nomJoueur,&score) != EOF) // lecture du fichier scores, enregistrement de la lecture dans le tableau de Score scoretab
	{
		strcpy(scoretab[i].nom,nomJoueur);
		scoretab[i].score = score;
		i++;
	}
	fclose(file);

	bubbleSort(scoretab,i);

	if(i > 10)
	{
		i = 10;	// i est le nombre de scores que la fonction va afficher, on ne veut pas en afficher plus de 10
	}


	Point hg = {0,0};
	Point texte = {longueur/3,10};
	dessiner_rectangle(hg,longueur,hauteur,blanc);	// affichage de l'arrière-plan

	for(int j = i - 1; j >= 0; j--)	// conversion des Score en str et affichage graphique
	{
		char str[TAILLENOM+TAILLESCORE];
		sprintf(str, "%s %d", scoretab[j].nom,scoretab[j].score);
		printf("%s\n",str);
		afficher_texte(str,TAILLETEXTE,texte,rouge);
		texte.y += 30;
	}
	afficher_texte("Press q to quit",TAILLETEXTE,texte,bleu);
	actualiser();

	// Attente de la sortie et retour au menu
	int touche = attendre_touche();
	while(touche != SDLK_q)
	{
		touche = attendre_touche();
	}
	Start_Menu(p);
}
void swap(Score *xp, Score *yp) 
{ 
    Score temp = *xp; 
    *xp = *yp; 
    *yp = temp; 
} 
  
// A function to implement bubble sort 
void bubbleSort(Score arr[], int n) 
{ 
   int i, j; 
   for (i = 0; i < n-1; i++)       
  
       // Last i elements are already in place    
       for (j = 0; j < n-i-1; j++)  
           if (arr[j].score > arr[j+1].score) 
              swap(&arr[j], &arr[j+1]); 
} 


void gotPoints(int x)
{
	Point p = {0,0};
	char buf[6];
	sprintf(buf,"+%d",x);
	printf("%s\n",buf);
	afficher_texte(buf,TAILLETEXTE,p,rouge);
}

void cleanNavbar(Partie p){
	for (int i = 0; i < p.C; ++i)
	{
		Pos pos = {0,i};
		dessiner_sprite(p,pos);
	}
}