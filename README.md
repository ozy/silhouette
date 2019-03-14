# silhouette

Silhouette is a raytracer that renders STL files written in C. 

## Usage

```bash
./bin/main fileName.stl > fileName.ppm
```
Basicly write your STL (Non Ascii) file as the first argument and forward the output into a ppm file.

## Adjusting Camera

```camMatrix``` variable is responsible for camera placement and direction. Depending on the size of the model, camera needs to be readjusted.

``` C
camMatrix = lookAt((V3f){100,100,150},(V3f){0,0,0});
//                     ^ Camera Origin,    ^ Target
```

## Some Renders

![A cat render](https://github.com/ozy/silhouette/raw/master/renders/cat.png "A cat render")

*Colored based on the angle between the ray and the triangle normal -- An angle shader*

![A cube render](https://github.com/ozy/silhouette/raw/master/renders/cube.png "A cube render")

*Also angle shader but colored uniquely per axis*

![A fan render](https://github.com/ozy/silhouette/raw/master/renders/fan.png "A fan render")

*Rendered with an experimental depth shader that resets every x mm*
