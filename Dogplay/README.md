# mylittlegame
Move the dog by using the keyboard arrows to reach the ball and avoid the black arrows<br>
Whenever you catch the ball your score increases and if you get touched by an arrow your score decreases<br>
If your score is less than **minScore** then you lose<br>
You can use bombs by pressing the space bar, a bomb stops all arrows and lets you touch them for **bombsDuration** seconds, but You have only **numberOfBombs** bombs<br>
You can changer some constants such as **numberOfArrows**, **dogSpeed**, **numberOfBombs** in _src/main.cxx_ file. Then use the command "make all" then "./bin/Dogplay".<br>
This project is made for Linux, so If you're on windows then replace libraries in _lib_ folder by the specific SFML libraries of Windows, also change the Makefile so that the commands will be adapted to your OS.<br>
Have fun !
