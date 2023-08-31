# Stencil Shadows
This is an OpenGL implementation of the stencil shadow volume technique in C++. The project was developed for a computer graphics course as a school project. Stencil shadows are a reliable method for generating dynamic and real-time shadows. The technique supports point lights, self-shadowing, and utilizes a geometry shader to create shadow volumes and triangles with adjacency information. In addition to the shadows, the application includes an environmental map projected to infinity and a functional camera for navigating the scene.

The rendering process involves four passes:

1. **Depth pass:** This pass fills the z-buffer and is essential for the Z-fail algorithm to ensure accurate shadows, even when the camera is inside the shadow volume.
2. **Environmental pass (optional):** This pass displays the environmental map projected to infinity.
3. **Shadow pass:** During this pass, the shadow volumes are created and rendered to the stencil buffer.
4. **Lighting pass:** This pass illuminates the scene using the stencil buffer as a mask to apply lighting effects.

Mesh conditions to support functionnal stencil shadows: 

1. Shadow casting mesh must be a crack free surface.
2. It must be a topological 2 manifold, i.e. a topological space whose points all have open disks as neighborhoods.
3. It must be homeomorphic to the sphere.
4. It must look locally like the plane everywhere.
5. It must be able to find the silhouette edges of every shadow caster and build the corresponding shadow volume.

<!---You can notice that the moose on the third picture has a shadow artifact, that's because...--->


# Screenshots
## Final result 
![bear](https://github.com/krivmi/StencilShadows/assets/35463969/23318bc1-6030-43f2-b6d7-73fb38b7f929)
![img4](https://github.com/krivmi/StencilShadows/assets/35463969/65c7f899-2def-44e0-9c40-82548eb8b7e6)
![img1](https://github.com/krivmi/StencilShadows/assets/35463969/6041fe4e-7d59-43f1-8ed1-86760c5346e3)

## Colored shadow volumes without caps:
![shadow_volume](https://github.com/krivmi/StencilShadows/assets/35463969/ddc31a7a-af27-4bca-ad10-4771d2ce35ab)

## Extruded silhouettes to infinity:
![silhuettes_in_infinity](https://github.com/krivmi/StencilShadows/assets/35463969/bf780bc2-be90-4bc8-945b-1828bb25173b)
<!--
## It does not work for all types of meshes:
![img6](https://github.com/krivmi/StencilShadows/assets/35463969/fd204cff-0092-4421-81c8-102120dc58ae)--->

# References
To code this whole project, I had to watch and read many sources. Here are some important ones that really helped me:
1. Understanding of the stencil buffer - https://learnopengl.com/Advanced-OpenGL/Stencil-testing
2. Creating silhouettes with the geometry shader - https://ogldev.org/www/tutorial39/tutorial39.html
3. Stencil Shadow Volume by OGL - https://ogldev.org/www/tutorial40/tutorial40.html
