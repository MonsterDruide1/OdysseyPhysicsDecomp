#include "Scene/SceneObjFactory.h"

#include "Library/Scene/ISceneObj.h"
#include "Library/Scene/SceneObjHolder.h"

#include "Item/CoinCollectHolder.h"
#include "Item/CoinCollectWatcher.h"

static al::ISceneObj* sceneObjCreator(s32 id) {
    switch (id) {
        case SceneObjID_CoinCollectHolder:
            return new CoinCollectHolder();
        case SceneObjID_CoinCollectWatcher:
            return new CoinCollectWatcher();
        default:
            printf("Unknown SceneObjID: %d\n", id);
            break;
    }
    // TODO
    return nullptr;
}

al::SceneObjHolder* SceneObjFactory::createSceneObjHolder() {
    return new al::SceneObjHolder(&sceneObjCreator, SceneObjID_Max);
}
