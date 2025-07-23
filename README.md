# Grim Fetcher

[Link to the download page (Itch.io)](https://lycorisdev.itch.io/grim-fetcher)  

![](./screenshot.png)

## Explanation

*Grim Fetcher* is a 2D game written in C. It originates from a school project 
which uses the MinilibX, a barebone C graphics library created for educational 
purposes. I'm publishing *Grim Fetcher* to GitHub because it goes much further 
than what the subject required. I've also seized the opportunity to port the 
project from MinilibX and its XPM format for images to OpenGL and PNG.  

As to whether this would ever become a real game, who knows? I love the 
concept, but I don't currently have time for this project. I don't even have 
time for a refactoring, and gods know it's needed. It has been tempting to 
scrap the whole thing and start over. That being said, I'm still proud of what 
I've made, and I can appreciate my progress from then to 
[Olafur](https://github.com/LycorisDev/c_game_olafur).  

## Features

- X window button, Alt+F4 or ESC to close the game.
- F11 to toggle fullscreen mode.
- Scroll to zoom in and out.
- WASD, arrow keys or mouse left click to move.
- The clicked path is calculated with the A\* pathfinding algorithm.
- No attack button, dodge because you die on contact.
- Enemies wait for the player's first step to move.

## Lore

The main character is a little dog whom people around me kept calling a cat, so 
now it's his name: Sua. It indeed means "cat" in Sumerian. Why Sumerian though, 
is he that old? Only gods know. What I know though is that I'm quite fond of 
the word, as this was the first one I've learned in this language (not that I 
know Sumerian, don't @ me). In any case, Sua is a very good boy, with an 
important job. A dungeon keeps the souls of fallen adventurers, preventing them 
from joining the afterlife, and the dungeon is too dangerous for clerics to 
come resurrect them. This is when Sua dives in: he comes down into the dungeon 
to fetch as many souls he can before going back to the surface to release them.  
