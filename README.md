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

map will consist 16 x 16 grid of blocks
each block will be represented by 16 bits, 8 for state and 8 for texture id

### Sprites

angle and position for each enemy (how many enemies?)
position: 2 floats
angle: 1 float
sprite texture (16 x 16 pixels, 16 bits for each pixel)

### Player

position vector and view angle vector

### Current structure of SPI state transfer to FPGA
| Start byte      | Field                 | Byte size | Type      |
|-----------------|-----------------------|-----------|-----------|
| 0               | Packet size           | 2         | uint16    |
| 2               | Packet mode           | 1         | uint8     |
| 3               | Free real estate      | 1         | uint8     |
| 4               | Player x-pos          | 4         | fix point |
| 8               | Player y-pos          | 4         | fix point |
| 12              | Player x-dir forward  | 4         | fix point |
| 16              | Player y-dir forward  | 4         | fix point |
| 20              | Map states            | 32        | bitmap    |
| 52              | *Garbage*             | 480       | uint8     |
| 532             | 90 degree left x-dir  | 4         | fix point |
| 536             | 90 degree left y-dir  | 4         | fix point |
| 540             | 45 degree left x-dir  | 4         | fix point |
| 544             | 45 degree left y-dir  | 4         | fix point |
| 548             | 90 degree right x-dir | 4         | fix point |
| 552             | 90 degree right y-dir | 4         | fix point |
| 556             | 45 degree right x-dir | 4         | fix point |
| 560             | 45 degree right y-dir | 4         | fix point |
| 564             | Enemy 1 x-pos         | 4         | fix point |
| 568             | Enemy 1 y-pos         | 4         | fix point |
| 572             | Enemy 2 x-pos         | 4         | fix point |
| 576             | Enemy 2 y-pos         | 4         | fix point |
|                 |                       |           |           |
| **Total bytes** | 580                   |           |           |

