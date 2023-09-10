# Rank pictures

## Description
You choose the folder for pictures, and then the application proceed to Main menu, where you can click on one picture out of two. You also can go to the Rank menu, where you can find out current statistics for the pictures.

The project is made basing on library SDL. All animations, transitions, communication between menus: everything made from scratch. TO-DO: statistic is read and saved to the json file in the folder where pictures are stored.

## Showcase
[sample.webm](https://github.com/AndrewChmutov/rank-pictures/assets/67659210/0212ae7f-d04e-46b5-ba56-99bec8e3a6e6)

## Install (Linux)

### Dependencies
First things first, the required libraries:
- SDL2
- SDL images subsystem
- SDL fonts subsystem
- nlohmann/json - single include

To download them on Debian-based linux distributions, the command is following:
```
sudo apt install libsdl1.2-dev libsdl2-image-dev libsdl2-ttf-dev
```

Library that is already included in this repository is the following:
https://github.com/nlohmann/json

Huge thanks to Niels Lohmann!


### Repository

First, you have to clone the repository:
```
git clone https://github.com/AndrewChmutov/rank-pictures
```

Then, you have to setup CMake and build it:
```
cmake -S . -B build
cmake --build build
```

Once it is done, run the application:
```
./build/rank
```

## How to use
You choose the folder. Currently, you have to change the folder location in main.cpp, but soon there will be nice GUI way to do this.
Then, you are given two random pictures. 

To choose one that you like more, just click. There will be transition and everything repeats.

If you wish to see statistics, press SPACE to go to Rank menu. Again, if you want to go back, press SPACE.


## License

Distributed under the [MIT](https://choosealicense.com/licenses/mit/) License.
See [LICENSE](https://github.com/AndrewChmutov/rank-pictures/blob/master/LICENSE) for more information.
