# StencilShadows
OpenGL implementation of the stencil shadow volume technique in C++. This application was created as a school project for a course in computer graphics. Shadow volumes are a solid method for creating dynamic and real-time shadows. This technique supports point lights and self-shadowing and is implemented using a geometry shader by the creation of the shadow volumes and triangles with adjacency information. Apart from the shadows, the application consists of an environmental map projected to infinity and a functional camera for moving around the scene. 

The scene is rendered with four passes:

1. A depth pass, which fills the z-buffer and is necessary for the Z-fail algorithm to ensure valid shadows when the camera is inside the shadow volume.
2. An environmental pass (optional) that displays the environmetal map projected to infinity.
3. A shadow pass where the shadow volumes are created and rendered to the stencil buffer.
4. A lightning pass is used for lighting up the scene with light using the stencil buffer as a mask.

# Screenshots
Final result 

Colored shadow volumes:

Extruded silhouettes to infinity:

# References
To code this project I had to watch/read many sources. Here are some important ones that really helped me.
1. Understanding of the stencil buffer - https://learnopengl.com/Advanced-OpenGL/Stencil-testing
2. Creating silhouettes with the geometry shader - https://ogldev.org/www/tutorial39/tutorial39.html
3. Stencil Shadow Volume by OGL - https://ogldev.org/www/tutorial40/tutorial40.html
