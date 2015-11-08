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

#include "newton-dynamics-master/coreLibrary_300/source/newton/Newton.h"
#include "newton-dynamics-master/packages/dMath/dVector.h"
#include "newton-dynamics-master/packages/dMath/dMatrix.h"
#include "newton-dynamics-master/packages/dMath/dQuaternion.h"
/*#include "newton-dynamics-master/packages/dNewton/dNewton.h"
#include "newton-dynamics-master/packages/dNewton/dNewtonCollision.h"
#include "newton-dynamics-master/packages/dNewton/dNewtonDynamicBody.h"*/

using namespace irr;

class Entity {
private:
    scene::ISceneNode *mNode;
    NewtonBody *mBody;

public:
    Entity(scene::ISceneNode *node) : mNode(node), mBody(0) {}
    Entity(scene::ISceneNode *node, NewtonBody *body) : mNode(node), mBody(body) {}

    scene::ISceneNode* getSceneNode() {
        return mNode;
    }

    NewtonBody* getBody() {
        return mBody;
    }

    void setBody(NewtonBody *body) {
        mBody = body;
    }
};

class ScriptManager {
private:
    Lua luaState;

    std::map<int, bool> keyStates;
    std::map<std::string, Entity*> entities;

    video::IVideoDriver *driver;
    scene::ISceneManager *smgr;
    NewtonWorld *newtonWorld;

private:
    void bindFunctions() {
        LuaTable global = luaState.GetGlobalEnvironment();

        auto createSphere = luaState.CreateFunction<void(std::string, std::string)>(
                [&](std::string name, std::string tex) -> void { createSphereNode(name, tex); });

        auto createCube = luaState.CreateFunction<void(std::string, std::string)>(
                [&](std::string name, std::string tex) -> void { createCubeNode(name, tex); });

        auto createAnimatedMesh = luaState.CreateFunction<void(std::string, std::string, std::string, int, int, int)>(
                [&](std::string name, std::string model, std::string texture, int frameFrom, int frameTo,
                    int animSpeed) -> void {
                    createAnimatedNode(name, model, texture, frameFrom, frameTo, animSpeed);
                });

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

        auto addForwardAnimator = luaState.CreateFunction<void(std::string, LuaTable, LuaTable, int, bool)>(
                [&](std::string name, LuaTable from, LuaTable to, int animationTime,
                    bool loop) -> void { addNodeForwardAnimator(name, from, to, animationTime, loop); });

        auto createSphereBody = luaState.CreateFunction<void(std::string, float, float)>(
                [&](std::string name, float radius, float mass) ->
                        void { createSphereBodyForNode(name, radius, mass); });

        auto createBoxBody = luaState.CreateFunction<void(std::string, LuaTable, float)>(
                [&](std::string name, LuaTable size, float mass) -> void { createBoxBodyForNode(name, size, mass); });

        auto addForceFn = luaState.CreateFunction<void(std::string, LuaTable)>(
                [&](std::string name, LuaTable vec) -> void { addForce(name, vec); });

        auto addImpulseFn = luaState.CreateFunction<void(std::string, LuaTable)>(
                [&](std::string name, LuaTable vec) -> void { addImpulse(name, vec); });

        global.Set("createSphere", createSphere);
        global.Set("createCube", createCube);
        global.Set("createAnimatedMesh", createAnimatedMesh);

        global.Set("setPosition", setPosition);
        global.Set("setRotation", setRotation);
        global.Set("setScale", setScale);

        global.Set("move", move);

        global.Set("addCircleAnimator", addCircleAnimator);
        global.Set("addForwardAnimator", addForwardAnimator);

        global.Set("createSphereBody", createSphereBody);
        global.Set("createBoxBody", createBoxBody);

        global.Set("addForce", addForceFn);
        global.Set("addImpulse", addImpulseFn);
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

    Entity* findEntity(std::string name) {
        Entity* entity = entities[name];

        if (!entity) {
            throw fmt::format("Could not find entity `{0}`", name);
        }

        return entity;
    }

    core::vector3df tableToVector3df(LuaTable pos) {
        if (pos.GetTypeOfValueAt("x") == LuaType::nil) {
            return core::vector3df(pos.Get<float>(0), pos.Get<float>(1), pos.Get<float>(2));
        } else {
            return core::vector3df(pos.Get<float>("x"), pos.Get<float>("y"), pos.Get<float>("z"));
        }
    }

    LuaTable vector3dfToTable(core::vector3df pos) {
        LuaTable table = luaState.CreateTable();

        table.Set<float>("x", pos.X);
        table.Set<float>("y", pos.Y);
        table.Set<float>("z", pos.Z);

        return table;
    }

    dVector tableToDvector(LuaTable vec) {
        if (vec.GetTypeOfValueAt("x") == LuaType::nil) {
            return dVector(vec.Get<float>(0), vec.Get<float>(1), vec.Get<float>(2));
        } else {
            return dVector(vec.Get<float>("x"), vec.Get<float>("y"), vec.Get<float>("z"));
        }
    }

    void addAnimator(scene::ISceneNode *node, scene::ISceneNodeAnimator *anim) {
        if (anim) {
            node->addAnimator(anim);
            anim->drop();
        }
    }

//    NewtonCollision* createSphereCollisionShape(float radius, scene::ISceneNode *node) {
    NewtonCollision* createSphereCollisionShape(float radius) {
//        core::vector3df position = node->getPosition();
//        core::vector3df center = (core::vector3df(radius, radius, radius) * 0.5f) + position;
        core::vector3df center = (core::vector3df(radius, radius, radius) * 0.5f);
        dVector origin(center.X, center.Y, center.Z);

        dMatrix offset(dGetIdentityMatrix());
        offset.m_posit = origin;

        int shapeId = 0;

        return NewtonCreateSphere(newtonWorld, radius, shapeId, &offset[0][0]);
    }

    NewtonCollision* createBoxCollisionShape(core::vector3df shapeSize) {
        //    NewtonCollision* createBoxCollisionShape(core::vector3df shapeSize, scene::ISceneNode *node) {
        dVector size(shapeSize.X, shapeSize.Y, shapeSize.Z);

//        core::vector3df position = node->getPosition();
//        core::vector3df center = (shapeSize * 0.5f + position);
        core::vector3df center = (shapeSize * 0.5f);
        dVector origin(center.X, center.Y, center.Z);

        dMatrix offset(dGetIdentityMatrix());
        offset.m_posit = origin;

        int shapeId = 0;

        return NewtonCreateBox(newtonWorld, size.m_x, size.m_y, size.m_z, shapeId, &offset[0][0]);
    }

    NewtonBody* createDynamicBody(NewtonCollision* shape, float mass) {
        dMatrix matrix(dGetIdentityMatrix());
        NewtonBody *body = NewtonCreateDynamicBody(newtonWorld, shape, &matrix[0][0]);

        NewtonBodySetMassProperties(body, mass, shape);
        NewtonDestroyCollision(shape);

        NewtonBodySetTransformCallback(body, transformCallback);
        NewtonBodySetForceAndTorqueCallback(body, applyForceAndTorqueCallback);

        return body;
    }

    NewtonBody* createKinematicBody(NewtonCollision* shape) {
        dMatrix matrix(dGetIdentityMatrix());
        NewtonBody *body = NewtonCreateKinematicBody(newtonWorld, shape, &matrix[0][0]);

        NewtonBodySetTransformCallback(body, transformCallback);
        NewtonBodySetForceAndTorqueCallback(body, applyForceAndTorqueCallback);

        return body;
    }

    static void transformCallback(const NewtonBody* body, const dFloat* matrix, int threadIndex)
    {
        Entity *entity = (Entity*) NewtonBodyGetUserData(body);
        scene::ISceneNode* node = entity->getSceneNode();

        if (node)
        {
            core::matrix4 transform;
            transform.setM(matrix);

            node->setPosition(transform.getTranslation());
            node->setRotation(transform.getRotationDegrees());
        }
    }

    static void applyForceAndTorqueCallback(const NewtonBody* body, dFloat timestep, int threadIndex)
    {
        dFloat Ixx, Iyy, Izz;
        dFloat mass;

        NewtonBodyGetMassMatrix(body, &mass, &Ixx, &Iyy, &Izz);

        dVector gravityForce(0.0f, mass * -9.8f, 0.0f, 1.0f);
        NewtonBodySetForce(body, &gravityForce[0]);
    }

    void initPhysics() {
        newtonWorld = NewtonCreate();
        NewtonSetSolverModel(newtonWorld, 1);
    }

    void updatePhysics(float dt) {
         NewtonUpdate(newtonWorld, dt);
    }

    void stopPhysics() {
        NewtonDestroyAllBodies(newtonWorld);
        NewtonDestroy(newtonWorld);
    }

public:
    ScriptManager(scene::ISceneManager *_smgr, video::IVideoDriver *_driver) {
        driver = _driver;
        smgr = _smgr;

        initPhysics();
    }

    void createSphereNode(const std::string name, const std::string textureFile) {
        scene::ISceneNode *node = smgr->addSphereSceneNode();

        if (node) {
            node->setMaterialTexture(0, driver->getTexture(textureFile.c_str()));
            node->setMaterialFlag(video::EMF_LIGHTING, false);
        }

        entities[name] = new Entity(node);
    }

    void createCubeNode(const std::string name, const std::string textureFile) {
        scene::ISceneNode *node = smgr->addCubeSceneNode();

        if (node) {
            node->setMaterialTexture(0, driver->getTexture(textureFile.c_str()));
            node->setMaterialFlag(video::EMF_LIGHTING, false);
        }

        entities[name] = new Entity(node);
    }

    void createAnimatedNode(const std::string name, const std::string modelFile, const std::string textureFile,
                            unsigned int framesFrom, unsigned int framesTo, unsigned int animationSpeed) {
        scene::IAnimatedMeshSceneNode *node = smgr->addAnimatedMeshSceneNode(smgr->getMesh(modelFile.c_str()));

        node->setMaterialFlag(video::EMF_LIGHTING, false);

        node->setFrameLoop(framesFrom, framesTo);
        node->setAnimationSpeed(animationSpeed);
        // node->setMD2Animation(scene::EMAT_RUN);

        node->setMaterialTexture(0, driver->getTexture(textureFile.c_str()));

        entities[name] = new Entity(node);
    }

    void addNodeCircleAnimator(const std::string name, LuaTable center, float radius) {
        scene::ISceneNode *node = findEntity(name)->getSceneNode();
        scene::ISceneNodeAnimator *anim = smgr->createFlyCircleAnimator(tableToVector3df(center), radius);

        addAnimator(node, anim);
    }

    void addNodeForwardAnimator(const std::string name, LuaTable from, LuaTable to, unsigned int animationTime,
                                bool loop) {
        scene::ISceneNode *node = findEntity(name)->getSceneNode();
        scene::ISceneNodeAnimator *anim = smgr->createFlyStraightAnimator(tableToVector3df(from), tableToVector3df(to),
                                                                          animationTime, loop);

        addAnimator(node, anim);
    }

    void setNodePosition(const std::string name, LuaTable pos) {
        Entity *entity = findEntity(name);

        if (!entity->getBody()) {
            scene::ISceneNode *node = findEntity(name)->getSceneNode();
            core::vector3df vec = tableToVector3df(pos);

            node->setPosition(vec);
        } else {
            NewtonBody *body = entity->getBody();
            dMatrix matrix = dGetIdentityMatrix();
            NewtonBodyGetMatrix(body, &matrix[0][0]);

            dVector vPos = tableToDvector(pos);
            matrix.m_posit = vPos;

            NewtonBodySetMatrix(body, &matrix[0][0]);
        }
    }

    void moveNode(const std::string name, LuaTable pos) {
        scene::ISceneNode *node = findEntity(name)->getSceneNode();
        core::vector3df vec = tableToVector3df(pos);

        core::matrix4 m;

        core::vector3df rot = node->getRotation();
        m.setRotationDegrees(rot);

        m.transformVect(vec);
        node->setPosition(node->getPosition() + vec);
        node->updateAbsolutePosition();
    }

    void setNodeRotation(const std::string name, LuaTable rot) {
        scene::ISceneNode *node = findEntity(name)->getSceneNode();
        core::vector3df vec = tableToVector3df(rot);

        node->setRotation(vec);
    }

    void setNodeScale(const std::string name, LuaTable scale) {
        scene::ISceneNode *node = findEntity(name)->getSceneNode();
        core::vector3df vec = tableToVector3df(scale);

        node->setScale(vec);
    }

    LuaTable getNodePosition(const std::string name) {
        LuaTable pos = luaState.CreateTable();
        scene::ISceneNode *node = findEntity(name)->getSceneNode();

        return vector3dfToTable(node->getPosition());
    }

    void createSphereBodyForNode(const std::string name, float radius, float mass) {
        Entity *entity = entities[name];

        NewtonCollision *shape = createSphereCollisionShape(radius);
        NewtonBody *body = createDynamicBody(shape, mass);
        NewtonBodySetUserData(body, entity);
        NewtonInvalidateCache(newtonWorld);

        entity->setBody(body);
    }

    void createBoxBodyForNode(const std::string name, LuaTable size, float mass) {
        Entity *entity = entities[name];

        NewtonCollision *shape = createBoxCollisionShape(tableToVector3df(size));
        NewtonBody *body = createDynamicBody(shape, mass);
        NewtonBodySetUserData(body, entity);
        NewtonInvalidateCache(newtonWorld);

        entity->setBody(body);
    }

    void addForce(const std::string name, LuaTable vec) {
        Entity *entity = entities[name];
        NewtonBody *body = entity->getBody();

        dVector forceVec = tableToDvector(vec);

        NewtonBodyAddForce(body, &forceVec[0]);
    }

    void addImpulse(const std::string name, LuaTable vec) {
        Entity *entity = entities[name];
        NewtonBody *body = entity->getBody();

        dVector forceVec = tableToDvector(vec);
        dVector center;

        NewtonBodyGetCentreOfMass(body, &center[0]);

        NewtonBodyAddImpulse(body, &forceVec[0], &center[0]);
    }

    void handleFrame(float dt) {
        auto handler = luaState.GetGlobalEnvironment().Get<LuaFunction<void(void)>>("handleFrame");

        updatePhysics(dt);
        setKeyStates();

        handler.Invoke();
    }

    void handleExit() {
        stopPhysics();
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

        driver->beginScene(true, true, video::SColor(255, 113, 113, 133));

        smgr->drawAll(); // draw the 3d scene
        device->getGUIEnvironment()->drawAll(); // draw the gui environment (the logo)

        scriptMgr->handleFrame(frameDeltaTime); // run scripts handling frame being rendered

        driver->endScene();

        int fps = driver->getFPS();

        if (lastFPS != fps) {
            std::string title = fmt::sprintf("Newtonian Physics [{0} FPS]", fps);

            device->setWindowCaption((const wchar_t *) title.c_str());
            lastFPS = fps;
        }
    }

    /*
    In the end, delete the Irrlicht device.
    */
    device->drop();

    scriptMgr->handleExit();
    delete scriptMgr;

    return 0;
}

/*
That's it. Compile and play around with the program.
**/