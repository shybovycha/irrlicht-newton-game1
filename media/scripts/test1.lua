function handleFrame()
    -- w
--    if KEY_STATE[0x57] == true then
--        move("sphere1", { x = 0, y = 1, z = 0 })
--    end
--
--    -- s
--    if KEY_STATE[0x53] == true then
--        move("sphere1", { x = 0, y = -1, z = 0 })
--    end

    -- Space
    if KEY_STATE[KEY_SPACE] == true then
        addImpulse("sphere1", {0, 0.25, 0})
    end

    -- Esc
    if KEY_STATE[KEY_ESCAPE] == true then
        exit()
    end
end

function main()
    createMesh("colliseum", "media/models/ramp_floor.3ds", "media/models/ramp_floor.png")
    createMeshBody("colliseum")

    createSphere("sphere1", "media/textures/wall.bmp")
    setScale("sphere1", { 0.5, 0.5, 0.5 })
    createSphereBody("sphere1", 2.5, 15.0)
    setPosition("sphere1", { x = 0, y = 20, z = 10 })

    createCube("cube1", "media/textures/t351sml.jpg")
    setPosition("cube1", { 0, -10, 0 })
    -- createBoxBody("cube1", { 100, 0.1, 100 }, 0)

    createAnimatedMesh("ninja", "media/models/ninja.b3d", "media/textures/nskinbl.jpg", 0, 13, 15)
    setRotation("ninja", { x = 0, y = -90, z = 0 })
    setScale("ninja", { x = 2, y = 2, z = 2 })
    addForwardAnimator("ninja", { x = 100, y = 0, z = 60 }, { x = -100, y = 0, z = 60 }, 3500, true)
end