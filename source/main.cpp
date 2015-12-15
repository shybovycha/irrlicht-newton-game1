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
    dVector summaryForce;

public:
    Entity(scene::ISceneNode *node) : mNode(node), mBody(0), summaryForce(dVector(0, 0, 0)) {}
    Entity(scene::ISceneNode *node, NewtonBody *body) : mNode(node), mBody(body), summaryForce(dVector(0, 0, 0)) {}

    scene::ISceneNode* getSceneNode() const {
        return mNode;
    }

    NewtonBody* getBody() const {
        return mBody;
    }

    void setBody(NewtonBody *body) {
        mBody = body;
    }

    void addForce(dVector force) {
        summaryForce += force;
    }

    dVector getSummaryForce() const {
        return summaryForce;
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

        auto createSphereFn = luaState.CreateFunction<void(std::string, std::string)>(
                [&](std::string name, std::string tex) -> void { createSphereNode(name, tex); });

        auto createCubeFn = luaState.CreateFunction<void(std::string, std::string)>(
                [&](std::string name, std::string tex) -> void { createCubeNode(name, tex); });

        auto createAnimatedMeshFn = luaState.CreateFunction<void(std::string, std::string, std::string, int, int, int)>(
                [&](std::string name, std::string model, std::string texture, int frameFrom, int frameTo,
                    int animSpeed) -> void {
                    createAnimatedNode(name, model, texture, frameFrom, frameTo, animSpeed);
                });

        auto createMeshFn = luaState.CreateFunction<void(std::string, std::string)>(
                [&](std::string name, std::string tex) -> void { createMeshNode(name, tex); });

        auto setPositionFn = luaState.CreateFunction<void(std::string, LuaTable)>(
                [&](std::string name, LuaTable pos) -> void { setNodePosition(name, pos); });

        auto setRotationFn = luaState.CreateFunction<void(std::string, LuaTable)>(
                [&](std::string name, LuaTable rot) -> void { setNodeRotation(name, rot); });

        auto setScaleFn = luaState.CreateFunction<void(std::string, LuaTable)>(
                [&](std::string name, LuaTable scale) -> void { setNodeScale(name, scale); });

        auto moveFn = luaState.CreateFunction<void(std::string, LuaTable)>(
                [&](std::string name, LuaTable delta) -> void { moveNode(name, delta); });

        auto addCircleAnimatorFn = luaState.CreateFunction<void(std::string, LuaTable, float)>(
                [&](std::string name, LuaTable center, float radius) -> void {
                    addNodeCircleAnimator(name, center, radius);
                });

        auto addForwardAnimatorFn = luaState.CreateFunction<void(std::string, LuaTable, LuaTable, int, bool)>(
                [&](std::string name, LuaTable from, LuaTable to, int animationTime,
                    bool loop) -> void { addNodeForwardAnimator(name, from, to, animationTime, loop); });

        auto createSphereBodyFn = luaState.CreateFunction<void(std::string, float, float)>(
                [&](std::string name, float radius, float mass) ->
                        void { createSphereBody(name, radius, mass); });

        auto createBoxBodyFn = luaState.CreateFunction<void(std::string, LuaTable, float)>(
                [&](std::string name, LuaTable size, float mass) -> void { createBoxBody(name, size, mass); });

        auto createMeshBodyFn = luaState.CreateFunction<void(std::string)>(
                [&](std::string name) -> void { createMeshBody(name); });


        auto addForceFn = luaState.CreateFunction<void(std::string, LuaTable)>(
                [&](std::string name, LuaTable vec) -> void { addForce(name, vec); });

        auto addImpulseFn = luaState.CreateFunction<void(std::string, LuaTable)>(
                [&](std::string name, LuaTable vec) -> void { addImpulse(name, vec); });

        global.Set("createSphere", createSphereFn);
        global.Set("createCube", createCubeFn);
        global.Set("createAnimatedMesh", createAnimatedMeshFn);
        global.Set("createMesh", createMeshFn);

        global.Set("setPosition", setPositionFn);
        global.Set("setRotation", setRotationFn);
        global.Set("setScale", setScaleFn);

        global.Set("move", moveFn);

        global.Set("addCircleAnimator", addCircleAnimatorFn);
        global.Set("addForwardAnimator", addForwardAnimatorFn);

        global.Set("createSphereBody", createSphereBodyFn);
        global.Set("createBoxBody", createBoxBodyFn);
        global.Set("createMeshBody", createMeshBodyFn);

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
            return core::vector3df(pos.Get<float>(1), pos.Get<float>(2), pos.Get<float>(3));
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
            return dVector(vec.Get<float>(1), vec.Get<float>(2), vec.Get<float>(3));
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

    NewtonCollision* createSphereCollisionShape(scene::ISceneNode *node, float radius) {
        dQuaternion q(node->getRotation().X, node->getRotation().Y, node->getRotation().Z, 1.f);
        dVector v(node->getPosition().X, node->getPosition().Y, node->getPosition().Z);
        dMatrix origin(q, v);

        int shapeId = 0;

        return NewtonCreateSphere(newtonWorld, radius, shapeId, &origin[0][0]);
    }

    NewtonCollision* createBoxCollisionShape(scene::ISceneNode *node, core::vector3df shapeSize) {
        dVector size(shapeSize.X, shapeSize.Y, shapeSize.Z);

        core::vector3df center = node->getPosition() + (shapeSize);

        dQuaternion q(node->getRotation().X, node->getRotation().Y, node->getRotation().Z, 1.f);
        dVector v(center.X, center.Y, center.Z);
        dMatrix origin(q, v);

        int shapeId = 0;

        return NewtonCreateBox(newtonWorld, size.m_x, size.m_y, size.m_z, shapeId, &origin[0][0]);
    }

    NewtonBody* createDynamicBody(NewtonCollision* shape, float mass) {
        dMatrix origin;
        NewtonCollisionGetMatrix(shape, &origin[0][0]);
        NewtonBody *body = NewtonCreateDynamicBody(newtonWorld, shape, &origin[0][0]);

        dVector inertia;
        NewtonConvexCollisionCalculateInertialMatrix(shape, &inertia[0], &origin[0][0]);
        NewtonBodySetMassMatrix(body, mass, mass * inertia.m_x, mass * inertia.m_y, mass * inertia.m_z);
        NewtonBodySetCentreOfMass(body, &origin[0][0]);

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
        Entity* entity = (Entity*) NewtonBodyGetUserData(body);

        dVector force = entity->getSummaryForce() + dVector(0, -9.8f, 0);
        NewtonBodySetForce(body, &force[0]);
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

    void trimeshFromStandardVertices(irr::scene::IMeshBuffer* meshBuffer, NewtonCollision* treeCollision, irr::core::vector3df scale = irr::core::vector3df(1, 1, 1)) {
        irr::core::vector3df vArray[3];

        irr::video::S3DVertex* mb_vertices = (irr::video::S3DVertex*) meshBuffer->getVertices();

        u16* mb_indices  = meshBuffer->getIndices();

        for (unsigned int j = 0; j < meshBuffer->getIndexCount(); j += 3) {
            int v1i = mb_indices[j + 0];
            int v2i = mb_indices[j + 1];
            int v3i = mb_indices[j + 2];

            vArray[0] = mb_vertices[v1i].Pos * scale.X;
            vArray[1] = mb_vertices[v2i].Pos * scale.Y;
            vArray[2] = mb_vertices[v3i].Pos * scale.Z;

            NewtonTreeCollisionAddFace(treeCollision, 3, &vArray[0].X, sizeof(irr::core::vector3df), 1);
        }
    }

    void trimeshFrom2TCoordVertices(irr::scene::IMeshBuffer* meshBuffer, NewtonCollision* treeCollision, irr::core::vector3df scale = irr::core::vector3df(1, 1, 1)) {
        irr::core::vector3df vArray[3];

        irr::video::S3DVertex2TCoords* mb_vertices = (irr::video::S3DVertex2TCoords*) meshBuffer->getVertices();

        u16* mb_indices  = meshBuffer->getIndices();

        for (unsigned int j = 0; j < meshBuffer->getIndexCount(); j += 3) {
            int v1i = mb_indices[j + 0];
            int v2i = mb_indices[j + 1];
            int v3i = mb_indices[j + 2];

            vArray[0] = mb_vertices[v1i].Pos * scale.X;
            vArray[1] = mb_vertices[v2i].Pos * scale.Y;
            vArray[2] = mb_vertices[v3i].Pos * scale.Z;

            NewtonTreeCollisionAddFace(treeCollision, 3, &vArray[0].X, sizeof(irr::core::vector3df), 1);
        }
    }

    void trimeshFromTangentVertices(irr::scene::IMeshBuffer* meshBuffer, NewtonCollision* treeCollision, irr::core::vector3df scale = irr::core::vector3df(1, 1, 1)) {
        irr::core::vector3df vArray[3];

        irr::video::S3DVertexTangents* mb_vertices = (irr::video::S3DVertexTangents*) meshBuffer->getVertices();

        u16* mb_indices  = meshBuffer->getIndices();

        for (unsigned int j = 0; j < meshBuffer->getIndexCount(); j += 3) {
            int v1i = mb_indices[j + 0];
            int v2i = mb_indices[j + 1];
            int v3i = mb_indices[j + 2];

            vArray[0] = mb_vertices[v1i].Pos * scale.X;
            vArray[1] = mb_vertices[v2i].Pos * scale.Y;
            vArray[2] = mb_vertices[v3i].Pos * scale.Z;

            NewtonTreeCollisionAddFace(treeCollision, 3, &vArray[0].X, sizeof(irr::core::vector3df), 1);
        }
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

    void createMeshNode(const std::string name, const std::string modelFile) {
        scene::ISceneNode *node = smgr->addMeshSceneNode(smgr->getMesh(modelFile.c_str()));

        node->setMaterialFlag(video::EMF_LIGHTING, true);

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

    void createSphereBody(const std::string name, float radius, float mass) {
        Entity *entity = entities[name];

        NewtonCollision *shape = createSphereCollisionShape(entity->getSceneNode(), radius);
        NewtonBody *body;

        body = createDynamicBody(shape, mass);

        NewtonBodySetUserData(body, entity);
        NewtonInvalidateCache(newtonWorld);

        entity->setBody(body);
    }

    void createBoxBody(const std::string name, LuaTable size, float mass) {
        Entity *entity = entities[name];

        NewtonCollision *shape = createBoxCollisionShape(entity->getSceneNode(), tableToVector3df(size));
        NewtonBody *body;

        body = createDynamicBody(shape, mass);

        NewtonBodySetUserData(body, entity);
        NewtonInvalidateCache(newtonWorld);

        entity->setBody(body);
    }

    void createMeshBody(const std::string name) {
        Entity *entity = entities[name];
        irr::scene::IMeshSceneNode *node = (irr::scene::IMeshSceneNode*) entity->getSceneNode();
        NewtonCollision *treeCollision;
        treeCollision = NewtonCreateTreeCollision(newtonWorld, 0);
        NewtonTreeCollisionBeginBuild(treeCollision);

        irr::scene::IMesh *mesh = node->getMesh();

        for (unsigned int i = 0; i < mesh->getMeshBufferCount(); i++) {
            irr::scene::IMeshBuffer *mb = mesh->getMeshBuffer(i);

            switch(mb->getVertexType()) {
                case irr::video::EVT_STANDARD:
                    trimeshFromStandardVertices(mb, treeCollision, node->getScale());
                    break;

                case irr::video::EVT_2TCOORDS:
                    trimeshFrom2TCoordVertices(mb, treeCollision, node->getScale());
                    break;

                case irr::video::EVT_TANGENTS:
                    trimeshFromTangentVertices(mb, treeCollision, node->getScale());
                    break;

                default:
                    printf("Newton error: Unknown vertex type in static mesh: %d\n", mb->getVertexType());
            }
        }

        NewtonTreeCollisionEndBuild(treeCollision, 1);

        NewtonBody* body;

        body = createDynamicBody(treeCollision, 0.0);

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

        dMatrix matrix = dGetIdentityMatrix();
        NewtonBodyGetMatrix(body, &matrix[0][0]);

        dVector center;
        NewtonBodyGetCentreOfMass(body, &center[0]);

        dVector globalPoint;
        matrix.RotateVector(forceVec);
        globalPoint = matrix.TransformVector(dVector(0, 0, 0));

        NewtonBodyAddImpulse(body, &forceVec[0], &globalPoint[0]);
    }

    void drawPhysicsDebug() {
        for (auto& kv : entities) {
            Entity *entity = kv.second;

            if (!entity->getBody())
                continue;

            NewtonCollision *shape = NewtonBodyGetCollision(entity->getBody());

            dMatrix offset = dGetIdentityMatrix();

            dVector bodyPos;
            NewtonBodyGetPosition(entity->getBody(), &bodyPos[0]);

            dVector minBodyBBox, maxBodyBBox;
            NewtonCollisionCalculateAABB(shape, &offset[0][0], &minBodyBBox[0], &maxBodyBBox[0]);

            core::vector3df pos(bodyPos.m_x, bodyPos.m_y, bodyPos.m_z);

            core::vector3df minBBox = core::vector3df(minBodyBBox.m_x, minBodyBBox.m_y, minBodyBBox.m_z) + pos;
            core::vector3df maxBBox = core::vector3df(maxBodyBBox.m_x, maxBodyBBox.m_y, maxBodyBBox.m_z) + pos;

            video::SMaterial mat;
            mat.Lighting = false;
            driver->setMaterial(mat);

            driver->setTransform(video::ETS_WORLD, core::matrix4());
            driver->draw3DBox(core::aabbox3d<f32>(minBBox, maxBBox), video::SColor(255, 0, 255, 0));
        }
    }

    void handleFrame(float dt) {
        auto handler = luaState.GetGlobalEnvironment().Get<LuaFunction<void(void)>>("handleFrame");

        updatePhysics(dt);
        setKeyStates();

        handler.Invoke();

        drawPhysicsDebug();
    }

    void handleExit() {
        stopPhysics();
    }

    void loadScript(const std::string filename) {
        std::ifstream inf(filename);

        if (!inf.good()) {
            throw fmt::format("Could not find script `{0}`", filename);
        }

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
    irr::scene::ICameraSceneNode* camera = smgr->addCameraSceneNodeFPS();
    device->getCursorControl()->setVisible(false);

    camera->setPosition(irr::core::vector3df(0, 10.0, -140.0f));

    /*
    Add a colorful irrlicht logo
    */
    device->getGUIEnvironment()->addImage(
            driver->getTexture("media/textures/irrlichtlogoalpha2.tga"),
            core::position2d<s32>(10, 20));

    // In order to do framerate independent movement, we have to know
    // how long it was since the last frame
    u32 then = device->getTimer()->getTime();

    while (device->run()) {
        // Work out a frame delta time.
        const u32 now = device->getTimer()->getTime();
        const f32 frameDeltaTime = (f32) (now - then);
        then = now;

        driver->beginScene(true, true, video::SColor(255, 113, 113, 133));

        smgr->drawAll(); // draw the 3d scene
        device->getGUIEnvironment()->drawAll(); // draw the gui environment (the logo)

        scriptMgr->handleFrame(frameDeltaTime); // run scripts handling frame being rendered

        driver->endScene();
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