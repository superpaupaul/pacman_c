## Pacman 0.5



![Pacman](https://i.ibb.co/F4Rk4Tq/Pacman.png)


[Pacman 0.5](https://github.com/superpaupaul/pacman_c) is a game created on the occasion of the UIT Pacman Project. The goal was to try to recreate the mythical game using only the C Programming language. For almost two months we have been trying to create our version of Pacman. At the end of the project we were able to design a pacman that was visually but somewhat lacking in fluidity. It’s with great pleasure that we present you our creation: Pacman 0.5

![](https://i.imgur.com/X1FlseB.png)
*Menu*
![img](https://i.imgur.com/n01sQf9.png)
*Hall Of Fame*
![img](https://i.imgur.com/fnBmphO.png)
*Level 1*
![img](https://i.imgur.com/f0sZAQU.png)
*Level 2*
![img](https://i.imgur.com/k1RdV3T.png)    
*Level 3*

## Prerequisites
This project needs a graphical library which is called SDL in order to work. It is made for Debian systems and needs some dependencies. You can install those by typing:
`sudo apt install libsdl1.2debian libsdl1.2-dev libsdl-ttf2.0-0 libsdl-ttf2.0-dev`
    
## How to use
The project does not need to be compiled, it has already been.
Once [downloaded](https://github.com/superpaupaul/pacman_c/archive/master.zip), the project folder will look like this:
| Item | Description |
| ------: | -----------: |
| BMP  | folder containing all the .bmp images needed |
| data | folder containing all the .txt levels, scores etc |
| lib    | folder containing the graphical library |
| obj    | folder containing the obj files |
| src    | folder containing the code |
| debug.sh | A script we have used to debug the program |
| makefile | The file used for compilation |
| start.sh | The script for starting the program |

---

In order to play you might want to set the start script as executable, so cd into the project folder and type `chmod +x start.sh`
Once this is done you’re good to go!
Just type `./start.sh`

## What is working ?
Even though this project was created by two **great** developers, it has some flaws due to time limits, but it is still ~~very~~ somehow playable. Here is what is working:
| Functionality | State |
| ------: | -----------: |
| menu | works |
| player movements | works |
| ghosts movements | works (almost) |
| pac-gums collecting| works |
| bonus collecting | works |
| end game | works |
| score writing | works |
| score viewing | works |
| game restart | not working |

## acknowledgements
Of course, we didn't know much about programming since this is our first year, so our teachers helped us a lot. We want to thank Laurent Marsan for giving us a skeleton of the project and teaching us how to get into it, Danièle Gardy who helped us too and special mention to Lin Yu Wang who was always there when we were screaming at our monitors and insulting all of those weird bearded guys who invented the C language. Finally, we'll thank our english teacher, Miss Hoguin without whom we wouldn't be able to write such great and structured phrases.

## Who are we?
We are two students from the UIT of Vélizy, FR. We have created this project for school purposes.

## I want to contribute!
Did you find a bug? File it in the issues section! Do you know how to fix stuff? Make a pull request! Or perhaps you want to send us a tip? You can do so here!

[![paypal](https://img.shields.io/badge/donate-on%20paypal-0079CD?logo=paypal&style=for-the-badge)](https://paypal.me/pools/c/8w1gMnu5yl)





