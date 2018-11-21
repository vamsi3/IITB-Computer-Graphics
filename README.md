# The Music Box: Modelling

## Team Details

* Satti Vamsi Krishna Reddy ([vamsi3](https://github.com/vamsi3)) - 160050064
* Vighnesh Reddy Konda ([scopegeneral](https://github.com/scopegeneral)) - 160050090

## Pledge

We pledge that this code in the repository is our own work and that we have not copied it from anywhere.

## Description

All Primitives like ellipsoids (sphere is a special case of cones), cones (cylinder is a special case of cones), cuboids, discs and quads are implemented in `primitive.cpp` and `primitive.hpp`

All the above mentioned primitives are rendered using GL_TRIANGLES.

Hierarchical modelling is implemented in `hierarchy_node.cpp` and `hierarchy_node.hpp`

The main code is implemented in `model.cpp` and `model.hpp`

All the key bindings are implemented in `gl_framework.cpp` and `gl_framework.hpp`



### Control of Units
"1" Toggle wall light
"2" Toggle lamp light
"3" Start animation
"V" Start Bezier curve camera animation
"B" Capture keyframe
"X" Delete keyframes.txt

"W", "A", "S", "D" camera rotation
"I", "J", "K", "L" camera translation



### Bezier Curve Camera Animation

In the beginning one can chooose the points by left clicks and translating with "I","J","K","L". After clicking the points press "V" to begin the animation of camera. This is followed by the keyframe animation automatically.

### Keyframe Animation

Press "3" for animating with already stored keyframes. Or after pressing "V" as stated in Bezier curve animation.


### Model Controls

- The default model selected initially is the room.


“M”	choose the model by typing the input in terminal.	
​	Model 1 is the humanoid clown, Model 2 is the box, Model 3 is R2D2, Model4 is the Room

“C”	choose the limb to rotate by typing in the terminal

“R”	rotate the selected limb about z axis

“Y”	rotate the selected limb about z axis

“T”	rotate the selected limb about x axis

“G” 	rotate the selected limb about x axis

“F” 	rotate the selected limb about y axis

“H” 	rotate the selected limb about y axis
​	

To rotate about the centroids choose the respective center limb.

Center limbs:
 - Model1: Torso
 - Model2: There is only one limb and it is selected always
 - Model3: Torso

### Hierarchy Tree for Model 1


Key for body parts:

| Number | Part |
| ----- | ----- |
| 0 |	LowerTorso |
| 1 |	UpperTorso |
| 2 |	Neck |
| 3 |	Head |
| 4 | 	RShoulder |
| 5 |	LShoulder |
| 6 |	RUpperArm |
| 7 |	LUpperArm |
| 8 |	RElbow |
| 9 |	LElbow |
| 10 |	RLowerArm |
| 11 |	LLowerArm |
| 12 |	Hip |
| 13 |	RLegShoulder |
| 14 |	LLegShoulder |
| 15 |	RUpperLeg |
| 16 |	LUpperLeg |
| 17 |	RKnee |
| 18 |	LKnee |
| 19 |	RLowerLeg |
| 20 |	LLowerLeg |


| Parent | Child |
| ------ | ----- |
| 0 |		1 |
| 0 |		2 |
| 2 |		3 |
| 0 |		4 |
| 0 |		5 |
| 4 |		6 |
| 5 |		7 |
| 6 |		8 |
| 7 |		9 |
| 8 |		10 |
| 9 |		11 |
| 0 |		12 |
| 12 |		13 |
| 12 |		14 |
| 13 |		15 |
| 14 |		16 |
| 15 |		17 |
| 16 |		18 |
| 17 |		19 |
| 18 |		20 |

### Hierarchy Tree for Model 3


Key for body parts:

| Number | Part |
| ----- | ----- |
| 0 |	Torso |
| 1 |	Head |
| 2 |	Bottom Leg |
| 3 |	Bottom Feet |
| 4 | 	RShoulder |
| 5 |	LShoulder |
| 6 |	RArm |
| 7 |	LArm |
| 8 |	RHand |
| 9 |	LHand |
| 10 |	RArm-Disc |
| 11 |	LArm-Disc |
| 12 |	Torso-Bottom-Disk |
| 13 |	Eye-Scanner |
| 14 |	Eye-Scanner-Disc |


| Parent | Child |
| ------ | ----- |
| NULL  | 0 |
| 0  | 1 |
| 0  | 2 |
| 2  | 3 |
| 0  | 4 |
| 0  | 5 |
| 4  | 6 |
| 5  | 7 |
| 6  | 8 |
| 7  | 9 |
| 4  | 10 |
| 5  | 11 |
| 0  | 12 |
| 1  | 13 |
| 13  | 14 |


