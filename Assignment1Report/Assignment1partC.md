# Assignment 1 - Part 3
*Aya Mahagna ID: 314774639*

<b>1.</b> I've removed the centering trick that was used in Assignment 1 part B, I also implemented orthographic and perspective projection with adjustable view volume (that can be changed using sliders) and I used Canonical view as defaul, and Near and Far sliders have no effect in this assignment.
In this picture we can see the model in its default view volume:

In this picture second I increased the view volume and the model looks smaller:

<b>2.</b> We added Camera LookAt Control to change the camera position (Eye, At , Up) we implemented using glm::LookAt function in Camera class and change the position according to it.

<b>3.</b> To allow the user to set the view volume based on the window size, I used oRendererFrame function and got the window size and I've added a function that notify the renderer that the size was changed so it can change the view volume automatically based on it.
