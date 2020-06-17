# The Music Box: OpenGL Animation

> This project is a music-box animation created using OpenGL.

This was made as the final project for CS475: **Computer Graphics** course in Autumn 2018 at Indian Institute of Technology (IIT) Bombay, India.

*For technical details, you may want to check out the [documentation](docs/Documentation.pdf) file or refer to description section below.*

## Video

<div align="center">
<a href="http://www.youtube.com/watch?feature=player_embedded&v=pgxX7Zjkb3k " target="_blank"><img src="http://img.youtube.com/vi/pgxX7Zjkb3k/0.jpg" alt="Animation Video" width="480" height="360" border="10" /></a>
</div>


## Getting Started

- Run the command `make && ./model` to start the window.
- Also, create a folder named `frames` if you would like to store the `.ppm`files (*i.e.* the frames of scenes sequentially) - [to do this you'll also need to uncomment line 1190 in `model.cpp`].

## Description

- All Primitives like ellipsoids (sphere is a special case of cones), cones (cylinder is a special case of cones), cuboids, discs and quads are implemented in `primitive.cpp` and `primitive.hpp`
- All the above mentioned primitives are rendered using `GL_TRIANGLES`.
- Hierarchical modelling is implemented in `hierarchy_node.cpp` and `hierarchy_node.hpp`
- The main code is implemented in `model.cpp` and `model.hpp`
- All the key bindings are implemented in `gl_framework.cpp` and `gl_framework.hpp`

### Control of Units

|        Key         |                Action                 |             Key              |               Action                |
| :----------------: | :-----------------------------------: | :--------------------------: | :---------------------------------: |
|        `1`         |           Toggle wall light           |             `2`              |          Toggle lamp light          |
|        `3`         | Start animation with stored keyframes |             `V`              | Start Bezier curve camera animation |
|        `B`         |           Capture keyframe            |             `X`              |        Delete keyframes.txt         |
| `W`, `A`, `S`, `D` |             Rotate camera             | `I`, `J`, `K`, `L`, `O`, `U` |          Translate camera           |

### Bezier Curve Camera Animation

In the beginning one can choose the points by left clicks and translating with `I`, `J`, `K`, `L`, `O`, `U` only (don't use `W`, `A`, `S`, `D`). After clicking the points press `V` to begin the animation of camera. This is followed by the keyframe animation automatically.

### Keyframe Animation

Press `3` for animating with already stored keyframes. Or after pressing `V` as stated in Bezier curve animation.

### Model Controls

The default model selected initially is the room. All models in this project are listed below:

- Model 1: Humanoid Clown
- Model 2: Box
- Model 3: R2D2
- Model 4: Room

| Key  |                Action                | Key  |                Action                 |
| :--: | :----------------------------------: | :--: | :-----------------------------------: |
|  M   | Choose model with ID *via.* terminal |  C   | Choose limb to rotate *via.* terminal |
|  R   |  Rotate selected limb about Z-axis   |  Y   |   Rotate selected limb about Z-axis   |
|  T   |  Rotate selected limb about X-axis   |  G   |   Rotate selected limb about X-axis   |
|  F   |  Rotate selected limb about Y-axis   |  H   |   Rotate selected limb about Y-axis   |

To rotate about the centroids choose the respective center limb which are listed here:

- Model 1: Torso
- Model 2: There is only one limb and it is selected always
- Model 3: Torso

### Hierarchy Trees

#### Model 1

The various parts of the model are listed below: 

| Index |    Part    | Index |     Part     | Index |     Part     |
| :---: | :--------: | :---: | :----------: | :---: | :----------: |
|   0   | LowerTorso |   7   |  LUpperArm   |  14   | LLegShoulder |
|   1   | UpperTorso |   8   |    RElbow    |  15   |  RUpperLeg   |
|   2   |    Neck    |   9   |    LElbow    |  16   |  LUpperLeg   |
|   3   |    Head    |  10   |  RLowerArm   |  17   |    RKnee     |
|   4   | RShoulder  |  11   |  LLowerArm   |  18   |    LKnee     |
|   5   | LShoulder  |  12   |     Hip      |  19   |  RLowerLeg   |
|   6   | RUpperArm  |  13   | RLegShoulder |  20   |  LLowerLeg   |

The joints connecting the parts are between index pairs listed below:

| Parent | Child | Parent | Child | Parent | Child | Parent | Child |
| :----: | :---: | :----: | :---: | :----: | :---: | :----: | :---: |
|   0    |   1   |   4    |   6   |   9    |  11   |   14   |  16   |
|   0    |   2   |   5    |   7   |   0    |  12   |   15   |  17   |
|   2    |   3   |   6    |   8   |   12   |  13   |   16   |  18   |
|   0    |   4   |   7    |   9   |   12   |  14   |   17   |  19   |
|   0    |   5   |   8    |  10   |   13   |  15   |   18   |  20   |

#### Model 3

The various parts of the model are listed below: 

| Index |    Part     | Index |   Part    | Index |       Part        |
| :---: | :---------: | :---: | :-------: | :---: | :---------------: |
|   0   |    Torso    |   5   | LShoulder |  10   |     RArm-Disc     |
|   1   |    Head     |   6   |   RArm    |  11   |     LArm-Disc     |
|   2   | Bottom Leg  |   7   |   LArm    |  12   | Torso-Bottom-Disk |
|   3   | Bottom Feet |   8   |   RHand   |  13   |    Eye-Scanner    |
|   4   |  RShoulder  |   9   |   LHand   |  14   | Eye-Scanner-Disc  |

The joints connecting the parts are between index pairs listed below:

| Parent | Child | Parent | Child | Parent | Child | Parent | Child |
| :----: | :---: | :----: | :---: | :----: | :---: | :----: | :---: |
|   0    |   1   |   0    |   5   |   7    |   9   |   1    |  13   |
|   0    |   2   |   4    |   6   |   4    |  10   |   13   |  14   |
|   2    |   3   |   5    |   7   |   5    |  11   |        |       |
|   0    |   4   |   6    |   8   |   0    |  12   |        |       |

## Authors

* **Vamsi Krishna Reddy Satti** - [vamsi3](https://github.com/vamsi3)
* Vighnesh Reddy Konda - [scopegeneral](https://github.com/scopegeneral)

## License

This project is licensed under the MIT License - please see the [LICENSE](LICENSE) file for details.