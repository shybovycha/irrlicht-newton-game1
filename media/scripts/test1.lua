createSphere("sphere1", "media/textures/wall.bmp")
setPosition("sphere1", { x=0, y=0, z=30 })

createCube("cube1", "media/textures/t351sml.jpg")
addCircleAnimator("cube1", { x=0, y=0, z=30 }, 20.0)

createAnimatedMesh("ninja", "media/models/ninja.b3d", "media/textures/nskinbl.jpg", 0, 13, 15)
setRotation("ninja", { x=0, y=-90, z=0 })
setScale("ninja", { x=2, y=2, z=2 })
addForwardAnimator("ninja", { x=100, y=0, z=60 }, { x=-100, y=0, z=60 }, 3500, true)
