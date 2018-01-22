# Mini Arcade
Presented by team tha_swami: Jack Cruse, Grace Cuenca, and Naotaka Kinoshita (period 10 Systems)

### The Project
Our mini arcade consists of three features:
1. Wack-a-Mole
2. Scrolling Text 
3. Memory Matrix

#### 1. Wack-a-Mole
In our implementation of Wack-a-mole, all clients connected to the server will serve as "holes" that the "mole" can appear in.
Each client should have one terminal screen open. The terminal's background color will changed based on whether a mole is present.
- The terminal will be red if there is no mole
- The terminal will be blue is there is a mole
When the user hits the enter key on a client that has a mole, the mole will move to another client.

#### 2. Scrolling Text
In our implementation of scrolling text, all clients connected to a server will receive a message that scrolls from one client to the other, in the other that they connected to the server. The message can be anything from a 'hi' to a more complex image created by characters.

#### 3. Memory Matrix
In our implementation of memory matrix, all clients connected to the server will have a designated color that the terminal will change to. Upon beginning the game, the server will randomly select a client to 'flash', indicating that it has to be hit. The game progressively gets harder as more and more clients are randomly selected, as the user has to hit enter on the clients in the order that they were flashed.

### Running the Arcade
First, clone the repo

`$ git clone https://github.com/gracecuenca/tha_swami.git`

To compile all the files needed to run every game, first run

`$ make`

To start up the server

`$ ./server`

Now that the server is setup, you can choose which game you'd like to play by setting up the respective clients.
To run wack-a-mole

`$ ./color_client`

To run scrolling text

`$ ./scroll_client`

To run memory matrix

//tbd

