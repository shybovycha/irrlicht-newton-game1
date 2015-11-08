/** Example 004 Movement

This Tutorial shows how to move and animate SceneNodes. The
basic concept of SceneNodeAnimators is shown as well as manual
movement of nodes using the keyboard.  We'll demonstrate framerate
independent movement, which means moving by an amount dependent
on the duration of the last run of the Irrlicht loop.

Example 19.MouseAndJoystick shows how to handle those kinds of input.

As always, I include the header files, use the irr namespace,
and tell the linker to link with the .lib file.
*/
#ifdef _MSC_VER
// We'll also define this to stop MSVC complaining about sprintf().
#define _CRT_SECURE_NO_WARNINGS
#pragma comment(lib, "Irrlicht.lib")
#endif

#include <fstream>
#include <irrlicht.h>
#include <map>
#include <sstream>
#include <iostream>

#include "luacppinterface-master/include/luacppinterface.h"
#include "cppformat-master/format.h"

using namespace irr;

/*
To receive events like mouse and keyboard input, or GUI events like "the OK
button has been clicked", we need an object which is derived from the
irr::IEventReceiver object. There is only one method to override:
irr::IEventReceiver::OnEvent(). This method will be called by the engine once
when an event happens. What we really want to know is whether a key is being
held down, and so we will remember the current state of each key.
*/

class ScriptManager {
private:
    Lua luaState;
    std::map<int, bool> keyStates;
    std::map<std::string, scene::ISceneNode *> nodes;
    video::IVideoDriver *driver;
    scene::ISceneManager *smgr;

    void bindFunctions() {
        LuaTable global = luaState.GetGlobalEnvironment();

        auto createSphere = luaState.CreateFunction<void(std::string, std::string)>(
                [&](std::string name, std::string tex) -> void { createSphereNode(name, tex); });

        auto createCube = luaState.CreateFunction<void(std::string, std::string)>(
                [&](std::string name, std::string tex) -> void { createCubeNode(name, tex); });

        auto createAnimatedMesh = luaState.CreateFunction<void(std::string, std::string, std::string, int, int, int)>(
                [&](std::string name, std::string model, std::string texture, int frameFrom, int frameTo, int animSpeed) -> void { createAnimatedNode(name, model, texture, frameFrom, frameTo, animSpeed); });

        auto setPosition = luaState.CreateFunction<void(std::string, LuaTable)>(
                [&](std::string name, LuaTable pos) -> void { setNodePosition(name, pos); });

        auto setRotation = luaState.CreateFunction<void(std::string, LuaTable)>(
                [&](std::string name, LuaTable rot) -> void { setNodeRotation(name, rot); });

        auto setScale = luaState.CreateFunction<void(std::string, LuaTable)>(
                [&](std::string name, LuaTable scale) -> void { setNodeScale(name, scale); });

        auto move = luaState.CreateFunction<void(std::string, LuaTable)>(
                [&](std::string name, LuaTable delta) -> void { moveNode(name, delta); });

        auto addCircleAnimator = luaState.CreateFunction<void(std::string, LuaTable, float)>(
                [&](std::string name, LuaTable center, float radius) -> void {
                    addNodeCircleAnimator(name, center, radius);
                });

        auto addForwardAnimator = luaState.CreateFunction<void(std::string, LuaTable, LuaTable, int, bool)>([&](std::string name, LuaTable from, LuaTable to, int animationTime, bool loop) -> void { addNodeForwardAnimator(name, from, to, animationTime, loop); });

        global.Set("createSphere", createSphere);
        global.Set("createCube", createCube);
        global.Set("createAnimatedMesh", createAnimatedMesh);

        global.Set("setPosition", setPosition);
        global.Set("setRotation", setRotation);
        global.Set("setScale", setScale);

        global.Set("move", move);

        global.Set("addCircleAnimator", addCircleAnimator);
        global.Set("addForwardAnimator", addForwardAnimator);
    }

    void setGlobalVariables() {
        setKeyStates();
    }

    void setKeyStates() {
        LuaTable keysTable = luaState.CreateTable();

        for (auto &kv : keyStates) {
            keysTable.Set(kv.first, kv.second);
        }

        luaState.GetGlobalEnvironment().Set("KEY_STATE", keysTable);
    }

    scene::ISceneNode *findNode(std::string name) {
        scene::ISceneNode *node = nodes[name];

        if (!node) {
            throw fmt::format("Could not find node {1}", name);
        }

        return node;
    }

    core::vector3df tableToVector3df(LuaTable pos) {
        float x = pos.Get<float>("x"),
                y = pos.Get<float>("y"),
                z = pos.Get<float>("z");

        return core::vector3df(x, y, z);
    }

    LuaTable vector3dfToTable(core::vector3df pos) {
        LuaTable table = luaState.CreateTable();

        table.Set<float>("x", pos.X);
        table.Set<float>("y", pos.Y);
        table.Set<float>("z", pos.Z);

        return table;
    }

    void addAnimator(scene::ISceneNode *node, scene::ISceneNodeAnimator *anim) {
        if (anim) {
            node->addAnimator(anim);
            anim->drop();
        }
    }

public:
    ScriptManager(scene::ISceneManager *_smgr, video::IVideoDriver *_driver) {
        driver = _driver;
        smgr = _smgr;
    }

    void createSphereNode(const std::string name, const std::string textureFile) {
        scene::ISceneNode *node = smgr->addSphereSceneNode();

        if (node) {
            node->setMaterialTexture(0, driver->getTexture(textureFile.c_str()));
            node->setMaterialFlag(video::EMF_LIGHTING, false);
        }

        nodes[name] = node;
    }

    void createCubeNode(const std::string name, const std::string textureFile) {
        scene::ISceneNode *node = smgr->addCubeSceneNode();

        if (node) {
            node->setMaterialTexture(0, driver->getTexture(textureFile.c_str()));
            node->setMaterialFlag(video::EMF_LIGHTING, false);
        }

        nodes[name] = node;
    }

    void createAnimatedNode(const std::string name, const std::string modelFile, const std::string textureFile,
                            unsigned int framesFrom, unsigned int framesTo, unsigned int animationSpeed) {
        scene::IAnimatedMeshSceneNode *node = smgr->addAnimatedMeshSceneNode(smgr->getMesh(modelFile.c_str()));

        node->setMaterialFlag(video::EMF_LIGHTING, false);

        node->setFrameLoop(framesFrom, framesTo);
        node->setAnimationSpeed(animationSpeed);
        // node->setMD2Animation(scene::EMAT_RUN);

        node->setMaterialTexture(0, driver->getTexture(textureFile.c_str()));

        nodes[name] = node;
    }

    void addNodeCircleAnimator(const std::string name, LuaTable center, float radius) {
        scene::ISceneNode *node = findNode(name);
        scene::ISceneNodeAnimator *anim = smgr->createFlyCircleAnimator(tableToVector3df(center), radius);

        addAnimator(node, anim);
    }

    void addNodeForwardAnimator(const std::string name, LuaTable from, LuaTable to, unsigned int animationTime,
                                bool loop) {
        scene::ISceneNode *node = findNode(name);
        scene::ISceneNodeAnimator *anim = smgr->createFlyStraightAnimator(tableToVector3df(from), tableToVector3df(to),
                                                                          animationTime, loop);

        addAnimator(node, anim);
    }

    void setNodePosition(const std::string name, LuaTable pos) {
        scene::ISceneNode *node = findNode(name);
        core::vector3df vec = tableToVector3df(pos);

        node->setPosition(vec);
    }

    void moveNode(const std::string name, LuaTable pos) {
        scene::ISceneNode *node = findNode(name);
        core::vector3df vec = tableToVector3df(pos);

        core::matrix4 m;

        core::vector3df rot = node->getRotation();
        m.setRotationDegrees(rot);

        m.transformVect(vec);
        node->setPosition(node->getPosition() + vec);
        node->updateAbsolutePosition();
    }

    void setNodeRotation(const std::string name, LuaTable rot) {
        scene::ISceneNode *node = findNode(name);
        core::vector3df vec = tableToVector3df(rot);

        node->setRotation(vec);
    }

    void setNodeScale(const std::string name, LuaTable scale) {
        scene::ISceneNode *node = findNode(name);
        core::vector3df vec = tableToVector3df(scale);

        node->setScale(vec);
    }

    LuaTable getNodePosition(const std::string name) {
        LuaTable pos = luaState.CreateTable();
        scene::ISceneNode *node = findNode(name);

        return vector3dfToTable(node->getPosition());
    }

    void handleFrame() {
        auto handler = luaState.GetGlobalEnvironment().Get<LuaFunction<void(void)>>("handleFrame");

        setKeyStates();

        handler.Invoke();
    }

    void loadScript(const std::string filename) {
        std::ifstream inf(filename);
        std::string code((std::istreambuf_iterator<char>(inf)), std::istreambuf_iterator<char>());

        bindFunctions();
        setGlobalVariables();

        luaState.RunScript(code);

        auto scriptMainFn = luaState.GetGlobalEnvironment().Get<LuaFunction<void(void)>>("main");
        scriptMainFn.Invoke();
    }

    void setKeyState(int key, bool state) {
        keyStates[key] = state;
    }
};

class MyEventReceiver : public IEventReceiver {
public:
    MyEventReceiver(ScriptManager *scriptManager) {
        scriptMgr = scriptManager;

        for (u32 i = 0; i < KEY_KEY_CODES_COUNT; ++i)
            scriptMgr->setKeyState(i, false);
    }

    // This is the one method that we have to implement
    virtual bool OnEvent(const SEvent &event) {
        // Remember whether each key is down or up
        if (event.EventType == irr::EET_KEY_INPUT_EVENT)
            scriptMgr->setKeyState(event.KeyInput.Key, event.KeyInput.PressedDown);

        return false;
    }

private:
    ScriptManager *scriptMgr;
};

/*
The event receiver for keeping the pressed keys is ready, the actual responses
will be made inside the render loop, right before drawing the scene. So lets
just create an irr::IrrlichtDevice and the scene node we want to move. We also
create some other additional scene nodes, to show that there are also some
different possibilities to move and animate scene nodes.
*/
int main() {
    IrrlichtDevice *device = createDevice(video::EDT_OPENGL,
                                          core::dimension2d<u32>(640, 480), 16, false, false, false);

    if (device == 0)
        return 1; // could not create selected driver.

    video::IVideoDriver *driver = device->getVideoDriver();
    scene::ISceneManager *smgr = device->getSceneManager();

    ScriptManager *scriptMgr = new ScriptManager(smgr, driver);

    MyEventReceiver receiver(scriptMgr);

    device->setEventReceiver(&receiver);

    scriptMgr->loadScript("media/scripts/test1.lua");

    /*
    To be able to look at and move around in this scene, we create a first
    person shooter style camera and make the mouse cursor invisible.
    */
    smgr->addCameraSceneNodeFPS();
    device->getCursorControl()->setVisible(false);

    /*
    Add a colorful irrlicht logo
    */
    device->getGUIEnvironment()->addImage(
            driver->getTexture("media/textures/irrlichtlogoalpha2.tga"),
            core::position2d<s32>(10, 20));

    /*
    We have done everything, so lets draw it. We also write the current
    frames per second and the name of the driver to the caption of the
    window.
    */
    int lastFPS = -1;

    // In order to do framerate independent movement, we have to know
    // how long it was since the last frame
    u32 then = device->getTimer()->getTime();

    // This is the movemen speed in units per second.
    const f32 MOVEMENT_SPEED = 5.f;

    while (device->run()) {
        // Work out a frame delta time.
        const u32 now = device->getTimer()->getTime();
        const f32 frameDeltaTime = (f32) (now - then) / 1000.f; // Time in seconds
        then = now;

        /* Check if keys W, S, A or D are being held down, and move the
        sphere node around respectively. */
        /*core::vector3df nodePosition = node->getPosition();

        if (receiver.IsKeyDown(irr::KEY_KEY_W))
            nodePosition.Y += MOVEMENT_SPEED * frameDeltaTime;
        else if (receiver.IsKeyDown(irr::KEY_KEY_S))
            nodePosition.Y -= MOVEMENT_SPEED * frameDeltaTime;

        if (receiver.IsKeyDown(irr::KEY_KEY_A))
            nodePosition.X -= MOVEMENT_SPEED * frameDeltaTime;
        else if (receiver.IsKeyDown(irr::KEY_KEY_D))
            nodePosition.X += MOVEMENT_SPEED * frameDeltaTime;

        node->setPosition(nodePosition);*/

        driver->beginScene(true, true, video::SColor(255, 113, 113, 133));

        smgr->drawAll(); // draw the 3d scene
        device->getGUIEnvironment()->drawAll(); // draw the gui environment (the logo)
        scriptMgr->handleFrame(); // run scripts handling frame being rendered

        driver->endScene();

        int fps = driver->getFPS();

        if (lastFPS != fps) {
            core::stringw tmp(L"Movement Example - Irrlicht Engine [");
            tmp += driver->getName();
            tmp += L"] fps: ";
            tmp += fps;

            device->setWindowCaption(tmp.c_str());
            lastFPS = fps;
        }
    }

    /*
    In the end, delete the Irrlicht device.
    */
    device->drop();

    return 0;
}

/*
That's it. Compile and play around with the program.
**/