# mcu

## Setup

the project only contains .c and .h files as simplicity studio files cannot be copy pasted to different machines and still compile :( all project files are ignored

1. Install Simplicity Studio or GNU Arm Embedded Toolchain
2. Make a new empty project with GNU ARM build tools and `EFM32 Giant Gecko Starter Kit Board (BRD2200A Rev A03)` as the target board
3. git init
4. add this repo as remote origin
5. git pull
6. success?

## Memory

for floats we can use standard floats on the mcu but they will need to be converted to fixed point floats

### Map

map will consist of a 64 x 64 grid of blocks
each block will be represented by 16 bits, 8 for state and 8 for texture id

### Sprites

angle and position for each enemy (how many enemies?)
position: 2 floats
angle: 1 float
sprite texture (16 x 16 pixels, 16 bits for each pixel)

### Player

position and angle

### Current structure of SPI state transfer to FPGA
(One row = 1 byte)

packet size  
packet size  
packet mode  
*Free byte for later use*  

player xpos  
player xpos  
player xpos  
player xpos  
player ypos  
player ypos  
player ypos  
player ypos  
player xdir  
player xdir  
player xdir  
player xdir  
player ydir  
player ydir  
player ydir  
player ydir  

(The following square is repeated for the size of the map, which is currently 64x64)  
map square state  
map square textureID  

## todo

-- (X) make function for converting regular floats to fixed point precision floats with 16 int bits and 16 bits for fractional part

-- (X) make spi controller (koke?)

-- (X) simple move and rotation functions for player

-- IO code for buttons

-- simple move function for enemies
