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

    if KEY_STATE[0x20] == true then
        addImpulse("sphere1", {0, 20, 0})
    end
end

function main()
    createSphere("sphere1", "media/textures/wall.bmp")
    createSphereBody("sphere1", 10.0, 10.0)
    setPosition("sphere1", { x = 0, y = 0, z = 30 })

    createCube("cube1", "media/textures/t351sml.jpg")
    addCircleAnimator("cube1", { x = 0, y = 0, z = 30 }, 20.0)

    createAnimatedMesh("ninja", "media/models/ninja.b3d", "media/textures/nskinbl.jpg", 0, 13, 15)
    setRotation("ninja", { x = 0, y = -90, z = 0 })
    setScale("ninja", { x = 2, y = 2, z = 2 })
    addForwardAnimator("ninja", { x = 100, y = 0, z = 60 }, { x = -100, y = 0, z = 60 }, 3500, true)
end