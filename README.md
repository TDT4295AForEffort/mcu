# mcu

## setup
install simplicity studio or GNU Arm Embedded Toolchain

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

## todo
-- make function for converting regular floats to fixed point precision floats with 16 int bits and 16 bits for fractional part

-- make spi controller (koke?)

-- simple move and rotation functions for player

-- IO code for buttons

-- simple move function for enemies
