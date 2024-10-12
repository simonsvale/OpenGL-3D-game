# Introduction
This is was my first attempt at a C++ 3D game engine, partly based on code from the site, Learn OpenGL(https://learnopengl.com/). 
I worked on it from the start of fall to the end of december 2023, but ultimately stopped developing it due to bad design which was caused by my inexperience in OpenGL when i started the project.
I have chosen to release the project as i hope people may find some of the things i have created insipiring or maybe even useful.

# Engine features
- My own map file format .arrmap (array map), which is JSON inspired. In hindsight simply using JSON or BSON would have been easier and more efficient.
- Semi efficient memory utilization if many vertices are shared. This is done using vertex buffer objects (VAOs) in conjuntion with element/indices buffer objects (EBOs).
- Shadow mapping, however illumination is only possible from a single light source.
- Display of textured and shaded models using diffuse and specular maps in the .arrmap format.
- Reflection probes for cubemapping (Only reflects the skybox correctly).
- A skybox.

# Libraries
- SDL2 for user input and writing to the screen with OpenGL.
- glad for OpenGL headers.
- OpenGL mathmatics (GLM) for rotation matrices and character controls.
- stb_image for loading in textures from .PNG images.

# Development images

Developing the loading of .arrmap models.
![image](https://github.com/user-attachments/assets/95fef4b5-e073-40ef-8852-213bf13ee881)

First iteration of the shading model.
![image](https://github.com/user-attachments/assets/e6e40ba7-a394-4b0a-9c6e-787be803a6ec)

Testing depthmapping / shadowmapping.
![image](https://github.com/user-attachments/assets/48ae2b49-c711-4407-93ab-7208800ff873)

Testing reflection mapping of the skybox for all models in the scene.
![image](https://github.com/user-attachments/assets/b687a4e6-7fdc-4796-ab0c-11ecc2480821)

Development of reflection probes.
![image](https://github.com/user-attachments/assets/7ba39e21-6e7f-4759-90e0-fe0bf56eb0c1)

Short gif:



