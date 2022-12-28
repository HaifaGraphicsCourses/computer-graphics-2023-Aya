# Assignment 1 - Part 3
<b> Aya Mahagna ID: 314774639 </b> 

<b>1.</b> I've removed the centering trick that was used in Assignment 1 part B, I also implemented orthographic and perspective projection with adjustable view volume (that can be changed using sliders) and I used Canonical view as defaul, and Near and Far sliders have no effect in this assignment.

In this picture we can see the model in its default view volume:

![Task1](https://user-images.githubusercontent.com/95486750/209868244-384c42e2-f07b-412c-bf0e-d41c146298ec.png)

In this picture second I increased the view volume and the model looks smaller:

![Task1Change](https://user-images.githubusercontent.com/95486750/209868382-3a685954-77d3-4512-8dda-fdcbadaa2e3d.png)

<b>2.</b> We added Camera LookAt Control to change the camera position (Eye, At , Up) we implemented using glm::LookAt function in Camera class and change the position according to it.

![image](https://user-images.githubusercontent.com/95486750/209868686-9fd03ed6-6bb0-497e-8e5c-841605d5feb1.png)

<b>3.</b> To allow the user to set the view volume based on the window size, I used oRendererFrame function and got the window size and I've added a function that notify the renderer that the size was changed so it can change the view volume automatically based on it.

<b>4.</b> Picture of axes in the model frame:
![image](https://user-images.githubusercontent.com/95486750/209868850-40749b9e-049f-4cdf-a03b-77ba94d710f3.png)

Picture of axes in the world frame:
![image](https://user-images.githubusercontent.com/95486750/209868905-c31aa00e-4cc0-4af5-b3e3-7f99d9ca212b.png)

<b>5.</b> T1 = Scale, T2 = Translate-X

In this picture I applied T1 on the world frame then T2 on local frame:

![image](https://user-images.githubusercontent.com/95486750/209870244-373ee5d3-0d5d-48fc-868b-1d5260ba5e4c.png)

In this picture I applied T1 on the local frame and T2 on world frame, in both pictures the model is in different position:

![image](https://user-images.githubusercontent.com/95486750/209870319-81077dfa-e7ab-42f5-90a3-1656bc4637f7.png)

<b>6.</b> In order to draw the model's bounding box, we calculated the minimum and maximum values of each axis so we can draw the box.
Orthogtaphic:
![Task6](https://user-images.githubusercontent.com/95486750/209870573-3ed3e9b7-4dc9-40ac-ac0a-d17d991db480.png)
perspective:
![image](https://user-images.githubusercontent.com/95486750/209870627-07358615-9f24-4319-bccb-57e4c6611644.png)

<b>7.</b> Sanity check for the vertex normals and face normals.
Picture of face normals:

After translation and rotation:

Picture of vertex normals:

After translation and rotation:


<b>8.</b>  Orthographic projection:


![Orthographic](https://user-images.githubusercontent.com/95486750/209871150-dbc26a44-3533-4f87-9644-d6601af4b1aa.png)


![Orthographic_BoundingBox](https://user-images.githubusercontent.com/95486750/209871165-011d7ef1-ab57-489c-9937-0a1bbc329bdb.png)


Perspective projection:


![Perspective](https://user-images.githubusercontent.com/95486750/209871179-2728ed68-dcc9-4b11-a363-c08954e1bd75.png)


![Perspective_BoundingBox](https://user-images.githubusercontent.com/95486750/209871205-b3d24632-7b3f-40b8-a22f-957d248f116e.png)


<b>13.</b>

![image](https://user-images.githubusercontent.com/95486750/209870851-9ba578f8-9902-4b30-a64f-d8d2a6766357.png)

<b> 14. </b> Additional features:

I've added checkboxes to toggle:
- Model frame axes
- World frame axes
- Faces normals
- Vertexes normals
- Bounding box
