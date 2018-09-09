========================
Just Dance Fortnite Edition
========================

This is the source code of the OBS filter.
Note: current version of the code is only doing pose detection and does no pose matching.

https://www.youtube.com/watch?v=mKVvQLzAXaM

The player stands with a green screen background and has to mimic the dancers of Fortnite.

In front of the player, a screen contain 3 layers.
At one  side, the fortnite dancer. At the other side, the player.
The background animation and music is taken from Just Dance game video songs.

The project core is using Open Broadcaster Software (OBS).
Fortnite videos are loaded to one pane.
Just Dance videos are loaded as background.
Player camera is loaded to left pane with Chroma Key (green screen) filter.

OBS filter pose detection and matching implemented using OpenCV and OpenPose MobileNet deep neural network.

Project members: Rafael Mizrahi, Mickey Martin, Myriam Schmidt

https://rafaelmizrahi.blogspot.com/