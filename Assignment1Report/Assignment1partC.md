# Assignment 1 - Part 3
*Aya Mahagna ID: 314774639*

<b>1.</b> I've removed the centering trick that was used in Assignment 1 part B, I also implemented orthographic and perspective projection with adjustable view volume (that can be changed using sliders) and I used Canonical view as defaul, and Near and Far sliders have no effect in this assignment.

In this picture we can see the model in its default view volume:

In this picture second I increased the view volume and the model looks smaller:

<b>2.</b> We added Camera LookAt Control to change the camera position (Eye, At , Up) we implemented using glm::LookAt function in Camera class and change the position according to it.

<b>3.</b> To allow the user to set the view volume based on the window size, I used oRendererFrame function and got the window size and I've added a function that notify the renderer that the size was changed so it can change the view volume automatically based on it.

<b>4.</b> Picture of axes in the model frame:

Picture of axes in the world frame:

<b>5.</b> T1 = Rotate-Y, T2 = Translate-X

In this picture I applied T1 on the world frame then T2 on local frame:

In this picture I applied T1 on the local frame and T2 on world frame, in both pictures the model is in different position:

<b>6.</b> In order to draw the model's bounding box, we calculated the minimum and maximum values of each axis so we can draw the box.

<b>7.</b> Sanity check for the vertex normals and face normals.
Picture of face normals:

After translation and rotation:

Picture of vertex normals:

After translation and rotation:

<b>8.</b>  Orthographic projection:

Perspective projectio:

<b>13.</b>

<b> 14. </b> Additional features:

I've added checkboxes to toggle:
- Model frame axes
- World frame axes
- Faces normals
- Vertexes normals
- Bounding box
